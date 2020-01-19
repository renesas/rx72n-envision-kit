
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
#include "r_cmt_rx_if.h"
#include "r_pinset.h"

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
#define COMMAND_NOT_FOUND "command not found\r\n$ "

#define SERIAL_BUFFER_QUEUE_NUMBER 1024
#define SERIAL_BUFFER_SIZE 1

#define COMMAND_UNKNOWN -1
#define COMMAND_FREERTOS 1
#define COMMAND_VERSION 2

#if !defined(MY_BSP_CFG_AFR_TERM_SCI)
#error "Error! Need to define MY_BSP_CFG_SERIAL_TERM_SCI in r_bsp_config.h"
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (0)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI0()
#define SCI_CH_serial_term          SCI_CH0
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (1)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI1()
#define SCI_CH_serial_term          SCI_CH1
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (2)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI2()
#define SCI_CH_serial_term          SCI_CH2
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (3)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI3()
#define SCI_CH_serial_term          SCI_CH3
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (4)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI4()
#define SCI_CH_serial_term          SCI_CH4
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (5)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI5()
#define SCI_CH_serial_term          SCI_CH5
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (6)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI6()
#define SCI_CH_serial_term          SCI_CH6
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (7)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI7()
#define SCI_CH_serial_term          SCI_CH7
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (8)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI8()
#define SCI_CH_serial_term          SCI_CH8
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (9)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI9()
#define SCI_CH_serial_term          SCI_CH9
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (10)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI10()
#define SCI_CH_serial_term          SCI_CH10
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (11)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI11()
#define SCI_CH_serial_term          SCI_CH11
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (12)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI12()
#define SCI_CH_serial_term          SCI_CH12
#else
#error "Error! Invalid setting for MY_BSP_CFG_SERIAL_TERM_SCI in r_bsp_config.h"
#endif

/******************************************************************************
 External variables
 ******************************************************************************/

/******************************************************************************
 Private global variables
 ******************************************************************************/
static void display_serial_terminal_putstring_with_uart(WM_HWIN hWin_handle, sci_hdl_t sci_handle, char *string);
static void sci_callback(void *pArgs);
static void execute_command(uint8_t *command_line);
static int32_t get_command_code(uint8_t *command);

static sci_hdl_t sci_handle;
static QueueHandle_t xQueue;
static char stats_buffer[1024 * 8];
static uint32_t _1us_timer_count;

/******************************************************************************
 External functions
 ******************************************************************************/
extern void display_serial_terminal_putstring(WM_HWIN hWin_handle, char *string);

/*******************************************************************************
 global variables and functions
********************************************************************************/
void serial_terminal_task( void * pvParameters );
void _1us_timer_tick(void *arg);
void vConfigureTimerForRunTimeStats(void);
uint32_t ulGetRunTimeCounterValue(void);

/******************************************************************************
 Function Name   : serial_terminal_task
 Description     : serial_terminal_task
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void serial_terminal_task( void * pvParameters )
{
    sci_cfg_t   sci_config;
    char tmp[2];
    tmp[1] = 0;
    char sci_buffer[2048];
    uint32_t current_buffer_pointer = 0;
    uint8_t command[16], arg1[256], arg2[256], arg3[256], arg4[256];

    memset(sci_buffer, 0, sizeof(sci_buffer));

    WM_HWIN hWinSerialTerminalindow = (*(WM_HWIN *)pvParameters);

    R_SCI_PinSet_serial_term();

    /* Set up the configuration data structure for asynchronous (UART) operation. */
    sci_config.async.baud_rate    = MY_BSP_CFG_SERIAL_TERM_SCI_BITRATE;
    sci_config.async.clk_src      = SCI_CLK_INT;
    sci_config.async.data_size    = SCI_DATA_8BIT;
    sci_config.async.parity_en    = SCI_PARITY_OFF;
    sci_config.async.parity_type  = SCI_EVEN_PARITY;
    sci_config.async.stop_bits    = SCI_STOPBITS_1;
    sci_config.async.int_priority = MY_BSP_CFG_SERIAL_TERM_SCI_INTERRUPT_PRIORITY;    // 1=lowest, 15=highest
    R_SCI_Open(SCI_CH_serial_term, SCI_MODE_ASYNC, &sci_config, sci_callback, &sci_handle);

    /* create queue */
	xQueue = xQueueCreate(SERIAL_BUFFER_QUEUE_NUMBER, SERIAL_BUFFER_SIZE);

	display_serial_terminal_putstring_with_uart(hWinSerialTerminalindow, sci_handle, PROMPT);
	while(1)
	{
		xQueueReceive(xQueue, &tmp, portMAX_DELAY);
		display_serial_terminal_putstring_with_uart(hWinSerialTerminalindow, sci_handle, tmp);
		sci_buffer[current_buffer_pointer++] = tmp[0];
		if((tmp[0] == 0x0a) && (sci_buffer[current_buffer_pointer - 2] == 0x0d))
		{
			display_serial_terminal_putstring_with_uart(hWinSerialTerminalindow, sci_handle, PROMPT);
		    /* command execution */
		    if ( 0 != sscanf((char*)sci_buffer, "%16s %256s %256s %256s %256s", command, arg1, arg2, arg3, arg4))
		    {
		        switch(get_command_code(command))
		        {
		        	case COMMAND_FREERTOS:
		        		if(!strcmp(arg1, "cpuload"))
		        		{
		        			if(!strcmp(arg2, "read"))
		        			{
				        		vTaskGetRunTimeStats(stats_buffer);
				        		display_serial_terminal_putstring_with_uart(hWinSerialTerminalindow, sci_handle, stats_buffer);
		        			}
		        			if(!strcmp(arg2, "reset"))
		        			{
		        				vTaskClearUsage();
				        		vTaskGetRunTimeStats(stats_buffer);
				        		display_serial_terminal_putstring_with_uart(hWinSerialTerminalindow, sci_handle, stats_buffer);
		        			}
		        		}
		        		break;
		        	case COMMAND_VERSION:
		        		break;
		        	default:
		        		display_serial_terminal_putstring_with_uart(hWinSerialTerminalindow, sci_handle, COMMAND_NOT_FOUND);
		        		break;
		        }
		    }
		    else
		    {

		    }
		    memset(sci_buffer, 0, sizeof(sci_buffer));
		    current_buffer_pointer = 0;
		    command[0] = 0;
		    arg1[0] = 0;
		    arg2[0] = 0;
		    arg3[0] = 0;
		    arg4[0] = 0;
		}
		else if(current_buffer_pointer == sizeof(sci_buffer))
		{
		    memset(sci_buffer, 0, sizeof(sci_buffer));
		    current_buffer_pointer = 0;
		}
	}
}

static int32_t get_command_code(uint8_t *command)
{
    int32_t return_code;

    if(!strcmp((char*)command, "freertos"))
    {
    	return_code = COMMAND_FREERTOS;
    }
    else if(!strcmp((char*)command, "version"))
    {
    	return_code = COMMAND_VERSION;
    }
    else
    {
        return_code = COMMAND_UNKNOWN;
    }
    return return_code;
}

static void display_serial_terminal_putstring_with_uart(WM_HWIN hWin_handle, sci_hdl_t sci_handle, char *string)
{
    uint16_t str_length = 0;
    uint16_t transmit_length = 0;
    sci_err_t sci_err;
    uint32_t retry = 0xFFFF;

	display_serial_terminal_putstring(hWin_handle, string);

    str_length = (uint16_t)strlen(string);

    while ((retry > 0) && (str_length > 0))
    {

        R_SCI_Control(sci_handle, SCI_CMD_TX_Q_BYTES_FREE, &transmit_length);

        if(transmit_length > str_length)
        {
            transmit_length = str_length;
        }

        sci_err = R_SCI_Send(sci_handle, (uint8_t *) string,
                             transmit_length);

        if ((sci_err == SCI_ERR_XCVR_BUSY) || (sci_err == SCI_ERR_INSUFFICIENT_SPACE))
        {
            retry--; // retry if previous transmission still in progress or tx buffer is insufficient.
            continue;
        }

        str_length -= transmit_length;
        string += transmit_length;
    }

    if (SCI_SUCCESS != sci_err)
    {
    	R_BSP_NOP(); //TODO error handling code
    }
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
    else if (args->event == SCI_EVT_TEI)
    {
        nop();
    }
}

void vConfigureTimerForRunTimeStats(void)
{
	uint32_t channel;
	/* 1us tick timer start */
	R_CMT_CreatePeriodic(1000000, _1us_timer_tick, &channel);
}

uint32_t ulGetRunTimeCounterValue(void)
{
	return _1us_timer_count;
}

void _1us_timer_tick(void *arg)
{
	_1us_timer_count++;
}
