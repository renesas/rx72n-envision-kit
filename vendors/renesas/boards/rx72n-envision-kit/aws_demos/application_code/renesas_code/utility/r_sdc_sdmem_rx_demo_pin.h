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
* File Name    : r_sdc_sdmem_rx_demo_pin.h
* Device       : RX
* Tool-Chain   : RX Family C Compiler
* H/W Platform : RSK board for RX
* Description  : Sample
***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 03.09.2014 1.00    First Release
*              : 31.07.2017 2.00    Changed prefix from SDHI to SDC_SD.
*              : 31.12.2017 2.01    The sample program corresponded to the pin setting of Smart Configurator.
**********************************************************************************************************************/

#ifndef _SDC_SDMEM_RX_DEMO_PIN_H
#define _SDC_SDMEM_RX_DEMO_PIN_H

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "r_sdc_sdmem_rx_demo_pin_config.h"

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
/* **** Definition of bit of Open-Drain Control Register. **** */

/* ==== SDHI Card_No 0 ==== */
#if !defined(SDC_SD_CFG_POWER_PORT_NONE)
    /* ---- SDHI_POWER  ---- */
    #if ('0' == R_SDC_SD_CFG_POWER_CARD0_PORT)
        #define R_SDC_SD_CFG_POWER_CARD0_PORT_SFR         0
    #elif ('1' == R_SDC_SD_CFG_POWER_CARD0_PORT)
        #define R_SDC_SD_CFG_POWER_CARD0_PORT_SFR         1
    #elif ('2' == R_SDC_SD_CFG_POWER_CARD0_PORT)
        #define R_SDC_SD_CFG_POWER_CARD0_PORT_SFR         2
    #elif ('3' == R_SDC_SD_CFG_POWER_CARD0_PORT)
        #define R_SDC_SD_CFG_POWER_CARD0_PORT_SFR         3
    #elif ('4' == R_SDC_SD_CFG_POWER_CARD0_PORT)
        #define R_SDC_SD_CFG_POWER_CARD0_PORT_SFR         4
    #elif ('5' == R_SDC_SD_CFG_POWER_CARD0_PORT)
        #define R_SDC_SD_CFG_POWER_CARD0_PORT_SFR         5
    #elif ('6' == R_SDC_SD_CFG_POWER_CARD0_PORT)
        #define R_SDC_SD_CFG_POWER_CARD0_PORT_SFR         6
    #elif ('7' == R_SDC_SD_CFG_POWER_CARD0_PORT)
        #define R_SDC_SD_CFG_POWER_CARD0_PORT_SFR         7
    #elif ('8' == R_SDC_SD_CFG_POWER_CARD0_PORT)
        #define R_SDC_SD_CFG_POWER_CARD0_PORT_SFR         8
    #elif ('9' == R_SDC_SD_CFG_POWER_CARD0_PORT)
        #define R_SDC_SD_CFG_POWER_CARD0_PORT_SFR         9
    #elif (('A' == R_SDC_SD_CFG_POWER_CARD0_PORT) || ('a' == R_SDC_SD_CFG_POWER_CARD0_PORT))
        #define R_SDC_SD_CFG_POWER_CARD0_PORT_SFR         A
    #elif (('B' == R_SDC_SD_CFG_POWER_CARD0_PORT) || ('b' == R_SDC_SD_CFG_POWER_CARD0_PORT))
        #define R_SDC_SD_CFG_POWER_CARD0_PORT_SFR         B
    #elif (('C' == R_SDC_SD_CFG_POWER_CARD0_PORT) || ('c' == R_SDC_SD_CFG_POWER_CARD0_PORT))
        #define R_SDC_SD_CFG_POWER_CARD0_PORT_SFR         C
    #elif (('D' == R_SDC_SD_CFG_POWER_CARD0_PORT) || ('d' == R_SDC_SD_CFG_POWER_CARD0_PORT))
        #define R_SDC_SD_CFG_POWER_CARD0_PORT_SFR         D
    #elif (('E' == R_SDC_SD_CFG_POWER_CARD0_PORT) || ('e' == R_SDC_SD_CFG_POWER_CARD0_PORT))
        #define R_SDC_SD_CFG_POWER_CARD0_PORT_SFR         E
    #elif (('F' == R_SDC_SD_CFG_POWER_CARD0_PORT) || ('f' == R_SDC_SD_CFG_POWER_CARD0_PORT))
        #define R_SDC_SD_CFG_POWER_CARD0_PORT_SFR         F
    #elif (('G' == R_SDC_SD_CFG_POWER_CARD0_PORT) || ('g' == R_SDC_SD_CFG_POWER_CARD0_PORT))
        #define R_SDC_SD_CFG_POWER_CARD0_PORT_SFR         G
    #elif (('J' == R_SDC_SD_CFG_POWER_CARD0_PORT) || ('j' == R_SDC_SD_CFG_POWER_CARD0_PORT))
        #define R_SDC_SD_CFG_POWER_CARD0_PORT_SFR         J
    #endif

    #if ('0' == R_SDC_SD_CFG_POWER_CARD0_BIT)
        #define R_SDC_SD_CFG_POWER_CARD0_BIT_SFR          0
    #elif ('1' == R_SDC_SD_CFG_POWER_CARD0_BIT)
        #define R_SDC_SD_CFG_POWER_CARD0_BIT_SFR          1
    #elif ('2' == R_SDC_SD_CFG_POWER_CARD0_BIT)
        #define R_SDC_SD_CFG_POWER_CARD0_BIT_SFR          2
    #elif ('3' == R_SDC_SD_CFG_POWER_CARD0_BIT)
        #define R_SDC_SD_CFG_POWER_CARD0_BIT_SFR          3
    #elif ('4' == R_SDC_SD_CFG_POWER_CARD0_BIT)
        #define R_SDC_SD_CFG_POWER_CARD0_BIT_SFR          4
    #elif ('5' == R_SDC_SD_CFG_POWER_CARD0_BIT)
        #define R_SDC_SD_CFG_POWER_CARD0_BIT_SFR          5
    #elif ('6' == R_SDC_SD_CFG_POWER_CARD0_BIT)
        #define R_SDC_SD_CFG_POWER_CARD0_BIT_SFR          6
    #elif ('7' == R_SDC_SD_CFG_POWER_CARD0_BIT)
        #define R_SDC_SD_CFG_POWER_CARD0_BIT_SFR          7
    #endif
#endif  /* SDC_SD_CFG_POWER_PORT_NONE */

/* **** Definitions of string conversion to access I/O registers. **** */
#define SFR1_PDR( x,y )        PORT ## x.PDR.BIT.B ## y        /* String "PORTx.PDR.BIT.By"    */
#define SFR2_PDR( x,y )        SFR1_PDR( x , y )               /* Refer to __PDR( x )          */
#define SFR1_PIDR( x,y )       PORT ## x.PIDR.BIT.B ## y       /* String "PORTx.PIDR.BIT.By"   */
#define SFR2_PIDR( x,y )       SFR1_PIDR( x , y )              /* Refer to __PIDR( x )         */
#define SFR1_PODR( x,y )       PORT ## x.PODR.BIT.B ## y       /* String "PORTx.PODR.BIT.By"   */
#define SFR2_PODR( x,y )       SFR1_PODR( x , y )              /* Refer to __PODR( x )         */
#define SFR1_PMR( x,y )        PORT ## x.PMR.BIT.B ## y        /* String "PORTx.PMR.BIT.By"    */
#define SFR2_PMR( x,y )        SFR1_PMR( x , y )               /* Refer to __PMR( x )          */
#define SFR1_DSCR( x,y )       PORT ## x.DSCR.BIT.B ## y       /* String "PORTx.DCSR.BIT.By"   */
#define SFR2_DSCR( x,y )       SFR1_DSCR( x , y )              /* Refer to __DSCR( x )         */
#define SFR1_MPC( x,y )        MPC.P ## x ## y ## PFS.BYTE     /* String "MPC.PxyPFS.BYTE"     */
#define SFR2_MPC( x,y )        SFR1_MPC( x , y )               /* Refer to __MPC( x )          */
#define SFR1_PCR( x,y )        PORT ## x.PCR.BIT.B ## y        /* String "PORTx.PCR.BIT.By"    */
#define SFR2_PCR( x,y )        SFR1_PCR( x , y )               /* Refer to __PCR( x )          */

/* **** Definitions of I/O registers of ports used for SDHI. **** */

/* ==== SDHI Card_No 0 ==== */
/* ---- Registers of the port used as SDHI_POWER. ---- */
#if !defined(SDC_SD_CFG_POWER_PORT_NONE)
#define SDHI_POWER_CARD0_PDR    SFR2_PDR(R_SDC_SD_CFG_POWER_CARD0_PORT_SFR, R_SDC_SD_CFG_POWER_CARD0_BIT_SFR)
#define SDHI_POWER_CARD0_PODR   SFR2_PODR(R_SDC_SD_CFG_POWER_CARD0_PORT_SFR, R_SDC_SD_CFG_POWER_CARD0_BIT_SFR)
#define SDHI_POWER_CARD0_PMR    SFR2_PMR(R_SDC_SD_CFG_POWER_CARD0_PORT_SFR, R_SDC_SD_CFG_POWER_CARD0_BIT_SFR)
#define SDHI_POWER_CARD0_PCR    SFR2_PCR(R_SDC_SD_CFG_POWER_CARD0_PORT_SFR, R_SDC_SD_CFG_POWER_CARD0_BIT_SFR)
#endif  /* SDC_SD_CFG_POWER_PORT_NONE */


/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/
/* Available delay units. */
typedef enum
{
    SDC_SD_DELAY_MICROSECS = 1000000,  /* Requested delay amount is in microseconds */
    SDC_SD_DELAY_MILLISECS = 1000,     /* Requested delay amount is in milliseconds */
    SDC_SD_DELAY_SECS = 1              /* Requested delay amount is in seconds */
} sdc_sd_delay_units_t;

/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_demo_power_init(uint32_t card_no);
sdc_sd_status_t r_sdc_sdmem_demo_power_on(uint32_t card_no);
sdc_sd_status_t r_sdc_sdmem_demo_power_off(uint32_t card_no);
bool            r_sdc_sdmem_demo_softwaredelay(uint32_t delay, sdc_sd_delay_units_t units);

#endif /* _SDC_SDMEM_RX_DEMO_PIN_H */

