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
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : sdcard_task.c
 * Description  :
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 31.01.2020 1.00 First Release
 *********************************************************************************************************************/

#include <stdio.h>

#include "r_smc_entry.h"
#include "r_tfat_lib.h"
#include "r_sdhi_rx_if.h"
#include "r_pinset.h"
#include "r_sdc_sd_rx_if.h"
#include "r_sdc_sd_rx_config.h"
#include "r_sys_time_rx_if.h"
#include "r_cmt_rx_if.h"
#include "r_sdc_sdmem_rx_demo_pin.h"
#include "r_tfat_driver_rx_if.h"
#include "r_sys_time_rx_if.h"

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/
uint32_t g_sdc_sd_work[200];
uint32_t g_sdc_sd_card_no;

extern FATFS fatfs;
extern FILINFO filinfo;
extern DIR dir;

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
void sdcard_init(void);
void sdcard_task( void * pvParameters );
sdc_sd_status_t r_sdc_sd_callback(int32_t channel);
static void TFAT_sample(void);
static void R_error(uint8_t err_code);
static void trap(void);

//extern void firmware_update_list_add(char *pstring);
//extern void firmware_update_list_clear(void);
//extern void firmware_update_update_file_search(void);
extern void R_SDHI_PinSetTransfer(void);
extern void R_SDHI_PinSetInit(void);
extern void software_reset(void);

void sdcard_init(void)
{
    R_SDHI_PinSetInit();

    R_SYS_TIME_RegisterPeriodicCallback(R_SDC_SD_1msInterval, 1);

    g_sdc_sd_card_no = 0;
    r_sdc_sdmem_demo_power_init(g_sdc_sd_card_no);
    R_SDC_SD_Open(g_sdc_sd_card_no, 0, &g_sdc_sd_work);
}

void sdcard_task( void * pvParameters )
{
    sdc_sd_status_t sdc_sd_status = SDC_SD_SUCCESS;
    static sdc_sd_status_t sdc_sd_card_detection = SDC_SD_ERR;
    static sdc_sd_status_t previous_sdc_sd_card_detection = SDC_SD_ERR;
    sdc_sd_cfg_t sdc_sd_config;

    volatile uint32_t gui_delay_counter;
    uint32_t progress, previous_progress;
    char buff[256];
    uint32_t firmware_update_complete_flag = 0;
    uint32_t firmware_update_status;
    uint32_t firmware_update_finish_status;

    sdc_sd_card_detection = R_SDC_SD_GetCardDetection(g_sdc_sd_card_no);

    if(previous_sdc_sd_card_detection != sdc_sd_card_detection)
    {
        previous_sdc_sd_card_detection = sdc_sd_card_detection;
        if(SDC_SD_SUCCESS == sdc_sd_card_detection)
        {
            printf("Detected attached SD card.\n");
            r_sdc_sdmem_demo_power_on(g_sdc_sd_card_no);
            R_SDHI_PinSetTransfer();
            R_SDC_SD_IntCallback(g_sdc_sd_card_no, r_sdc_sd_callback);
            sdc_sd_config.mode     = SDC_SD_CFG_DRIVER_MODE;
            sdc_sd_config.voltage  = SDC_SD_VOLT_3_3;
            sdc_sd_status = R_SDC_SD_Initialize(g_sdc_sd_card_no, &sdc_sd_config, SDC_SD_MODE_MEM);
            if(SDC_SD_SUCCESS != sdc_sd_status)
            {
                trap();
            }
            R_tfat_f_mount(TFAT_DRIVE_NUM_0, &fatfs);
            //firmware_update_update_file_search();
        }
        else
        {
            previous_sdc_sd_card_detection = sdc_sd_card_detection;
            if(SDC_SD_SUCCESS != sdc_sd_card_detection)
            {
                //firmware_update_list_clear();
                r_sdc_sdmem_demo_power_off(g_sdc_sd_card_no);
            }
        }
    }

    //storage_benchmark_main();
    progress = firmware_update();
    load_firmware_status(&firmware_update_status, &firmware_update_finish_status);

    if(true == is_firmupdating())
    {
        nop();
        firmware_update_complete_flag = 0;
        if(previous_progress != progress)
        {
            previous_progress = progress;
            sprintf(buff, "installed %d %%\r\n", progress);
            firmware_update_log_string(buff);
            firmware_update_temporary_area_string(progress, (int)((((float)(progress)/100)*(float)(get_update_data_size()))/1024), (int)((float)(get_update_data_size()))/1024);
        }
    }
    else if(true == is_firmupdatewaitstart())
    {
        firmware_update_complete_flag = 0;
    }
    else
    {
        if(firmware_update_status == firmware_update_finish_status)
        {
            if(0 == firmware_update_complete_flag)
            {
                //firmware_update_ok_after_message();
                firmware_update_complete_flag = 1;
                software_reset();
            }
        }
        else
        {
            if(0 == firmware_update_complete_flag)
            {
                //firmware_update_ng_after_message();
                firmware_update_status_initialize();
                firmware_update_complete_flag = 1;
            }
        }
    }

    if(!(gui_delay_counter++ % 1000))
    {
        GUI_Delay(1);
    }

    //vTaskDelay(1);
}

/* End of function r_sdc_sd_callback() */
/*******************************************************************************
* Declaration  : sdc_sd_status_t r_sdc_sd_callback(int32_t channel)
* Function Name: r_sdc_sd_callback
* Description  : SD memory card callback function
* Arguments    : Error code
* Return Value : none
*******************************************************************************/
sdc_sd_status_t r_sdc_sd_callback(int32_t channel)
{
    return SDC_SD_SUCCESS;
}
/* End of function r_sdc_sd_callback() */
/*******************************************************************************
* Declaration  : static void TFAT_sample(void)
* Function Name: TFAT_sample
* Description  : TFAT sample
* Arguments    : none
* Return Value : none
*******************************************************************************/

/*******************************************************************************
* Declaration  : static void R_error(uint8_t err_code)
* Function Name: R_error
* Description  : Error handling function
* Arguments    : Error code
* Return Value : none
*******************************************************************************/
static void R_error(uint8_t err_code)
{
    uint8_t    i= 1;
    printf("!!!!! TFAT Error. !!!!!\n");
    switch (err_code)
    {
        case 1:
            /* Error while opening the file for writing */
            while (i);
            break;
        case 2:
            /* Write function was not successful or all the bytes were not written to the file */
            while (i);
            break;
        case 3:
            /* Error while opening the file for reading */
            while (i);
            break;
        case 4:
            /* Error during the file read operation    */
            while (i);
            break;
        case 5:
            /* File written on the CARD does not match filebuffer[] contents */
            while (i);
            break;
        case 6:
            /* R_tfat_f_mount function failed */
            while (i);
            break;
    }
}
/* End of function R_error() */

/*******************************************************************************
* Declaration  : void trap(void)
* Function Name: trap
* Description  : Error
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void trap(void)
{
    sdc_sd_status_t err_code;

    err_code = R_SDC_SD_GetErrCode(g_sdc_sd_card_no);

    R_SDC_SD_Log(0x00000001, 0x0000003f, 0x0001ffff);

    while(err_code);
} /* End of function trap() */
