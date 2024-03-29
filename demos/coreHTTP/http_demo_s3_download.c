/*
 * FreeRTOS V202203.00
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 */

/*
 * Demo for showing use of the HTTP API using a server-authenticated network
 * connection.
 *
 * This example, using a pre-signed URL, resolves a S3 domain, establishes a TCP
 * connection, validates the server's certificate using the root CA certificate
 * defined in the config header, then finally performs a TLS handshake with the
 * HTTP server so that all communication is encrypted. After which, the HTTP
 * client library API is used to download the S3 file (by sending multiple GET
 * requests, filling up the response buffer each time until all parts are
 * downloaded). If any request fails, an error code is returned.
 *
 * @note This demo requires user-generated pre-signed URLs to be pasted into
 * http_demo_s3_download_config.h. Please use the provided script
 * "presigned_urls_gen.py" (located in http_demo_helpers) to generate these
 * URLs. For detailed instructions, see the accompanied README.md.
 *
 * @note If your file requires more than 99 range requests to S3 (depending on the
 * size of the file and the length specified in democonfigRANGE_REQUEST_LENGTH),
 * your connection may be dropped by S3. In this case, either increase the
 * buffer size and range request length (if feasible), to reduce the number of
 * requests required, or re-establish the connection with S3 after receiving a
 * "Connection: close" response header.
 *
 * @note This demo uses retry logic to connect to the server if connection
 * attempts fail. The FreeRTOS/backoffAlgorithm library is used to calculate the
 * retry interval with an exponential backoff and jitter algorithm. For
 * generating random number required by the algorithm, the PKCS11 module is used
 * as it allows access to a True Random Number Generator (TRNG) if the vendor
 * platform supports it.
 * It is RECOMMENDED to seed the random number generator with a device-specific
 * entropy source so that probability of collisions from devices in connection
 * retries is mitigated.
 */

/**
 * @file http_demo_s3_download.c
 * @brief Demonstrates usage of the HTTP library.
 */

/* Standard includes. */
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Include demo config as the first non-system header. */
#include "http_demo_s3_download_config.h"

/* Include common demo header. */
#include "aws_demo.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Transport interface implementation include header for TLS. */
#include "transport_secure_sockets.h"

/* Common HTTP demo utilities. */
#include "http_demo_utils.h"

/* HTTP API header. */
#include "core_http_client.h"

/*------------- Demo configurations -------------------------*/

/* Check that the TLS port of the server is defined. */
#ifndef democonfigHTTPS_PORT
    #error "Please define democonfigHTTPS_PORT in http_demo_s3_download_config.h."
#endif

/* Check that the root CA certificate is defined. */
#ifndef democonfigROOT_CA_PEM
    #error "Please define democonfigROOT_CA_PEM in http_demo_s3_download_config.h."
#endif

/* Check that the pre-signed GET URL is defined. */
#ifndef democonfigS3_PRESIGNED_GET_URL
    #define democonfigS3_PRESIGNED_GET_URL    "GET-URL"
#endif

/* Check that a transport timeout for the transport send and receive functions
 * is defined. */
#ifndef democonfigTRANSPORT_SEND_RECV_TIMEOUT_MS
    #define democonfigTRANSPORT_SEND_RECV_TIMEOUT_MS    ( 5000 )
#endif

/* Check that the size of the user buffer is defined. */
#ifndef democonfigUSER_BUFFER_LENGTH
    #define democonfigUSER_BUFFER_LENGTH    ( 4096 )
#endif

/* Check that the range request length is defined. */
#ifndef democonfigRANGE_REQUEST_LENGTH
    #define democonfigRANGE_REQUEST_LENGTH    ( 2048 )
#endif

/**
 * @brief Length of the pre-signed GET URL defined in demo_config.h.
 */
#define httpexampleS3_PRESIGNED_GET_URL_LENGTH               ( sizeof( democonfigS3_PRESIGNED_GET_URL ) - 1 )

/**
 * @brief The length of the HTTP GET method.
 */
#define httpexampleHTTP_METHOD_GET_LENGTH                    ( sizeof( HTTP_METHOD_GET ) - 1 )

/**
 * @brief Field name of the HTTP range header to read from server response.
 */
#define httpexampleHTTP_CONTENT_RANGE_HEADER_FIELD           "Content-Range"

/**
 * @brief Length of the HTTP range header field.
 */
#define httpexampleHTTP_CONTENT_RANGE_HEADER_FIELD_LENGTH    ( sizeof( httpexampleHTTP_CONTENT_RANGE_HEADER_FIELD ) - 1 )

/**
 * @brief The HTTP status code returned for partial content.
 */
#define httpexampleHTTP_STATUS_CODE_PARTIAL_CONTENT          206

/**
 * @brief The maximum number of times to run the loop in this demo.
 */
#ifndef httpexampleMAX_DEMO_COUNT
    #define httpexampleMAX_DEMO_COUNT    ( 3 )
#endif

/**
 * @brief Time in ticks to wait between each cycle of the demo implemented
 * by RunCoreHttpS3DownloadDemo().
 */
#define httpexampleDELAY_BETWEEN_DEMO_ITERATIONS_TICKS    ( pdMS_TO_TICKS( 5000U ) )

/*-----------------------------------------------------------*/

/**
 * @brief Each compilation unit that consumes the NetworkContext must define it.
 * It should contain a single pointer to the type of your desired transport.
 * When using multiple transports in the same compilation unit, define this pointer as void *.
 *
 * @note Transport stacks are defined in amazon-freertos/libraries/abstractions/transport/secure_sockets/transport_secure_sockets.h.
 */
struct NetworkContext
{
    SecureSocketsTransportParams_t * pParams;
};

/*-----------------------------------------------------------*/

/**
 * @brief A buffer used in the demo for storing HTTP request headers, and HTTP
 * response headers and body.
 *
 * @note This demo shows how the same buffer can be re-used for storing the HTTP
 * response after the HTTP request is sent out. However, the user can decide how
 * to use buffers to store HTTP requests and responses.
 */
static uint8_t ucUserBuffer[ democonfigUSER_BUFFER_LENGTH ];

/**
 * @brief Represents header data that will be sent in an HTTP request.
 */
static HTTPRequestHeaders_t xRequestHeaders;

/**
 * @brief Configurations of the initial request headers that are passed to
 * #HTTPClient_InitializeRequestHeaders.
 */
static HTTPRequestInfo_t xRequestInfo;

/**
 * @brief Represents a response returned from an HTTP server.
 */
static HTTPResponse_t xResponse;

/**
 * @brief The host address string extracted from the pre-signed URL.
 *
 * @note httpexampleS3_PRESIGNED_GET_URL_LENGTH is set as the array length here as the
 * length of the host name string cannot exceed this value.
 */
static char cServerHost[ httpexampleS3_PRESIGNED_GET_URL_LENGTH ];

/**
 * @brief The length of the host address found in the pre-signed URL.
 */
static size_t xServerHostLength;

/**
 * @brief The location of the path within the pre-signed URL.
 */
static const char * pcPath;

/*-----------------------------------------------------------*/

/**
 * @brief Connect to HTTP server with reconnection retries.
 *
 * @param[out] pxNetworkContext The output parameter to return the created
 * network context.
 *
 * @return pdFAIL on failure; pdPASS on successful connection.
 */
static BaseType_t prvConnectToServer( NetworkContext_t * pxNetworkContext );

/**
 * @brief Retrieve the size of the S3 object that is specified in pcPath.
 *
 * @param[out] pxFileSize The size of the S3 object.
 * @param[in] pxTransportInterface The transport interface for making network
 * calls.
 * @param[in] pcHost The server host address. This string must be
 * null-terminated.
 * @param[in] xHostLen The length of the server host address.
 * @param[in] pcPath The Request-URI to the objects of interest. This string
 * should be null-terminated.
 *
 * @return The status of the file size acquisition using a GET request to the
 * server: pdPASS on success, pdFAIL on failure.
 */
static BaseType_t prvGetS3ObjectFileSize( size_t * pxFileSize,
                                          const TransportInterface_t * pxTransportInterface,
                                          const char * pcHost,
                                          size_t xHostLen,
                                          const char * pcPath );

/**
 * @brief Send multiple HTTP GET requests, based on a specified path, to
 * download a file in chunks from the host S3 server.
 *
 * @param[in] pxTransportInterface The transport interface for making network
 * calls.
 * @param[in] pcPath The Request-URI to the objects of interest. This string
 * should be null-terminated.
 *
 * @return The status of the file download using multiple GET requests to the
 * server: pdPASS on success, pdFAIL on failure.
 */
static BaseType_t prvDownloadS3ObjectFile( const TransportInterface_t * pxTransportInterface,
                                           const char * pcPath );

/*-----------------------------------------------------------*/

static BaseType_t prvConnectToServer( NetworkContext_t * pxNetworkContext )
{
    ServerInfo_t xServerInfo = { 0 };
    SocketsConfig_t xSocketsConfig = { 0 };
    BaseType_t xStatus = pdPASS;
    TransportSocketStatus_t xNetworkStatus = TRANSPORT_SOCKET_STATUS_SUCCESS;
    HTTPStatus_t xHTTPStatus = HTTPSuccess;

    /* The location of the host address within the pre-signed URL. */
    const char * pcAddress = NULL;

    /* Retrieve the address location and length from democonfigS3_PRESIGNED_GET_URL. */
    xHTTPStatus = getUrlAddress( democonfigS3_PRESIGNED_GET_URL,
                                 httpexampleS3_PRESIGNED_GET_URL_LENGTH,
                                 &pcAddress,
                                 &xServerHostLength );

    xStatus = ( xHTTPStatus == HTTPSuccess ) ? pdPASS : pdFAIL;

    if( xStatus == pdPASS )
    {
        /* cServerHost should consist only of the host address located in
         * democonfigS3_PRESIGNED_GET_URL. */
        memcpy( cServerHost, pcAddress, xServerHostLength );
        cServerHost[ xServerHostLength ] = '\0';

        /* Initializer server information. */
        xServerInfo.pHostName = cServerHost;
        xServerInfo.hostNameLength = xServerHostLength;
        xServerInfo.port = democonfigHTTPS_PORT;

        /* Configure credentials for TLS server-authenticated session. */
        xSocketsConfig.enableTls = true;
        xSocketsConfig.pAlpnProtos = NULL;
        xSocketsConfig.maxFragmentLength = 0;
        xSocketsConfig.disableSni = false;
        xSocketsConfig.pRootCa = democonfigROOT_CA_PEM;
        xSocketsConfig.rootCaSize = sizeof( democonfigROOT_CA_PEM );
        xSocketsConfig.sendTimeoutMs = democonfigTRANSPORT_SEND_RECV_TIMEOUT_MS;
        xSocketsConfig.recvTimeoutMs = democonfigTRANSPORT_SEND_RECV_TIMEOUT_MS;

        /* Establish a TLS session with the HTTP server. This example connects
         * to the server host located in democonfigPRESIGNED_GET_URL and
         * democonfigHTTPS_PORT in demo_config.h. */
        LogInfo( ( "Establishing a TLS session with %s:%d.",
                   cServerHost,
                   democonfigHTTPS_PORT ) );

        /* Attempt to create a server-authenticated TLS connection. */
        xNetworkStatus = SecureSocketsTransport_Connect( pxNetworkContext,
                                                         &xServerInfo,
                                                         &xSocketsConfig );

        if( xNetworkStatus != TRANSPORT_SOCKET_STATUS_SUCCESS )
        {
            xStatus = pdFAIL;
        }
    }

    return xStatus;
}

/*-----------------------------------------------------------*/

static BaseType_t prvGetS3ObjectFileSize( size_t * pxFileSize,
                                          const TransportInterface_t * pxTransportInterface,
                                          const char * pcHost,
                                          size_t xHostLen,
                                          const char * pcPath )
{
    BaseType_t xStatus = pdPASS;
    HTTPStatus_t xHTTPStatus = HTTPSuccess;

    /* The location of the file size in pcContentRangeValStr. */
    char * pcFileSizeStr = NULL;

    /* String to store the Content-Range header value. */
    char * pcContentRangeValStr = NULL;
    size_t xContentRangeValStrLength = 0;

    configASSERT( pxFileSize != NULL );
    configASSERT( pcHost != NULL );
    configASSERT( pcPath != NULL );

    /* Initialize all HTTP Client library API structs to 0. */
    ( void ) memset( &xRequestHeaders, 0, sizeof( xRequestHeaders ) );
    ( void ) memset( &xRequestInfo, 0, sizeof( xRequestInfo ) );
    ( void ) memset( &xResponse, 0, sizeof( xResponse ) );

    /* Initialize the request object. */
    xRequestInfo.pHost = pcHost;
    xRequestInfo.hostLen = xHostLen;
    xRequestInfo.pMethod = HTTP_METHOD_GET;
    xRequestInfo.methodLen = sizeof( HTTP_METHOD_GET ) - 1;
    xRequestInfo.pPath = pcPath;
    xRequestInfo.pathLen = strlen( pcPath );

    /* Set "Connection" HTTP header to "keep-alive" so that multiple requests
     * can be sent over the same established TCP connection. This is done in
     * order to download the file in parts. */
    xRequestInfo.reqFlags = HTTP_REQUEST_KEEP_ALIVE_FLAG;

    /* Set the buffer used for storing request headers. */
    xRequestHeaders.pBuffer = ucUserBuffer;
    xRequestHeaders.bufferLen = democonfigUSER_BUFFER_LENGTH;

    /* Initialize the response object. The same buffer used for storing request
     * headers is reused here. */
    xResponse.pBuffer = ucUserBuffer;
    xResponse.bufferLen = democonfigUSER_BUFFER_LENGTH;

    LogInfo( ( "Getting file object size from host..." ) );

    xHTTPStatus = HTTPClient_InitializeRequestHeaders( &xRequestHeaders,
                                                       &xRequestInfo );

    if( xHTTPStatus != HTTPSuccess )
    {
        LogError( ( "Failed to initialize HTTP request headers: Error=%s.",
                    HTTPClient_strerror( xHTTPStatus ) ) );
        xStatus = pdFAIL;
    }

    if( xStatus == pdPASS )
    {
        /* Add the header to get bytes=0-0. S3 will respond with a Content-Range
         * header that contains the size of the file in it. This header will
         * look like: "Content-Range: bytes 0-0/FILESIZE". The body will have a
         * single byte that we are ignoring. */
        xHTTPStatus = HTTPClient_AddRangeHeader( &xRequestHeaders, 0, 0 );

        if( xHTTPStatus != HTTPSuccess )
        {
            LogError( ( "Failed to add range header to request headers: Error=%s.",
                        HTTPClient_strerror( xHTTPStatus ) ) );
            xStatus = pdFAIL;
        }
    }

    if( xStatus == pdPASS )
    {
        /* Send the request and receive the response. */
        xHTTPStatus = HTTPClient_Send( pxTransportInterface,
                                       &xRequestHeaders,
                                       NULL,
                                       0,
                                       &xResponse,
                                       0 );

        if( xHTTPStatus != HTTPSuccess )
        {
            LogError( ( "Failed to send HTTP GET request to %s%s: Error=%s.",
                        pcHost, pcPath, HTTPClient_strerror( xHTTPStatus ) ) );
            xStatus = pdFAIL;
        }
    }

    if( xStatus == pdPASS )
    {
        LogDebug( ( "Received HTTP response from %s%s...",
                    pcHost, pcPath ) );
        LogDebug( ( "Response Headers:\n%.*s",
                    ( int32_t ) xResponse.headersLen,
                    xResponse.pHeaders ) );
        LogDebug( ( "Response Body:\n%.*s\n",
                    ( int32_t ) xResponse.bodyLen,
                    xResponse.pBody ) );

        if( xResponse.statusCode != httpexampleHTTP_STATUS_CODE_PARTIAL_CONTENT )
        {
            LogError( ( "Received an invalid response from the server "
                        "(Status Code: %u).",
                        xResponse.statusCode ) );
            xStatus = pdFAIL;
        }
    }

    if( xStatus == pdPASS )
    {
        LogInfo( ( "Received successful response from server "
                   "(Status Code: %u).",
                   xResponse.statusCode ) );

        xHTTPStatus = HTTPClient_ReadHeader( &xResponse,
                                             ( char * ) httpexampleHTTP_CONTENT_RANGE_HEADER_FIELD,
                                             ( size_t ) httpexampleHTTP_CONTENT_RANGE_HEADER_FIELD_LENGTH,
                                             ( const char ** ) &pcContentRangeValStr,
                                             &xContentRangeValStrLength );

        if( xHTTPStatus != HTTPSuccess )
        {
            LogError( ( "Failed to read Content-Range header from HTTP response: Error=%s.",
                        HTTPClient_strerror( xHTTPStatus ) ) );
            xStatus = pdFAIL;
        }
    }

    /* Parse the Content-Range header value to get the file size. */
    if( xStatus == pdPASS )
    {
        pcFileSizeStr = strstr( pcContentRangeValStr, "/" );

        if( pcFileSizeStr == NULL )
        {
            LogError( ( "'/' not present in Content-Range header value: %s.",
                        pcContentRangeValStr ) );
            xStatus = pdFAIL;
        }
    }

    if( xStatus == pdPASS )
    {
        pcFileSizeStr += sizeof( char );
        *pxFileSize = ( size_t ) strtoul( pcFileSizeStr, NULL, 10 );

        if( ( *pxFileSize == 0 ) || ( *pxFileSize == UINT32_MAX ) )
        {
            LogError( ( "Error using strtoul to get the file size from %s: xFileSize=%d.",
                        pcFileSizeStr, ( int32_t ) *pxFileSize ) );
            xStatus = pdFAIL;
        }
    }

    if( xStatus == pdPASS )
    {
        LogInfo( ( "The file is %d bytes long.", ( int32_t ) *pxFileSize ) );
    }

    return xStatus;
}

/*-----------------------------------------------------------*/

static BaseType_t prvDownloadS3ObjectFile( const TransportInterface_t * pxTransportInterface,
                                           const char * pcPath )
{
    /* Return value of this method. */
    BaseType_t xStatus = pdFAIL;
    HTTPStatus_t xHTTPStatus = HTTPSuccess;

    /* The size of the file we are trying to download in S3. */
    size_t xFileSize = 0;

    /* The number of bytes we want to request with in each range of the file
     * bytes. */
    size_t xNumReqBytes = 0;
    /* xCurByte indicates which starting byte we want to download next. */
    size_t xCurByte = 0;

    configASSERT( pcPath != NULL );

    /* Initialize all HTTP Client library API structs to 0. */
    ( void ) memset( &xRequestHeaders, 0, sizeof( xRequestHeaders ) );
    ( void ) memset( &xRequestInfo, 0, sizeof( xRequestInfo ) );
    ( void ) memset( &xResponse, 0, sizeof( xResponse ) );

    /* Initialize the request object. */
    xRequestInfo.pHost = cServerHost;
    xRequestInfo.hostLen = xServerHostLength;
    xRequestInfo.pMethod = HTTP_METHOD_GET;
    xRequestInfo.methodLen = httpexampleHTTP_METHOD_GET_LENGTH;
    xRequestInfo.pPath = pcPath;
    xRequestInfo.pathLen = strlen( pcPath );

    /* Set "Connection" HTTP header to "keep-alive" so that multiple requests
     * can be sent over the same established TCP connection. This is done in
     * order to download the file in parts. */
    xRequestInfo.reqFlags = HTTP_REQUEST_KEEP_ALIVE_FLAG;

    /* Set the buffer used for storing request headers. */
    xRequestHeaders.pBuffer = ucUserBuffer;
    xRequestHeaders.bufferLen = democonfigUSER_BUFFER_LENGTH;

    /* Initialize the response object. The same buffer used for storing request
     * headers is reused here. */
    xResponse.pBuffer = ucUserBuffer;
    xResponse.bufferLen = democonfigUSER_BUFFER_LENGTH;

    /* Verify the file exists by retrieving the file size. */
    xStatus = prvGetS3ObjectFileSize( &xFileSize,
                                      pxTransportInterface,
                                      cServerHost,
                                      xServerHostLength,
                                      pcPath );

    /* Set the number of bytes to request in each iteration, defined by the user
     * in democonfigRANGE_REQUEST_LENGTH. */
    if( xFileSize < democonfigRANGE_REQUEST_LENGTH )
    {
        xNumReqBytes = xFileSize;
    }
    else
    {
        xNumReqBytes = democonfigRANGE_REQUEST_LENGTH;
    }

    /* Here we iterate sending byte range requests until the full file has been
     * downloaded. We keep track of the next byte to download with xCurByte, and
     * increment by xNumReqBytes after each iteration. When xCurByte reaches
     * xFileSize, we stop downloading. */
    while( ( xStatus == pdPASS ) && ( xHTTPStatus == HTTPSuccess ) && ( xCurByte < xFileSize ) )
    {
        xHTTPStatus = HTTPClient_InitializeRequestHeaders( &xRequestHeaders,
                                                           &xRequestInfo );

        if( xHTTPStatus == HTTPSuccess )
        {
            xHTTPStatus = HTTPClient_AddRangeHeader( &xRequestHeaders,
                                                     xCurByte,
                                                     xCurByte + xNumReqBytes - 1 );
        }
        else
        {
            LogError( ( "Failed to initialize HTTP request headers: Error=%s.",
                        HTTPClient_strerror( xHTTPStatus ) ) );
        }

        if( xHTTPStatus == HTTPSuccess )
        {
            LogInfo( ( "Downloading bytes %d-%d, out of %d total bytes, from %s...:  ",
                       ( int32_t ) ( xCurByte ),
                       ( int32_t ) ( xCurByte + xNumReqBytes - 1 ),
                       ( int32_t ) xFileSize,
                       cServerHost ) );
            LogDebug( ( "Request Headers:\n%.*s",
                        ( int32_t ) xRequestHeaders.headersLen,
                        ( char * ) xRequestHeaders.pBuffer ) );
            xHTTPStatus = HTTPClient_Send( pxTransportInterface,
                                           &xRequestHeaders,
                                           NULL,
                                           0,
                                           &xResponse,
                                           0 );
        }
        else
        {
            LogError( ( "Failed to add Range header to request headers: Error=%s.",
                        HTTPClient_strerror( xHTTPStatus ) ) );
        }

        if( xHTTPStatus == HTTPSuccess )
        {
            LogDebug( ( "Received HTTP response from %s%s...",
                        cServerHost, pcPath ) );
            LogDebug( ( "Response Headers:\n%.*s",
                        ( int32_t ) xResponse.headersLen,
                        xResponse.pHeaders ) );
            LogInfo( ( "Response Body:\n%.*s\n",
                       ( int32_t ) xResponse.bodyLen,
                       xResponse.pBody ) );

            /* We increment by the content length because the server may not
             * have sent us the range we requested. */
            xCurByte += xResponse.contentLength;

            if( ( xFileSize - xCurByte ) < xNumReqBytes )
            {
                xNumReqBytes = xFileSize - xCurByte;
            }

            xStatus = ( xResponse.statusCode == httpexampleHTTP_STATUS_CODE_PARTIAL_CONTENT ) ? pdPASS : pdFAIL;
        }
        else
        {
            LogError( ( "An error occurred in downloading the file. "
                        "Failed to send HTTP GET request to %s%s: Error=%s.",
                        cServerHost, pcPath, HTTPClient_strerror( xHTTPStatus ) ) );
        }

        if( xStatus != pdPASS )
        {
            LogError( ( "Received an invalid response from the server "
                        "(Status Code: %u).",
                        xResponse.statusCode ) );
        }
    }

    return( ( xStatus == pdPASS ) && ( xHTTPStatus == HTTPSuccess ) );
}

/*-----------------------------------------------------------*/

/**
 * @brief Entry point of the demo.
 *
 * This example, using a pre-signed URL,  resolves a S3 domain, establishes a
 * TCP connection, validates the server's certificate using the root CA
 * certificate defined in the config header, then finally performs a TLS
 * handshake with the HTTP server so that all communication is encrypted. After
 * which, the HTTP Client library API is used to download the S3 file (by
 * sending multiple GET requests, filling up the response buffer each time until
 * all parts are downloaded). If any request fails, an error code is returned.
 *
 * @note This example is single-threaded and uses statically allocated memory.
 *
 */
int RunCoreHttpS3DownloadDemo( bool awsIotMqttMode,
                               const char * pIdentifier,
                               void * pNetworkServerInfo,
                               void * pNetworkCredentialInfo,
                               const IotNetworkInterface_t * pNetworkInterface )
{
    /* The transport layer interface used by the HTTP Client library. */
    TransportInterface_t xTransportInterface;
    /* The network context for the transport layer interface. */
    NetworkContext_t xNetworkContext = { 0 };
    TransportSocketStatus_t xNetworkStatus;
    BaseType_t xIsConnectionEstablished = pdFALSE;
    /* HTTPS Client library return status. */
    HTTPStatus_t xHTTPStatus = HTTPSuccess;
    BaseType_t xDemoRunCount = 0UL;
    SecureSocketsTransportParams_t secureSocketsTransportParams = { 0 };

    /* Upon return, pdPASS will indicate a successful demo execution.
    * pdFAIL will indicate some failures occurred during execution. The
    * user of this demo must check the logs for any failure codes. */
    BaseType_t xDemoStatus = pdPASS;

    /* Remove compiler warnings about unused parameters. */
    ( void ) awsIotMqttMode;
    ( void ) pIdentifier;
    ( void ) pNetworkServerInfo;
    ( void ) pNetworkCredentialInfo;
    ( void ) pNetworkInterface;

    /* The length of the path within the pre-signed URL. This variable is
     * defined in order to store the length returned from parsing the URL, but
     * it is unused. The path used for the requests in this demo needs all the
     * query information following the location of the object, to the end of the
     * S3 presigned URL. */
    size_t xPathLen = 0;
    xNetworkContext.pParams = &secureSocketsTransportParams;
    LogInfo( ( "HTTP Client Synchronous S3 download demo using pre-signed URL:\n%s",
               democonfigS3_PRESIGNED_GET_URL ) );

    do
    {
        /**************************** Connect. ******************************/

        /* Establish TLS connection on top of TCP connection using Secure Sockets. */

        /* Attempt to connect to S3. If connection fails, retry after a timeout.
         * The timeout value will be exponentially increased until either the
         * maximum number of attempts or the maximum timeout value is reached.
         * The function returns pdFAIL if a TCP connection with the server
         * cannot be established  after the configured number of attempts. */
        xDemoStatus = connectToServerWithBackoffRetries( prvConnectToServer,
                                                         &xNetworkContext );

        if( xDemoStatus == pdFAIL )
        {
            /* Log an error to indicate connection failure after all
             * reconnect attempts are over. */
            LogError( ( "Failed to connect to HTTP server %s.",
                        cServerHost ) );
        }

        /* Define the transport interface. */
        if( xDemoStatus == pdPASS )
        {
            /* Set a flag indicating that a TLS connection exists. */
            xIsConnectionEstablished = pdTRUE;

            /* Define the transport interface. */
            xTransportInterface.pNetworkContext = &xNetworkContext;
            xTransportInterface.send = SecureSocketsTransport_Send;
            xTransportInterface.recv = SecureSocketsTransport_Recv;
        }

        /******************** Download S3 Object File. **********************/

        if( xDemoStatus == pdPASS )
        {
            /* Retrieve the path location from democonfigS3_PRESIGNED_GET_URL. This
             * function returns the length of the path without the query into
             * xPathLen, which is left unused in this demo. */
            xHTTPStatus = getUrlPath( democonfigS3_PRESIGNED_GET_URL,
                                      httpexampleS3_PRESIGNED_GET_URL_LENGTH,
                                      &pcPath,
                                      &xPathLen );

            xDemoStatus = ( xHTTPStatus == HTTPSuccess ) ? pdPASS : pdFAIL;
        }

        if( xDemoStatus == pdPASS )
        {
            xDemoStatus = prvDownloadS3ObjectFile( &xTransportInterface,
                                                   pcPath );
        }

        /************************** Disconnect. *****************************/

        /* Close the network connection to clean up any system resources that the
         * demo may have consumed. */
        if( xIsConnectionEstablished == pdTRUE )
        {
            /* Close the network connection.  */
            xNetworkStatus = SecureSocketsTransport_Disconnect( &xNetworkContext );

            if( xNetworkStatus != TRANSPORT_SOCKET_STATUS_SUCCESS )
            {
                xDemoStatus = pdFAIL;
                LogError( ( "SecureSocketsTransport_Disconnect() failed to close the network connection. "
                            "StatusCode=%d.", ( int ) xNetworkStatus ) );
            }
        }

        /* Increment the demo run count. */
        xDemoRunCount++;

        if( xDemoStatus == pdPASS )
        {
            LogInfo( ( "Demo iteration %lu is successful.", xDemoRunCount ) );
        }
        /* Attempt to retry a failed iteration of demo for up to #httpexampleMAX_DEMO_COUNT times. */
        else if( xDemoRunCount < httpexampleMAX_DEMO_COUNT )
        {
            LogWarn( ( "Demo iteration %lu failed. Retrying...", xDemoRunCount ) );
            vTaskDelay( httpexampleDELAY_BETWEEN_DEMO_ITERATIONS_TICKS );
        }
        /* Failed all #httpexampleMAX_DEMO_COUNT demo iterations. */
        else
        {
            LogError( ( "All %d demo iterations failed.", httpexampleMAX_DEMO_COUNT ) );
            break;
        }
    } while( xDemoStatus != pdPASS );

    if( xDemoStatus == pdPASS )
    {
        LogInfo( ( "Demo completed successfully." ) );
    }

    return ( xDemoStatus == pdPASS ) ? EXIT_SUCCESS : EXIT_FAILURE;
}
