
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
 * File Name    : task_manager_task.c
 * Description  : task_manager_task
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
#include "r_sys_time_rx_if.h"

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

/******************************************************************************
 External variables
 ******************************************************************************/

/******************************************************************************
 Private global variables
 ******************************************************************************/

/******************************************************************************
 External functions
 ******************************************************************************/

/*******************************************************************************
 global variables and functions
********************************************************************************/
void task_manager_task( void * pvParameters );

/******************************************************************************
 Function Name   : serial_terminal_task
 Description     : serial_terminal_task
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void task_manager_task( void * pvParameters )
{
	TASK_INFO *task_info = (WM_HWIN *)pvParameters;

    /* wait completing gui initializing */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    while(1)
    {
    	vTaskDelay(1);
    }
}
