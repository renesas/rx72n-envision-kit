
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
#include "Generated/Resource.h"

/* for using Amazon FreeRTOS */
#include "FreeRTOS.h"
#include "FreeRTOS_IP.h"

/* for RX72N Envision Kit system common header */
#include "rx72n_envision_kit_system.h"

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/
#define DEMO_NAME_D2_AUDIO_RECORD_AND_PLAY "D2 Audio Record And Play"
#define DEMO_NAME_FIRMWARE_UPDATE_VIA_SD_CARD "Firmware Update via SD Card"
#define DEMO_NAME_SYSTEM_LOG "Amazon FreeRTOS Log"
#define DEMO_NAME_TITLE_LOGO "Title Logo"
#define DEMO_NAME_SERIAL_TERMINAL "Serial Terminal"
#define DEMO_NAME_TASK_MANAGER "Task Manager"

#define MINIMUM_TIME_TICK_FOR_UPDATE_DISPLAY 10 /* 100fps */

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
static DEMO_WINDOW_LIST *demo_window_list_head;
static DEMO_WINDOW_LIST* demo_window_add_list(DEMO_WINDOW_LIST *pdemo_window_list_head, WM_HWIN new_handle, WM_HWIN frame_handle, char *demo_name);
static void demo_window_free_list(DEMO_WINDOW_LIST *pdemo_window_list);
static void demo_window_display_previous(DEMO_WINDOW_LIST *pdemo_window_list_head);
static void demo_window_display_next(DEMO_WINDOW_LIST *pdemo_window_list_head);

static void main_10ms_display_update(TASK_INFO *task_info);
static void main_1s_display_update(TASK_INFO *task_info);

static int32_t next_button_id, prev_button_id;

/******************************************************************************
 External functions
 ******************************************************************************/

extern void display_update_usb_stat(WM_HWIN hWin, int8_t usb_stat);
extern void display_update_sd_stat(WM_HWIN hWin, int8_t sd_stat);
extern void display_update_ip_stat(WM_HWIN hWin, uint8_t *ip_address);
extern void display_update_time(WM_HWIN hWin, SYS_TIME *sys_time);
extern void display_syslog_putstring(WM_HWIN hWin, char *string);
extern void display_update_cpu_load(WM_HWIN hWin, uint32_t cpu_load);
extern void display_update_freertos_ram(WM_HWIN hWin, int32_t remaining, int32_t max);
extern void display_update_ip_stat(WM_HWIN hWin, uint8_t *ip_address);

extern void vTaskGetCombinedRunTimeStats( char* pcWriteBuffer,  UBaseType_t uxClear);
extern void vTaskClearUsage(void);
extern void vTaskClearUsageSingleList(List_t *pxList);

/*******************************************************************************
 global variables and functions
********************************************************************************/
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
    static uint32_t counter_10ms = 0, counter_100ms = 0;
    TASK_INFO *task_info = (TASK_INFO *)pvParameters;

    /* GUI initialization */
    APPW_X_Setup();
    APPW_Init(APPW_PROJECT_PATH);
    APPW_CreateRoot(APPW_INITIAL_SCREEN, WM_HBKWIN);

#if 0
    /* generate frame window */
    demo_window_free_list(demo_window_list_head);
    task_info->hWin_frame = CreateFrameWindow();

    /* generate sub windows */
    task_info->hWin_d2_audio = CreateD2AudioDemo();
    demo_window_list_head = demo_window_add_list(demo_window_list_head, task_info->hWin_d2_audio, task_info->hWin_frame, DEMO_NAME_D2_AUDIO_RECORD_AND_PLAY);

    task_info->hWin_system_log = CreateSystemLogWindow();
    demo_window_list_head = demo_window_add_list(demo_window_list_head, task_info->hWin_system_log, task_info->hWin_frame, DEMO_NAME_SYSTEM_LOG);

    task_info->hWin_serial_terminal = CreateSerialTerminalWindow();
    demo_window_list_head = demo_window_add_list(demo_window_list_head, task_info->hWin_serial_terminal, task_info->hWin_frame, DEMO_NAME_SERIAL_TERMINAL);

    task_info->hWin_firmware_update_via_sd_card = CreateFirmwareUpdateViaSDCard();
    demo_window_list_head = demo_window_add_list(demo_window_list_head, task_info->hWin_firmware_update_via_sd_card, task_info->hWin_frame, DEMO_NAME_FIRMWARE_UPDATE_VIA_SD_CARD);

#if 0
    task_info->hWin_task_manager = CreateTaskManager();
    demo_window_list_head = demo_window_add_list(demo_window_list_head, task_info->hWin_task_manager, task_info->hWin_frame, DEMO_NAME_TASK_MANAGER);
#endif

    task_info->hWin_title_logo = CreateTitleLogoWindow();
    demo_window_list_head = demo_window_add_list(demo_window_list_head, task_info->hWin_title_logo, task_info->hWin_frame, DEMO_NAME_TITLE_LOGO);
#endif
    main_10ms_display_update(task_info);
    main_1s_display_update(task_info);

    while(1)
    {
        main_10ms_display_update(task_info);
        counter_10ms++;
        if(counter_10ms > 10)
        {
            counter_10ms = 0;
            counter_100ms++;
            if(counter_100ms > 10)
            {
                counter_100ms = 0;
                main_1s_display_update(task_info);
            }
        }
    }
}

void main_10ms_display_update(TASK_INFO *task_info)
{
    TickType_t before_appw_exec_time_tick = xTaskGetTickCount();
    APPW_Exec();
    GUI_Exec();
    TickType_t after_appw_exec_time_tick = xTaskGetTickCount();
    if(after_appw_exec_time_tick >= before_appw_exec_time_tick)
    {
        if( MINIMUM_TIME_TICK_FOR_UPDATE_DISPLAY > (after_appw_exec_time_tick - before_appw_exec_time_tick))
        {
            /* >100fps pattern */
            vTaskDelay(MINIMUM_TIME_TICK_FOR_UPDATE_DISPLAY - (after_appw_exec_time_tick - before_appw_exec_time_tick));
            task_info->current_fps = 1000 / MINIMUM_TIME_TICK_FOR_UPDATE_DISPLAY;
        }
        else
        {
            /* <100fps pattern */
            vTaskDelay(1);
            task_info->current_fps = (float)((float)1000 / (float)(after_appw_exec_time_tick - before_appw_exec_time_tick));
        }
        task_info->history_fps[task_info->history_fps_index++] = task_info->current_fps;
        if(MAX_HISTORY_FPS_INDEX == task_info->history_fps_index)
        {
            task_info->history_fps_index = 0;
        }
    }
    else
    {
        /* nothing to do */
    }
}

void main_1s_display_update(TASK_INFO *task_info)
{
    TaskStatus_t *stats_buffer;
    uint32_t task_number, priority, hwm, cpu_time, idle_cpu_time, total_cpu_time, cpu_load, state;
    float idle_rate;
    static uint32_t previous_total_cpu_time = 0, previous_idle_cpu_time = 0;
    static uint8_t ip_address_array[4];
    uint32_t ip_address;
    uint32_t number_of_tasks;

    /* get/set freertos CPU load info */
    number_of_tasks = uxTaskGetNumberOfTasks();
    stats_buffer = pvPortMalloc(number_of_tasks * sizeof(TaskStatus_t));
    number_of_tasks = uxTaskGetSystemState(stats_buffer, number_of_tasks, &total_cpu_time);
    for(int i = 0; i < number_of_tasks; i++)
    {
        if(!strcmp(stats_buffer[i].pcTaskName, "IDLE"))
        {
            idle_cpu_time = stats_buffer[i].ulRunTimeCounter;
        }
    }
    idle_rate = ((float)(idle_cpu_time - previous_idle_cpu_time) / (float)(total_cpu_time - previous_total_cpu_time));
    cpu_load = 100 - (uint32_t)(idle_rate * 100);
    task_info->cpu_load = cpu_load;
    previous_idle_cpu_time = idle_cpu_time;
    previous_total_cpu_time = total_cpu_time;

    /* get/set IP address info */
    ip_address = FreeRTOS_GetIPAddress();
#if (__LIT)
    ip_address_array[3] = (ip_address & 0xff000000) >> 24;
    ip_address_array[2] = (ip_address & 0x00ff0000) >> 16;
    ip_address_array[1] = (ip_address & 0x0000ff00) >>  8;
    ip_address_array[0] = (ip_address & 0x000000ff) >>  0;
#else
    ip_address_array[0] = (ip_address & 0xff000000) >> 24;
    ip_address_array[1] = (ip_address & 0x00ff0000) >> 16;
    ip_address_array[2] = (ip_address & 0x0000ff00) >>  8;
    ip_address_array[3] = (ip_address & 0x000000ff) >>  0;
#endif
    task_info->ip_address = ip_address_array;

    /* get/set FPS averatge info */
    float tmp_fps = 0;
    for(int i = 0; i < MAX_HISTORY_FPS_INDEX; i++)
    {
        tmp_fps += task_info->history_fps[i];
    }
    task_info->average_fps = tmp_fps / MAX_HISTORY_FPS_INDEX;

    vPortFree(stats_buffer);
}

