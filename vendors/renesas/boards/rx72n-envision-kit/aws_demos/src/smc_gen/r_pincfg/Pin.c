/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : Pin.c
* Version      : 1.0.2
* Device(s)    : R5F572NNHxFB
* Description  : This file implements SMC pin code generation.
* Creation Date: 2019-12-29
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Pins_Create
* Description  : This function initializes Smart Configurator pins
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Pins_Create(void)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set CLKOUT25M pin */
    MPC.P56PFS.BYTE = 0x2AU;
    PORT5.PMR.BYTE |= 0x40U;

    /* Set CTS1# pin */
    MPC.P31PFS.BYTE = 0x0BU;
    PORT3.PMR.BYTE |= 0x02U;

    /* Set ET0_COL pin */
    MPC.PC7PFS.BYTE = 0x11U;
    PORTC.PMR.BYTE |= 0x80U;

    /* Set ET0_CRS pin */
    MPC.P83PFS.BYTE = 0x11U;
    PORT8.PMR.BYTE |= 0x08U;

    /* Set ET0_ERXD0 pin */
    MPC.P75PFS.BYTE = 0x11U;
    PORT7.PMR.BYTE |= 0x20U;

    /* Set ET0_ERXD1 pin */
    MPC.P74PFS.BYTE = 0x11U;
    PORT7.PMR.BYTE |= 0x10U;

    /* Set ET0_ERXD2 pin */
    MPC.PC1PFS.BYTE = 0x11U;
    PORTC.PMR.BYTE |= 0x02U;

    /* Set ET0_ERXD3 pin */
    MPC.PC0PFS.BYTE = 0x11U;
    PORTC.PMR.BYTE |= 0x01U;

    /* Set ET0_ETXD0 pin */
    MPC.P81PFS.BYTE = 0x11U;
    PORT8.PMR.BYTE |= 0x02U;

    /* Set ET0_ETXD1 pin */
    MPC.P82PFS.BYTE = 0x11U;
    PORT8.PMR.BYTE |= 0x04U;

    /* Set ET0_ETXD2 pin */
    MPC.PC5PFS.BYTE = 0x11U;
    PORTC.PMR.BYTE |= 0x20U;

    /* Set ET0_ETXD3 pin */
    MPC.PC6PFS.BYTE = 0x11U;
    PORTC.PMR.BYTE |= 0x40U;

    /* Set ET0_MDC pin */
    MPC.P72PFS.BYTE = 0x11U;
    PORT7.PMR.BYTE |= 0x04U;

    /* Set ET0_MDIO pin */
    MPC.P71PFS.BYTE = 0x11U;
    PORT7.PMR.BYTE |= 0x02U;

    /* Set ET0_RX_CLK pin */
    MPC.P76PFS.BYTE = 0x11U;
    PORT7.PMR.BYTE |= 0x40U;

    /* Set ET0_RX_DV pin */
    MPC.PC2PFS.BYTE = 0x11U;
    PORTC.PMR.BYTE |= 0x04U;

    /* Set ET0_RX_ER pin */
    MPC.P77PFS.BYTE = 0x11U;
    PORT7.PMR.BYTE |= 0x80U;

    /* Set ET0_TX_CLK pin */
    MPC.PC4PFS.BYTE = 0x11U;
    PORTC.PMR.BYTE |= 0x10U;

    /* Set ET0_TX_EN pin */
    MPC.P80PFS.BYTE = 0x11U;
    PORT8.PMR.BYTE |= 0x01U;

    /* Set ET0_TX_ER pin */
    MPC.PC3PFS.BYTE = 0x11U;
    PORTC.PMR.BYTE |= 0x08U;

    /* Set LCD_CLK pin */
    MPC.PB5PFS.BYTE = 0x25U;
    PORTB.PMR.BYTE |= 0x20U;

    /* Set LCD_DATA0 pin */
    MPC.PB0PFS.BYTE = 0x25U;
    PORTB.PMR.BYTE |= 0x01U;

    /* Set LCD_DATA1 pin */
    MPC.PA7PFS.BYTE = 0x25U;
    PORTA.PMR.BYTE |= 0x80U;

    /* Set LCD_DATA2 pin */
    MPC.PA6PFS.BYTE = 0x25U;
    PORTA.PMR.BYTE |= 0x40U;

    /* Set LCD_DATA3 pin */
    MPC.PA5PFS.BYTE = 0x25U;
    PORTA.PMR.BYTE |= 0x20U;

    /* Set LCD_DATA4 pin */
    MPC.PA4PFS.BYTE = 0x25U;
    PORTA.PMR.BYTE |= 0x10U;

    /* Set LCD_DATA5 pin */
    MPC.PA3PFS.BYTE = 0x25U;
    PORTA.PMR.BYTE |= 0x08U;

    /* Set LCD_DATA6 pin */
    MPC.PA2PFS.BYTE = 0x25U;
    PORTA.PMR.BYTE |= 0x04U;

    /* Set LCD_DATA7 pin */
    MPC.PA1PFS.BYTE = 0x25U;
    PORTA.PMR.BYTE |= 0x02U;

    /* Set LCD_DATA8 pin */
    MPC.PA0PFS.BYTE = 0x25U;
    PORTA.PMR.BYTE |= 0x01U;

    /* Set LCD_DATA9 pin */
    MPC.PE7PFS.BYTE = 0x25U;
    PORTE.PMR.BYTE |= 0x80U;

    /* Set LCD_DATA10 pin */
    MPC.PE6PFS.BYTE = 0x25U;
    PORTE.PMR.BYTE |= 0x40U;

    /* Set LCD_DATA11 pin */
    MPC.PE5PFS.BYTE = 0x25U;
    PORTE.PMR.BYTE |= 0x20U;

    /* Set LCD_DATA12 pin */
    MPC.PE4PFS.BYTE = 0x25U;
    PORTE.PMR.BYTE |= 0x10U;

    /* Set LCD_DATA13 pin */
    MPC.PE3PFS.BYTE = 0x25U;
    PORTE.PMR.BYTE |= 0x08U;

    /* Set LCD_DATA14 pin */
    MPC.PE2PFS.BYTE = 0x25U;
    PORTE.PMR.BYTE |= 0x04U;

    /* Set LCD_DATA15 pin */
    MPC.PE1PFS.BYTE = 0x25U;
    PORTE.PMR.BYTE |= 0x02U;

    /* Set LCD_TCON0 pin */
    MPC.PB4PFS.BYTE = 0x25U;
    PORTB.PMR.BYTE |= 0x10U;

    /* Set LCD_TCON2 pin */
    MPC.PB2PFS.BYTE = 0x25U;
    PORTB.PMR.BYTE |= 0x04U;

    /* Set LCD_TCON3 pin */
    MPC.PB1PFS.BYTE = 0x25U;
    PORTB.PMR.BYTE |= 0x02U;

    /* Set RXD1 pin */
    MPC.P30PFS.BYTE = 0x0AU;
    PORT3.PMR.BYTE |= 0x01U;

    /* Set RXD2 pin */
    MPC.P12PFS.BYTE = 0x0AU;
    PORT1.PMR.BYTE |= 0x04U;

    /* Set RXD7 pin */
    MPC.P92PFS.BYTE = 0x0AU;
    PORT9.PMR.BYTE |= 0x04U;

    /* Set SDHI_CD pin */
    MPC.P25PFS.BYTE = 0x1AU;
    PORT2.PMR.BYTE |= 0x20U;

    /* Set SDHI_CLK pin */
    MPC.P21PFS.BYTE = 0x1AU;
    PORT2.PMR.BYTE |= 0x02U;

    /* Set SDHI_CMD pin */
    MPC.P20PFS.BYTE = 0x1AU;
    PORT2.PMR.BYTE |= 0x01U;

    /* Set SDHI_D0 pin */
    MPC.P22PFS.BYTE = 0x1AU;
    PORT2.PMR.BYTE |= 0x04U;

    /* Set SDHI_D1 pin */
    MPC.P23PFS.BYTE = 0x1AU;
    PORT2.PMR.BYTE |= 0x08U;

    /* Set SDHI_D2 pin */
    MPC.P87PFS.BYTE = 0x1AU;
    PORT8.PMR.BYTE |= 0x80U;

    /* Set SDHI_D3 pin */
    MPC.P17PFS.BYTE = 0x1AU;
    PORT1.PMR.BYTE |= 0x80U;

    /* Set SSCL6 pin */
    MPC.P33PFS.BYTE = 0x0AU;
    PORT3.PMR.BYTE |= 0x08U;

    /* Set SSDA6 pin */
    MPC.P32PFS.BYTE = 0x0AU;
    PORT3.PMR.BYTE |= 0x04U;

    /* Set TXD1 pin */
    PORT2.PODR.BYTE |= 0x40U;
    MPC.P26PFS.BYTE = 0x0AU;
    PORT2.PDR.BYTE |= 0x40U;

    /* Set TXD2 pin */
    PORT1.PODR.BYTE |= 0x08U;
    MPC.P13PFS.BYTE = 0x0AU;
    PORT1.PDR.BYTE |= 0x08U;

    /* Set TXD7 pin */
    PORT9.PODR.BYTE |= 0x01U;
    MPC.P90PFS.BYTE = 0x0AU;
    PORT9.PDR.BYTE |= 0x01U;

    /* Set USB0_OVRCURA pin */
    MPC.P14PFS.BYTE = 0x12U;
    PORT1.PMR.BYTE |= 0x10U;

    /* Set USB0_VBUSEN pin */
    MPC.P16PFS.BYTE = 0x12U;
    PORT1.PMR.BYTE |= 0x40U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}   

