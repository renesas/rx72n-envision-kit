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
* Copyright (C) 2014(2015-2017) Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/
/**********************************************************************************************************************
* System Name  : SD Card Driver demo
* File Name    : r_sdc_sdmem_rx_demo_pin.c
* Version      : 2.00
* Device       : RX
* Abstract     : API & Sub module
* Tool-Chain   : For RX e2_studio
* OS           : not use
* H/W Platform : RSK board for RX
* Description  : Interface file for SDC SD API for RX
* Limitation   : None
**********************************************************************************************************************/
/**********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 03.09.2014 1.00    First Release
*              : 31.07.2017 2.00    Changed prefix from SDHI to SDC_SD.
*              : 31.12.2017 2.01    The sample program corresponded to the pin setting of Smart Configurator.
**********************************************************************************************************************/

/**********************************************************************************************************************
Includes <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "platform.h"
#include "r_sdc_sd_rx_if.h"
#include "r_sdc_sd_rx_config.h"
#include "r_sdc_sdmem_rx_demo_pin_config.h"
#include "r_sdc_sdmem_rx_demo_pin.h"

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
#define SDC_SD_OVERHEAD_LOOP_COUNT     (4)
        /* Overhead of 20 cycles or 4 loops to call/return. */
#define SDC_SD_CPU_CYCLES_PER_LOOP     (4)
        /* Known number (4) of CPU cycles required to execute the loop. */

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/

/**********************************************************************************************************************
* Outline      : Initialize Pin Setting Processing
* Function Name: r_sdc_sdmem_demo_power_init
* Description  : Initializes hardware to access SDHI.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        :  RX             SD Card
*              :  SDHI_CD   ---- Card Detect
*              :  SDHI_WP   ---- Write Protect
*              :  SDHI_D0   ---- DAT0
*              :  SDHI_D1   ---- DAT1
*              :  SDHI_D2   ---- DAT2
*              :  SDHI_D3   ---- DAT3
*              :  SDHI_CMD  ---- CMD
*              :  SDHI_CLK  ---- SDCLK
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_demo_power_init(uint32_t card_no)
{
    if (SDC_SD_CARD_NO0 == card_no)
    {
#if !defined(SDC_SD_CFG_POWER_PORT_NONE)
        SDHI_POWER_CARD0_PMR    = 0x00;
        SDHI_POWER_CARD0_PCR    = 0x00;
    #if (SDC_SD_CFG_POWER_HIGH_ACTIVE == 1)
        SDHI_POWER_CARD0_PODR   = 0x00;     /* SDHI_POWER off */
    #else
        SDHI_POWER_CARD0_PODR   = 0x01;     /* SDHI_POWER off */
    #endif  /* (SDC_SD_CFG_POWER_HIGH_ACTIVE == 1) */
        SDHI_POWER_CARD0_PDR    = 0x01;
#endif  /* #if !defined(SDC_SD_CFG_POWER_PORT_NONE) */
    }
    else
    {
        /* User must create processing for device number 1 or later. */
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sdmem_demo_power_init() */

/**********************************************************************************************************************
* Outline      : Power-on Processing
* Function Name: r_sdc_sdmem_demo_power_on
* Description  : Supplies the power to the SD Card and waits.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_demo_power_on(uint32_t card_no)
{
#if !defined(SDC_SD_CFG_POWER_PORT_NONE)
    /* ---- Power On ---- */
    if (SDC_SD_CARD_NO0 == card_no)
    {
#if (SDC_SD_CFG_POWER_HIGH_ACTIVE == 1)
        SDHI_POWER_CARD0_PODR   = 0x01;     /* SDHI_POWER on */
#else
        SDHI_POWER_CARD0_PODR   = 0x00;     /* SDHI_POWER on */
#endif  /* (SDC_SD_CFG_POWER_HIGH_ACTIVE == 1) */
        if (1 == SDHI_POWER_CARD0_PODR)
        {
            /* Wait for the write completion. */
            nop();
        }
    }
    else
    {
        /* User must create processing for device number 1 or later. */
    }
    
    /* ---- Supplies the Power to the SD Card and waits. ---- */
    if (true != r_sdc_sdmem_demo_softwaredelay(SDC_SD_CFG_POWER_ON_WAIT, SDC_SD_DELAY_MILLISECS))
    {
        return SDC_SD_ERR;
    }
#endif  /* #if !defined(SDC_SD_CFG_POWER_PORT_NONE) */
    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sdmem_demo_power_on() */

/**********************************************************************************************************************
* Outline      : Power-off Processing
* Function Name: r_sdc_sdmem_demo_power_off
* Description  : Stops supplying the power to the SD Card.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_demo_power_off(uint32_t card_no)
{
#if !defined(SDC_SD_CFG_POWER_PORT_NONE)
    /* ---- Power Off ---- */
    if (SDC_SD_CARD_NO0 == card_no)
    {
#if (SDC_SD_CFG_POWER_HIGH_ACTIVE == 1)
        SDHI_POWER_CARD0_PODR   = 0x00;     /* SDHI_POWER off */
#else
        SDHI_POWER_CARD0_PODR   = 0x01;     /* SDHI_POWER off */
#endif  /* (SDC_SD_CFG_POWER_HIGH_ACTIVE == 1) */
        if (1 == SDHI_POWER_CARD0_PODR)
        {
            /* Wait for the write completion. */
            nop();
        }
    }
    else
    {
        /* User must create processing for device number 1 or later. */
    }

    /* ---- Stops the Power to the SD Card and waits. ---- */
    if (true != r_sdc_sdmem_demo_softwaredelay(SDC_SD_CFG_POWER_OFF_WAIT, SDC_SD_DELAY_MILLISECS))
    {
        return SDC_SD_ERR;
    }
#endif  /* #if !defined(SDC_SD_CFG_POWER_PORT_NONE) */
    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sdmem_demo_power_off() */

/************************************************************************************************
* Function Name: r_sdc_sdmem_demo_delaywait
* Description  : This asm loop executes a known number (5) of CPU cycles.
*              ; If a value of '4' is passed in as an argument,
*              ; then this function would consume 20 CPU cycles before returning.
* Arguments    : unsigned long      loop_cnt            ;   The number of 'units' to delay.
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
#pragma inline_asm r_sdc_sdmem_demo_delaywait
static void r_sdc_sdmem_demo_delaywait (unsigned long loop_cnt)
{
    BRA ?+
    NOP
    ?:
    NOP
    SUB #01H, R1
    BNE ?-
} /* End of function r_sdc_sdmem_demo_delaywait() */

/************************************************************************************************
* Function Name: r_sdc_sdmem_demo_softwaredelay
* Description  : Delay the specified duration in units and return.
* Arguments    : uint32_t               delay           ;   The number of 'units' to delay.
*              : sdc_sd_delay_units_t     units           ;   the 'base' for the units specified.
*              :                                        ;   Valid values are: SDC_SD_DELAY_MICROSECS,
*              :                                        ;                     SDC_SD_DELAY_MILLISECS,
*              :                                        ;                     SDC_SD_DELAY_SECS
* Return Value : true           ;   if delay executed.
*              : false          ;   if delay/units combination resulted in overflow/underflow.
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
bool r_sdc_sdmem_demo_softwaredelay(uint32_t delay, sdc_sd_delay_units_t units)
{
    uint64_t loop_cnt;

    /* Calculate the number of loops, accounting for the overhead of 20 cycles (4 loops at 5 cycles/loop) */
    loop_cnt = (((uint64_t)delay * (uint64_t)BSP_ICLK_HZ) / (SDC_SD_CPU_CYCLES_PER_LOOP * units))
                - SDC_SD_OVERHEAD_LOOP_COUNT;

    /* Make sure the request is valid and did not result in an overflow. */
    if ((loop_cnt > 0xFFFFFFFF) || (loop_cnt == 0) ||
        ((units != SDC_SD_DELAY_MICROSECS) && (units != SDC_SD_DELAY_MILLISECS) && (units != SDC_SD_DELAY_SECS)))
    {
        return (false);
    }
    r_sdc_sdmem_demo_delaywait((uint32_t)loop_cnt);
    return (true);
} /* End of function r_sdc_sdmem_demo_softwaredelay() */

/* End of File */
