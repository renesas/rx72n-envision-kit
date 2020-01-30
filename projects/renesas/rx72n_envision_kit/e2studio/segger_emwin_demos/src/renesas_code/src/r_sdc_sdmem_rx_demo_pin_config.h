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
* Copyright (C) 2014(2015-2018) Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/

/**********************************************************************************************************************
* File Name    : r_sdc_sdmem_rx_demo_pin_config.h
* Device       : RX
* Tool-Chain   : RX Family C Compiler
* H/W Platform : RSK board for RX
* Description  : Sample
***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 03.09.2014 1.00    First Release
*              : 31.07.2017 2.00    Changed prefix from SDHI to SDC_SD.
*              :                    Deleted pin setting for CD, WP, D0-D4, CMD and CLK, because pin setting
*              :                    is executed by SD Card driver.
*              : 31.12.2017 2.01    The sample program corresponded to the pin setting of Smart Configurator.
*              : 29.06.2018 2.02    The sample program corresponded to the DMA/DTC of Smart Configurator.
**********************************************************************************************************************/

#ifndef _SDC_SDMEM_RX_DEMO_PIN_CONFIG_H_
#define _SDC_SDMEM_RX_DEMO_PIN_CONFIG_H_

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "r_sdhi_rx_config.h"

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
/* ==== #define for the card power supply ==== */
/* If do not need the port for the SD card power supply control,
   please enable #define SDC_SD_CFG_POWER_PORT_NONE. */
/* #define SDC_SD_CFG_POWER_PORT_NONE */

#if !defined(SDC_SD_CFG_POWER_PORT_NONE)
/* 0 : Supplies Low to a port controlling a card power supply circuit.
   1 : Supplies High to a port controlling a card power supply circuit. */
#define SDC_SD_CFG_POWER_HIGH_ACTIVE    (1)
#endif /* #if !defined(SDC_SD_CFG_POWER_PORT_NONE) */

/* ==== #define for Supplies the Power to the SD Card and waits ==== */
#define SDC_SD_CFG_POWER_ON_WAIT        (100)   /* Uint : ms */
#define SDC_SD_CFG_POWER_OFF_WAIT       (100)   /* Uint : ms */

/* ==== The #defines specify the ports used for card power supply. ==== */
/* ==== SDHI Card No. 0 ==== */
#define R_SDC_SD_CFG_POWER_CARD0_PORT   ('4')   /* Port Number D*/
#define R_SDC_SD_CFG_POWER_CARD0_BIT    ('2')   /* Bit Number  0*/

/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/

#endif /* _SDC_SDMEM_RX_DEMO_PIN_CONFIG_H_ */

