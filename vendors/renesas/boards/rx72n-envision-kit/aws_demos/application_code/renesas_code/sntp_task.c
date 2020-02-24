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
 * File Name    : sntp_task.c
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

/* TCP/IP abstraction includes. */
#include "iot_secure_sockets.h"
#include "platform/iot_network.h"

/* for RX72N Envision Kit system common header */
#include "rx72n_envision_kit_system.h"

/* for RX Family FIT */
#include "r_sys_time_rx_if.h"

#define RECEIVE_BUFFER_LENGTH 1500

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/
extern SYS_TIME sys_time;

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
void sntp_task( void * pvParameters );

/* Rx and Tx time outs are used to ensure the sockets do not wait too long for
 * missing data. */
static const TickType_t xReceiveTimeOut = pdMS_TO_TICKS( 2000 );
static const TickType_t xSendTimeOut = pdMS_TO_TICKS( 2000 );

/* NICT time server via HTTP: http://ntp-a1.nict.go.jp/cgi-bin/jst */
const char nics_time_server_host_name[] = "ntp-a1.nict.go.jp";
const char time_reqest[] = {
		"GET /cgi-bin/jst HTTP/1.1\r\n"\
		"Host: ntp-a1.nict.go.jp\r\n"\
		"Connection: keep-alive\r\n"\
		"\r\n"};

void sntp_task( void * pvParameters )
{
	TASK_INFO *task_info = (WM_HWIN *)pvParameters;
	Socket_t xSocket;
	uint32_t nics_time_server_ip_address;
	SocketsSockaddr_t xTimeServerAddress;
	uint32_t *receive_buffer;
	uint32_t unix_time;
	char *s_ptr, *s_ptr2;

    while(1)
    {
        /* We should wait for the network to be up before getting time. */
        while( FreeRTOS_IsNetworkUp() == pdFALSE )
        {
            vTaskDelay(300);
        }

        /* Connect to the echo server. */
    	nics_time_server_ip_address = FreeRTOS_gethostbyname(nics_time_server_host_name);
        configPRINTF( ( "Resolving NTP server host name(www.nict.go.jp): OK.\r\n" ) );

        /* Create the socket. */
        xSocket = SOCKETS_Socket( SOCKETS_AF_INET,
                                    SOCKETS_SOCK_STREAM,
                                    SOCKETS_IPPROTO_TCP );
        configASSERT( xSocket != SOCKETS_INVALID_SOCKET );

        /* Set a time out so a missing reply does not cause the task to block
         * indefinitely. */
        SOCKETS_SetSockOpt( xSocket, 0, SOCKETS_SO_RCVTIMEO, &xReceiveTimeOut, sizeof( xReceiveTimeOut ) );
        SOCKETS_SetSockOpt( xSocket, 0, SOCKETS_SO_SNDTIMEO, &xSendTimeOut, sizeof( xSendTimeOut ) );

        xTimeServerAddress.usPort = SOCKETS_htons( 80 );
        xTimeServerAddress.ulAddress = nics_time_server_ip_address;

        if( SOCKETS_Connect( xSocket, &xTimeServerAddress, sizeof( xTimeServerAddress ) ) == 0 )
        {
            configPRINTF( ( "Connecting NTP server: OK.\r\n" ) );
            /* Send the string to the socket. */
            SOCKETS_Send( xSocket,                /* The socket being sent to. */
                          ( void * ) time_reqest, /* The data being sent. */
						  strlen(time_reqest),    /* The length of the data being sent. */
                          0 );                    /* No flags. */
            receive_buffer = pvPortMalloc(RECEIVE_BUFFER_LENGTH);
            SOCKETS_Recv( xSocket,                /* The socket being received from. */
            		      receive_buffer,         /* The buffer into which the received data will be written. */
						  RECEIVE_BUFFER_LENGTH,  /* The size of the buffer provided to receive the data. */
                          0 );                    /* No flags. */

			s_ptr = strstr((char*)receive_buffer, "<BODY>");
			if(s_ptr)
			{
				s_ptr = strstr(s_ptr, "\n");
				if(s_ptr)
				{
					s_ptr += 1;
					s_ptr2 = strstr(s_ptr, ".");
					*s_ptr2 = '\0';
		            sscanf(s_ptr, "%d", &unix_time);
		            R_SYS_TIME_ConvertUnixTimeToSystemTime(unix_time, &sys_time, SYS_TIME_UTC_PLUS_0900);
		            R_SYS_TIME_SetCurrentTime(&sys_time);
				}
			}
            vPortFree(receive_buffer);
        }
        else
        {
        	configPRINTF( ( "Connecting NTP server: NG.\r\n" ) );
        }
        configPRINTF( ( "Shutting down connection to time server.\r\n" ) );
        SOCKETS_Shutdown( xSocket, SOCKETS_SHUT_RDWR );

        /* execute every 60 minutes */
    	vTaskDelay(60 * 60 * 1000);
    }
}
