
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
 * File Name    : gui_task.c
 * Description  : gui task
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
#define DEMO_NAME_FIRMWARE_UPDATE_VIA_SD_CARD "Firmware Update via SD Card"
#define DEMO_NAME_SYSTEM_LOG "Amazon FreeRTOS Log"
#define DEMO_NAME_TITLE_LOGO "Title Logo"
#define DEMO_NAME_SERIAL_TERMINAL "Serial Terminal"
#define DEMO_NAME_TASK_MANAGER "Task Manager"

typedef struct _demo_window_list
{
	WM_HWIN demo_window_handle;
	WM_HWIN frame_window_handle;
	uint32_t current_displayed;
	char demo_name[32];
	struct  _demo_window_list *next;
}DEMO_WINDOW_LIST;


/******************************************************************************
 External variables
 ******************************************************************************/

/******************************************************************************
 Private global variables
 ******************************************************************************/
static SYS_TIME sys_time;
static DEMO_WINDOW_LIST *demo_window_list_head;
static DEMO_WINDOW_LIST* demo_window_add_list(DEMO_WINDOW_LIST *pdemo_window_list_head, WM_HWIN new_handle, WM_HWIN frame_handle, char *demo_name);
static void demo_window_free_list(DEMO_WINDOW_LIST *pdemo_window_list);
static void demo_window_display_previous(DEMO_WINDOW_LIST *pdemo_window_list_head);
static void demo_window_display_next(DEMO_WINDOW_LIST *pdemo_window_list_head);

static void main_10ms_emWin_update(TASK_INFO *task_info);
static void main_100ms_display_update(TASK_INFO *task_info);

static int32_t next_button_id, prev_button_id;

/******************************************************************************
 External functions
 ******************************************************************************/
extern WM_HWIN CreateFrameWindow(void);
extern WM_HWIN CreateSystemLogWindow(void);
extern WM_HWIN CreateTaskManager(void);
extern WM_HWIN CreateFirmwareUpdateViaSDCard(void);
extern WM_HWIN CreateTitleLogoWindow(void);
extern WM_HWIN CreateFirmwareUpdateWindow(void);
extern WM_HWIN CreateSerialTerminalWindow(void);

extern void display_update_usb_stat(WM_HWIN hWin, int8_t usb_stat);
extern void display_update_sd_stat(WM_HWIN hWin, int8_t sd_stat);
extern void display_update_ip_stat(WM_HWIN hWin, uint8_t *ip_address);
extern void display_update_time(WM_HWIN hWin, SYS_TIME *sys_time);
extern void display_update_demo_name(WM_HWIN hWin, char *demo_name);
extern void display_syslog_putstring(WM_HWIN hWin, char *string);
extern int get_prev_button_id(void);
extern int get_next_button_id(void);
extern int frame_next_button_enable(WM_HWIN hWin, uint8_t onoff);
extern int frame_prev_button_enable(WM_HWIN hWin, uint8_t onoff);

/*******************************************************************************
 global variables and functions
********************************************************************************/

volatile int32_t first_touch_wait_flag;
volatile int32_t gui_initialize_complete_flag;

void gui_task( void * pvParameters );
void emWinCallback(WM_MESSAGE * pMsg);
void callback_frame_window_to_main(int32_t id, int32_t event);

/******************************************************************************
 Function Name   : gui_task
 Description     : gui_task
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void gui_task( void * pvParameters )
{
	static uint32_t counter = 0;
	TASK_INFO *task_info = (TASK_INFO *)pvParameters;

	/* GUI initialize complete */
	gui_initialize_complete_flag = 1;

	/* generate frame window */
	demo_window_free_list(demo_window_list_head);
	task_info->hWin_frame = CreateFrameWindow();

	/* generate sub windows */
	task_info->hWin_system_log = CreateSystemLogWindow();
	demo_window_list_head = demo_window_add_list(demo_window_list_head, task_info->hWin_system_log, task_info->hWin_frame, DEMO_NAME_SYSTEM_LOG);

	task_info->hWin_serial_terminal = CreateSerialTerminalWindow();
	demo_window_list_head = demo_window_add_list(demo_window_list_head, task_info->hWin_serial_terminal, task_info->hWin_frame, DEMO_NAME_SERIAL_TERMINAL);

	task_info->hWin_firmware_update_via_sd_card = CreateFirmwareUpdateViaSDCard();
	demo_window_list_head = demo_window_add_list(demo_window_list_head, task_info->hWin_firmware_update_via_sd_card, task_info->hWin_frame, DEMO_NAME_FIRMWARE_UPDATE_VIA_SD_CARD);

	task_info->hWin_task_manager = CreateTaskManager();
	demo_window_list_head = demo_window_add_list(demo_window_list_head, task_info->hWin_task_manager, task_info->hWin_frame, DEMO_NAME_TASK_MANAGER);

	task_info->hWin_title_logo = CreateTitleLogoWindow();
	demo_window_list_head = demo_window_add_list(demo_window_list_head, task_info->hWin_title_logo, task_info->hWin_frame, DEMO_NAME_TITLE_LOGO);

	/* get each GUI IDs */
	prev_button_id = get_prev_button_id();
	next_button_id = get_next_button_id();

	/* wait until first touch screen */
	GUI_Delay(10);
	vTaskDelay(1000);	/* this wait needs for ignoring touch event at WM_TOUCH_CHILD in TitleLogoWindowDLG.c when initializing. */

	first_touch_wait_flag = 1;
	while(first_touch_wait_flag)
	{
		GUI_Delay(10);
		vTaskDelay(10);
	}

    /* notify completing GUI initialization and first touch to main task */
	xTaskNotifyGive(task_info->main_task_handle);

	while(1)
	{
		main_10ms_emWin_update(task_info);
		vTaskDelay(10);
		counter++;
		if(counter > 10)
		{
			counter = 0;
			main_100ms_display_update(task_info);
		}
	}
}

void main_10ms_emWin_update(TASK_INFO *task_info)
{
	GUI_Exec(); /* Do the background work ... Update windows etc.) */
//	GUI_X_ExecIdle(); /* Nothing left to do for the moment ... Idle processing */
}

void main_100ms_display_update(TASK_INFO *task_info)
{
	DEMO_WINDOW_LIST *p;
	static DEMO_WINDOW_LIST *prev_p;

	R_SYS_TIME_GetCurrentTime(&sys_time);

	display_update_time(task_info->hWin_frame, &sys_time);

	p = demo_window_list_head;
	while(1)
	{
		if(p->current_displayed)
		{
			if(prev_p != p)
			{
				prev_p = p;
				display_update_demo_name(task_info->hWin_frame, p->demo_name);
				WM_BringToTop(p->demo_window_handle);
				if(p->demo_window_handle != task_info->hWin_title_logo)
				{
		    		WM_DeleteWindow(task_info->hWin_title_logo);
				}
			}
			break;
		}
		else
		{
			if(p->next == 0)
			{
				break;
			}
			else
			{
				p = p->next;
			}
		}
	}
}

void emWinCallback(WM_MESSAGE * pMsg)
{

}

void callback_frame_window_to_main(int32_t id, int32_t event)
{
	if(id == next_button_id)
	{
		if(event == WM_NOTIFICATION_CLICKED)
		{
			demo_window_display_next(demo_window_list_head);
		}
	}
	else if(id == prev_button_id)
	{
		if(event == WM_NOTIFICATION_CLICKED)
		{
			demo_window_display_previous(demo_window_list_head);
		}
	}
}

void delete_window_to_main(WM_HWIN delete_handle)
{
	DEMO_WINDOW_LIST *p;
	DEMO_WINDOW_LIST *pbefore;

	p = demo_window_list_head; /*global */
	pbefore = NULL;
	if(p != NULL)
	{
		while (p->next != NULL)
		{
			if(p->demo_window_handle == delete_handle)
			{
				break;
			}
			if(pbefore == NULL)
			{
				pbefore = demo_window_list_head;
			}
			else
			{
				pbefore = p;
			}
			p = p->next;
		}
		if(p->next == NULL)
		{
			return;
		}
		if(pbefore != NULL)
		{
			pbefore->next = p->next;
			if(p->current_displayed == 1)
			{
				pbefore->current_displayed = 1;
			}
		}
		else
		{
			demo_window_list_head = p->next;
			if(p->current_displayed == 1)
			{
				demo_window_list_head->current_displayed = 1;
			}
		}
		free(p);
	}
}


static DEMO_WINDOW_LIST* demo_window_add_list(DEMO_WINDOW_LIST *pdemo_window_list_head, WM_HWIN new_handle, WM_HWIN frame_handle, char *demo_name)
{
	DEMO_WINDOW_LIST *p;

	p = malloc(sizeof(DEMO_WINDOW_LIST));
	if(p == 0)
	{
		/* malloc error, nothing to do */
	}
	else
	{
		p->demo_window_handle = new_handle;
		p->frame_window_handle = frame_handle;
		strcpy(p->demo_name, demo_name);
		if(pdemo_window_list_head == NULL)
		{
			p->current_displayed = 1;
			p->next = NULL;
		}
		else
		{
			p->next = pdemo_window_list_head;
			pdemo_window_list_head = p;
			p = pdemo_window_list_head;
			pdemo_window_list_head->current_displayed = 1;
			pdemo_window_list_head->next->current_displayed = 0;
		}
	}
	return p;
}

static void demo_window_free_list(DEMO_WINDOW_LIST *pdemo_window_list_head)
{
	DEMO_WINDOW_LIST *p;

	if(pdemo_window_list_head != NULL)
	{
		while (pdemo_window_list_head->next != NULL)
		{
			p = pdemo_window_list_head->next;
			free(pdemo_window_list_head);
			pdemo_window_list_head = p;
		}
		pdemo_window_list_head = 0;
	}
}

static void demo_window_display_previous(DEMO_WINDOW_LIST *pdemo_window_list_head)
{
	DEMO_WINDOW_LIST *p, *p_prev;
	p = pdemo_window_list_head;

	while (1)
	{
		if(p->current_displayed)
		{
			break;
		}
		p_prev = p;
		p = p->next;
	}

	if(p == pdemo_window_list_head)
	{
		/* nothing to do */
	}
	else
	{
		p_prev->current_displayed = 1;
		p->current_displayed = 0;
		if(p_prev->demo_window_handle == pdemo_window_list_head->demo_window_handle)
		{
			frame_prev_button_enable(p->frame_window_handle, 0);
		}
	}
}

static void demo_window_display_next(DEMO_WINDOW_LIST *pdemo_window_list_head)
{
	DEMO_WINDOW_LIST *p;

	p = pdemo_window_list_head;

	while (1)
	{
		if(p->current_displayed)
		{
			break;
		}
		p = p->next;
	}

	if(p->next == 0)
	{
		/* nothing to do */
	}
	else
	{
		(p->next)->current_displayed = 1;
		p->current_displayed = 0;
		if((p->next)->next == NULL)
		{
			frame_next_button_enable(p->frame_window_handle, 0);
		}
	}
}
