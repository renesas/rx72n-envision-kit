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
 * File Name    : switch_button_function.c
 * Description  :
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 31.01.2020 1.00 First Release
 *********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
#include "platform.h"
#include "r_flash_rx_if.h"
#include "r_irq_rx_if.h"
#include "r_irq_rx_pinset.h"
#include "r_tfat_lib.h"

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
#define LONG_PRESS_COUNT_VALUE      5000
#define BUTTON_MASHING_COUNT_VALUE  3000
#define SWITCH_RISING_COUNT_VALUE   3
#define IRQ_LOW     0
#define IRQ_HIGH    1
#define UPDATE_FILE_NAME "userprog.rsu"

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/
int in_main_menu_flag;
FATFS fatfs;
FILINFO filinfo;
DIR dir;

void SwitchButtonInit(void);
void CheckPressedState(void);
void CheckFirmUpdateState(void);

extern void bank_swap_with_software_reset(void);
extern uint32_t firmware_update(void);
extern void firmware_update_request(char *string);
extern void load_firmware_status(uint32_t *now_status, uint32_t *finish_status);

/***********************************************************************************************************************
 Private global variables and functions
 ***********************************************************************************************************************/
static irq_handle_t  my_irq_handle;
static uint8_t in_sw_active = 0;
static uint32_t in_sw_rising_count = 0;

static void my_irq_callback(void *pdata);

void SwitchButtonInit(void)
{
    R_ICU_PinSet();
    R_IRQ_Open(IRQ_NUM_15, IRQ_TRIG_FALLING, IRQ_PRI_3, &my_irq_handle, my_irq_callback);
}

void CheckPressedState(void)
{
    static uint32_t long_press_count = 0;
    irq_trigger_t my_trig_mode;

    if (in_main_menu_flag)
    {
        if (in_sw_active)
        {
            if ((BUTTON_MASHING_COUNT_VALUE > long_press_count) && (SWITCH_RISING_COUNT_VALUE <= in_sw_rising_count))
            {
                bank_swap_with_software_reset();
                while(1);
            }
            else if ((LONG_PRESS_COUNT_VALUE <= long_press_count) && (0 == in_sw_rising_count))
            {
                R_IRQ_InterruptEnable(my_irq_handle, false);
                long_press_count = 0;
                in_sw_active = 0;
                in_sw_rising_count = 0;
                firmware_update();
                firmware_update_request(UPDATE_FILE_NAME);
            }
            else if ((BUTTON_MASHING_COUNT_VALUE <= long_press_count) && 
                     (0 < in_sw_rising_count) && (SWITCH_RISING_COUNT_VALUE > in_sw_rising_count))
            {
                R_IRQ_InterruptEnable(my_irq_handle, false);
                long_press_count = 0;
                in_sw_active = 0;
                in_sw_rising_count = 0;
                my_trig_mode = IRQ_TRIG_FALLING;
                R_IRQ_Control(my_irq_handle, IRQ_CMD_SET_TRIG, &my_trig_mode);
                R_IRQ_InterruptEnable(my_irq_handle, true);
            }
            else
            {
                long_press_count++;
            }
        }
        else
        {
            long_press_count = 0;
            in_sw_rising_count = 0;
        }
    }
}

static void my_irq_callback(void *pdata)
{
    irq_number_t  my_irq_number;
    irq_trigger_t my_trig_mode;
    uint8_t       level;

    my_irq_number = *((irq_number_t *)pdata);
    if (IRQ_NUM_15 == my_irq_number)
    {
        R_IRQ_ReadInput(my_irq_handle, &level);
        if (IRQ_LOW == level)
        {
            /* Change trigger mode to rising edge. */
            if (in_main_menu_flag)
            {
                in_sw_active = 1;
                in_sw_rising_count = 0;
                my_trig_mode = IRQ_TRIG_RISING;
                R_IRQ_Control(my_irq_handle, IRQ_CMD_SET_TRIG, &my_trig_mode);
            }
        }
        else
        {
            if (in_main_menu_flag)
            {
                in_sw_rising_count++;
            }
        }
    }
}

/* end of file */
