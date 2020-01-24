/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

***** emWin - Graphical user interface for embedded applications *****
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : APP_BankSwap.c
Purpose     : Bank swap demo
----------------------------------------------------------------------
*/

#include "platform.h"
#include "r_flash_rx_if.h"

#if 0
#include "DIALOG.h"

#include "EnvisionKit.h"
#include "Resource.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_BUTTON_HOME   (GUI_ID_USER + 0)
#define ID_BUTTON_HELP   (GUI_ID_USER + 1)
#define ID_BUTTON_SWAP   (GUI_ID_USER + 2)

#define BORDER 20  // Distance from buttons to border

#define BM_HOME       &bmHome_40x40
#define BM_HELP_0     &bmButtonHelp_0_60x40
#define BM_HELP_1     &bmButtonHelp_1_60x40
#define BM_SWAP_0     &bmButtonSwap_0_60x40
#define BM_SWAP_1     &bmButtonSwap_1_60x40

#define FONT_SMALL    &GUI_Font24_AA4
#define FONT_MEDIUM   &GUI_Font32_AA4

#define ID_TIMER0     (GUI_ID_USER + 3)

#ifndef WIN32
extern int _IsRunning;
//extern void bank_swap(void);
#endif

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _cbButtonHelp
*/
static void _cbButtonHelp(WM_MESSAGE * pMsg) {
  const GUI_BITMAP * pBm;
  
  switch (pMsg->MsgId) {
  case WM_PAINT:
    if (BUTTON_IsPressed(pMsg->hWin)) {
      pBm = BM_HELP_1;
    } else {
      pBm = BM_HELP_0;
    }
    GUI_DrawBitmap(pBm, 0, 0);
    break;
  default:
    BUTTON_Callback(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbButtonSwap
*/
static void _cbButtonSwap(WM_MESSAGE * pMsg) {
  const GUI_BITMAP * pBm;
  
  switch (pMsg->MsgId) {
  case WM_PAINT:
    pBm = BUTTON_IsPressed(pMsg->hWin) ? BM_SWAP_1 : BM_SWAP_0;
    GUI_DrawBitmap(pBm, 0, 0);
    break;
  default:
    BUTTON_Callback(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbButtonHome
*/
static void _cbButtonHome(WM_MESSAGE * pMsg) {
  const GUI_BITMAP * pBm;
  
  switch (pMsg->MsgId) {
  case WM_PAINT:
    pBm = BM_HOME;
    GUI_DrawBitmap(pBm, 0, 0);
    break;
  default:
    BUTTON_Callback(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbAnimation
*/
static void _cbAnimation(WM_MESSAGE * pMsg) {
  WM_HTIMER hTimer;
  static int Index;

  switch (pMsg->MsgId) {
  case WM_CREATE:
    WM_CreateTimer(pMsg->hWin, ID_TIMER0, 500, 0);
    break;
  case WM_TIMER:
    hTimer = (WM_HTIMER)pMsg->Data.v;
    WM_RestartTimer(hTimer, 500);
    WM_Invalidate(pMsg->hWin);
    break;
  case WM_PAINT:
    GUI_DrawBitmap(apbmSwapBank_80x80[Index], 0, 0);
    Index = !Index;
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbWin
*/
static void _cbWin(WM_MESSAGE * pMsg) {
  const GUI_BITMAP * pBm;
  WM_HWIN hItem;
  int xSizeWindow, ySizeWindow, yPos, ySizeFont;
  int Id, NCode;
  GUI_RECT Rect;

  switch (pMsg->MsgId) {
  case WM_CREATE:
	in_main_menu_flag = 1;
    xSizeWindow = WM_GetWindowSizeX(pMsg->hWin);
    ySizeWindow = WM_GetWindowSizeY(pMsg->hWin);
    //
    // Create home button
    //
    pBm = BM_HOME;
    hItem = BUTTON_CreateUser(BORDER, BORDER, pBm->XSize, pBm->YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_HOME, 0);
    WM_SetCallback(hItem, _cbButtonHome);
    //
    // Create help button
    //
    pBm = BM_HELP_0;
    hItem = BUTTON_CreateUser(xSizeWindow - pBm->XSize - BORDER, BORDER, pBm->XSize, pBm->YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_HELP, 0);
    WM_SetCallback(hItem, _cbButtonHelp);
    //
    // Create swap button
    //
    pBm = BM_SWAP_0;
    hItem = BUTTON_CreateUser(xSizeWindow - pBm->XSize - BORDER, ySizeWindow - pBm->YSize - BORDER, pBm->XSize, pBm->YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_SWAP, 0);
    WM_SetCallback(hItem, _cbButtonSwap);
    //
    // Create animated window
    //
    WM_CreateWindowAsChild(BORDER, ySizeWindow - 1 - BORDER - apbmSwapBank_80x80[0]->YSize, apbmSwapBank_80x80[0]->XSize, apbmSwapBank_80x80[0]->YSize, pMsg->hWin, WM_CF_SHOW, _cbAnimation, 0);
    break;
  case WM_NOTIFY_PARENT:
	in_main_menu_flag = 0;
	if(!_IsRunning) {
    Id = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch (Id) {
    case ID_BUTTON_HELP:
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        WM_DeleteWindow(pMsg->hWin);
        HelpDialog(DEMO_3, BankSwap);
        break;
      }
      break;
    case ID_BUTTON_HOME:
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        WM_DeleteWindow(pMsg->hWin);
        StartScreen();
        break;
      }
      break;
    case ID_BUTTON_SWAP:
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
    	load_firmware_process();
    	bank_swap();
        break;
      }
      break;
    }
    break;
    }
    break;
  case WM_PAINT:
    xSizeWindow = WM_GetWindowSizeX(pMsg->hWin);
    ySizeWindow = WM_GetWindowSizeY(pMsg->hWin);
    GUI_SetBkColor(COLOR_BK);
    GUI_Clear();
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(FONT_MEDIUM);
    GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
    pBm = BM_HOME;
    GUI_DispStringAt("Bank Swap Demo\n", 230, BORDER + pBm->YSize / 2);
    GUI_SetFont(FONT_SMALL);
    yPos = GUI_GetDispPosY();
    GUI_SetTextAlign(GUI_TA_HCENTER);
    GUI_DispStringAt("RX65N/RX651 have dual bank flash!!\n\n", 230, yPos);
    ySizeFont = GUI_GetFontSizeY();
    Rect.y0 = GUI_GetDispPosY();
    Rect.x0 = 120;
    Rect.y1 = Rect.y0 + 1 * ySizeFont;
    Rect.x1 = 359;
    GUI_DispStringInRect("Swap the bank?", &Rect, GUI_TA_HCENTER);

    Rect.y0 = GUI_GetDispPosY() + 1 * ySizeFont;
    Rect.x0 = 120;
    Rect.y1 = Rect.y0 + 2 * ySizeFont;
    Rect.x1 = 459;
    GUI_DispStringInRect("Bank A: Standard FW rev 1.00\n"
                         "Bank B: Benchmark FW rev x.xx", &Rect, GUI_TA_LEFT);
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       BankSwap
*/
void BankSwap(void) {
  WM_CreateWindow(0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_CF_SHOW, _cbWin, 0);
}
#endif

void bank_swap_with_software_reset(void)
{
	/* stop all interrupt completely */
    set_psw(0);
    R_BSP_InterruptsDisable();
    R_FLASH_Control(FLASH_CMD_BANK_TOGGLE, NULL);
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
    SYSTEM.SWRR = 0xa501;
    while(1);   /* software reset */
}

/*************************** End of file ****************************/
