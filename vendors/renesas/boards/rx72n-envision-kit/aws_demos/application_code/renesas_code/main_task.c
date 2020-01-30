  
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
 * Copyright (C) 2014(2016) Renesas Electronics Corporation. All rights reserved.
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
#include "r_pinset.h"
#include "r_sys_time_rx_if.h"
#include "r_gpio_rx_if.h"
#include "r_flash_rx_if.h"
#include "r_usb_basic_if.h"
#include "r_tfat_lib.h"
#include "Pin.h"

/* for using Segger emWin */
#include "GUI.h"
#include "DIALOG.h"

/* for using Amazon FreeRTOS */
#include "FreeRTOS.h"
#include "aws_application_version.h"

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/******************************************************************************
 External variables
 ******************************************************************************/

/******************************************************************************
 Private global variables
 ******************************************************************************/
static TaskHandle_t serial_terminal_task_handle;
static TaskHandle_t sdcard_task_handle;
static TaskHandle_t gui_task_handle;

/******************************************************************************

 External functions
 ******************************************************************************/
extern void LCDCONF_EnableDave2D(void);

extern void serial_terminal_task( void * pvParameters );
extern void sdcard_task( void * pvParameters );
extern void gui_task( void * pvParameters );

/*******************************************************************************
 global variables and functions
********************************************************************************/
int32_t wait_first_display(void);
void firmware_version_read(char **ver_str);
void main_task(void);

extern WM_HWIN hWinSerialTerminalWindow;
extern WM_HWIN hWinFirmwareUpdateViaSDCardWindow;

/******************************************************************************
 Function Name   : main
 Description     : Main task
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void main_task(void)
{
	uint32_t bank_info;

    /* enable MCU pins */
    R_Pins_Create();

	/* system timer initialization */
	R_SYS_TIME_Open();

	/* GUI initialization */
    GUI_Exit();
	GUI_Init();
	LCDCONF_EnableDave2D();
	WM_MULTIBUF_Enable(1);

	/* flash initialization */
	R_FLASH_Open();
    R_FLASH_Control(FLASH_CMD_BANK_GET, &bank_info);

    /* serial terminal task creation */
    xTaskCreate(serial_terminal_task, "terminal", 4096, &hWinSerialTerminalWindow, tskIDLE_PRIORITY, &serial_terminal_task_handle);

    /* GUI task creation */
    xTaskCreate(gui_task, "gui", 4096, NULL, configMAX_PRIORITIES - 1, &gui_task_handle);

    /* sdcard task creation */
    xTaskCreate(sdcard_task, "sdcard", configMINIMAL_STACK_SIZE, &hWinFirmwareUpdateViaSDCardWindow, tskIDLE_PRIORITY, &sdcard_task_handle);

	/* main loop */
	while(1)
	{
    	vTaskDelay(1000);
	}
}
/******************************************************************************
 End of function main_task()
 ******************************************************************************/

void firmware_version_read(char **ver_str)
{
	static char firmware_version[16];
	sprintf(firmware_version, "v%d.%d.%d", APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_BUILD);
	*ver_str = (char*)firmware_version;
}

/******************************************************************************
 End  Of File
 ******************************************************************************/

