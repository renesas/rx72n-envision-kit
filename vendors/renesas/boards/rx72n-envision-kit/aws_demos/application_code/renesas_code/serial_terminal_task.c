
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
#include "r_sys_time_rx_if.h"
#include "r_simple_filesystem_on_dataflash_if.h"

/* for using Segger emWin */
#include "GUI.h"
#include "DIALOG.h"

/* for using Amazon FreeRTOS */
#include "FreeRTOS.h"
#include "aws_application_version.h"

/* for RX72N Envision Kit system common header */
#include "rx72n_envision_kit_system.h"

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/
#define PROMPT "RX72N Envision Kit\r\n$ "
#define COMMAND_NOT_FOUND "command not found\r\n$ "

#define SERIAL_BUFFER_QUEUE_NUMBER 1024
#define SERIAL_BUFFER_SIZE 1
#define SCI_BUFFER_SIZE 2048
#define COMMAND_SIZE 16
#define ARGUMENT1_SIZE 256
#define ARGUMENT2_SIZE 256
#define ARGUMENT3_SIZE 4096
#define ARGUMENT4_SIZE 4096
#define MESSAGE_BUFFER_SIZE 1024 * 8

#define COMMAND_UNKNOWN -1
#define COMMAND_FREERTOS 1
#define COMMAND_VERSION 2
#define COMMAND_TIMEZONE 3
#define COMMAND_RESET 4
#define COMMAND_DATAFLASH 5

#if !defined(MY_BSP_CFG_AFR_TERM_SCI)
#error "Error! Need to define MY_BSP_CFG_SERIAL_TERM_SCI in r_bsp_config.h"
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (0)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI0()
#define SCI_CH_serial_term          SCI_CH0
#define SCI_TX_buffer_size          SCI_CFG_CH0_TX_BUFSIZ
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (1)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI1()
#define SCI_CH_serial_term          SCI_CH1
#define SCI_TX_buffer_size          SCI_CFG_CH1_TX_BUFSIZ
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (2)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI2()
#define SCI_CH_serial_term          SCI_CH2
#define SCI_TX_buffer_size          SCI_CFG_CH2_TX_BUFSIZ
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (3)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI3()
#define SCI_CH_serial_term          SCI_CH3
#define SCI_TX_buffer_size          SCI_CFG_CH3_TX_BUFSIZ
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (4)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI4()
#define SCI_CH_serial_term          SCI_CH4
#define SCI_TX_buffer_size          SCI_CFG_CH4_TX_BUFSIZ
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (5)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI5()
#define SCI_CH_serial_term          SCI_CH5
#define SCI_TX_buffer_size          SCI_CFG_CH5_TX_BUFSIZ
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (6)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI6()
#define SCI_CH_serial_term          SCI_CH6
#define SCI_TX_buffer_size          SCI_CFG_CH6_TX_BUFSIZ
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (7)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI7()
#define SCI_CH_serial_term          SCI_CH7
#define SCI_TX_buffer_size          SCI_CFG_CH7_TX_BUFSIZ
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (8)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI8()
#define SCI_CH_serial_term          SCI_CH8
#define SCI_TX_buffer_size          SCI_CFG_CH8_TX_BUFSIZ
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (9)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI9()
#define SCI_CH_serial_term          SCI_CH9
#define SCI_TX_buffer_size          SCI_CFG_CH9_TX_BUFSIZ
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (10)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI10()
#define SCI_CH_serial_term          SCI_CH10
#define SCI_TX_buffer_size          SCI_CFG_CH10_TX_BUFSIZ
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (11)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI11()
#define SCI_CH_serial_term          SCI_CH11
#define SCI_TX_buffer_size          SCI_CFG_CH11_TX_BUFSIZ
#elif MY_BSP_CFG_SERIAL_TERM_SCI == (12)
#define R_SCI_PinSet_serial_term()  R_SCI_PinSet_SCI12()
#define SCI_CH_serial_term          SCI_CH12
#define SCI_TX_buffer_size          SCI_CFG_CH12_TX_BUFSIZ
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
static SemaphoreHandle_t xSemaphore;
static char *message_buffer;
static uint32_t _10us_timer_count;

/******************************************************************************
 External functions
 ******************************************************************************/
extern void display_serial_terminal_putstring(WM_HWIN hWin_handle, char *string);

extern void vTaskGetCombinedRunTimeStats( char* pcWriteBuffer,  UBaseType_t uxClear);
extern void vTaskClearUsage(void);
extern void vTaskClearUsageSingleList(List_t *pxList);

extern void firmware_version_read(char **ver_str);
extern void software_reset(void);

/*******************************************************************************
 global variables and functions
********************************************************************************/
void serial_terminal_task( void * pvParameters );
void _10us_timer_tick(void *arg);
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
	TASK_INFO *task_info = (TASK_INFO *)pvParameters;

    sci_cfg_t   sci_config;
    char tmp[2];
    char *ver;
    tmp[1] = 0;
    char *sci_buffer;
    uint32_t current_buffer_pointer = 0;
    uint8_t *command, *arg1, *arg2, *arg3, *arg4;

    char timezone_label[] = "timezone";
	char client_private_key_label[] = "client_private_key";
	char client_certificate_label[] = "client_certificate";

	SFD_HANDLE sfd_handle_timezone, sfd_handle_tmp;
	uint8_t *timezone;
	uint32_t timezone_length;

    uint8_t *label, *data;
    uint32_t label_length, data_length;

    /* wait completing gui initializing */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    sci_buffer = pvPortMalloc(SCI_BUFFER_SIZE);
    command = pvPortMalloc(COMMAND_SIZE);
    arg1 = pvPortMalloc(ARGUMENT1_SIZE);
    arg2 = pvPortMalloc(ARGUMENT2_SIZE);
    arg3 = pvPortMalloc(ARGUMENT3_SIZE);
    arg4 = pvPortMalloc(ARGUMENT4_SIZE);
    message_buffer = pvPortMalloc(MESSAGE_BUFFER_SIZE);

    memset(sci_buffer, 0, SCI_BUFFER_SIZE);
    memset(command, 0, COMMAND_SIZE);
    memset(arg1, 0, ARGUMENT1_SIZE);
    memset(arg2, 0, ARGUMENT2_SIZE);
    memset(arg3, 0, ARGUMENT3_SIZE);
    memset(arg4, 0, ARGUMENT4_SIZE);
    memset(message_buffer, 0, MESSAGE_BUFFER_SIZE);

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

    /* create semaphore */
	xSemaphore = xSemaphoreCreateBinary();

	display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, PROMPT);
	while(1)
	{
		xQueueReceive(xQueue, &tmp, portMAX_DELAY);
		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, tmp);
		sci_buffer[current_buffer_pointer++] = tmp[0];
		if((tmp[0] == 0x0a) && (sci_buffer[current_buffer_pointer - 2] == 0x0d))
		{
		    /* command execution */
		    if ( 0 != sscanf((char*)sci_buffer, "%16s %256s %256s %4096s %4096s", command, arg1, arg2, arg3, arg4))
		    {
		        switch(get_command_code(command))
		        {
		        	case COMMAND_FREERTOS:
		        		if(!strcmp((const char *)arg1, "cpuload"))
		        		{
		        			if(!strcmp((const char *)arg2, "read"))
		        			{
		        				vTaskGetCombinedRunTimeStats(message_buffer, 0);	/* 0 means read */
				        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, message_buffer);
		        			}
		        			if(!strcmp((const char *)arg2, "reset"))
		        			{
		        				vTaskGetCombinedRunTimeStats(message_buffer, 1);	/* 1 means read->reset */
		        				vTaskGetCombinedRunTimeStats(message_buffer, 0);	/* 0 means read */
				        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, message_buffer);
		        			}
		        		}
		        		break;
		        	case COMMAND_VERSION:
		        		firmware_version_read(&ver);
		        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, ver);
		        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, "\r\n");
		        		break;
		        	case COMMAND_TIMEZONE:
	        			R_SFD_Open();
		        		sfd_handle_timezone = R_SFD_SaveObject((uint8_t *)timezone_label, strlen(timezone_label), arg1, strlen((char *)arg1));
		        		R_SFD_GetObjectValue(sfd_handle_timezone, (uint8_t **)&timezone, &timezone_length);
			            if(SYS_TIME_SUCCESS == R_SYS_TIME_ConvertUnixTimeToSystemTime(task_info->sys_time.unix_time, &task_info->sys_time, timezone))
			            {
			        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, "timezone is accepted.\r\n");
			        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, "system time is updated.\r\n");
			            }
			            else
			            {
			        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, "timezone is not accepted.\r\n");
			            }
	        			R_SFD_Close();
		        		break;
		        	case COMMAND_RESET:
		        		software_reset();
		        		break;
		        	case COMMAND_DATAFLASH:
		        		if(!strcmp((const char *)arg1, "info"))
		        		{
		        			R_SFD_Open();
		        			sprintf(message_buffer, "physical size = %d bytes.\n", R_SFD_ReadPysicalSize());
			        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, message_buffer);
		        			sprintf(message_buffer, "allocated size = %d bytes.\n", R_SFD_ReadAllocatedStorageSize());
			        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, message_buffer);
		        			sprintf(message_buffer, "free size = %d bytes.\n", R_SFD_ReadFreeSize());
			        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, message_buffer);
		        			R_SFD_Close();
		        		}
		        		else if(!strcmp((const char *)arg1, "write"))
		        		{
			        		if(!strcmp((const char *)arg2, "aws"))
			        		{
				        		if((!strcmp((const char *)arg3, "clientprivatekey")) || (!strcmp((const char *)arg3, "clientcertificate")))
				        		{
				        			current_buffer_pointer = 0;
								    memset(sci_buffer, 0, SCI_BUFFER_SIZE);
				        			while(1)
				        			{
										xQueueReceive(xQueue, &tmp, portMAX_DELAY);
										display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, tmp);
										sci_buffer[current_buffer_pointer++] = tmp[0];
										if((strstr(sci_buffer, "-----END RSA PRIVATE KEY-----\n")) || (strstr(sci_buffer, "-----END CERTIFICATE-----\n")))
										{
											char *target;
											if((!strcmp((const char *)arg3, "clientprivatekey")) && strstr(sci_buffer, "-----END RSA PRIVATE KEY-----\n"))
											{
												target = client_private_key_label;
											}
											else if((!strcmp((const char *)arg3, "clientcertificate")) && strstr(sci_buffer, "-----END CERTIFICATE-----\n"))
											{
												target = client_certificate_label;
											}
											else
											{
							        			sprintf(message_buffer, "could not store data into dataflash.\n");
								        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, message_buffer);
												break;
											}

						        			R_SFD_Open();
						        			sci_buffer[current_buffer_pointer] = 0;
						        			/* +1 means '\0' as a string terminator */
						        			if(SFD_HANDLE_INVALID != R_SFD_SaveObject((uint8_t *)target, strlen(target), (uint8_t *)sci_buffer, strlen((const char *)sci_buffer) + 1))
						        			{
							        			sprintf(message_buffer, "stored data into dataflash correctly.\n");
								        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, message_buffer);
						        			}
						        			else
						        			{
							        			sprintf(message_buffer, "could not store data into dataflash.\n");
								        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, message_buffer);
						        			}
						        			R_SFD_Close();
											break;
										}
										if((strstr(sci_buffer, "quit")) || (strstr(sci_buffer, "exit")))
										{
											break;
										}
										if(current_buffer_pointer == SCI_BUFFER_SIZE)
										{
										    memset(sci_buffer, 0, SCI_BUFFER_SIZE);
										    current_buffer_pointer = 0;
										}
				        			}
				        		}
				        		else
				        		{
				        			sprintf(message_buffer, "unknown argument3 = %s.\n", arg3);
					        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, message_buffer);
				        		}
			        		}
			        		else
			        		{
			        			sprintf(message_buffer, "unknown argument2 = %s.\n", arg2);
				        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, message_buffer);
			        		}
		        		}
		        		else if(!strcmp((const char *)arg1, "read"))
		        		{
	        				R_SFD_ResetScan();
		        			while(1)
		        			{
		        				if(SFD_SUCCESS == R_SFD_Scan(&label, &label_length, &data, &data_length))
		        				{
				        			sprintf(message_buffer, "label = ");
					        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, message_buffer);
				        			memcpy(message_buffer, label, label_length);
				        			message_buffer[label_length] = 0;
					        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, message_buffer);
				        			sprintf(message_buffer, "\n");
					        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, message_buffer);
				        			sprintf(message_buffer, "data = ");
					        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, message_buffer);
				        			memcpy(message_buffer, data, data_length);
				        			message_buffer[data_length] = 0;
					        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, message_buffer);
					        		vTaskDelay(100); /* 長い文字列をSCI(UART)出力した際に最後のほうが出力されない。完了待ちの方法が不明なためとりあえず時間待ち */
				        			sprintf(message_buffer, "\n\n");
					        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, message_buffer);
		        				}
		        				else
		        				{
		        					break;
		        				}
		        			}
		        		}
		        		else
		        		{
		        			sprintf(message_buffer, "unknown argument1 = %s.\n", arg1);
			        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, message_buffer);
		        		}
		        		break;
		        	default:
		        		display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, COMMAND_NOT_FOUND);
		        		break;
		        }
				display_serial_terminal_putstring_with_uart(task_info->hWin_serial_terminal, sci_handle, PROMPT);
		    }
		    else
		    {

		    }
		    current_buffer_pointer = 0;
		    memset(sci_buffer, 0, SCI_BUFFER_SIZE);
		    memset(command, 0, COMMAND_SIZE);
		    memset(arg1, 0, ARGUMENT1_SIZE);
		    memset(arg2, 0, ARGUMENT2_SIZE);
		    memset(arg3, 0, ARGUMENT3_SIZE);
		    memset(arg4, 0, ARGUMENT4_SIZE);
		    memset(message_buffer, 0, MESSAGE_BUFFER_SIZE);
		}
		else if(current_buffer_pointer == SCI_BUFFER_SIZE)
		{
		    memset(sci_buffer, 0, SCI_BUFFER_SIZE);
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
    else if(!strcmp((char*)command, "timezone"))
    {
    	return_code = COMMAND_TIMEZONE;
    }
    else if(!strcmp((char*)command, "reset"))
    {
    	return_code = COMMAND_RESET;
    }
    else if(!strcmp((char*)command, "dataflash"))
    {
    	return_code = COMMAND_DATAFLASH;
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
    volatile uint16_t transmit_length = 0;
    sci_err_t sci_err;
    uint32_t retry = 0xFFFF;

	display_serial_terminal_putstring(hWin_handle, string);

    str_length = (uint16_t)strlen(string);

    while ((retry > 0) && (str_length > 0))
    {
        R_SCI_Control(sci_handle, SCI_CMD_TX_Q_BYTES_FREE, (void *)&transmit_length);

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

        xSemaphoreTake(xSemaphore, portMAX_DELAY);

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
    static BaseType_t xHigherPriorityTaskWoken;

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
    	xSemaphoreGiveFromISR( xSemaphore, &xHigherPriorityTaskWoken );
    }
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void vConfigureTimerForRunTimeStats(void)
{
	uint32_t channel;
	/* 10us tick timer start */
	R_CMT_CreatePeriodic(100000, _10us_timer_tick, &channel);
}

uint32_t ulGetRunTimeCounterValue(void)
{
	return _10us_timer_count;
}

void _10us_timer_tick(void *arg)
{
	_10us_timer_count++;
}
