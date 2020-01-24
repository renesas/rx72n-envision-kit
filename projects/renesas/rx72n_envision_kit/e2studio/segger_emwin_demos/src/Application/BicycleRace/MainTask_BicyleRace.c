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
  int   * pBarData;
  float * pKmh;
  float * pKmhAvg;
} PROGWIN_DATA;

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
#define BM_TRACK            bmTrack_1x140
#define BM_LANDSCAPE        bmLandscape_480x132
#define APBM_RIDER          apbmBicycleRider_15F_134x122

#define SIZE_ROTARY         150
#define SIZE_BORDER         10
#define MAX_BARPOS          186

#define ID_TIMER_0          0
#define ID_TIMER_1          1
#define ID_TIMER_2          2
#define ID_TIMER_3          3
#define ID_TIMER_4          4
#define ID_TIMER_5          5
#define ID_TIMER_6          6
#define ID_TIMER_TIME       10

#define ID_ROTARY_0         (GUI_ID_USER + 0x00)

#define TRACK_DIST          750
#define MS_BETWEEN_COUNT    1000

#define COLOR_ASPHALT       GUI_MAKE_COLOR(0x00514E48)
#define COLOR_SKYBLUE1      GUI_MAKE_COLOR(0x00E1B979)
#define COLOR_SKYBLUE2      GUI_MAKE_COLOR(0x00FACE87)
#define COLOR_GRAY1         GUI_MAKE_COLOR(0x00808080)
#define COLOR_GRAY2         GUI_MAKE_COLOR(0x00E7E7E7)
#define COLOR_GRAY3         GUI_MAKE_COLOR(0x00A9A9A9)
#define COLOR_ACCEL_BAR     GUI_MAKE_COLOR(0x002100F0)

#define BANNER_STRING       "BICYCLE RACE"
#define MAX_STAMINA         1000

#define POS_PER_M           11.30112F
#define MIN_BARPOS          40
#define GOAL_POS            (int)(POS_PER_M * TRACK_DIST)

#define TOTAL_FRAMES        GUI_COUNTOF(APBM_RIDER)

#define TITLE_YSIZE         50
#define TITLE_YPOS          20
#define COLOR_TITLE_1       GUI_MAKE_COLOR(0x003AD4F0)
#define COLOR_TITLE_2       GUI_MAKE_COLOR(0x00DAB222)


/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static char  * _acName;
static int     _LastFrame;
static int     _PausePedal;
static int     _DisplayStats;
static WM_HWIN _hStatWin;
static WM_HWIN _hExhaustWin;
static WM_HWIN _hKnob;
static WM_HWIN _hProgWin;
static WM_HWIN _hCountWin;

static const WM_HWIN * _apWindows[] = {
  &_hStatWin,
  &_hProgWin,
  &_hExhaustWin,
  &_hKnob,
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _cbCountdown
*/
static void _cbCountdown(WM_MESSAGE * pMsg) {
         int       Id;
  static int       xSize, ySize;
  static int       GoDisp;
  static int       TimerStart;
  static int       Countdown;
  static WM_HTIMER hTimerCountdown;

  switch(pMsg->MsgId) {
  case WM_CREATE:
    Countdown       = 3;
    hTimerCountdown = WM_CreateTimer(pMsg->hWin, ID_TIMER_4, 0, 0);
    TimerStart      = 1;
    GoDisp          = 0;
    xSize           = WM_GetWindowSizeX(pMsg->hWin);
    ySize           = WM_GetWindowSizeY(pMsg->hWin);
    break;
  case WM_PAINT:
    GUI_SetFont(FONT_C64_24);
    if(TimerStart) {
      if(Countdown) {
        GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
        GUI_SetTextMode(GUI_TM_TRANS);
        GUI_SetFont(FONT_C64_32);
        GUI_SetColor(GUI_RED);
        GUI_DispDecAt(Countdown, xSize / 2, ySize / 2, 1);
        Countdown--;
        WM_RestartTimer(hTimerCountdown, MS_BETWEEN_COUNT);
      } else if (GoDisp == 0) {
        GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
        GUI_SetTextMode(GUI_TM_TRANS);
        GUI_SetFont(FONT_C64_32);
        GUI_SetColor(GUI_RED);
        GUI_DispStringAt("GO!", xSize / 2, ySize / 2);
        GoDisp = 1;
        WM_RestartTimer(hTimerCountdown, MS_BETWEEN_COUNT);
      }
    }
    break;
  case WM_TIMER:
    Id = WM_GetTimerId(pMsg->Data.v);
    switch(Id) {
    case ID_TIMER_4:
      if(GoDisp == 0) {
        WM_InvalidateWindow(pMsg->hWin);
        WM_RestartTimer(hTimerCountdown, MS_BETWEEN_COUNT);
      } else {
        WM_DeleteTimer(hTimerCountdown);
        hTimerCountdown = 0;
        WM_DeleteWindow(pMsg->hWin);
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
*       _AnimWindowDelete
*/
static void _AnimWindowDelete(void * pVoid) {
  ANIM_WIN_DATA * pData;
  WM_HWIN         hParent;

  pData = (ANIM_WIN_DATA*) pVoid;

  //
  // Create countdown window
  //
  if (pData) {
    hParent    = WM_GetParent(pData->hWin);
    _hCountWin = WM_CreateWindowAsChild((LCD_X / 2) - (90 / 2), (LCD_Y / 2) - (40 / 2), 90, 40, hParent, WM_CF_SHOW | WM_CF_HASTRANS, _cbCountdown, 0);
  }
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
  aData[0].Dist = 133;
  aData[1].Dist = -33;
  aData[2].Dist = 50;
  aData[3].Dist = -(SIZE_ROTARY + SIZE_BORDER);

  hAnim = GUI_ANIM_Create(300, 25, &aData[3], NULL);
  GUI_ANIM_AddItem(hAnim, 0, 300, ANIM_ACCELDECEL, &aData[0], _AnimWindowInX);
  GUI_ANIM_AddItem(hAnim, 0, 300, ANIM_ACCELDECEL, &aData[1], _AnimWindowInY);
  GUI_ANIM_AddItem(hAnim, 0, 300, ANIM_ACCELDECEL, &aData[2], _AnimWindowInY);
  GUI_ANIM_AddItem(hAnim, 0, 300, ANIM_ACCELDECEL, &aData[3], _AnimWindowInX);
  GUI_ANIM_StartEx(hAnim, 1, _AnimWindowDelete);
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
    GUI_SetFont(FONT_OCTIN_48);
    xDist = GUI_GetStringDistX(BANNER_STRING);
    xSize = WM_GetWindowSizeX(pMsg->hWin);
    GUI_DispStringAt(BANNER_STRING, (xSize / 2) - (xDist / 2) + 1, 0);
    GUI_SetColor(COLOR_TITLE_1);
    GUI_DispStringAt(BANNER_STRING, (xSize / 2) - (xDist / 2) - 2, 0);
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _AnimateStats
*/
static void _AnimateStats(GUI_ANIM_INFO * pInfo, void * pVoid) {
  STAT_WIN_ANIM_DATA * pData;
  int                  xPos;
  int                  yPos;
  int                  xSize;
  int                  ySize;

  pData = (STAT_WIN_ANIM_DATA *)pVoid;
  xPos  = pData->xPos + ((pData->xDist * pInfo->Pos) / GUI_ANIM_RANGE);
  yPos  = pData->yPos + ((pData->yDist * pInfo->Pos) / GUI_ANIM_RANGE);
  xSize = pData->xSizeOrg + (((pData->xSizeNew - pData->xSizeOrg) * pInfo->Pos) / GUI_ANIM_RANGE);
  ySize = pData->ySizeOrg + (((pData->ySizeNew - pData->ySizeOrg) * pInfo->Pos) / GUI_ANIM_RANGE);
  WM_SetWindowPos(pData->hWin, xPos, yPos, xSize, ySize);
}

/*********************************************************************
*
*       _AnimDelete
*/
static void _AnimDelete(void * pVoid) {
  WM_HWIN              hItem;
  STAT_WIN_ANIM_DATA * pData;

  pData         = (STAT_WIN_ANIM_DATA *) pVoid;
  _DisplayStats = 1;
  hItem         = WM_GetDialogItem(WM_GetParent(pData->hWin), ID_BUTTON_MENU);
  WM_EnableWindow(hItem);
}

/*********************************************************************
*
*       _cbExhaustWin
*/
static void _cbExhaustWin(WM_MESSAGE * pMsg) {
         GUI_RECT Rect;
         int      i;
  static int    * pData;
         char     acBuf[8];

  switch (pMsg->MsgId) {
  case WM_PAINT:
    WM_GetClientRect(&Rect);
    Rect.x0 += 5;
    Rect.x1 -= 5;
    Rect.y1 -= 10;
    SHRINK_RECT(&Rect, 1);
    GUI_SetColor(GUI_BLACK);
    GUI_AA_DrawRoundedRectEx(&Rect, 3);
    SHRINK_RECT(&Rect, 1);
    GUI_SetColor(COLOR_EXHAUST);
    GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x0 + (((float)*pData / MAX_STAMINA) * (WM_GetXSize(pMsg->hWin) - 15)), Rect.y1, 1);
    GUI_SetColor(GUI_BLACK);
    GUI_DrawBitmap(&bmEnergy_18x19, (WM_GetXSize(pMsg->hWin) / 2) - (bmEnergy_18x19.XSize / 2), (WM_GetYSize(pMsg->hWin) / 2) - (bmEnergy_18x19.YSize / 2) - 5);

    //
    // Draw percentage
    //
    GUI_SetTextMode(GUI_TM_TRANS);
    Rect.x0 += 3;
    Rect.y1 += 2;
    for (i = 0; i < 3; i++) {
      GUI_DrawVLine(Rect.x0 + (i * 89), Rect.y1, Rect.y1 + 2);
      sprintf(acBuf, "%i%%", (i * 50));
      GUI_DispStringAt(acBuf, Rect.x0 + (i * 89) - pow(i + 2, 2), Rect.y1 + 4);
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
*       _cbStatWin
*/
static void _cbStatWin(WM_MESSAGE * pMsg) {
         GUI_RECT             Rect;
         int                  i;
         char                 acKmh[32];
  static GUI_ANIM_HANDLE      hAnim;
  static STAT_WIN_ANIM_DATA   AnimData;
  static STATS              * pData;

  switch(pMsg->MsgId) {
  case WM_PAINT:
    WM_GetClientRect(&Rect);
    SHRINK_RECT(&Rect, 1);
    GUI_SetColor(COLOR_STATWIN_2);
    GUI_FillRoundedRectEx(&Rect, 3);

    GUI_SetColor(GUI_BLACK);
    GUI_SetPenSize(2);
    GUI_AA_DrawRoundedRectEx(&Rect, 3);

    if(_DisplayStats == 0) {
      GUI_SetColor(GUI_RED);
      GUI_SetFont(FONT_C64_12);
      GUI_SetTextMode(GUI_TM_TRANS);
      if(_acName == 0) {
        memcpy(acKmh, "PLAYER 1", 9);
      } else {
        memcpy(acKmh, _acName, 11);
      }
      if(acKmh[0] == '\0') {
        GUI_DispStringAt("PLAYER 1", 7, 4);
      } else {
        GUI_DispStringAt(acKmh, 7, 4);
      }

      GUI_SetColor(GUI_BLUE);
      GUI_SetFont(FONT_C64_14);
      i = pData->Meters;
      i = (i > TRACK_DIST) ? TRACK_DIST : i;
      sprintf(acKmh, "%dm", i);
      GUI_DispStringAt(acKmh, 7, 20);
      i = (pData->TimeStart) ? GUI_GetTime() - pData->TimeStart : 0;
      i = (pData->FinalTime) ? pData->FinalTime : i;
      PRINT_TIME(acKmh, i);
      GUI_DispStringAt(acKmh, 7, 38);
    } else {
      GUI_SetColor(GUI_RED);
      GUI_SetFont(&GUI_FontCommodore64Angled20);
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_SetTextAlign(GUI_TA_HCENTER);
      GUI_DispStringAt("YOUR STATS", WM_GetWindowSizeX(pMsg->hWin) / 2, 8);

      GUI_SetColor(GUI_BLUE);
      GUI_SetFont(FONT_C64_18);
      sprintf(acKmh, "%dm", TRACK_DIST);
      GUI_DispStringAt(acKmh, STATWIN_BORDER + 2, 45);

      i = pData->FinalTime;
      PRINT_TIME(acKmh, i);
      GUI_DispStringAt(acKmh, (WM_GetWindowSizeX(pMsg->hWin) / 2) + STATWIN_BORDER - 2, 45);

      sprintf(acKmh, "%d km/h", pData->KmhBest);
      GUI_DispStringAt(acKmh, STATWIN_BORDER + 2, 100);
      sprintf(acKmh, "%d km/h", pData->KmhAvg);
      GUI_DispStringAt(acKmh, (WM_GetWindowSizeX(pMsg->hWin) / 2) + STATWIN_BORDER - 2, 100);

      GUI_SetColor(GUI_BLACK);
      GUI_SetFont(FONT_C64_12);
      GUI_DispStringAt("DISTANCE RODE", STATWIN_BORDER + 2, 70);
      GUI_DispStringAt("TIME", (WM_GetWindowSizeX(pMsg->hWin) / 2) + STATWIN_BORDER - 2, 70);
      GUI_DispStringAt("FASTEST SPEED", STATWIN_BORDER + 2, 125);
      GUI_DispStringAt("AVERAGE SPEED", (WM_GetWindowSizeX(pMsg->hWin) / 2) + STATWIN_BORDER - 2, 125);
    }
    break;
  case WM_USER_DATA:
    WM_GetUserData(pMsg->hWin, &pData, sizeof(STATS *));
    break;
  case MSG_ANIMATE:
    AnimData.hWin     = pMsg->hWin;
    AnimData.xPos     = WM_GetWindowOrgX(pMsg->hWin);
    AnimData.yPos     = WM_GetWindowOrgY(pMsg->hWin);
    AnimData.xSizeNew = 330;
    AnimData.ySizeNew = 160;
    AnimData.xSizeOrg = WM_GetWindowSizeX(pMsg->hWin);
    AnimData.ySizeOrg = WM_GetWindowSizeY(pMsg->hWin);
    AnimData.xDist    = ((WM_GetWinSizeX(WM_GetParent(pMsg->hWin)) - AnimData.xSizeNew) / 2) - AnimData.xPos;
    AnimData.yDist    = ((WM_GetWinSizeY(WM_GetParent(pMsg->hWin)) - AnimData.ySizeNew) / 2) - AnimData.yPos;
    hAnim = GUI_ANIM_Create(ANIM_PERIOD_STATS, 25, &AnimData, NULL);
    GUI_ANIM_AddItem(hAnim, 0, ANIM_PERIOD_STATS, ANIM_ACCELDECEL, &AnimData, _AnimateStats);
    GUI_ANIM_StartEx(hAnim, 1, _AnimDelete);
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }

}

/*********************************************************************
*
*       _cbRotary
*/
static void _cbRotary(WM_MESSAGE * pMsg) {
  int Radius;

  switch (pMsg->MsgId) {
  case WM_PAINT:
    //
    // Draw Rotary
    //
    Radius = SIZE_ROTARY >> 1;
    GUI_DrawBitmap(&bmKnob_150x150, 0, 0);

    GUI_SetColor(GUI_BLACK);
    GUI_SetPenSize(9);
    GUI_AA_DrawCircle(Radius, Radius, Radius - 4);
    GUI_AA_FillCircle(Radius, Radius, 14);
    break;
  default:
    KNOB_Callback(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbProgbar
*/
static void _cbProgbar(WM_MESSAGE * pMsg) {
         GUI_RECT       Rect;
  static PROGWIN_DATA * pData;
         char           acKmh[16];

  switch (pMsg->MsgId) {
  case WM_PAINT:
    //
    // Draw Accel Bar
    //
    GUI_SetColor(GUI_WHITE);
    GUI_SetBkColor(COLOR_ASPHALT);
    GUI_SetPenSize(2);
    Rect.x0 = 0;
    Rect.y0 = 3;
    Rect.x1 = 190;
    Rect.y1 = 23;
    GUI_AA_DrawRoundedRectEx(&Rect, 3);

    if (pData) {
      GUI_SetColor(COLOR_ACCEL_BAR);
      SHRINK_RECT(&Rect, 2);
      GUI_AA_FillRoundedRect(Rect.x0, Rect.y0, Rect.x0 + *(pData->pBarData), Rect.y1, 2);

      //
      // Display km/h
      //
      GUI_SetFont(FONT_C64_12);
      sprintf(acKmh, "%2.0f km/h", *(pData->pKmh));
      GUI_DispStringAt(acKmh, 200, 0);
      
      sprintf(acKmh, "%2.0f km/h (avg.)", *(pData->pKmhAvg));      
      GUI_SetColor(GUI_WHITE);
      GUI_DispStringAt(acKmh, 200, 15);
    }
    break;
  case WM_USER_DATA:
    WM_GetUserData(pMsg->hWin, &pData, sizeof(PROGWIN_DATA *));
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbBicycle
*/
static void _cbBicycle(WM_MESSAGE * pMsg) {
         WM_HWIN         hItem;
         int             Index;
         int             Value;
         int             xPos, yPos;
         int             xLen, yLen;
         int             Radius;
         int             i, j;
         int             Id;
         int             NCode;
         int             CosHQ;
         int             SinHQ;
         int             CurrentPos;
  static float           Kmh;
  static float           KmhAvg;
         STATS         * pStats;
  static PROGWIN_DATA    Data;
         PROGWIN_DATA  * pData;
  static WM_HTIMER       hTimer;
  static WM_HTIMER       hTimer1;
  static WM_HTIMER       hTimerEnd;
  static WM_HTIMER       hTimerStart;
  static WM_HTIMER       hTimerDecr;
  static WM_HTIMER       hTimerKnob;
  static int             rPos;
  static int             OldValue;
  static int             IsMoving;
  static int             BarPos; // max = 186
  static int             Pos0;
  static int             GoalReached;
  static STATS           Stats;
  static ANIM_TITLE_DATA AnimData;
  static GUI_ANIM_HANDLE hAnim;
  static int             Knob;
         int           * pExhaustion;
  static int             Exhaustion;
  const  GUI_POINT       aPoly[] = { { 0, 0 }, { -4, 4 }, { 65, 4 }, { 69, 0 }, };

  switch (pMsg->MsgId) {
  case WM_CREATE:
    in_main_menu_flag = 0;
	xPos = LCD_X - SIZE_ROTARY - SIZE_BORDER;
    yPos = LCD_Y - SIZE_ROTARY - SIZE_BORDER;

    GoalReached = IsMoving = BarPos = Pos0 = rPos = 0;
    GUI_ZEROFILL(&Stats, sizeof(Stats));
    //
    // Create menu button
    //
    APP_CreateMenuButton(pMsg->hWin);
    //
    // Create knob
    //
    _hKnob = KNOB_CreateEx(LCD_X, yPos, SIZE_ROTARY + 1, SIZE_ROTARY + 1, pMsg->hWin, WM_CF_SHOW, ID_ROTARY_0);
    WM_SetCallback(_hKnob, _cbRotary);
    KNOB_SetPeriod(_hKnob, 25);
    WM_DisableWindow(_hKnob);
    //
    // Create statistics window
    //
    _hStatWin = WM_CreateWindowAsChild(-125, 4, 125, 60, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbStatWin, sizeof(STATS *));
    pStats   = &Stats;
    WM_SetUserData(_hStatWin, &pStats, sizeof(STATS *));
    //
    // Create progress window
    //
    _hProgWin     = WM_CreateWindowAsChild(10, LCD_Y, 340, 40, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbProgbar, sizeof(PROGWIN_DATA *));
    Data.pBarData = &BarPos;
    Data.pKmh     = &Kmh;
    Data.pKmhAvg  = &KmhAvg;
    pData         = &Data;
    WM_SetUserData(_hProgWin, &pData, sizeof(PROGWIN_DATA *));
    //
    // Create stamina bar
    //
    _hExhaustWin = WM_CreateWindowAsChild(160, -40, 200, 40, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbExhaustWin, sizeof(int *));
    pExhaustion  = &Exhaustion;
    WM_SetUserData(_hExhaustWin, &pExhaustion, sizeof(int *));
    //
    // Create title window
    //
    hItem = WM_CreateWindowAsChild(0, -TITLE_YSIZE, LCD_X, TITLE_YSIZE, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbTitle, 0);
    WM_SetUntouchable(hItem, 1);
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
    //
    // Reset variables
    //
    Exhaustion = MAX_STAMINA;
    IsMoving   = 0;
    OldValue   = 0;
    rPos       = 0;
    hTimer     = 0;
    break;
  case WM_PAINT:
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    //
    // Draw sky and road
    //
    xPos = (rPos * LCD_X) / 3600;
    GUI_DrawGradientV(0, 0, LCD_X, 140, COLOR_SKYBLUE1, COLOR_SKYBLUE2);
    yPos = WM_GetWindowSizeY(pMsg->hWin) - 140;
    GUI_SetColor(COLOR_ASPHALT);
    GUI_FillRect(0, yPos, LCD_X, LCD_Y);
    //
    // Draw road markings
    //
    GUI_SetColor(GUI_WHITE);
    for(i = 0; i < 5; i++) {
      CurrentPos = (75 + 50) * i - ((Pos0 * 2) % (75 + 50));
      GUI_FillPolygon(aPoly, 4, CurrentPos, 205);
    }
    
    xPos = -(rPos * (LCD_X + 60 + 2000)) / 3600;
    //
    // Draw background buildings
    //
    for(i = 0; i < 3; i++) {
      CurrentPos = (bmBuildings_358x132.XSize) * i - ((Pos0) % (bmBuildings_358x132.XSize));
      GUI_DrawBitmap(&bmBuildings_358x132, CurrentPos, 0);
    }
    //
    // Draw crowd
    //
    for(i = 0; i < 6; i++) {
      CurrentPos = (bmCrowd_130x90.XSize - 10) * i - ((Pos0 * 2) % (bmCrowd_130x90.XSize - 10));
      GUI_DrawBitmap(&bmCrowd_130x90, CurrentPos, 66);
    }
    //
    // Barriers and banners
    //
    for (i = 0; i < 4; i++) {
      CurrentPos = (bmBarrier_224x56.XSize + 5) * i - ((Pos0 * 2) % (bmBarrier_224x56.XSize + 5));
      GUI_SetColor(COLOR_GRAY3);
      GUI_DrawBitmap(&bmBarrier_224x56, CurrentPos, 100);

      GUI_SetColor(COLOR_GRAY2);
      GUI_AA_FillRoundedRect(CurrentPos + 8, 106, CurrentPos + 216, 142, 4);

      GUI_SetColor(COLOR_GRAY1);
      GUI_AA_DrawRoundedRect(CurrentPos + 8, 106, CurrentPos + 216, 142, 4);

      GUI_FillRect(CurrentPos + 20, 100, CurrentPos + 21, 106);
      GUI_FillRect(CurrentPos + 204, 100, CurrentPos + 205, 106);
    }
    //
    // Draw content of banners
    //
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(FONT_C64_18);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_DispStringAt(BANNER_STRING, LCD_X - (((Pos0 * 2 + 452)) % (LCD_X + 207)), 115);
    GUI_SetColor(GUI_GRAY_60);
    GUI_DispStringAt(DEMO_TITLE, LCD_X - (((Pos0 * 2 + 916)) % (LCD_X + 207)), 115);
    GUI_SetColor(RENESAS_BLUE);
    GUI_DrawBitmap(&BM_RENESAS, LCD_X - ((Pos0 * 2 + 8) % (LCD_X + 207)), 109);
    //
    // Draw right part of start
    //
    GUI_DrawBitmap(&bmStartFinish_Right_125x264, (90) - ((Pos0 * 2) % (30000 * 2 + LCD_X)), 0);
    //
    // Draw right part of goal
    //
    GUI_DrawBitmap(&bmStartFinish_Right_125x264, (GOAL_POS * 2 + 100) - ((Pos0 * 2) % (GOAL_POS * 2 + 100 + LCD_X)), 0);
    //
    // Draw spokes
    //
    GUI_SetColor(GUI_BLACK);
    for(i = 0; i < 2; i++) {
      Radius = 25;
      xPos   = 47 + (i * 80);
      yPos   = 208;
      GUI_SetPenSize(1);
      for(j = 0; j < 20; j++) {
        CosHQ = GUI__CosHQ((((Pos0 * 4) % 3600) + (j * 90)) * 200);
        SinHQ = GUI__SinHQ((((Pos0 * 4) % 3600) + (j * 90)) * 200);
        xLen  = (CosHQ * (Radius)) >> 16;
        yLen  = (SinHQ * (Radius)) >> 16;

        GUI_AA_DrawLine(xPos, yPos, xPos + xLen, yPos + yLen);
      }
    }
    //
    // Draw biker
    //
    if(_PausePedal && _LastFrame != -1) {
      Index = _LastFrame;
    } else {
      if(_LastFrame != -1 && ((Pos0 / 5) % TOTAL_FRAMES) != _LastFrame) {
        Index = _LastFrame;
      } else if(_LastFrame != -1 && ((Pos0 / 5) % TOTAL_FRAMES) == _LastFrame) {
        Index = ((Pos0 / 5) % TOTAL_FRAMES);
        _LastFrame = -1;
      } else if(_LastFrame == -1) {
        Index = ((Pos0 / 5) % TOTAL_FRAMES);
      }
    }
    GUI_DrawBitmap(APBM_RIDER[Index], 20, LCD_Y - 160);
    //
    // Draw left part of start
    //
    GUI_DrawBitmap(&bmStartFinish_Left_26x160, (90) - ((Pos0 * 2) % (30000 * 2 + LCD_X)), 112);
    //
    // Draw left part of goal
    //
    GUI_DrawBitmap(&bmStartFinish_Left_26x160, (GOAL_POS * 2 + 100) - ((Pos0 * 2) % (GOAL_POS * 2 + 100 + LCD_X)), 112);
    //
    // Calculate km/h
    //
    Kmh = (BarPos) ? ((((float)BarPos / 20.0F + 1) / POS_PER_M) * 15.0F) * 3.6F + 1 : 0;
    if(Kmh > Stats.KmhBest) {
      Stats.KmhBest = Kmh;
    }

    i = GUI_GetTime() - Stats.TimeStart;
    KmhAvg  = (i > 1000) ? ((float)Stats.Meters / ((float)i / 1000.0F)) * 3.6F : 0;
    KmhAvg *= 0.79F;
    if(GoalReached) {
      KmhAvg = Stats.KmhAvg;
    }
    break;
  case WM_TIMER:
    Id = WM_GetTimerId(pMsg->Data.v);
    switch(Id) {
    case ID_TIMER_4:
      if(IsMoving == 0) {
        BarPos -= 1;
        if (Exhaustion < MAX_STAMINA) {
          Exhaustion += 5;
        }
        if (Exhaustion <= 0) {
          BarPos -= 1;
        }
        if(BarPos <= MIN_BARPOS) {
          BarPos = MIN_BARPOS;
          _PausePedal = 0;
        }
      }
      WM_RestartTimer(hTimerDecr, 25);
      break;
    case ID_TIMER_TIME:
      hItem  = WM_GetDialogItem(pMsg->hWin, ID_ROTARY_0);
      if (!IsMoving) {
        OldValue -= BarPos / 2;
        KNOB_SetPos(hItem, OldValue * 2);
        Value = KNOB_GetValue(hItem);
        WM_RestartTimer(hTimer, 25);
      }
      WM_InvalidateWindow(pMsg->hWin);
      break;
    case ID_TIMER_1:
      if(Pos0 / POS_PER_M >= TRACK_DIST && GoalReached == 0) {
        GoalReached = 1;
        hTimerEnd   = WM_CreateTimer(pMsg->hWin, ID_TIMER_2, 0, 0);

        WM_DeleteTimer(hTimer);
        hTimer = 0;
        WM_DeleteTimer(hTimerDecr);
        hTimerDecr = 0;
        Stats.FinalTime = GUI_GetTime() - Stats.TimeStart;
        Stats.KmhAvg    = KmhAvg;

        hItem = WM_GetDialogItem(pMsg->hWin, ID_ROTARY_0);
        WM_DisableWindow(hItem);
      }

      if(GoalReached == 0) {
        Pos0 += BarPos / 20 + 1;
      } else {
        Pos0 += BarPos / 20;
      }
      Stats.Meters = (GoalReached) ? TRACK_DIST : Pos0 / POS_PER_M;

      WM_RestartTimer(hTimer1, 25);
      WM_InvalidateWindow(pMsg->hWin);
      break;
    case ID_TIMER_2:
      if(BarPos > 0) {
        BarPos -= 3;
        if (Exhaustion < MAX_STAMINA) {
          Exhaustion += 20;
          if (Exhaustion > MAX_STAMINA) {
            Exhaustion = MAX_STAMINA;
          }
        }
        if(BarPos < 0) {
          BarPos = 0;
        }
      } else {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_MENU);
        WM_DisableWindow(hItem);
        WM_CreateTimer(pMsg->hWin, ID_TIMER_5, 1500, 0);
        WM_DeleteTimer(hTimerEnd);
        hTimerEnd = 0;
      }
      WM_InvalidateWindow(pMsg->hWin);
      WM_RestartTimer(hTimerEnd, 25);
      break;
    case ID_TIMER_3:
      if (BarPos < MIN_BARPOS) {
        BarPos += 2;
        WM_RestartTimer(hTimerStart, 10);
      } else {
        BarPos = MIN_BARPOS;
        hItem = WM_GetDialogItem(pMsg->hWin, ID_ROTARY_0);
        WM_EnableWindow(hItem);
        hTimerDecr = WM_CreateTimer(pMsg->hWin, ID_TIMER_4, 25, 0);
        WM_DeleteTimer(hTimerStart);
        hTimerStart = 0;
      }
      break;
    case ID_TIMER_5:
      WM_SendMessageNoPara(_hStatWin, MSG_ANIMATE);
      break;
    case ID_TIMER_6:
      Knob     = 1;
      IsMoving = 0;
      break;
    }
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch (Id) {
    case ID_ROTARY_0:
      switch (NCode) {
      case WM_NOTIFICATION_CLICKED:
        IsMoving    = 1;
        _PausePedal = 0;
        _LastFrame  = -1;
        Knob        = 0;
        if(hTimerKnob == 0) {
          hTimerKnob = WM_CreateTimer(pMsg->hWin, ID_TIMER_6, 500, 0);
        }
        break;
      case WM_NOTIFICATION_RELEASED:
      case WM_NOTIFICATION_MOVED_OUT:
        IsMoving    = 0;
        Knob        = 0;
        _PausePedal = 1;
        _LastFrame  = (Pos0 / 9) % GUI_COUNTOF(APBM_RIDER);
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        Value = KNOB_GetValue(pMsg->hWinSrc)/ 2;
        if(hTimerKnob) {
          WM_DeleteTimer(hTimerKnob);
          hTimerKnob = 0;
        }
        if (Value > OldValue) {
          break;
        }
        if(Knob == 0 && Exhaustion > 0) {
          BarPos     += (abs(Value - OldValue)) / 250;
          Exhaustion -= (BarPos / 40);
        }
        if (Exhaustion <= 0) {
          BarPos -= 1;
        }
        if(BarPos >= MAX_BARPOS) {
          BarPos = MAX_BARPOS;
        }
        OldValue = Value;
        rPos     = (-Value) % 3600;
        WM_InvalidateWindow(pMsg->hWin);
        break;
      }
      break;
    case ID_BUTTON_MENU:
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        if(Stats.FinalTime) {
          AddHighscoreCycling(Stats.FinalTime);
        }
        GUI_ANIM_DeleteAll();
        RemoveCurrentWindow();
        _DisplayStats = 0;
        in_main_menu_flag = 1;
        break;
      }
      break;
    default:
      if(NCode == WM_NOTIFICATION_CHILD_DELETED) {
        if(_hCountWin == pMsg->hWinSrc) {
          Stats.TimeStart = GUI_GetTime();
          hTimer1         = WM_CreateTimer(pMsg->hWin, ID_TIMER_1, 0, 0);
          hTimerStart     = WM_CreateTimer(pMsg->hWin, ID_TIMER_3, 0, 0);
        }
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
/*********************************************************************
*
*       CreateWindowCycling
*/
WM_HWIN CreateWindowCycling(char * acName) {
  WM_HWIN hWin;

  _acName = acName;
  hWin    = WM_CreateWindowAsChild(0, 0, LCD_X, LCD_Y, WM_HBKWIN, WM_CF_SHOW, _cbBicycle, 0);
  return hWin;
}

/*************************** End of file ****************************/
