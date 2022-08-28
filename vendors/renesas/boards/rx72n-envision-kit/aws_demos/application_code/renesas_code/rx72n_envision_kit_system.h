
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
#include "r_simple_filesystem_on_dataflash_if.h"
#include "Pin.h"

#define DATAFLASH_LABEL_NAME_TIMEZONE "timezone"
#define DATAFLASH_LABEL_NAME_CLIENT_PRIVATE_KEY "client_private_key"
#define DATAFLASH_LABEL_NAME_CLIENT_CERTIFICATE "client_certificate"
#define DATAFLASH_LABEL_NAME_IOT_THING_NAME "iot_thing_name"
#define DATAFLASH_LABEL_NAME_MQTT_BROKER_ENDPOINT "mqtt_broker_endpoint"
#define DATAFLASH_LABEL_NAME_CODE_SIGNER_CERTIFICATE "code_signer_certificate"
#define DATAFLASH_LABEL_NAME_TCP_SEND_PERFORMANCE_SERVER_IP_ADDRESS "tcp_send_performance_server_ip_address"
#define DATAFLASH_LABEL_NAME_TCP_SEND_PERFORMANCE_SERVER_PORT_NUMBER "tcp_send_performance_server_port_number"
#define DATAFLASH_LABEL_NAME_TRACEALYZER_SERVER_IP_ADDRESS "tracealyzer_server_ip_address"
#define DATAFLASH_LABEL_NAME_TRACEALYZER_SERVER_PORT_NUMBER "tracealyzer_server_port_number"

#define MAX_HISTORY_FPS_INDEX 300 /* for 3 seconds average */
#define UNIQUE_ID_LENGTH 16
#define EMWIN_VERSION_STRING_LENGTH 16
#define FIRMWARE_VERSION_STRING_LENGTH 16

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/
typedef struct _hardware_info
{
	char *cpu_name;
	char *memory_size;
	char *frequency;
	char *crypto;
	char *board_capability;
	char *unique_id;
}HARDWARE_INFO;

typedef struct _software_info
{
	char *firmware_version;
	char *amazon_freertos_version;
	char *emwin_version;
	char *compiled_time;
}SOFTWARE_INFO;

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
    TaskHandle_t audio_task_handle;

    /* for emWin */
    WM_HWIN hWin_serial_terminal;
    WM_HWIN hWin_d2_audio;
    WM_HWIN hWin_firmware_update_via_sd_card;
    WM_HWIN hWin_task_manager;
    WM_HWIN hWin_system_log;
    WM_HWIN hWin_frame;
    WM_HWIN hWin_title_logo;

    /* for demo system time */
    SYS_TIME sys_time;

    /* for cpuload info */
    uint32_t cpu_load;

    /* for sd card info */
    uint32_t sd_status;        // attach = 1 or detach = 0 (default)

    /* for IP address info (4bytes array pointer) */
    uint8_t *ip_address;

    /* for frame per second info */
    float history_fps[MAX_HISTORY_FPS_INDEX];
    uint32_t history_fps_index;
    float current_fps;
    float average_fps;

    /* for static Software info */
    SOFTWARE_INFO software_info;

    /* for static Hardware info */
    HARDWARE_INFO hardware_info;

    /* system flag */
    volatile uint32_t gui_initialize_complete_flag;

}TASK_INFO;

extern const char timezone_label[];
extern const char client_private_key_label[];
extern const char client_certificate_label[];
extern const char iot_thing_name_label[];
extern const char mqtt_broker_endpoint_label[];
extern const char code_signer_certificate_label[];
extern const char tcp_send_performance_server_ip_address_label[];
extern const char tcp_send_performance_server_port_number_label[];
extern const char tracealyzer_server_ip_address_label[];
extern const char tracealyzer_server_port_number_label[];

extern const char cpu_name_string[];
extern const char memory_size_string[];
extern const char frequency_string[];
extern const char crypto_string[];
extern const char board_capability_string[];
extern char unique_id_string[];

extern char *firmware_version_string;
extern const char amazon_freertos_version_string[];
extern char emwin_version_string[];
extern const char compiled_time_string[];

extern char *iot_thing_name;
extern char *mqtt_broker_endpoint;
extern char *signing_certificate_pem;

extern xSemaphoreHandle xSemaphoreCodeFlashAccess;
extern FATFS fatfs;
extern FILINFO filinfo;
extern DIR dir;

extern TASK_INFO * get_task_info(void);
