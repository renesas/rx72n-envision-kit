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
File        : APP_Update.c
Purpose     : Software update via USB
----------------------------------------------------------------------
*/
#include "DIALOG.h"
#include "Resource.h"
#include "platform.h"
#include "r_irq_rx_pinset.h"

#define LONG_PRESS_COUNT_VALUE      5000
#define BUTTON_MASHING_COUNT_VALUE  3000
#define SWITCH_RISING_COUNT_VALUE   3
#define IRQ_LOW     0
#define IRQ_HIGH    1

irq_handle_t  my_irq15_handle;
int in_main_menu_flag = 1;
int in_sw_active = 0;
int in_sw_rising_count = 0;

#if 0
#include "DIALOG.h"

#include "EnvisionKit.h"
#include "Resource.h"

#ifndef WIN32
#include "r_irq_rx_if.h"     //Contains prototypes for the IRQ driver
#include "r_irq_rx_config.h" //Contains user configurable options and setup definitions
#include "rpbrx65n_2mb.h"
#endif

#define UPDATE_FILE_NAME "bench.rsu"

void GUI_AddRect(GUI_RECT * pDest, const GUI_RECT * pRect, int Dist);
#ifndef WIN32
extern uint32_t load_firmware_process(void);
extern void load_firmware_status(uint32_t *now_status, uint32_t *finish_status);
extern void firmware_update_request(char *string);
extern void firmware_update_status_initialize(void);
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define COLOR_FRAME    GUI_MAKE_COLOR(0x82593A)
#define COLOR_L_TOP    GUI_MAKE_COLOR(0x84E9C9)
#define COLOR_L_BOTTOM GUI_MAKE_COLOR(0x51CEAA)
#define COLOR_R        GUI_MAKE_COLOR(0xF0F0F0)

#define XSIZE_WINDOW     240
#define YSIZE_WINDOW      61
#define XSIZE_PROGBAR    140
#define YSIZE_PROGBAR     21
#define XSIZE_TEXT        40

#define YSIZE_TEXT     YSIZE_PROGBAR

#define BM_L           &bmProgbarDotL_21x21
#define BM_R           &bmProgbarDotR_21x21

#define PROGBAR_PERIOD  3000
#define UPDATEE_PERIOD 30000

#define SET_VALUE      (WM_USER + 0)

extern int in_main_menu_flag;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
int _IsRunning;

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _IntToPercent
*/
static void _IntToPercent(unsigned v, char * s) {
  char * p;
  char * t;
  char * m;
  char c;

  //
  // Convert
  //
  p = s;
  if (v) {
    while (v) {
      *s++ = '0' + v % 10;
      v /= 10;
    }
  } else {
    *s++ = '0';
  }
  //
  // Mirror
  //
  t = s;
  m = (char *)(((U32)p + (U32)s) / 2);
  while (p != m) {
    c = *(--t);
    *(t) = *(p);
    *(p++) = c;
  }
  //
  // Append
  //
  *s++ = '%';
  *s++ = 0;
}

/*********************************************************************
*
*       _cbProgbar
*/
static void _cbProgbar(WM_MESSAGE * pMsg) {
  int xSizeWindow, ySizeWindow, Value;
  int * pValue;
  const GUI_BITMAP * pBm;
  GUI_RECT Rect;
  
  switch (pMsg->MsgId) {
  case WM_PAINT:
    xSizeWindow = WM_GetWindowSizeX(pMsg->hWin);
    ySizeWindow = WM_GetWindowSizeY(pMsg->hWin);
    WM_GetUserData(pMsg->hWin, &pValue, sizeof(pValue));
    Value = *pValue;
    Rect.x0 = Rect.y0 = 0;
    Rect.x1 = (Value * xSizeWindow) / 100 - 1;
    Rect.y1 = ySizeWindow - 1;
    GUI_SetBkColor(COLOR_BK);
    GUI_Clear();
    //
    // R
    //
    pBm = BM_R;
    if (Rect.x1 < pBm->XSize / 2) {
      GUI_DrawBitmap(pBm, 0, 0);
    }
    GUI_DrawBitmap(pBm, xSizeWindow - pBm->XSize - 1, 0);
    GUI_SetColor(COLOR_R);
    GUI_FillRect(10, 0, xSizeWindow - 10 - 1, ySizeWindow - 1);
    //
    // L
    //
    pBm = BM_L;
    WM_SetUserClipRect(&Rect);
    GUI_DrawBitmap(pBm, 0, 0);
    GUI_DrawGradientV(Rect.x0 + 10, Rect.y0, Rect.x1 - 10, Rect.y1 - 1, COLOR_L_TOP, COLOR_L_BOTTOM);
    if (Rect.x1 > pBm->XSize) {
      WM_SetUserClipRect(NULL);
      Rect.x0 = Rect.x1 - 10;
      WM_SetUserClipRect(&Rect);
      GUI_DrawBitmap(pBm, Rect.x1 - pBm->XSize, 0);
    }
    WM_SetUserClipRect(NULL);
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
  char acBuffer[10];
  static GUI_TIMER_TIME t0;
  GUI_TIMER_TIME tNow;
  WM_HWIN hText;
  static WM_HWIN hProg;
  int xSizeWindow, yPos;
  static int Value;
  unsigned long now_status;
  unsigned long finish_status;
  int * pValue;
  GUI_RECT Rect;
  static int OldState;
  
  switch (pMsg->MsgId) {
  case WM_DELETE:
    InitUpdate();
    load_firmware_process();
    Value = 0;
    _IsRunning = 0;
    if (OldState == 0) {
      WM_MULTIBUF_Enable(0);
    }
    break;
  case WM_CREATE:
    OldState = WM_MULTIBUF_Enable(1);
    xSizeWindow = WM_GetWindowSizeX(pMsg->hWin);
    yPos = (YSIZE_WINDOW - YSIZE_PROGBAR) / 2;
    //
    // Create progress text
    //
    hText = TEXT_CreateUser(xSizeWindow - yPos - XSIZE_TEXT - 1, yPos, XSIZE_TEXT, YSIZE_TEXT, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_TEXT0, "", 0);
    TEXT_SetTextAlign(hText, GUI_TA_HCENTER);
    //
    // Create progress window
    //
    hProg = WM_CreateWindowAsChild(yPos, yPos, XSIZE_PROGBAR, YSIZE_PROGBAR, pMsg->hWin, WM_CF_SHOW, _cbProgbar, sizeof(pValue));
    pValue = &Value;
    WM_SetUserData(hProg, &pValue, sizeof(pValue));
    //
    // Create timer for progress
    //
    WM_CreateTimer(pMsg->hWin, 0, 50, 0);
    t0 = GUI_GetTime();
    break;
  case WM_TIMER:
#ifndef WIN32
    Value = load_firmware_process();
    load_firmware_status(&now_status, &finish_status);
#else
    Value++;
#endif
    WM_RestartTimer(pMsg->Data.v, 0);
//    if (Value == 100) {
    if (Value == 100 && now_status >= finish_status) {
      WM_DeleteTimer(pMsg->Data.v);
      WM_DeleteWindow(pMsg->hWin);
      if(now_status != finish_status)
      {
          firmware_update_status_initialize();
      }
    } else {
      tNow = GUI_GetTime();
//      Value = ((tNow - t0) * 100) / PROGBAR_PERIOD;
      Value = Value > 100 ? 100 : Value;
      WM_InvalidateWindow(hProg);
      hText = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT0);
      _IntToPercent(Value, acBuffer);
      TEXT_SetText(hText, acBuffer);
    }
    break;
  case WM_PAINT:
    WM_GetClientRect(&Rect);
    GUI_SetColor(COLOR_FRAME);
    GUI_DrawRectEx(&Rect);
    GUI_AddRect(&Rect, &Rect, -1);
    GUI_DrawRectEx(&Rect);
    GUI_AddRect(&Rect, &Rect, -1);
    GUI_SetColor(GUI_WHITE);
    GUI_FillRectEx(&Rect);
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

#ifdef WIN32

/*********************************************************************
*
*       _OnTimerPressUpdate
*/
static void _OnTimerPressUpdate(GUI_TIMER_MESSAGE * pTM) {
  UpdatePressed = 1;
  GUI_TIMER_Delete(pTM->hTimer);
}

#else

/*********************************************************************
*
*       _SW1_ISR
*/
static void _SW1_ISR(void * p) {
  UpdatePressed = 1;
}

#endif
#endif

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       DoUpdate
*/
void DoUpdate(void) {
#if 0
  int xSize;
  int ySize;

  if (_IsRunning == 0) {
    _IsRunning = 1;
    xSize = LCD_GetXSize();
    ySize = LCD_GetYSize();
    WM_CreateWindowAsChild((xSize - XSIZE_WINDOW) / 2, (ySize - YSIZE_WINDOW) / 2, XSIZE_WINDOW, YSIZE_WINDOW, WM_HBKWIN, WM_CF_SHOW | WM_CF_STAYONTOP, _cbWin, 0);
    load_firmware_process();
    firmware_update_request(UPDATE_FILE_NAME);
  }
#endif
}
/*********************************************************************
*
*       _OnTimerCheckPressedState
*/
void CheckPressedState(void)
{
    static uint32_t long_press_count = 0;
    irq_trigger_t my_trig_mode;

    if (in_main_menu_flag)
    {
        if (in_sw_active)
        {
            if ((BUTTON_MASHING_COUNT_VALUE > long_press_count) && (SWITCH_RISING_COUNT_VALUE <= in_sw_rising_count))
            {
                bank_swap_with_software_reset();
                while(1);
            }
            else if ((LONG_PRESS_COUNT_VALUE <= long_press_count) && (0 == in_sw_rising_count))
            {
                DoUpdate();
                while(1);
            }
            else if ((BUTTON_MASHING_COUNT_VALUE <= long_press_count) && 
                     (0 < in_sw_rising_count) && (SWITCH_RISING_COUNT_VALUE > in_sw_rising_count))
            {
                R_IRQ_InterruptEnable(my_irq15_handle, false);
                long_press_count = 0;
                in_sw_active = 0;
                in_sw_rising_count = 0;
                my_trig_mode = IRQ_TRIG_FALLING;
                R_IRQ_Control(my_irq15_handle, IRQ_CMD_SET_TRIG, &my_trig_mode);
                R_IRQ_InterruptEnable(my_irq15_handle, true);
            }
            else
            {
                long_press_count++;
            }
        }
        else
        {
            long_press_count = 0;
            in_sw_rising_count = 0;
        }
    }
}

void my_irq15_callback(void *pdata)
{
    irq_number_t  my_irq_number;
    irq_trigger_t my_trig_mode;
    uint8_t       level;

    my_irq_number = *((irq_number_t *)pdata);
    if (IRQ_NUM_15 == my_irq_number)
    {
        R_IRQ_ReadInput(my_irq15_handle, &level);
        if (IRQ_LOW == level)
        {
            /* Change trigger mode to rising edge. */
            in_sw_active = 1;
            in_sw_rising_count = 0;
            my_trig_mode = IRQ_TRIG_RISING;
            R_IRQ_Control(my_irq15_handle, IRQ_CMD_SET_TRIG, &my_trig_mode);
        }
        else
        {
            in_sw_rising_count++;
        }
    }
}

#if 0
/*********************************************************************
*
*       InitUpdate
*/
void InitUpdate(void) {
#ifdef WIN32
  GUI_TIMER_Create(_OnTimerPressUpdate, GUI_GetTime() + UPDATEE_PERIOD, 0, 0);
#else
#endif
}
#endif
/*************************** End of file ****************************/
