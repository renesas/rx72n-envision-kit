/**********************************************************************************************************************
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
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : tcp_receive_performance_task.c
 * Description  :
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 31.01.2020 1.00 First Release
 *********************************************************************************************************************/

#include <stdio.h>

/* for using C standard library */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* for using Segger emWin */
#include "GUI.h"
#include "DIALOG.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_DNS.h"
#include "FreeRTOS_sockets.h"

/* TCP/IP abstraction includes. */
#include "iot_secure_sockets.h"
#include "platform/iot_network.h"

/* for RX72N Envision Kit system common header */
#include "rx72n_envision_kit_system.h"
#include "r_simple_filesystem_on_dataflash_if.h"

/* for RX Family FIT */
#include "r_sys_time_rx_if.h"

#define RECEIVE_DATA_UNIT_LENGTH 1460 * 3

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
void tcp_receive_performance_task( void * pvParameters );

static uint8_t receive_buffer[RECEIVE_DATA_UNIT_LENGTH];

void tcp_receive_performance_task( void * pvParameters )
{
    Socket_t xListeningSocket, xConnectedSocket;
    SocketsSockaddr_t xTimeServerAddress;
    struct freertos_sockaddr xClient, xBindAddress;
    const BaseType_t xBacklog = 20;
    socklen_t xSize = sizeof( xClient );
    static const TickType_t xReceiveTimeOut = portMAX_DELAY;
    BaseType_t return_value;

    while(1)
    {
        /* We should wait for the network to be up before getting time. */
        while( FreeRTOS_IsNetworkUp() == pdFALSE )
        {
            vTaskDelay(300);
        }

        /* Create the socket. */
        xListeningSocket = FreeRTOS_socket( FREERTOS_AF_INET,
                                    FREERTOS_SOCK_STREAM,
                                    FREERTOS_IPPROTO_TCP );
        configASSERT( xListeningSocket != FREERTOS_INVALID_SOCKET );

        /* Set a time out so accept() will just wait for a connection. */
        FreeRTOS_setsockopt( xListeningSocket,
                             0,
                             FREERTOS_SO_RCVTIMEO,
                             &xReceiveTimeOut,
                             sizeof( xReceiveTimeOut ) );

        /* Wait connect from the iperf client. */
        xBindAddress.sin_port = ( uint16_t ) 5001;
        xBindAddress.sin_port = FreeRTOS_htons( xBindAddress.sin_port );

        /* Bind the socket to the port that the client RTOS task will send to. */
        FreeRTOS_bind( xListeningSocket, &xBindAddress, sizeof( xBindAddress ) );

        /* Set the socket into a listening state so it can accept connections.
        The maximum number of simultaneous connections is limited to 20. */
        FreeRTOS_listen( xListeningSocket, xBacklog );

        /* Wait for incoming connections. */
        xConnectedSocket = FreeRTOS_accept( xListeningSocket, &xClient, &xSize );

        configPRINTF( ( "Connected from iperf client: OK.\r\n" ) );

        while(1)
        {
            /* Receive the string from the socket. */
            return_value = FreeRTOS_recv( xConnectedSocket, /* The socket being received to. */
                          ( void * ) receive_buffer,        /* The data being received. */
                          RECEIVE_DATA_UNIT_LENGTH,         /* The length of the data being received. */
                          0 );                              /* No flags. */
            if(0 > return_value)
            {
                break;
            }
        }
        configPRINTF( ( "Shutting down connection from iperf client.\r\n" ) );
        SOCKETS_Shutdown( xConnectedSocket, SOCKETS_SHUT_RDWR );

        /* finish */
        while(1)
        {
            vTaskDelay(0xffffffff);
        }
    }
}
