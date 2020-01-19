
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

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/
#define DEMO_NAME_NETWORK_STAT "Network Statistics"
#define DEMO_NAME_SYSTEM_LOG "Amazon FreeRTOS Log"
#define DEMO_NAME_STORAGEBENCH  "Storage Benchmark"
#define DEMO_NAME_FIRMWARE_UPDATE "Firmware Update"
#define DEMO_NAME_TITLE_LOGO "Title Logo"
#define DEMO_NAME_SERIAL_TERMINAL "Serial Terminal"

typedef struct _demo_window_list
{
	WM_HWIN demo_window_handle;
	uint32_t current_displayed;
	char demo_name[32];
	struct  _demo_window_list *next;
}DEMO_WINDOW_LIST;


/******************************************************************************
 External variables
 ******************************************************************************/
extern WM_HWIN hWinFrameWindow, hWinNetworkStatWindow, hWinSerialTerminalindow;

/******************************************************************************
 Private global variables
 ******************************************************************************/
static SYS_TIME sys_time;
static DEMO_WINDOW_LIST *demo_window_list_head;
static DEMO_WINDOW_LIST* demo_window_add_list(DEMO_WINDOW_LIST *pdemo_window_list_head, WM_HWIN new_handle, char *demo_name);
static void demo_window_free_list(DEMO_WINDOW_LIST *pdemo_window_list);
static void demo_window_display_previous(DEMO_WINDOW_LIST *pdemo_window_list_head);
static void demo_window_display_next(DEMO_WINDOW_LIST *pdemo_window_list_head);

static void main_10ms_emWin_update(void);
static void main_100ms_display_update(void);

static int32_t next_button_id, prev_button_id;

/******************************************************************************
 External functions
 ******************************************************************************/
extern WM_HWIN CreateFrameWindow(void);
extern WM_HWIN CreateSystemLogWindow(void);
extern WM_HWIN CreateNetworkStatWindow(void);
extern WM_HWIN CreateStorageBenchmark(void);
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
WM_HWIN hWinFirmwareUpdatewindow, hWinTitleLogoWindow, hWinStorageBenchmark, hWinCryptoBenchmark1, hWinCryptoBenchmark2, hWinSystemLogWindow;
WM_HWIN hWinFrameWindow, hWinNetworkStatWindow, hWinSerialTerminalindow;

volatile int32_t first_touch_wait_flag;
volatile int32_t gui_initialize_complete_flag;

void gui_task( void * pvParameters );
void emWinCallback(WM_MESSAGE * pMsg);
void callback_frame_window_to_main(int32_t id, int32_t event);



/******************************************************************************
 Function Name   : serial_terminal_task
 Description     : serial_terminal_task
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void gui_task( void * pvParameters )
{
	/* GUI initialize complete */
	gui_initialize_complete_flag = 1;

	/* generate frame window */
	demo_window_free_list(demo_window_list_head);
	hWinFrameWindow = CreateFrameWindow();

	/* generate sub windows */
	hWinSystemLogWindow = CreateSystemLogWindow();
	demo_window_list_head = demo_window_add_list(demo_window_list_head, hWinSystemLogWindow, DEMO_NAME_SYSTEM_LOG);

#if 0
	hWinNetworkStatWindow = CreateNetworkStatWindow();
	demo_window_list_head = demo_window_add_list(demo_window_list_head, hWinNetworkStatWindow, DEMO_NAME_NETWORK_STAT);

	hWinStorageBenchmark = CreateStorageBenchmark();
	demo_window_list_head = demo_window_add_list(demo_window_list_head, hWinStorageBenchmark, DEMO_NAME_STORAGEBENCH);
#endif
	hWinSerialTerminalindow = CreateSerialTerminalWindow();
	demo_window_list_head = demo_window_add_list(demo_window_list_head, hWinSerialTerminalindow, DEMO_NAME_SERIAL_TERMINAL);

	hWinFirmwareUpdatewindow = CreateFirmwareUpdateWindow();
	demo_window_list_head = demo_window_add_list(demo_window_list_head, hWinFirmwareUpdatewindow, DEMO_NAME_FIRMWARE_UPDATE);

	hWinTitleLogoWindow = CreateTitleLogoWindow();
	demo_window_list_head = demo_window_add_list(demo_window_list_head, hWinTitleLogoWindow, DEMO_NAME_TITLE_LOGO);

	/* get each GUI IDs */
	prev_button_id = get_prev_button_id();
	next_button_id = get_next_button_id();

	/* wait until first touch screen */
	GUI_Exec();
	vTaskDelay(1000);	/* this wait needs for ignoring touch event at WM_TOUCH_CHILD in TitleLogoWindowDLG.c when initializing. */

	first_touch_wait_flag = 1;
	while(first_touch_wait_flag)
	{
		GUI_Exec();
		vTaskDelay(10);
	}

	uint32_t counter = 0;
	while(1)
	{
		main_10ms_emWin_update();
		vTaskDelay(10);
		counter++;
		if(counter == 10)
		{
			counter = 0;
			main_100ms_display_update();
		}
	}
}

void main_10ms_emWin_update(void)
{
	GUI_Exec();
}

void main_100ms_display_update(void)
{
	DEMO_WINDOW_LIST *p;
	static DEMO_WINDOW_LIST *prev_p;

	R_SYS_TIME_GetCurrentTime(&sys_time);

	display_update_time(hWinFrameWindow, &sys_time);

	p = demo_window_list_head;
	while(1)
	{
		if(p->current_displayed)
		{
			if(prev_p != p)
			{
				prev_p = p;
				display_update_demo_name(hWinFrameWindow, p->demo_name);
				WM_BringToTop(p->demo_window_handle);
				if(p->demo_window_handle != hWinTitleLogoWindow)
				{
		    		WM_DeleteWindow(hWinTitleLogoWindow);
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

void goto_user_program_screen(void)
{
	demo_window_display_next(demo_window_list_head);
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


static DEMO_WINDOW_LIST* demo_window_add_list(DEMO_WINDOW_LIST *pdemo_window_list_head, WM_HWIN new_handle, char *demo_name)
{
	DEMO_WINDOW_LIST *p;

	p = malloc(sizeof(DEMO_WINDOW_LIST));
	if(p == 0)
	{
		printf("malloc error occurred at demo_window_add_list()\n");
	}
	else
	{
		p->demo_window_handle = new_handle;
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
			frame_prev_button_enable(hWinFrameWindow, 0);
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
			frame_next_button_enable(hWinFrameWindow, 0);
		}

	}
}
