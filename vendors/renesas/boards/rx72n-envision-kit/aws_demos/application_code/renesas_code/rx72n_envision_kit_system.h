
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
 * File Name    : rx72n_envision_kit_system.h
 * Description  : rx72n_envision_kit_system header
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 29.12.2019 1.00 First Release
 *********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

/* for using Segger emWin */
#include "GUI.h"
#include "DIALOG.h"

/* for using Amazon FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "aws_application_version.h"

/* for using C standard library */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* for RX Family FIT */
#include "r_sys_time_rx_if.h"

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

typedef struct _task_info
{
	/* for FreeRTOS */
	TaskHandle_t main_task_handle;
	TaskHandle_t serial_terminal_task_handle;
	TaskHandle_t gui_task_handle;
	TaskHandle_t sdcard_task_handle;
	TaskHandle_t task_manager_task_handle;
	TaskHandle_t sntp_task_handle;
	TaskHandle_t tcp_send_performance_task_handle;
	TaskHandle_t tcp_receive_performance_task_handle;
	TaskHandle_t serial_flash_task_handle;

	/* for emWin */
	WM_HWIN hWin_serial_terminal;
	WM_HWIN hWin_firmware_update_via_sd_card;
	WM_HWIN hWin_task_manager;
	WM_HWIN hWin_system_log;
	WM_HWIN hWin_frame;
	WM_HWIN hWin_title_logo;

	/* for demo system time */
	SYS_TIME sys_time;
}TASK_INFO;
