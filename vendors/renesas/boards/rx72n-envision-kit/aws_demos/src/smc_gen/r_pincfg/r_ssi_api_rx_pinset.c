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
* Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_ssi_api_rx_pinset.c
* Version      : 1.0.2
* Device(s)    : R5F572NNHxFB
* Tool-Chain   : RXC toolchain
* Description  : Setting of port and mpc registers
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_ssi_api_rx_pinset.h"
#include "platform.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_SSI_PinSet_SSIE0
* Description  : This function initializes pins for r_ssi_rx module
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_SSI_PinSet_SSIE0()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set SSIBCK0 pin */
    MPC.P01PFS.BYTE = 0x17U;
    PORT0.PMR.BIT.B1 = 1U;

    /* Set SSILRCK0 pin */
    MPC.PF5PFS.BYTE = 0x17U;
    PORTF.PMR.BIT.B5 = 1U;

    /* Set SSIRXD0 pin */
    MPC.PJ5PFS.BYTE = 0x17U;
    PORTJ.PMR.BIT.B5 = 1U;

    /* Set AUDIO_CLK pin */
    MPC.P00PFS.BYTE = 0x17U;
    PORT0.PMR.BIT.B0 = 1U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

/***********************************************************************************************************************
* Function Name: R_SSI_PinSet_SSIE1
* Description  : This function initializes pins for r_ssi_rx module
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_SSI_PinSet_SSIE1()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set SSIBCK1 pin */
    MPC.P02PFS.BYTE = 0x17U;
    PORT0.PMR.BIT.B2 = 1U;

    /* Set SSILRCK1 pin */
    MPC.P05PFS.BYTE = 0x17U;
    PORT0.PMR.BIT.B5 = 1U;

    /* Set SSIDATA1 pin */
    MPC.P03PFS.BYTE = 0x17U;
    PORT0.PMR.BIT.B3 = 1U;

    /* Set AUDIO_CLK pin */
    MPC.P00PFS.BYTE = 0x17U;
    PORT0.PMR.BIT.B0 = 1U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

