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
 * File Name    : serial_flash_task.c
 * Description  :
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 17.04.2020 1.00 First Release
 *********************************************************************************************************************/

#include "r_smc_entry.h"
#include "r_flash_spi_if.h"
#include "r_flash_spi_config.h"
#include "r_sys_time_rx_if.h"

/* for using C standard library */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* for using Amazon FreeRTOS */
#include "FreeRTOS.h"

/* for RX72N Envision Kit system common header */
#include "rx72n_envision_kit_system.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define SERIAL_FLASH_TASK_DATA_SIZE (0x00100000)
#define SERIAL_FLASH_64KB_SIZE (0x00010000)
#define SERIAL_FLASH_PAGE_SIZE (256)

/*******************************************************************************
Typedef definitions
*******************************************************************************/
typedef enum e_serial_flash_state
{
    SERIAL_FLASH_STATE_ERASE,
    SERIAL_FLASH_STATE_ERASE_WAIT_COMPLETE,
    SERIAL_FLASH_STATE_WRITE,
    SERIAL_FLASH_STATE_WRITE_WAIT_COMPLETE,
    SERIAL_FLASH_STATE_READ,
    SERIAL_FLASH_STATE_FINISH,
    SERIAL_FLASH_STATE_ERROR
} serial_flash_state_t;

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/
const uint32_t cbuf1[SERIAL_FLASH_PAGE_SIZE/sizeof(uint32_t)] = {
        0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0,
        0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0,
        0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0,
        0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0,
        0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0,
        0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0,
        0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0,
        0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0, 0x12345678, 0x9abcdef0
};
static uint32_t cbuf2[SERIAL_FLASH_PAGE_SIZE/sizeof(uint32_t)];
static uint8_t  gDevNo;
static uint8_t  gStat;
static serial_flash_state_t g_serial_flash_state;

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
void serial_flash_task( void * pvParameters );
static serial_flash_state_t serial_flash_update(void);

void serial_flash_task( void * pvParameters )
{
    serial_flash_state_t state;

    /* wait completing gui initializing */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    R_SYS_TIME_RegisterPeriodicCallback(R_FLASH_SPI_1ms_Interval, 1);

    gDevNo = FLASH_SPI_DEV0;
    R_FLASH_SPI_Open(gDevNo);
    R_FLASH_SPI_Set_4byte_Address_Mode(gDevNo);
    R_FLASH_SPI_Quad_Enable(gDevNo);
    R_FLASH_SPI_Read_Status(gDevNo, &gStat); // debug (gStat & 0x40) != 0x00
    R_FLASH_SPI_Set_Write_Protect(gDevNo, 0);
    R_FLASH_SPI_Read_Status(gDevNo, &gStat); // debug gStat & (0x0f << 2) !=0x00
    g_serial_flash_state = SERIAL_FLASH_STATE_ERASE;
    while(1)
    {
        serial_flash_update();
        vTaskDelay(1000);
    }
}
/* End of function serial_flash_task() */

static serial_flash_state_t serial_flash_update(void)
{
    static uint32_t serial_flash_address = 0;
    static flash_spi_info_t Flash_Info_W;
    static flash_spi_info_t Flash_Info_R;
    static flash_spi_erase_info_t Flash_Info_E;

    switch (g_serial_flash_state)
    {
        case SERIAL_FLASH_STATE_ERASE:
            Flash_Info_E.addr   = serial_flash_address;
            Flash_Info_E.mode   = FLASH_SPI_MODE_B64K_ERASE;
            if (R_FLASH_SPI_Erase(gDevNo, &Flash_Info_E) == FLASH_SPI_SUCCESS)
            {
                g_serial_flash_state = SERIAL_FLASH_STATE_ERASE_WAIT_COMPLETE;
            }
            else
            {
                g_serial_flash_state = SERIAL_FLASH_STATE_ERROR;
            }
            break;
        case SERIAL_FLASH_STATE_ERASE_WAIT_COMPLETE:
            if (R_FLASH_SPI_Polling(gDevNo, FLASH_SPI_MODE_ERASE_POLL) == FLASH_SPI_SUCCESS)
            {
				serial_flash_address += SERIAL_FLASH_64KB_SIZE;
				if (SERIAL_FLASH_TASK_DATA_SIZE == serial_flash_address)
				{
					serial_flash_address = 0;
					Flash_Info_W.cnt     = SERIAL_FLASH_TASK_DATA_SIZE;
					g_serial_flash_state = SERIAL_FLASH_STATE_WRITE;
				}
				else
				{
					g_serial_flash_state = SERIAL_FLASH_STATE_ERASE;
				}
            }
            break;
        case SERIAL_FLASH_STATE_WRITE:
            Flash_Info_W.addr    = serial_flash_address;
            Flash_Info_W.p_data  = (uint8_t *)cbuf1;
            Flash_Info_W.op_mode = FLASH_SPI_QUAD;
            if (R_FLASH_SPI_Write_Data_Page(gDevNo, &Flash_Info_W) == FLASH_SPI_SUCCESS)
            {
                g_serial_flash_state = SERIAL_FLASH_STATE_WRITE_WAIT_COMPLETE;
            }
            else
            {
                g_serial_flash_state = SERIAL_FLASH_STATE_ERROR;
            }
            break;
        case SERIAL_FLASH_STATE_WRITE_WAIT_COMPLETE:
            if (R_FLASH_SPI_Polling(gDevNo, FLASH_SPI_MODE_PROG_POLL) == FLASH_SPI_SUCCESS)
            {
				serial_flash_address += SERIAL_FLASH_PAGE_SIZE;
				if (SERIAL_FLASH_TASK_DATA_SIZE == serial_flash_address)
				{
					serial_flash_address = 0;
					g_serial_flash_state = SERIAL_FLASH_STATE_READ;
				}
				else
				{
					g_serial_flash_state = SERIAL_FLASH_STATE_WRITE;
				}
            }
            break;
        case SERIAL_FLASH_STATE_READ:
            Flash_Info_R.cnt     = SERIAL_FLASH_PAGE_SIZE;
            Flash_Info_R.addr    = serial_flash_address;
            Flash_Info_R.p_data  = (uint8_t *)cbuf2;
            Flash_Info_R.op_mode = FLASH_SPI_QUAD;
            if (R_FLASH_SPI_Read_Data(gDevNo, &Flash_Info_R) == FLASH_SPI_SUCCESS)
            {
				serial_flash_address += SERIAL_FLASH_PAGE_SIZE;
				if (SERIAL_FLASH_TASK_DATA_SIZE == serial_flash_address)
				{
                     serial_flash_address = 0;
                     g_serial_flash_state = SERIAL_FLASH_STATE_FINISH;
                }
            }
            else
            {
                g_serial_flash_state = SERIAL_FLASH_STATE_ERROR;
            }
            break;
        case SERIAL_FLASH_STATE_FINISH:
            /* If do not want to repeat SERIAL_FLASH_STATE_FINISH, update the state. */
            break;
        case SERIAL_FLASH_STATE_ERROR:
            R_FLASH_SPI_Write_Di(gDevNo);
            break;
        default:
            break;
    }
    return g_serial_flash_state;
}
/* End of function serial_flash_update() */
