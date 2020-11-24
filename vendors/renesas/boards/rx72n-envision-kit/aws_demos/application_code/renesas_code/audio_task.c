/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : audio_task.c
 * Description  : 
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 20.11.2020 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
/* for using C standard library */
#include <stdio.h>
#include <string.h>
//#include <stdlib.h>

/* for using FIT Module */
#include "platform.h"
#include "r_ssi_api_rx_config.h"
#include "r_ssi_api_rx_if.h"
#include "r_gpio_rx_if.h"
#include "r_mpc_rx_if.h"
#include "r_dmaca_rx_if.h"
#include "r_ssi_api_rx_pinset.h"

/* for using Amazon FreeRTOS */
#include "FreeRTOS.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
void audio_task( void * pvParameters );

/**********************************************************************************************************************
 Private global variables and functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

static void private_function (void);

/**********************************************************************************************************************
 * Function Name: audio_task
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void audio_task( void * pvParameters )
{
    /* test */
    private_function();

    /* finish */
    while(1)
    {
        vTaskDelay(0xffffffff);
    }
}
/**********************************************************************************************************************
 * End of function audio_task
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: private_function
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void private_function (void)
{
    while(1)
    {
        vTaskDelay(100);
    }
}
/**********************************************************************************************************************
 * End of function private_function
 *********************************************************************************************************************/

