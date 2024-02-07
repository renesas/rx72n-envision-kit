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
* File Name    : r_sdhi_rx_pinset.c
* Version      : 1.0.2
* Device(s)    : R5F572NNHxFB
* Tool-Chain   : RXC toolchain
* Description  : Setting of port and mpc registers
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_sdhi_rx_pinset.h"
#include "platform.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: R_SDHI_PinSetInit
* Description  : This function initializes pins for r_sdhi_rx module
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_SDHI_PinSetInit()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set SDHI_CLK pin */
    PORT2.PMR.BIT.B1 = 0U;
    PORT2.PCR.BIT.B1 = 0U;
    PORT2.PODR.BIT.B1 = 0U;
    PORT2.PDR.BIT.B1 = 1U;
    MPC.P21PFS.BYTE = 0x00U;

    /* Set SDHI_CMD pin */
    PORT2.PMR.BIT.B0 = 0U;
    PORT2.PCR.BIT.B0 = 0U;
    PORT2.PODR.BIT.B0 = 0U;
    PORT2.PDR.BIT.B0 = 1U;
    MPC.P20PFS.BYTE = 0x00U;

    /* Set SDHI_CD pin */
    PORT2.PMR.BIT.B5 = 0U;
    PORT2.PCR.BIT.B5 = 0U;
    PORT2.PDR.BIT.B5 = 0U;
    MPC.P25PFS.BYTE = 0x1AU;
    PORT2.PMR.BIT.B5 = 1U;

    /* Set SDHI_D0 pin */
    PORT2.PMR.BIT.B2 = 0U;
    PORT2.PCR.BIT.B2 = 0U;
    PORT2.PODR.BIT.B2 = 0U;
    PORT2.PDR.BIT.B2 = 1U;
    MPC.P22PFS.BYTE = 0x00U;

    /* Set SDHI_D1 pin */
    PORT2.PMR.BIT.B3 = 0U;
    PORT2.PCR.BIT.B3 = 0U;
    PORT2.PODR.BIT.B3 = 0U;
    PORT2.PDR.BIT.B3 = 1U;
    MPC.P23PFS.BYTE = 0x00U;

    /* Set SDHI_D2 pin */
    PORT8.PMR.BIT.B7 = 0U;
    PORT8.PCR.BIT.B7 = 0U;
    PORT8.PODR.BIT.B7 = 0U;
    PORT8.PDR.BIT.B7 = 1U;
    MPC.P87PFS.BYTE = 0x00U;

    /* Set SDHI_D3 pin */
    PORT1.PMR.BIT.B7 = 0U;
    PORT1.PCR.BIT.B7 = 0U;
    PORT1.PODR.BIT.B7 = 0U;
    PORT1.PDR.BIT.B7 = 1U;
    MPC.P17PFS.BYTE = 0x00U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}


/***********************************************************************************************************************
* Function Name: R_SDHI_PinSetTransfer
* Description  : This function enables SD communication
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_SDHI_PinSetTransfer()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set SDHI_CLK pin */
    MPC.P21PFS.BYTE = 0x1AU;
    PORT2.PMR.BIT.B1 = 1U;

    /* Set SDHI_CMD pin */
    MPC.P20PFS.BYTE = 0x1AU;
    PORT2.PMR.BIT.B0 = 1U;

    /* Set SDHI_CD pin */
    MPC.P25PFS.BYTE = 0x1AU;
    PORT2.PMR.BIT.B5 = 1U;

    /* Set SDHI_D0 pin */
    MPC.P22PFS.BYTE = 0x1AU;
    PORT2.PMR.BIT.B2 = 1U;

    /* Set SDHI_D1 pin */
    MPC.P23PFS.BYTE = 0x1AU;
    PORT2.PMR.BIT.B3 = 1U;

    /* Set SDHI_D2 pin */
    MPC.P87PFS.BYTE = 0x1AU;
    PORT8.PMR.BIT.B7 = 1U;

    /* Set SDHI_D3 pin */
    MPC.P17PFS.BYTE = 0x1AU;
    PORT1.PMR.BIT.B7 = 1U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

/***********************************************************************************************************************
* Function Name: R_SDHI_PinSetDetection
* Description  : This function disables SD communication and enables card detection and write protection
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_SDHI_PinSetDetection()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set SDHI_CLK pin */
    PORT2.PMR.BIT.B1 = 0U;
    MPC.P21PFS.BYTE = 0x00U;

    /* Set SDHI_CMD pin */
    PORT2.PMR.BIT.B0 = 0U;
    MPC.P20PFS.BYTE = 0x00U;

    /* Set SDHI_CD pin */
    MPC.P25PFS.BYTE = 0x1AU;
    PORT2.PMR.BIT.B5 = 1U;

    /* Set SDHI_D0 pin */
    PORT2.PMR.BIT.B2 = 0U;
    MPC.P22PFS.BYTE = 0x00U;

    /* Set SDHI_D1 pin */
    PORT2.PMR.BIT.B3 = 0U;
    MPC.P23PFS.BYTE = 0x00U;

    /* Set SDHI_D2 pin */
    PORT8.PMR.BIT.B7 = 0U;
    MPC.P87PFS.BYTE = 0x00U;

    /* Set SDHI_D3 pin */
    PORT1.PMR.BIT.B7 = 0U;
    MPC.P17PFS.BYTE = 0x00U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

/***********************************************************************************************************************
* Function Name: R_SDHI_PinSetEnd
* Description  : This function ends pins for r_sdhi_rx module
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_SDHI_PinSetEnd()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set SDHI_CLK pin */
    PORT2.PMR.BIT.B1 = 0U;
    MPC.P21PFS.BYTE = 0x00U;

    /* Set SDHI_CMD pin */
    PORT2.PMR.BIT.B0 = 0U;
    MPC.P20PFS.BYTE = 0x00U;

    /* Set SDHI_CD pin */
    PORT2.PMR.BIT.B5 = 0U;
    MPC.P25PFS.BYTE = 0x00U;

    /* Set SDHI_D0 pin */
    PORT2.PMR.BIT.B2 = 0U;
    MPC.P22PFS.BYTE = 0x00U;

    /* Set SDHI_D1 pin */
    PORT2.PMR.BIT.B3 = 0U;
    MPC.P23PFS.BYTE = 0x00U;

    /* Set SDHI_D2 pin */
    PORT8.PMR.BIT.B7 = 0U;
    MPC.P87PFS.BYTE = 0x00U;

    /* Set SDHI_D3 pin */
    PORT1.PMR.BIT.B7 = 0U;
    MPC.P17PFS.BYTE = 0x00U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

