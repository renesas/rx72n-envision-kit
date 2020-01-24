#include "DIALOG.h"
#include "../MainMenu/Resource.h"

/*********************************************************************
*
*       Externals
*
**********************************************************************
*/

/*********************************************************************
*
*       Types
*
**********************************************************************
*/

typedef struct {
  WM_HWIN hWin;
  int     xPos;
  int     yPos;
  int     Dist;
} ANIM_WIN_DATA;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define ID_TIMER_0              0
#define ID_TIMER_1              1
#define ID_TIMER_2              2
#define ID_TIMER_3              3
#define ID_TIMER_4              4
#define ID_TIMER_5              5
#define ID_TIMER_6              6
#define ID_TIMER_7              7
#define ID_TIMER_8              8
#define ID_TIMER_9              9
#define ID_TIMER_10             10
#define ID_TIMER_11             11
#define ID_TIMER_12             12
#define ID_TIMER_13             13
#define ID_TIMER_14             14

#define BK_COLOR                GUI_MAKE_COLOR(0x00478924)
#define COLOR_WEIGHT_BLACK      GUI_MAKE_COLOR(0x00252525)
#define COLOR_WEIGHT_RED        GUI_MAKE_COLOR(0x000808A4)
#define COLOR_SWEAT             GUI_MAKE_COLOR(0x00E2C67C)
#define COLOR_BUTTON_0          GUI_MAKE_COLOR(0x0084A66B)
#define COLOR_BUTTON_1          GUI_MAKE_COLOR(0x009CAE8C)
#define COLOR_BUTTON_OUTLINE    GUI_MAKE_COLOR(0x00427519)

#define APBM_LIFT               apbmLifter_146x148
#define DEMO_NAME               "WEIGHTLIFTING"

#define FOCUS_MAX_X             184
#define TIMER_POS_X             220
#define TIMER_POS_Y             252
#define LIFTER_POS_X            164
#define LIFTER_POS_Y            49

#define MAX_TRIES               3

#define TITLE_YSIZE             50
#define TITLE_YPOS              40
#define COLOR_TITLE_1           GUI_MAKE_COLOR(0x002238F9)
#define COLOR_TITLE_2           GUI_BLACK

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static int     _LockWeight;
static int     _Weight;
static int     _Record;
static WM_HWIN _hButtonMinus;
static WM_HWIN _hButtonPlus;
static WM_HWIN _hButtonAction;
static WM_HWIN _hProgwin;
static WM_HWIN _hAction;
static WM_HWIN _hWeight;
static WM_HWIN _hRecord;

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

static int _aWeightPos[] = {
  152,     // Frame 0
  152,     // Frame 1
  152,     // Frame 2
  152,     // Frame 3
  152,     // Frame 4
  152,     // Frame 5
  143,     // Frame 6
  118,     // Frame 7
  80,      // Frame 8
  89,      // Frame 9
  76,      // Frame 10
  29,      // Frame 11
  29,      // Frame 12
};

static int _aHitRectSize[] = {
  30,      // 70 kg weight
  28,      // 80 kg weight
  24,      // 90 kg weight
  22,      // 100 kg weight
  20,      // 110 kg weight
  18,      // 120 kg weight
  16,      // 130 kg weight
  14,      // 140 kg weight
  12,      // 150 kg weight
  10,      // 160 kg weight
  8,       // 170 kg weight
  4,       // 180 kg weight
};

static int _aEndAnimFrames[] = {
  10,
  6,
  2,
  1,
  0,
};

static int _aWeightTimeMs[] = {
  15000,   // 70 kg weight
  15000,   // 80 kg weight
  15000,   // 90 kg weight
  10000,   // 100 kg weight
  10000,   // 110 kg weight
  10000,   // 120 kg weight
  15000,   // 130 kg weight
  15000,   // 140 kg weight
  15000,   // 150 kg weight
  15000,   // 160 kg weight
  15000,   // 170 kg weight
  15000,   // 180 kg weight
};

static const GUI_BITMAP * _apbmWeight[] = {
  &bmWeight_small_6x50,
  &bmWeight_med_6x50,
  &bmWeight_big_6x50,
};

static const char * _acActionText[] = {
  "PICK A WEIGHT",
  "PUSH",
  "FOCUS",
  "YOU DID IT!",
  "TRY AGAIN!",
  "NO MORE TRIES",
  "TIME IS UP",
};

static const WM_HWIN * _apWindows[] = {
  &_hButtonMinus,
  &_hButtonPlus,
  &_hButtonAction,
  &_hProgwin,
  &_hAction,
  &_hWeight,
  &_hRecord,
};

/*********************************************************************
*
*       _cbTitle
*/
static void _cbTitle(WM_MESSAGE * pMsg) {
  int xDist;
  int xSize;

  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetColor(COLOR_TITLE_2);
    GUI_SetBkColor(GUI_BLACK);
    GUI_SetFont(FONT_SPORTS_48);
    xDist = GUI_GetStringDistX(DEMO_NAME);
    xSize = WM_GetWindowSizeX(pMsg->hWin);
    GUI_DispStringAt(DEMO_NAME, (xSize / 2) - (xDist / 2) + 1, 0);
    GUI_SetColor(COLOR_TITLE_1);
    GUI_DispStringAt(DEMO_NAME, (xSize / 2) - (xDist / 2) - 2, 0);
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _AnimWindowInX
*/
static void _AnimWindowInX(GUI_ANIM_INFO * pInfo, void * pVoid) {
  ANIM_WIN_DATA * pData;
  int             xPos;

  pData = (ANIM_WIN_DATA *)pVoid;
  xPos  = pData->xPos + (pData->Dist * pInfo->Pos) / GUI_ANIM_RANGE;
  WM_MoveTo(pData->hWin, xPos, pData->yPos);
}

/*********************************************************************
*
*       _AnimWindowInY
*/
static void _AnimWindowInY(GUI_ANIM_INFO * pInfo, void * pVoid) {
  ANIM_WIN_DATA * pData;
  int             yPos;

  pData = (ANIM_WIN_DATA *)pVoid;
  yPos  = pData->yPos + (pData->Dist * pInfo->Pos) / GUI_ANIM_RANGE;
  WM_MoveTo(pData->hWin, pData->xPos, yPos);
}

/*********************************************************************
*
*       _AnimateWindows
*/
static void _AnimateWindows(void) {
         int             i;
  static ANIM_WIN_DATA   aData[GUI_COUNTOF(_apWindows)];
  static GUI_ANIM_HANDLE hAnim;

  for (i = 0; i < GUI_COUNTOF(_apWindows); i++) {
    aData[i].hWin = *(*(_apWindows + i));
    aData[i].xPos = WM_GetWinOrgX(*(*(_apWindows + i)));
    aData[i].yPos = WM_GetWinOrgY(*(*(_apWindows + i)));
  }
  aData[0].Dist = 41;
  aData[1].Dist = 41;
  aData[2].Dist = -140;
  aData[3].Dist = -29;
  aData[4].Dist = -27;
  aData[5].Dist = -29;
  aData[6].Dist = 155;

  hAnim = GUI_ANIM_Create(300, 25, NULL, NULL);
  GUI_ANIM_AddItem(hAnim, 0, 300, ANIM_ACCELDECEL, &aData[0], _AnimWindowInX);
  GUI_ANIM_AddItem(hAnim, 0, 300, ANIM_ACCELDECEL, &aData[1], _AnimWindowInX);
  GUI_ANIM_AddItem(hAnim, 0, 300, ANIM_ACCELDECEL, &aData[2], _AnimWindowInX);
  GUI_ANIM_AddItem(hAnim, 0, 300, ANIM_ACCELDECEL, &aData[3], _AnimWindowInY);
  GUI_ANIM_AddItem(hAnim, 0, 300, ANIM_ACCELDECEL, &aData[4], _AnimWindowInY);
  GUI_ANIM_AddItem(hAnim, 0, 300, ANIM_ACCELDECEL, &aData[5], _AnimWindowInY);
  GUI_ANIM_AddItem(hAnim, 0, 300, ANIM_ACCELDECEL, &aData[6], _AnimWindowInX);
  GUI_ANIM_StartEx(hAnim, 1, NULL);
}

/*********************************************************************
*
*       _AnimTitleIn
*/
static void _AnimTitleIn(GUI_ANIM_INFO * pInfo, void * pVoid) {
  ANIM_TITLE_DATA * pData;
  int               yPos;

  pData = (ANIM_TITLE_DATA*) pVoid;
  yPos  = pData->yPos + ((TITLE_YPOS - pData->yPos) * pInfo->Pos) / GUI_ANIM_RANGE;
  WM_MoveTo(pData->hWin, 0, yPos);
  if (pInfo->State == GUI_ANIM_END) {
    pData->yPos = TITLE_YPOS;
  }
}

/*********************************************************************
*
*       _AnimTitleOut
*/
static void _AnimTitleOut(GUI_ANIM_INFO * pInfo, void * pVoid) {
  ANIM_TITLE_DATA * pData;
  int               xPos;

  pData = (ANIM_TITLE_DATA*) pVoid;
  xPos  = pData->xPos - (pData->xSize * pInfo->Pos) / GUI_ANIM_RANGE;
  WM_MoveTo(pData->hWin, xPos, pData->yPos);
  if (pInfo->State == GUI_ANIM_START) {
    _AnimateWindows();
  }
}

/*********************************************************************
*
*       _cbPlusMinusButton
*/
static void _cbPlusMinusButton(WM_MESSAGE * pMsg) {
  int      Id;
  GUI_RECT Rect;

  switch(pMsg->MsgId) {
  case WM_PAINT:
    Id = WM_GetId(pMsg->hWin);
    GUI_GetClientRect(&Rect);
    switch(Id) {
    case ID_BUTTON_WEIGHT_LESS:
      if(BUTTON_IsPressed(pMsg->hWin)) {
        GUI_SetColor(COLOR_BUTTON_1);
      } else {
        GUI_SetColor(COLOR_BUTTON_0);
      }
      GUI_AA_FillRoundedRectEx(&Rect, 3);
      GUI_SetColor(GUI_BLACK);
      GUI_FillRect(9, 16, 26, 19);

      GUI_SetColor(COLOR_BUTTON_OUTLINE);
      GUI_FillRect(9, 15, 26, 15);
      GUI_FillRect(27, 16, 27, 19);
      GUI_FillRect(8, 16, 8, 19);
      GUI_FillRect(9, 20, 26, 20);
      break;
    case ID_BUTTON_WEIGHT_MORE:
      if(BUTTON_IsPressed(pMsg->hWin)) {
        GUI_SetColor(COLOR_BUTTON_1);
      } else {
        GUI_SetColor(COLOR_BUTTON_0);
      }
      GUI_AA_FillRoundedRectEx(&Rect, 3);
      GUI_SetColor(GUI_BLACK);
      GUI_FillRect(16, 9, 19, 26);
      GUI_FillRect(9, 16, 26, 19);

      GUI_SetColor(COLOR_BUTTON_OUTLINE);
      GUI_FillRect(16, 8, 19, 8);
      GUI_FillRect(15, 9, 15, 15);
      GUI_FillRect(9, 15, 15, 15);
      GUI_FillRect(8, 16, 8, 19);
      GUI_FillRect(9, 20, 15, 20);
      GUI_FillRect(15, 20, 15, 26);
      GUI_FillRect(16, 27, 19, 27);
      GUI_FillRect(20, 20, 20, 26);
      GUI_FillRect(20, 20, 26, 20);
      GUI_FillRect(27, 16, 27, 19);
      GUI_FillRect(20, 15, 26, 15);
      GUI_FillRect(20, 9, 20, 15);
      break;
    }
    
    GUI_SetColor(BK_COLOR);
    GUI_FillRect(0, 0, 1, 1);
    GUI_FillRect(0, 34, 1, 35);
    GUI_FillRect(34, 34, 35, 35);
    GUI_FillRect(34, 0, 35, 1);

    GUI_SetColor(GUI_BLACK);
    GUI_SetPenSize(2);
    SHRINK_RECT(&Rect, 1);
    GUI_AA_DrawRoundedRectEx(&Rect, 3);
    break;
  default:
    BUTTON_Callback(pMsg);
    break;
  }

}

/*********************************************************************
*
*       _cbActionButton
*/
static void _cbActionButton(WM_MESSAGE * pMsg) {

  switch(pMsg->MsgId) {
  case WM_PAINT:
    if(BUTTON_IsPressed(pMsg->hWin)) {
      GUI_DrawBitmap(&bmActionButton_Pressed_130x80, 0, 0);
    } else {
      GUI_DrawBitmap(&bmActionButton_Unpressed_130x80, 0, 0);
    }
    break;
  default:
    BUTTON_Callback(pMsg);
    break;
  }

}

/*********************************************************************
*
*       _DrawWeight
*/
static void _DrawWeight(int Weight, int Index, int Pos) {
  GUI_DrawBitmap(_apbmWeight[Weight], 158 - bmWeight_small_6x50.XSize * Index, _aWeightPos[Pos]);
  GUI_DrawBitmap(_apbmWeight[Weight], 310 + bmWeight_small_6x50.XSize * Index, _aWeightPos[Pos]);
}

/*********************************************************************
*
*       _DrawWeights
*/
static void _DrawWeights(int WeightIndex, int Pos) {
  int NumPasses;
  int i;

  NumPasses = (WeightIndex / GUI_COUNTOF(_apbmWeight));
  for (i = 0; i < NumPasses; i++) {
    if(i % 2) {
      GUI_SetColor(COLOR_WEIGHT_RED);
    } else {
      GUI_SetColor(COLOR_WEIGHT_BLACK);
    }
    _DrawWeight(2, i, Pos);
  }

  if(NumPasses % 2) {
    GUI_SetColor(COLOR_WEIGHT_RED);
  } else {
    GUI_SetColor(COLOR_WEIGHT_BLACK);
  }
  _DrawWeight(WeightIndex % GUI_COUNTOF(_apbmWeight), NumPasses, Pos);
}

/*********************************************************************
*
*       _GetXPosAdd
*/
static int _GetXPosAdd(void) {
  srand(GUI_GetTime());

  switch(_Weight) {
  case 70:
    return 6;
  case 80:
    return (rand() % 2 == 0) ? 5 : 6;
  case 90:
    return 5;
  case 100:
    return (rand() % 2 == 0) ? 4 : 5;
  case 110:
    return 4;
  case 120:
    return (rand() % 2 == 0) ? 3 : 4;
  case 130:
    return 3;
  case 140:
    return (rand() % 2 == 0) ? 2 : 3;
  case 150:
    return 2;
  case 160:
    return (rand() % 2 == 0) ? 1 : 2;
  case 170:
    return 1;
  case 180:
    return rand() % 2;
  default:
    return 0;
  }
}

/*********************************************************************
*
*       _cbProgWin0
*/
static void _cbProgWin0(WM_MESSAGE * pMsg) {
  static int    * pData;
         GUI_RECT Rect;

  switch (pMsg->MsgId) {
  case WM_PAINT:
    WM_GetClientRect(&Rect);
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();

    GUI_SetColor(GUI_WHITE);
    GUI_AA_DrawRoundedRectEx(&Rect, 4);
    SHRINK_RECT(&Rect, 1);
    GUI_AA_DrawRoundedRectEx(&Rect, 4);

    if (pData) {
      GUI_SetColor(GUI_RED);
      Rect.x0 += 2;
      Rect.y0 += 2;
      Rect.x1  = Rect.x0 + *pData - 1;
      Rect.y1 -= 2;
      GUI_AA_FillRoundedRectEx(&Rect, 3);
    }
    break;
  case WM_USER_DATA:
    WM_GetUserData(pMsg->hWin, &pData, sizeof(int *));
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbProgWin1
*/
static void _cbProgWin1(WM_MESSAGE * pMsg) {
  static int    * pData;
         int      WeightIndex;
         GUI_RECT Rect;

  switch (pMsg->MsgId) {
  case WM_PAINT:
    WM_GetClientRect(&Rect);
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();

    GUI_SetColor(GUI_WHITE);
    GUI_AA_DrawRoundedRectEx(&Rect, 4);
    SHRINK_RECT(&Rect, 1);
    GUI_AA_DrawRoundedRectEx(&Rect, 4);

    WeightIndex = (_Weight - 70) / 10;
    GUI_SetColor(GUI_BLUE);
    Rect.x0  = WM_GetWindowSizeX(pMsg->hWin) / 2 - (_aHitRectSize[WeightIndex] / 2);
    Rect.y0 += 2;
    Rect.x1  = Rect.x0 + _aHitRectSize[WeightIndex];
    Rect.y1 -= 2;
    GUI_AA_FillRoundedRectEx(&Rect, 2);
    if (pData) {
      GUI_SetColor(GUI_RED);
      GUI_DrawVLine(*pData, 2, WM_GetWindowSizeY(pMsg->hWin) - 3);
    }
    break;
  case WM_USER_DATA:
    WM_GetUserData(pMsg->hWin, &pData, sizeof(int *));
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbRecord
*/
static void _cbRecord(WM_MESSAGE * pMsg) {
  char acBuf[4];

  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetFont(FONT_C64_14);
    GUI_SetColor(GUI_WHITE);
    GUI_SetBkColor(BK_COLOR);
    GUI_DispStringAt("RECORD: ", 0, 0);
    GUI_DispStringAt("kg", 121, 0);
    sprintf(acBuf, "%i", _Record);
    GUI_SetColor(GUI_BLUE);
    GUI_DispStringAt(acBuf, 85, 0);
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbCurrentWeight
*/
static void _cbCurrentWeight(WM_MESSAGE * pMsg) {
  char acBuf[4];

  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetFont(FONT_C64_18);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetColor(GUI_RED);
    sprintf(acBuf, "%i", _Weight);
    GUI_DispStringAt(acBuf, 0, 0);
    GUI_SetColor(GUI_WHITE);
    GUI_DispStringAt("kg", 45, 0);
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
  static WM_HTIMER       hTimerFirstAnim;
  static WM_HTIMER       hTimerPushBar;
  static WM_HTIMER       hTimerSweatAnim1;
  static WM_HTIMER       hTimerFocusBarStart;
  static WM_HTIMER       hTimerSweatAnim2;
  static WM_HTIMER       hTimerFocusBar;
  static WM_HTIMER       hTimerWin;
  static WM_HTIMER       hTimerResetActionText;
  static WM_HTIMER       hTimerEnd;
  static WM_HTIMER       hTimerStruggleAnim;
  static WM_HTIMER       hTimerEndAnim;
  static WM_HTIMER       hTimer11;
  static WM_HTIMER       hTimer12;
  static WM_HTIMER       hTimer13;
  static WM_HTIMER       hTimerFocusTimeout;
  static int             Index;
  static int             TimeLastPress;
  static int             TimeStart;
  static int             xPos;
  static int             Timer0IsRunning;
  static int             Timer1IsRunning;
  static int             Timer4IsRunning;
  static int             Timer11IsRunning;
  static int             Decrement;
  static int             OnCooldown;
  static int             i;
  static int             Tries;
  static int             SweatAnim1, SweatAnim2;
  static GUI_ANIM_HANDLE hAnim;
  static ANIM_TITLE_DATA AnimData;
         int             Degrees;
         float           Percent;
         int             NCode;
         int             Id;
         WM_HWIN         hItem;
         int             xDist;
         char            acKg[4];
         int             WeightIndex;
         int             Time;
         int             Sub;
         int           * pxData;
         GUI_RECT        Rect;

  switch (pMsg->MsgId) {
  case WM_CREATE:
    in_main_menu_flag = 0;
	//
    // Create minus button
    //
    _hButtonMinus = BUTTON_CreateEx(-36, 170, 36, 36, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_WEIGHT_LESS);
    WM_SetCallback(_hButtonMinus, _cbPlusMinusButton);
    //
    // Create plus button
    //
    _hButtonPlus = BUTTON_CreateEx(-36, 125, 36, 36, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_WEIGHT_MORE);
    WM_SetCallback(_hButtonPlus, _cbPlusMinusButton);
    //
    // Create action button
    //
    _hButtonAction = BUTTON_CreateEx(LCD_X, 140, 130, 80, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_WEIGHT_ACTION);
    WM_SetCallback(_hButtonAction, _cbActionButton);
    //
    // Create menu button
    //
    APP_CreateMenuButton(pMsg->hWin);
    _Record = 0;
    _Weight = 70;
    //
    // Create 'current weight' window
    //
    _hWeight = WM_CreateWindowAsChild(380, LCD_Y, 80, 30, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbCurrentWeight, 0);
    //
    // Create Progbar window
    //
    _hProgwin = WM_CreateWindowAsChild(10, LCD_Y, 187, 20, pMsg->hWin, WM_CF_SHOW, _cbProgWin0, sizeof(int *));
    pxData    = &xPos;
    WM_SetUserData(_hProgwin, &pxData, sizeof(int *));
    //
    // Create action text widget
    //
    _hAction = TEXT_CreateEx(210, LCD_Y, 160, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_TEXT_WEIGHT_ACTION, _acActionText[0]);
    TEXT_SetFont(_hAction, FONT_C64_14);
    TEXT_SetTextColor(_hAction, GUI_WHITE);
    TEXT_SetTextAlign(_hAction, TEXT_CF_HCENTER);
    //
    // Create record window
    //
    _hRecord = WM_CreateWindowAsChild(-150, 10, 150, 20, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbRecord, 0);
    //
    // Create "Tries" widgets
    //
    hItem = TEXT_CreateEx(5, 213, 100, 20, pMsg->hWin, WM_CF_HIDE, 0, ID_TEXT_WEIGHT_TRIES_T, "TRIES: ");
    TEXT_SetFont(hItem, FONT_C64_14);
    TEXT_SetTextColor(hItem, GUI_WHITE);
    TEXT_SetTextAlign(hItem, TEXT_CF_LEFT);
    hItem = TEXT_CreateEx(80, 213, 50, 20, pMsg->hWin, WM_CF_HIDE, 0, ID_TEXT_WEIGHT_TRIES, "");
    TEXT_SetFont(hItem, FONT_C64_14);
    TEXT_SetTextColor(hItem, GUI_BLUE);
    TEXT_SetTextAlign(hItem, TEXT_CF_LEFT);
    //
    // Create title window
    //
    hItem = WM_CreateWindowAsChild(0, -TITLE_YSIZE, LCD_X, TITLE_YSIZE, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbTitle, 0);
    //
    // Show Start-up animation
    //
    AnimData.hWin  = hItem;
    AnimData.xPos  = WM_GetWinOrgX(hItem);
    AnimData.yPos  = WM_GetWinOrgY(hItem);
    AnimData.xSize = WM_GetXSize(hItem);
    hAnim = GUI_ANIM_Create(2300, 25, &AnimData, NULL);
    GUI_ANIM_AddItem(hAnim, 0, 1000, ANIM_ACCEL, &AnimData, _AnimTitleIn);
    GUI_ANIM_AddItem(hAnim, 2000, 2300, ANIM_ACCEL, &AnimData, _AnimTitleOut);
    GUI_ANIM_StartEx(hAnim, 1, NULL);
    break;
  case WM_PAINT:
    //
    // Draw background
    //
    GUI_SetBkColor(BK_COLOR);
    GUI_Clear();
    GUI_SetColor(RENESAS_BLUE);
    GUI_DrawBitmap(&BM_RENESAS, 25, 90);
    GUI_SetColor(GUI_WHITE);
    GUI_DrawBitmap(&bmFloor_480x65, 0, 170);

    GUI_SetFont(FONT_C64_18);
    GUI_SetColor(GUI_WHITE);
    xDist = GUI_GetStringDistX("WEIGHT LIFTING");
    GUI_DispStringAt("WEIGHT LIFTING", (LCD_X - xDist) / 2, 35);

    GUI_SetFont(FONT_C64_20);
    Rect.x0 = 300;
    Rect.y0 = 80;
    Rect.x1 = Rect.x0 + 160;
    Rect.y1 = Rect.y0 + 70;
    GUI_DispStringInRectWrap(DEMO_TITLE, &Rect, GUI_TA_CENTER, GUI_WRAPMODE_WORD);
    //
    // Draw player
    //
    GUI_DrawBitmap(APBM_LIFT[Index], LIFTER_POS_X, LIFTER_POS_Y);
    //
    // Draw weights
    //
    WeightIndex = (_Weight - 70) / 10;
    _DrawWeights(WeightIndex, Index);
    //
    // Draw black bottom area
    //
    GUI_SetColor(GUI_BLACK);
    GUI_FillRect(0, 234, LCD_X, LCD_Y);
    //
    // Draw sweat
    //
    GUI_SetColor(COLOR_SWEAT);
    if(SweatAnim1) {
      GUI_FillRect(259, 101, 262, 104);
      GUI_FillRect(262, 111, 265, 113);
      GUI_FillRect(212, 102, 213, 104);
      GUI_FillRect(208, 111, 211, 112);
    }
    if(SweatAnim2) {
      GUI_FillRect(259, 88, 262, 92);
      GUI_FillRect(265, 95, 267, 97);
      GUI_FillRect(212, 88, 213, 91);
      GUI_FillRect(205, 95, 207, 97);
    }
    //
    // Draw timer
    //
    if(Timer4IsRunning) {
      Time = GUI_GetTime() - TimeStart;
      Percent = (float) Time / _aWeightTimeMs[WeightIndex];
      Percent = 1 - Percent;
      Degrees = (int) (360 * Percent);
      Degrees -= 270;
      GUI_SetColor(GUI_BLUE);
      GUI_DrawPie(TIMER_POS_X, TIMER_POS_Y, 14, -270, Degrees, 0);

      GUI_SetPenSize(2);
      GUI_SetColor(GUI_WHITE);
      GUI_AA_DrawCircle(TIMER_POS_X, TIMER_POS_Y, 13);
    }
    break;
  case WM_TIMER:
    Id = WM_GetTimerId(pMsg->Data.v);
    switch (Id) {
    case ID_TIMER_0: // hTimerFirstAnim
      if(Index < 5) {
        Index++;
        WM_RestartTimer(hTimerFirstAnim, 250);
      } else {
        if(hTimerSweatAnim1 == 0) {
          hTimerSweatAnim1 = WM_CreateTimer(pMsg->hWin, ID_TIMER_2, 250, 0);
        }
      }
      WM_InvalidateWindow(pMsg->hWin);
      break;
    case ID_TIMER_1: // hTimerPushBar
      Time = GUI_GetTime() - TimeLastPress;
      if(Time < 150) {
        xPos += _GetXPosAdd();
      } else {
        Sub = (_Weight > 130) ? 1 : 2;
        xPos = (xPos >= 2) ? xPos - Sub : 0;
      }
      if (xPos > 0) {
        WM_RestartTimer(hTimerPushBar, 50);
        if(xPos >= 181) {
          xPos = 181;
          WM_DeleteTimer(hTimerPushBar);
          WM_DeleteTimer(hTimerSweatAnim1);
          hTimerFocusBarStart = WM_CreateTimer(pMsg->hWin, ID_TIMER_3, 0, 0);
        }
      } else {
        Timer0IsRunning = Timer1IsRunning = 0;
        hTimer11 = WM_CreateTimer(pMsg->hWin, ID_TIMER_11, 3000, 0);
        Timer11IsRunning = 1;
      }
      WM_InvalidateWindow(_hProgwin);
      break;
    case ID_TIMER_2: // hTimerSweatAnim1
      Index = 5;
      SweatAnim1 = (SweatAnim1) ? 0 : 1;
      WM_InvalidateWindow(pMsg->hWin);
      WM_RestartTimer(hTimerSweatAnim1, 250);
      break;
    case ID_TIMER_3: // hTimerFocusBarStart
      SweatAnim1 = 0;
      if(Index < 8) {
        Index++;
        WM_RestartTimer(hTimerFocusBarStart, 250);
      } else {
        TEXT_SetText(_hAction, _acActionText[2]);

        TimeStart = GUI_GetTime();
        WM_DeleteTimer(hTimerFocusBarStart);
        hTimerSweatAnim2 = WM_CreateTimer(pMsg->hWin, ID_TIMER_4, 0, 0);
        Timer4IsRunning = 1;
        hTimerFocusBar = WM_CreateTimer(pMsg->hWin, ID_TIMER_5, 0, 0);
        WM_SetCallback(_hProgwin, _cbProgWin1);
        pxData = &xPos;
        WM_SetUserData(_hProgwin, &pxData, sizeof(int *));
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WEIGHT_TRIES_T);
        WM_ShowWindow(hItem);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WEIGHT_TRIES);
        WM_ShowWindow(hItem);

        WeightIndex = (_Weight - 70) / 10;
        hTimerFocusTimeout = WM_CreateTimer(pMsg->hWin, ID_TIMER_14, _aWeightTimeMs[WeightIndex], 0);
      }
      WM_InvalidateWindow(pMsg->hWin);
      break;
    case ID_TIMER_4: // hTimerSweatAnim2
      SweatAnim2 = (SweatAnim2) ? 0 : 1;
      WM_RestartTimer(hTimerSweatAnim2, 250);
      WM_InvalidateWindow(pMsg->hWin);
      break;
    case ID_TIMER_5: // hTimerFocusBar
      if(Decrement) {
        xPos -= (_Weight / 50);
        if(xPos < 3) {
          xPos = 2;
          Decrement = 0;
        }
      } else {
        xPos += (_Weight / 50);
        if(xPos >= FOCUS_MAX_X) {
          xPos = FOCUS_MAX_X;
          Decrement = 1;
        }
      }
      WM_RestartTimer(hTimerFocusBar, 10);
      WM_InvalidateWindow(_hProgwin);
      break;
    case ID_TIMER_6: // hTimerWin
      if(Index < 12) {
        Index++;
        WM_RestartTimer(hTimerWin, 250);
      } else {
        // Set record
        if(_Record < _Weight) {
          _Record = _Weight;
        }

        WM_DeleteTimer(hTimerWin);
        TEXT_SetText(_hAction, _acActionText[3]);
        WM_DeleteTimer(hTimerFocusTimeout);

        i = 0;
        hTimerEnd = WM_CreateTimer(pMsg->hWin, ID_TIMER_8, 3000, 0);
        hTimerEndAnim = WM_CreateTimer(pMsg->hWin, ID_TIMER_10, 1000, 0);
      }
      WM_InvalidateWindow(pMsg->hWin);
      break;
    case ID_TIMER_7: // hTimerResetActionText
      OnCooldown = 0;
      TEXT_SetText(_hAction, _acActionText[2]);

      WM_DeleteTimer(hTimerResetActionText);
      break;
    case ID_TIMER_8: // hTimerEnd
      //
      // Increase weights
      //
      if(_Weight < 180) {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WEIGHT_KG);
        sprintf(acKg, "%d", _Weight + 10);
        TEXT_SetText(hItem, acKg);
        _Weight += 10;
      }

      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WEIGHT_RECORD);
      sprintf(acKg, "%d", _Record);
      TEXT_SetText(hItem, acKg);

      TEXT_SetText(_hAction, _acActionText[0]);
      _LockWeight = 0;
      WM_DeleteTimer(hTimerEnd);

      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WEIGHT_TRIES_T);
      WM_HideWindow(hItem);
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WEIGHT_TRIES);
      WM_HideWindow(hItem);

      Index = 0;
      Timer0IsRunning = Timer1IsRunning = 0;
      hTimerFirstAnim = hTimerPushBar = hTimerSweatAnim1 = hTimerFocusBarStart = hTimerSweatAnim2 = hTimerFocusBar = hTimerWin = hTimerResetActionText = hTimerEnd = hTimerStruggleAnim = hTimerEndAnim = hTimer11 = hTimer13 = hTimerFocusTimeout = 0;
      xPos = 0;
      WM_SetCallback(_hProgwin, _cbProgWin0);
      WM_InvalidateWindow(pMsg->hWin);
      WM_InvalidateWindow(_hProgwin);
      break;
    case ID_TIMER_9: // hTimerStruggle
      if(Index < 10) {
        Index++;
        WM_RestartTimer(hTimerStruggleAnim, 250);
      } else {
        Index = 8;
        hTimerSweatAnim2 = WM_CreateTimer(pMsg->hWin, ID_TIMER_4, 0, 0);
        WM_DeleteTimer(hTimerStruggleAnim);
      }
      WM_InvalidateWindow(pMsg->hWin);
      break;
    case ID_TIMER_10: // hTimerEndAnim
      if(i < 5) {
        Index = _aEndAnimFrames[i];
        i++;
        WM_RestartTimer(hTimerEndAnim, 200);
      } else {
        WM_DeleteTimer(hTimerEndAnim);
      }
      WM_InvalidateWindow(pMsg->hWin);
      break;
    case ID_TIMER_11: // hTimer11
      _LockWeight = 0;
      WM_DeleteTimer(hTimerSweatAnim1);
      WM_DeleteTimer(hTimer11);
      SweatAnim1 = 0;
      i = 1;
      hTimerEndAnim = WM_CreateTimer(pMsg->hWin, ID_TIMER_10, 0, 0);

      Timer0IsRunning = Timer1IsRunning = Timer4IsRunning = Timer11IsRunning = 0;
      hTimerFirstAnim = hTimerPushBar = hTimerSweatAnim1 = hTimerFocusBarStart = hTimerSweatAnim2 = hTimerFocusBar = hTimerWin = hTimerResetActionText = hTimerEnd = hTimerStruggleAnim = hTimer11 = hTimer13 = hTimerFocusTimeout = 0;
      xPos = 0;

      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WEIGHT_TRIES_T);
      WM_HideWindow(hItem);
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WEIGHT_TRIES);
      WM_HideWindow(hItem);

      TEXT_SetText(_hAction, _acActionText[0]);
      break;
    case ID_TIMER_12: // hTimer12
      WM_SetCallback(_hProgwin, _cbProgWin0);
      _LockWeight = 0;
      OnCooldown = 0;

      Timer0IsRunning = Timer1IsRunning = Timer4IsRunning = Timer11IsRunning = 0;
      hTimerFirstAnim = hTimerPushBar = hTimerSweatAnim1 = hTimerFocusBarStart = hTimerSweatAnim2 = hTimerFocusBar = hTimerWin = hTimerResetActionText = hTimerEnd = hTimerStruggleAnim = hTimerEndAnim = hTimer11 = hTimer13 = hTimerFocusTimeout = 0;
      xPos = 0;

      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WEIGHT_TRIES_T);
      WM_HideWindow(hItem);
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WEIGHT_TRIES);
      WM_HideWindow(hItem);

      TEXT_SetText(_hAction, _acActionText[0]);
      WM_DeleteTimer(hTimer12);
      break;
    case ID_TIMER_13: // hTimer13
      if(Index < 10) {
        Index++;
        WM_RestartTimer(hTimer13, 200);
      } else {
        WM_DeleteTimer(hTimer13);
      }
      WM_InvalidateWindow(pMsg->hWin);
      break;
    case ID_TIMER_14: // hTimerFocusTimeout
      WM_DeleteTimer(hTimerSweatAnim2);
      WM_DeleteTimer(hTimerFocusBar);
      WM_DeleteTimer(hTimerFocusTimeout);
      Timer4IsRunning = SweatAnim2 = i = 0;
      TEXT_SetText(_hAction, _acActionText[6]);
      hTimerEndAnim = WM_CreateTimer(pMsg->hWin, ID_TIMER_10, 500, 0);
      hTimer12 = WM_CreateTimer(pMsg->hWin, ID_TIMER_12, 2000, 0);
      WM_InvalidateWindow(pMsg->hWin);
      break;
    }
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_WEIGHT_LESS:
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        if(_LockWeight == 0) {
          if(_Weight != 70) {
            _Weight -= 10;
          }
          WM_InvalidateWindow(pMsg->hWin);
        }
        break;
      }
      break;
    case ID_BUTTON_WEIGHT_MORE:
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        if(_LockWeight == 0) {
          if(_Weight != 180) {
            _Weight += 10;
          }
          WM_InvalidateWindow(pMsg->hWin);
        }
        break;
      }
      break;
    case ID_BUTTON_MENU:
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        AddHighscoreWeight(_Record);
        hTimerFirstAnim = hTimerPushBar = hTimerSweatAnim1 = hTimerFocusBarStart = hTimerSweatAnim2 = hTimerFocusBar = hTimerWin = hTimerResetActionText = hTimerEnd = hTimerStruggleAnim = hTimerEndAnim = hTimer11 = hTimer12 = hTimer13 = hTimerFocusTimeout = 0;
        Index = Decrement = OnCooldown = Tries = TimeLastPress = xPos = 0;
        Timer0IsRunning = Timer1IsRunning = Timer4IsRunning = Timer11IsRunning = 0;
        SweatAnim1 = SweatAnim2 = 0;
        GUI_ANIM_DeleteAll();
        RemoveCurrentWindow();
        in_main_menu_flag = 1;
        break;
      }
      break;
    case ID_BUTTON_WEIGHT_ACTION:
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        _LockWeight = 1;
        if(Timer11IsRunning == 1) {
          WM_DeleteTimer(hTimer11);
          Timer11IsRunning = 0;
        }
        if (Timer0IsRunning == 0) {
          if (hTimerFirstAnim == 0) {
            Tries = MAX_TRIES;
            sprintf(acKg, "%d", Tries);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WEIGHT_TRIES);
            TEXT_SetText(hItem, acKg);

            hTimerFirstAnim = WM_CreateTimer(pMsg->hWin, ID_TIMER_0, 0, 0);
            TEXT_SetText(_hAction, _acActionText[1]);
          } else {
            WM_RestartTimer(hTimerFirstAnim, 0);
          }
          Timer0IsRunning = 1;
        }
        TimeLastPress = GUI_GetTime();
        if (Timer1IsRunning == 0) {
          if (hTimerPushBar == 0) {
            hTimerPushBar = WM_CreateTimer(pMsg->hWin, ID_TIMER_1, 50, 0);
          } else {
            WM_RestartTimer(hTimerPushBar, 0);
          }
          Timer1IsRunning = 1;
        }
        if(Timer4IsRunning) {
          WeightIndex = (_Weight - 70) / 10;
          if(OnCooldown == 0) {
            if(xPos > (92 - (_aHitRectSize[WeightIndex] / 2)) && xPos < (93 + (_aHitRectSize[WeightIndex] / 2))) {
              WM_DeleteTimer(hTimerSweatAnim2);
              SweatAnim2 = 0;
              Timer4IsRunning = 0;
              WM_DeleteTimer(hTimerFocusBar);
              hTimerWin = WM_CreateTimer(pMsg->hWin, ID_TIMER_6, 0, 0);
            } else {
              Tries--;
              sprintf(acKg, "%d", Tries);
              hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WEIGHT_TRIES);
              TEXT_SetText(hItem, acKg);
              if(Tries) {
                OnCooldown = 1;
                TEXT_SetText(_hAction, _acActionText[4]);
                hTimerResetActionText = WM_CreateTimer(pMsg->hWin, ID_TIMER_7, 1000, 0);
                WM_DeleteTimer(hTimerSweatAnim2);
                SweatAnim2 = 0;
                hTimerStruggleAnim = WM_CreateTimer(pMsg->hWin, ID_TIMER_9, 0, 0);
              } else {
                i = 0;
                OnCooldown = 1;
                TEXT_SetText(_hAction, _acActionText[5]);
                WM_DeleteTimer(hTimerSweatAnim2);
                WM_DeleteTimer(hTimerFocusBar);
                WM_DeleteTimer(hTimerFocusTimeout);
                Timer4IsRunning = 0;
                SweatAnim2 = 0;

                hTimer13 = WM_CreateTimer(pMsg->hWin, ID_TIMER_13, 0, 0);
                hTimerEndAnim = WM_CreateTimer(pMsg->hWin, ID_TIMER_10, 800, 0);
                hTimer12 = WM_CreateTimer(pMsg->hWin, ID_TIMER_12, 2000, 0);
              }
            }
          }
        }
        break;
      }
      break;
    }
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
WM_HWIN CreateWindowWeightlifting(char * acName) {
  WM_HWIN hWin;

  GUI_USE_PARA(acName);
  hWin = WM_CreateWindowAsChild(0, 0, LCD_X, LCD_Y, WM_HBKWIN, WM_CF_SHOW, _cbWin, 0);
  return hWin;
}

/*************************** End of file ****************************/
