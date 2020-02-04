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
 * File Name    : progbar.c
 * Description  :
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 31.01.2020 1.00 First Release
 *********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
#include "DIALOG.h"
#include "Resource.h"

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
#define RR_ADD  1   // Value which needs to substracted of a rounded rect to fit into a window
#define RADIUS  20  // Radius used for the rounded rects

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/
void progbar_init(void);
void progbar_print(U32 prog);

/***********************************************************************************************************************
 Private global variables and functions
 ***********************************************************************************************************************/
static int _CustomSkin(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo);
static PROGBAR_Handle hProg;

static int _CustomSkin(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo)
{
  GUI_RECT                Rect;
  GUI_RECT                UserRect;

  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_CREATE:
    return PROGBAR_DrawSkinFlex(pDrawItemInfo);
  case WIDGET_ITEM_DRAW_BACKGROUND:
    //
    // Receive the area of the PROGBAR widget
    //
    WM_GetClientRectEx(pDrawItemInfo->hWin, &Rect);
    //
    // Adapt the rectangle to be used for rounded rects
    //
    Rect.x0 += RR_ADD;
    Rect.y0 += RR_ADD;
    Rect.x1 -= RR_ADD;
    Rect.y1 -= RR_ADD;
    //
    // Draw a green rounded rect over the complete area, this gets (partially) overwritten by a white one
    //
    GUI_SetColor(GUI_GREEN);
    GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, RADIUS);
    //
    // Set a user cliprect
    //
    UserRect.x0 = pDrawItemInfo->x0;
    UserRect.y0 = pDrawItemInfo->y0;
    UserRect.x1 = pDrawItemInfo->x1;
    UserRect.y1 = pDrawItemInfo->y1;
    WM_SetUserClipRect(&UserRect);
    //
    // Draw a white rounded rect over the whole PROGBAR area, but the drawing will be visible only in
    // the area of the cliprect. The size of the cliprect will decrease over time and the white rect
    // will get smaller.
    //
    GUI_SetColor(GUI_WHITE);
    GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, RADIUS);
    //
    // Very important, restore the the clipping area
    //
    WM_SetUserClipRect(NULL);
    //
    // Almost done, just a draw a red frame over the whole area
    //
    GUI_SetColor(GUI_RED);
    GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, RADIUS);
    return 0;
  case WIDGET_ITEM_DRAW_FRAME:
    //
    // We handle the frame on our own, so we return 0
    //
    return 0;
  case WIDGET_ITEM_DRAW_TEXT:
    //
    // Handle the drawing of the text by the default skinning routine.
    // This case could be commented. Just left it here to show this command
    // is available.
    //
    return PROGBAR_DrawSkinFlex(pDrawItemInfo);
  default:
    //
    // Anything not handled here will be handled by the default skinning routine.
    //
    return PROGBAR_DrawSkinFlex(pDrawItemInfo);
  }
}

void progbar_init(void)
{
	int            Min;
	int            Max;
	int            Value;

	//
	// Set up some variables
	//
	Min   = 0;
	Max   = 100;
	Value = Min;
	//
	// Enable automatic use of memory devices
	//
	WM_SetCreateFlags(WM_CF_MEMDEV);
	//
	// Initialize GUI
	//
	//GUI_Init();
	//
	// Set back ground color for the desktop window
	//
	WM_SetDesktopColor(GUI_BLACK);
	//
	// Create a progressbar
	//
	hProg = PROGBAR_CreateEx(10, 10, 150, 100, WM_HBKWIN, WM_CF_SHOW, PROGBAR_CF_HORIZONTAL, 0);
	//
	// Set minimal and maximal value of the PROGBAR widget
	//
	PROGBAR_SetMinMax(hProg, Min, Max);
	//
	// Set a custom skin, see above
	//
	PROGBAR_SetSkin(hProg, _CustomSkin);
	//
	// Set back ground of the PROGBAR widget to invisible
	//
	WM_SetHasTrans(hProg);
}

void progbar_print(U32 prog)
{
    PROGBAR_SetValue(hProg, prog);
}


/* end of file */
