/*
 * FreeRTOS PKCS #11 PAL for Renesas Starter Kit+ for RX65N-2MB V1.0.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/**
 * @file core_pkcs11_pal.c
 * @brief Device specific helpers for PKCS11 Interface.
 */

/* Amazon FreeRTOS Includes. */
#include "core_pkcs11.h"
#include "core_pkcs11_pal.h"
#include "iot_crypto.h"
#include "core_pkcs11_config.h"
#include "FreeRTOS.h"
#include "mbedtls/sha256.h"

/* C runtime includes. */
#include <stdio.h>
#include <string.h>

/* Renesas RX platform includes */
#include "platform.h"
#include "r_flash_rx_if.h"

/* RX72N Envision Kit system header include */
#include "rx72n_envision_kit_system.h"

//#define PKCS11_PAL_DEBUG_PRINT( X )      /* configPRINTF( X ) */
#define PKCS11_PAL_DEBUG_PRINT( X )

enum eObjectHandles
{
    eInvalidHandle = 0, /* According to PKCS #11 spec, 0 is never a valid object handle. */
    eAwsDevicePrivateKey = 1,
    eAwsDevicePublicKey,
    eAwsDeviceCertificate,
    eAwsCodeSigningKey,
    //eAwsRootCertificate
};

void prvPkcsHandleToLabel( char ** pcLabel, CK_OBJECT_HANDLE xHandle );
void prvPkcsHandleToSfdHandle( SFD_HANDLE * sfd_handle, CK_OBJECT_HANDLE xHandle);
void prvLabelToPkcsHandle( CK_OBJECT_HANDLE * xHandle, char * pcLabel);

CK_RV PKCS11_PAL_Initialize( void )
{
    CK_RV xResult = CKR_OK;

    if(SFD_SUCCESS != R_SFD_Open())
    {
        xResult = CKR_GENERAL_ERROR;
    }

    return xResult;
}

/**
* @brief Writes a file to local storage.
*
* Port-specific file write for crytographic information.
*
* @param[in] pxLabel       Label of the object to be saved.
* @param[in] pucData       Data buffer to be written to file
* @param[in] ulDataSize    Size (in bytes) of data to be saved.
*
* @return The file handle of the object that was stored.
*/
CK_OBJECT_HANDLE PKCS11_PAL_SaveObject( CK_ATTRIBUTE_PTR pxLabel,
                                        CK_BYTE_PTR pucData,
                                        CK_ULONG ulDataSize )
{
    CK_OBJECT_HANDLE pkcs_handle;
    if(SFD_HANDLE_INVALID != R_SFD_SaveObject((uint8_t *)pxLabel->pValue, pxLabel->ulValueLen, (uint8_t *)pucData, ulDataSize))
    {
        prvLabelToPkcsHandle(&pkcs_handle, pxLabel->pValue);
    }
    else
    {
        pkcs_handle = NULL;
    }
    return pkcs_handle;
}

/**
* @brief Translates a PKCS #11 label into an object handle.
*
* Port-specific object handle retrieval.
*
*
* @param[in] pLabel         Pointer to the label of the object
*                           who's handle should be found.
* @param[in] usLength       The length of the label, in bytes.
*
* @return The object handle if operation was successful.
* Returns eInvalidHandle if unsuccessful.
*/
CK_OBJECT_HANDLE PKCS11_PAL_FindObject( CK_BYTE_PTR pLabel,
                                        CK_ULONG usLength )
{
    CK_OBJECT_HANDLE pkcs_handle;
    uint8_t * ppucData;
    uint32_t pulDataSize;
    sfd_err_t sfd_err;
    SFD_HANDLE sfd_handle;

    sfd_handle = R_SFD_FindObject((uint8_t *)pLabel, usLength);
    if(SFD_HANDLE_INVALID != sfd_handle)
    {
        prvLabelToPkcsHandle(&pkcs_handle, (char *)pLabel);
        sfd_err = R_SFD_GetObjectValue(sfd_handle, &ppucData, &pulDataSize);
        /* Zeroed out object means it has been destroyed. */
        if( ( sfd_err != SFD_SUCCESS ) || ( ppucData[ 0 ] == 0x00 ) )
        {
            pkcs_handle = eInvalidHandle;
        }
    }
    else
    {
        pkcs_handle = NULL;
    }
    return pkcs_handle;
}

/**
* @brief Gets the value of an object in storage, by handle.
*
* Port-specific file access for cryptographic information.
*
* This call dynamically allocates the buffer which object value
* data is copied into.  PKCS11_PAL_GetObjectValueCleanup()
* should be called after each use to free the dynamically allocated
* buffer.
*
* @sa PKCS11_PAL_GetObjectValueCleanup
*
* @param[in] pcFileName    The name of the file to be read.
* @param[out] ppucData     Pointer to buffer for file data.
* @param[out] pulDataSize  Size (in bytes) of data located in file.
* @param[out] pIsPrivate   Boolean indicating if value is private (CK_TRUE)
*                          or exportable (CK_FALSE)
*
* @return CKR_OK if operation was successful.  CKR_KEY_HANDLE_INVALID if
* no such object handle was found, CKR_DEVICE_MEMORY if memory for
* buffer could not be allocated, CKR_FUNCTION_FAILED for device driver
* error.
*/
CK_RV PKCS11_PAL_GetObjectValue( CK_OBJECT_HANDLE xHandle,
                                 uint8_t ** ppucData,
                                 uint32_t * pulDataSize,
                                 CK_BBOOL * pIsPrivate )
{
    CK_RV xReturn = CKR_FUNCTION_FAILED;
    char * pcLabel;
    SFD_HANDLE sfd_handle;

    prvPkcsHandleToSfdHandle(&sfd_handle, xHandle);
    R_SFD_GetObjectValue(sfd_handle, ppucData, pulDataSize);

    if (xHandle == eAwsDevicePrivateKey)
    {
        *pIsPrivate = CK_TRUE;
    }
    else
    {
        *pIsPrivate = CK_FALSE;
    }
    xReturn = CKR_OK;
    return xReturn;
}


/**
* @brief Cleanup after PKCS11_GetObjectValue().
*
* @param[in] pucData       The buffer to free.
*                          (*ppucData from PKCS11_PAL_GetObjectValue())
* @param[in] ulDataSize    The length of the buffer to free.
*                          (*pulDataSize from PKCS11_PAL_GetObjectValue())
*/
void PKCS11_PAL_GetObjectValueCleanup( uint8_t * pucData,
                                       uint32_t ulDataSize )
{
    /* todo: nothing to do in now. Now, pkcs_data exists as static. I will fix this function when I will port this to heap memory. (Renesas/Ishiguro) */
}

/* Converts a handle to its respective label. */
void prvPkcsHandleToLabel( char ** pcLabel,
                       CK_OBJECT_HANDLE xHandle )
{
    if( pcLabel != NULL )
    {
        switch( xHandle )
        {
            case eAwsDeviceCertificate:
                *pcLabel = ( char * ) pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS;
                break;

            case eAwsDevicePrivateKey:
                *pcLabel = ( char * ) pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS;
                break;

            case eAwsDevicePublicKey:
                *pcLabel = ( char * ) pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS;
                break;

            case eAwsCodeSigningKey:
                *pcLabel = ( char * ) pkcs11configLABEL_CODE_VERIFICATION_KEY;
                break;

            default:
                *pcLabel = NULL;
                break;
        }
    }
}

/* Converts a handle to its respective label. */
void prvLabelToPkcsHandle( CK_OBJECT_HANDLE * xHandle,
                       char * pcLabel)
{
    if(!strcmp(pcLabel, pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS))
    {
        *xHandle = eAwsDeviceCertificate;
    }
    else if(!strcmp(pcLabel, pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS))
    {
        *xHandle = eAwsDevicePrivateKey;
    }
    else if(!strcmp(pcLabel, pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS))
    {
        *xHandle = eAwsDevicePublicKey;
    }
    else if(!strcmp(pcLabel, pkcs11configLABEL_CODE_VERIFICATION_KEY))
    {
        *xHandle = eAwsCodeSigningKey;
    }
    else
    {
        *xHandle = NULL;
    }
}

/* Converts a pkcs handle to its respective sfd handle. */
void prvPkcsHandleToSfdHandle( SFD_HANDLE * sfd_handle,
                       CK_OBJECT_HANDLE xHandle)
{
    char * pcLabel;

    if(xHandle == eAwsDevicePrivateKey)
    {
        prvPkcsHandleToLabel(&pcLabel, eAwsDevicePrivateKey);
        *sfd_handle = R_SFD_FindObject((uint8_t *)pcLabel, strlen(pcLabel));
    }
    else if(xHandle == eAwsDevicePublicKey)
    {
        prvPkcsHandleToLabel(&pcLabel, eAwsDevicePublicKey);
        *sfd_handle = R_SFD_FindObject((uint8_t *)pcLabel, strlen(pcLabel));
    }
    else if(xHandle == eAwsDeviceCertificate)
    {
        prvPkcsHandleToLabel(&pcLabel, eAwsDeviceCertificate);
        *sfd_handle = R_SFD_FindObject((uint8_t *)pcLabel, strlen(pcLabel));
    }
    else if(xHandle == eAwsCodeSigningKey)
    {
        prvPkcsHandleToLabel(&pcLabel, eAwsCodeSigningKey);
        *sfd_handle = R_SFD_FindObject((uint8_t *)pcLabel, strlen(pcLabel));
    }
#if 0
    else if(xHandle == eAwsRootCertificate)
    {
        prvPkcsHandleToLabel(&pcLabel, eAwsRootCertificate);
        *sfd_handle = R_SFD_FindObject(pcLabel, strlen(pcLabel));
    }
#endif
    else
    {
        *sfd_handle = NULL;
    }
}

CK_RV PKCS11_PAL_DestroyObject( CK_OBJECT_HANDLE xHandle )
{
    CK_RV xResult = CKR_OK;
    CK_BYTE_PTR pxZeroedData = NULL;
    CK_BYTE_PTR pxObject = NULL;
    CK_BBOOL xIsPrivate = ( CK_BBOOL ) CK_TRUE;
    CK_OBJECT_HANDLE xPalHandle2 = CK_INVALID_HANDLE;
    CK_ULONG ulObjectLength = sizeof( CK_BYTE );
    char * pcLabel = NULL;
    CK_ATTRIBUTE xLabel;

    prvPkcsHandleToLabel( &pcLabel, xHandle );

    if( pcLabel != NULL )
    {
        xLabel.type = CKA_LABEL;
        xLabel.pValue = pcLabel;
        xLabel.ulValueLen = strlen( pcLabel );

        xResult = PKCS11_PAL_GetObjectValue( xHandle, &pxObject, &ulObjectLength, &xIsPrivate );
    }   
    else
    {
        xResult = CKR_OBJECT_HANDLE_INVALID;
    }   

    if( xResult == CKR_OK )
    {
        /* Some ports return a pointer to memory for which using memset directly won't work. */
        pxZeroedData = pvPortMalloc( ulObjectLength );

        if( NULL != pxZeroedData )
        {
            /* Zero out the object. */
            ( void ) memset( pxZeroedData, 0x0, ulObjectLength );

            /* Overwrite the object in NVM with zeros. */
            xPalHandle2 = PKCS11_PAL_SaveObject( &xLabel, pxZeroedData, ( size_t ) ulObjectLength );

            if( xPalHandle2 != xHandle )
            {
                xResult = CKR_GENERAL_ERROR;
            }

            vPortFree( pxZeroedData );
        }
        else
        {
            xResult = CKR_HOST_MEMORY;
        }

        PKCS11_PAL_GetObjectValueCleanup( pxObject, ulObjectLength );
    }
    else
    {
        xResult = CKR_GENERAL_ERROR;
    }

    return xResult;
}
