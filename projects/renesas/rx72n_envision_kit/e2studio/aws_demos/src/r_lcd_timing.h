/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* File Name    : r_lcd_timing.h
* Description  : LCD panel for glcdc channel 0 definition header
******************************************************************************/
#ifndef LCD_TIMING_H
#define LCD_TIMING_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    "r_glcdc_rx_if.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define     LCD_CH0_OUT_CLK_DIV_RATIO     (GLCDC_PANEL_CLK_DIVISOR_24)    /* Division Ratio of LCD Panel Clock                 */

#define     LCD_CH0_W_VSYNC               (5u)                    /* Vertical Pulse Width(VPW)                                   */
#define     LCD_CH0_DISP_VS               (5u + 5u)                    /* LCD display area vertical start position
                                                                 = Vertical Pulse Width(VPW) + Vertical Back Porch(VBP)     */
#define     LCD_CH0_DISP_VW               (272u)                   /* Vertical Display Period(VDP)                                */

#define     LCD_CH0_W_HSYNC               (35u)                    /* Horizontal Pulse Width(HPW)                                 */
#define     LCD_CH0_DISP_HS               (35u + 35u)                    /* LCD display area horizontal start position
                                                                 = Horizontal Pulse Width(HPW) + Horizontal Back Porch(HBP) */
#define     LCD_CH0_DISP_HW               (480u)                   /* Horizontal Display Period(HDP)                              */

#define     LCD_CH0_SIG_FV                (287u)              /* Vertical Total Period(VTP)                                  */
#define     LCD_CH0_SIG_FH                (586u)             /* Horizontal Total Period(HTP)                                */

#define     LCD_CH0_W_VFP                 (5u)             /* Vertical Front Porch(VFP)                                   */
#define     LCD_CH0_W_HFP                 (36u)             /* Horizontal Front Porch(HFP)                                 */

#define     LCD_CH0_W_VBP                 (5u)             /* Vertical Back Porch(VBP)                                  */
#define     LCD_CH0_W_HBP                 (35u)             /* Horizontal Back Porch(HBP)                                */

#define     LCD_CH0_TCON_PIN_VSYNC        (GLCDC_TCON_PIN_0)      /* Select TCON of a Vsync signal (Vsync_TCON_select)           */
#define     LCD_CH0_TCON_PIN_HSYNC        (GLCDC_TCON_PIN_1)      /* Select TCON of a Hsync signal (Hsync_TCON_select)           */
#define     LCD_CH0_TCON_PIN_DE           (GLCDC_TCON_PIN_2)      /* Select TCON of a DE signal    (DE_TCON_select)              */

#define     LCD_CH0_TCON_POL_VSYNC        (GLCDC_SIGNAL_POLARITY_HIACTIVE)    /* Select polarity of a Vsync signal (Vsync_POL)               */
#define     LCD_CH0_TCON_POL_HSYNC        (GLCDC_SIGNAL_POLARITY_HIACTIVE)    /* Select polarity of a Hsync signal (Hsync_POL)               */
#define     LCD_CH0_TCON_POL_DE           (GLCDC_SIGNAL_POLARITY_HIACTIVE)/* Select polarity of a Hsync signal (DE_POL)                  */

#define     LCD_CH0_OUT_EDGE              (GLCDC_SIGNAL_SYNC_EDGE_RISING)       /* Output phase control of LCD_DATA signal(LCD_DATA_OUT_EDGE)  */
#define     LCD_CH0_OUT_FORMAT            (GLCDC_OUT_FORMAT_16BITS_RGB565) /* LCD output format select (LCD_OUT_FORMAT)                */
#define     LCD_CH0_OUT_BG_COLOR          (0x00000000)               /* Set a background color                                      */                             
#define     LCD_CH0_OUT_ENDIAN            (GLCDC_ENDIAN_LITTLE)      /* Select Bit-endian of Output Data                            */
#define     LCD_CH0_OUT_COLOR_ORDER       (GLCDC_COLOR_ORDER_RGB)    /* Select Pixel Order of Output Data                           */

#define     LCD_CH0_IN_GR1_VSIZE          (272u)                 /* Vertical width of image ddta (Graphic1)                    */
#define     LCD_CH0_IN_GR1_HSIZE          (480u)                 /* Horizontall width of image data (Graphic1)                 */
#define     LCD_CH0_IN_GR1_COORD_X        (0u)                   /* X-coordinate of display start position (Graphic1)          */
#define     LCD_CH0_IN_GR1_COORD_Y        (0u)                   /* Y-coordinate of display start position (Graphic1)          */
#define     LCD_CH0_IN_GR1_PBASE          (NULL)            /* Start address of the frame buffer                         */
#define     LCD_CH0_IN_GR1_FORMAT         (GLCDC_IN_FORMAT_32BITS_ARGB8888) /*Color format of output data (Graphic1)             */
#define     LCD_CH0_IN_GR1_LINEOFFSET     (1920u)                /* Macro line offset (Graphic1)                              */

#define     LCD_CH0_IN_GR2_VSIZE          (272u)                 /* Vertical width of image ddta (Graphic2)                    */
#define     LCD_CH0_IN_GR2_HSIZE          (480u)                 /* Horizontall width of image data (Graphic2)                 */
#define     LCD_CH0_IN_GR2_COORD_X        (0u)                   /* X-coordinate of display start position (Graphic2)          */
#define     LCD_CH0_IN_GR2_COORD_Y        (0u)                   /* Y-coordinate of display start position (Graphic2)          */
#define     LCD_CH0_IN_GR2_PBASE          (0x00800000)            /* Start address of the frame buffer                         */
#define     LCD_CH0_IN_GR2_FORMAT         (GLCDC_IN_FORMAT_16BITS_RGB565) /*Color format of output data (Graphic2)             */
#define     LCD_CH0_IN_GR2_LINEOFFSET     (960u)                /* Macro line offset (Graphic2)                              */

#define     LCD_CH0_DETECT_VPOS           (true)                 /* Enable VPOS Detect                                        */ 
#define     LCD_CH0_INTERRUPT_VPOS_ENABLE (true)                 /* Enable VPOS Interrupt                                     */
#define     LCD_CH0_CALLBACK_ENABLE       (true)                 /* Enable Callback Function                                  */
#define     LCD_CH0_PCALLBACK             (_VSYNC_ISR)        /* name of Callback Function                                 */




#endif  /* LCD_TIMING_H */

