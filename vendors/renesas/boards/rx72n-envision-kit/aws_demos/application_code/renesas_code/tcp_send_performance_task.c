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
 * File Name    : tcp_send_performance_task.c
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

/* for RX Family FIT */
#include "r_sys_time_rx_if.h"

#define SEND_DATA_UNIT_LENGTH 1460 * 3

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
void tcp_send_performance_task( void * pvParameters );

static uint8_t send_buffer[SEND_DATA_UNIT_LENGTH];

void tcp_send_performance_task( void * pvParameters )
{
	Socket_t xSocket;
	uint32_t iperf_server_ip_address;
	struct freertos_sockaddr xIperfServerAddress;
	BaseType_t return_value;

    while(1)
    {
        /* We should wait for the network to be up before getting time. */
        while( FreeRTOS_IsNetworkUp() == pdFALSE )
        {
            vTaskDelay(300);
        }

        /* Create a socket. */
        xSocket = FreeRTOS_socket( FREERTOS_AF_INET,
                                   FREERTOS_SOCK_STREAM,/* FREERTOS_SOCK_STREAM for TCP. */
                                   FREERTOS_IPPROTO_TCP );
        configASSERT( xSocket != FREERTOS_INVALID_SOCKET );

        /* Connect to the iperf server. */
        iperf_server_ip_address = FreeRTOS_inet_addr_quick( 192, 168, 1, 6 );
        xIperfServerAddress.sin_port = FreeRTOS_htons( 5001 );
        xIperfServerAddress.sin_addr = iperf_server_ip_address;

        if( FreeRTOS_connect( xSocket, &xIperfServerAddress, sizeof( xIperfServerAddress ) ) == 0 )
        {
            configPRINTF( ( "Connecting iperf server: OK.\r\n" ) );

            while(1)
            {
				/* Send the string to the socket. */
            	return_value = FreeRTOS_send( xSocket, /* The socket being sent to. */
							  ( void * ) send_buffer,  /* The data being sent. */
							  SEND_DATA_UNIT_LENGTH,   /* The length of the data being sent. */
							  0 );                     /* No flags. */
				if(0 > return_value)
				{
					break;
				}
            }
        }
        else
        {
        	configPRINTF( ( "Connecting iperf server: NG.\r\n" ) );
        }
        configPRINTF( ( "Shutting down connection to iperf server.\r\n" ) );
        SOCKETS_Shutdown( xSocket, SOCKETS_SHUT_RDWR );

        /* finish */
        while(1)
        {
        	vTaskDelay(0xffffffff);
        }
    }
}
