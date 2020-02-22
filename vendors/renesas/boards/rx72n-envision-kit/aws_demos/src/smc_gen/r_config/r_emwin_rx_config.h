/***********************************************************************************************************************
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
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_gpio_rx_config.h
* Description  : Configures the GPIO module.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 17.07.2020 1.00    First Release
***********************************************************************************************************************/
#ifndef EMWIN_RX_CONFIG_HEADER_FILE
#define EMWIN_RX_CONFIG_HEADER_FILE

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/

#define EMWIN_BOARD RX65N_ENVISION_KIT
//#define EMWIN_BOARD RX72N_ENVISION_KIT

#define EMWIN_GUI_NUM_BYTES (1024 * 80)

#define EMWIN_GUI_FRAME_BUFFER1 0x00800000
#define EMWIN_GUI_FRAME_BUFFER2 0x00840000

#if (EMWIN_BOARD) == (RX72N_ENVISION_KIT)
#define EMWIN_DISP_SIGNAL_PIN         GPIO_PORT_B_PIN_3
#define EMWIN_BACKLIGHT_PIN           GPIO_PORT_6_PIN_7
#define EMWIN_TOUCH_IC_RESET_PIN      GPIO_PORT_6_PIN_6
#elif  (EMWIN_BOARD) == (RX65N_ENVISION_KIT)
#define EMWIN_DISP_SIGNAL_PIN         GPIO_PORT_6_PIN_3
#define EMWIN_BACKLIGHT_PIN           GPIO_PORT_6_PIN_6
#define EMWIN_TOUCH_IC_RESET_PIN      GPIO_PORT_0_PIN_7
#endif

#endif /* EMWIN_RX_CONFIG_HEADER_FILE */


