
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
 * Copyright (C) 2014(2020) Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : main_task.c
 * Description  : main task
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 29.12.2019 1.00 First Release
 *********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
/* for using C standard library */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* for using FIT Module */
#include "platform.h"
#include "r_sci_rx_if.h"

/* for using Segger emWin */
#include "GUI.h"
#include "DIALOG.h"

/* for using Amazon FreeRTOS */
#include "FreeRTOS.h"
#include "aws_application_version.h"

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/
#define PROMPT "RX72N Envision Kit\r\n$ "

#define SERIAL_BUFFER_QUEUE_NUMBER 1024
#define SERIAL_BUFFER_SIZE 1

/******************************************************************************
 External variables
 ******************************************************************************/

/******************************************************************************
 Private global variables
 ******************************************************************************/
static void display_serial_terminal_putstring_with_uart(WM_HWIN hWin_handle, sci_hdl_t sci_handle, char *string);
static void sci_callback(void *pArgs);

static sci_hdl_t sci_handle;
static QueueHandle_t xQueue;

/******************************************************************************
 External functions
 ******************************************************************************/
extern void display_serial_terminal_putstring(WM_HWIN hWin_handle, char *string);

/*******************************************************************************
 global variables and functions
********************************************************************************/
void serial_terminal_task( void * pvParameters );

/******************************************************************************
 Function Name   : serial_terminal_task
 Description     : serial_terminal_task
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void serial_terminal_task( void * pvParameters )
{
    sci_cfg_t   sci_config;
    sci_err_t   sci_err;
    char tmp[2];
    tmp[1] = 0;

	WM_HWIN hWinSerialTerminalindow = (*(WM_HWIN *)pvParameters);

    /* Set up the configuration data structure for asynchronous (UART) operation. */
    sci_config.async.baud_rate    = 115200;
    sci_config.async.clk_src      = SCI_CLK_INT;
    sci_config.async.data_size    = SCI_DATA_8BIT;
    sci_config.async.parity_en    = SCI_PARITY_OFF;
    sci_config.async.parity_type  = SCI_EVEN_PARITY;
    sci_config.async.stop_bits    = SCI_STOPBITS_1;
    sci_config.async.int_priority = 15;    // 1=lowest, 15=highest
    sci_err = R_SCI_Open(SCI_CH2, SCI_MODE_ASYNC, &sci_config, sci_callback, &sci_handle);

    /* create queue */
	xQueue = xQueueCreate(SERIAL_BUFFER_QUEUE_NUMBER, SERIAL_BUFFER_SIZE);

	while(1)
	{
	    display_serial_terminal_putstring_with_uart(hWinSerialTerminalindow, sci_handle, PROMPT);
		xQueueReceive(xQueue, &tmp, portMAX_DELAY);
	    display_serial_terminal_putstring_with_uart(hWinSerialTerminalindow, sci_handle, tmp);
	}
}

static void display_serial_terminal_putstring_with_uart(WM_HWIN hWin_handle, sci_hdl_t sci_handle, char *string)
{
	display_serial_terminal_putstring(hWin_handle, string);
}

static void sci_callback(void *pArgs)
{
    sci_cb_args_t   *args;
    sci_err_t   sci_err;
    uint8_t tmp;

    args = (sci_cb_args_t *)pArgs;

    if (args->event == SCI_EVT_RX_CHAR)
    {
        /* From RXI interrupt; received character data is in args->byte */
        nop();
        sci_err = R_SCI_Receive(sci_handle, &tmp, 1);
        if(sci_err == SCI_SUCCESS)
        {
            xQueueSendFromISR(xQueue, &tmp, NULL);
        }
    }
    else if (args->event == SCI_EVT_RXBUF_OVFL)
    {
        /* From RXI interrupt; rx queue is full; 'lost' data is in args->byte
           You will need to increase buffer size or reduce baud rate */
        nop();
    }
    else if (args->event == SCI_EVT_OVFL_ERR)
    {
        /* From receiver overflow error interrupt; error data is in args->byte
           Error condition is cleared in calling interrupt routine */
        nop();
    }
    else if (args->event == SCI_EVT_FRAMING_ERR)
    {
        /* From receiver framing error interrupt; error data is in args->byte
           Error condition is cleared in calling interrupt routine */
        nop();
    }
    else if (args->event == SCI_EVT_PARITY_ERR)
    {
        /* From receiver parity error interrupt; error data is in args->byte
           Error condition is cleared in calling interrupt routine */
        nop();
    }
}
