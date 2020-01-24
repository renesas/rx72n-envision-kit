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
  int * pBarData;
  int * pKmh;
  int * pKmhAvg;
} PROGWIN_DATA;

typedef struct {
  WM_HWIN hWin;
  int     xPos;
  int     yPos;
  int     xDist;
} ANIM_WIN_DATA;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define BANNER_STRING         "400 METER RACE"

#define TIMER_PER_FRAME       100
#define SPEED_DEFAULT         250
#define PIXEL_PER_IMAGE       18
#define MIN_PROGPOS           50
#define MAX_PROGPOS           193

#define MAX_STAMINA           100

#define ID_TIMER_0            0
#define ID_TIMER_1            1
#define ID_TIMER_2            2
#define ID_TIMER_3            3
#define ID_TIMER_4            4
#define ID_TIMER_5            5
#define ID_TIMER_6            6
#define ID_TIMER_7            7
#define ID_TIMER_8            8
#define ID_TIMER_9            9
#define ID_TIMER_10           10
#define ID_TIMER_COUNTDOWN    20

#define COLOR_ACCEL_BAR       GUI_MAKE_COLOR(0x000718C2)
#define COLOR_KMH             COLOR_ACCEL_BAR
#define BK_COLOR_GREEN        GUI_MAKE_COLOR(0x0041A968)
#define BK_COLOR_GRAY         GUI_MAKE_COLOR(0x00d9d9d9)
#define BK_COLOR_BROWN        GUI_MAKE_COLOR(0x0031418C)
#define LIGHT_GRAY            GUI_MAKE_COLOR(0x00C0C0C0)

#define APBM_RUNNER           apbmRunner1_Small
#define TOTAL_FRAMES          GUI_COUNTOF(APBM_RUNNER)

#define KM_H                  0.1581541301338129083
#define METER_MULTIPLIER      14.55272F
#define M_IN_KMH              13.908F

#define TRACK_DIST_IN_M       400
#define MS_BETWEEN_COUNT      1000

#define X_OFFSET_MARKS        6

#define RUN_BUTTON_SIZE       100

#define TITLE_YSIZE           50
#define TITLE_YPOS            20

#define COLOR_TITLE_1         GUI_MAKE_COLOR(0x00201810)
#define COLOR_TITLE_2         GUI_MAKE_COLOR(0x0015E7FE)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static int     _SysTime0;
static int     _SysTime1;
static int     _SysTimeD;
static int     _DisplayStats;
static char *  _acName;
static WM_HWIN _hProgWin;
static WM_HWIN _hRunButton1;
static WM_HWIN _hRunButton2;
static WM_HWIN _hStatWin;
static WM_HWIN _hExhaustWin;
static WM_HWIN _hCountWin;

static const GUI_BITMAP * _apbmMeterMarks[] = {
  &bm100m_95x91,
  &bm200m_95x91,
  &bm300m_95x91,
};

static WM_HWIN * _apWindows[] = {
  &_hStatWin,
  &_hProgWin,
  &_hExhaustWin,
  &_hRunButton1,
  &_hRunButton2,
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       _cbRunButton
*/
static void _cbRunButton(WM_MESSAGE * pMsg) {
  GUI_RECT Rect;

  switch(pMsg->MsgId) {
  case WM_PAINT:
    if(BUTTON_IsPressed(pMsg->hWin)) {
      GUI_DrawBitmap(&bmShoe2_100x100, 0, 0);
    } else {
      GUI_DrawBitmap(&bmShoe1_100x100, 0, 0);
    }
    GUI_SetColor(GUI_BLACK);
    GUI_SetPenSize(2);
    WM_GetClientRect(&Rect);
    SHRINK_RECT(&Rect, 1);
    GUI_AA_DrawRoundedRectEx(&Rect, 5);
    break;
  default:
    BUTTON_Callback(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbExhaustWin
*/
static void _cbExhaustWin(WM_MESSAGE * pMsg) {
         GUI_RECT Rect;
         int      i;
  static int    * pData;

  switch (pMsg->MsgId) {
  case WM_PAINT:
    WM_GetClientRect(&Rect);
    SHRINK_RECT(&Rect, 1);
    Rect.x0 += 14;
    GUI_SetColor(GUI_BLACK);
    GUI_AA_DrawRoundedRectEx(&Rect, 3);
    SHRINK_RECT(&Rect, 1);
    GUI_SetColor(COLOR_EXHAUST);
    GUI_FillRoundedRect(Rect.x0, Rect.y1 - (((float)*pData / MAX_STAMINA) * (WM_GetYSize(pMsg->hWin) - 5)), Rect.x1, Rect.y1, 1);
    GUI_SetColor(GUI_BLACK);
    GUI_DrawBitmap(&bmEnergy_18x19, Rect.x0 + 2, (WM_GetYSize(pMsg->hWin) / 2) - (bmEnergy_18x19.YSize / 2));
    for (i = 0; i < 3; i++) {
      GUI_DrawHLine(Rect.y0 + (i * 48), Rect.x0 - 5, Rect.x0 - 3);
    }
    //
    // Draw percentage
    //
    WM_GetClientRect(&Rect);
    Rect.x1 = 12;
    Rect.y1 = WM_GetYSize(pMsg->hWin) + 4;
    GUI_SetFont(&GUI_Font10S_1);
    GUI_SetColor(GUI_BLACK);
    GUI_SetTextMode(GUI_TM_TRANS);
    Rect.y0 = Rect.y1 - 20;
    GUI_DispStringInRectEx("0%", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER, 3, GUI_ROTATE_CCW);
    Rect.y0 -= 45;
    Rect.y1 -= 45;
    GUI_DispStringInRectEx("50%", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER, 4, GUI_ROTATE_CCW);
    Rect.y0 -= 41;
    Rect.y1 -= 41;
    GUI_DispStringInRectEx("100%", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER, 5, GUI_ROTATE_CCW);
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
*       _cbProgWin
*/
static void _cbProgWin(WM_MESSAGE * pMsg) {
  static PROGWIN_DATA * pData;
         GUI_RECT       Rect;
         char           acKmh[32];

  switch(pMsg->MsgId) {
  case WM_PAINT:
    WM_GetClientRect(&Rect);
    Rect.y1 = 20;
    GUI_SetColor(BK_COLOR_GREEN);
    GUI_AA_FillRoundedRectEx(&Rect, 2);
    GUI_SetColor(GUI_WHITE);
    GUI_AA_DrawRoundedRectEx(&Rect, 3);
    SHRINK_RECT(&Rect, 1);
    GUI_AA_DrawRoundedRectEx(&Rect, 3);
    if(pData) {
      GUI_SetColor(COLOR_ACCEL_BAR);
      Rect.x0 += 2;
      Rect.y0 += 2;
      Rect.x1  = Rect.x0 + *(pData->pBarData) - 1;  // + 193 px is a full bar
      Rect.y1 -= 2;
      GUI_FillRectEx(&Rect);
    }

    //
    // Display km/h and avg. km/h
    //
    GUI_SetFont(FONT_C64_12);
    GUI_SetColor(COLOR_KMH);
    GUI_SetTextMode(GUI_TM_TRANS);
    sprintf(acKmh, "%d km/h", *(pData->pKmh));
    GUI_DispStringAt(acKmh, 2, 25);
    GUI_SetColor(GUI_WHITE);
    sprintf(acKmh, "%d km/h (avg.)", *(pData->pKmhAvg));
    GUI_DispStringAt(acKmh, 2, 40);
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
        GUI_SetBkColor(BK_COLOR_BROWN);
        GUI_DispDecAt(Countdown, xSize / 2, ySize / 2, 1);
        Countdown--;
        WM_RestartTimer(hTimerCountdown, MS_BETWEEN_COUNT);
      } else if (GoDisp == 0) {
        GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
        GUI_SetTextMode(GUI_TM_TRANS);
        GUI_SetFont(FONT_C64_32);
        GUI_SetColor(GUI_RED);
        GUI_SetBkColor(BK_COLOR_BROWN);
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
*       _AnimDelete
*/
static void _AnimDelete(void * pVoid) {
  GUI_USE_PARA(pVoid);
  _DisplayStats = 1;
}

/*********************************************************************
*
*       _AnimWindowIn
*/
static void _AnimWindowIn(GUI_ANIM_INFO * pInfo, void * pVoid) {
  ANIM_WIN_DATA * pData;
  int             xPos;

  pData = (ANIM_WIN_DATA *)pVoid;
  xPos  = pData->xPos + (pData->xDist * pInfo->Pos) / GUI_ANIM_RANGE;
  WM_MoveTo(pData->hWin, xPos, pData->yPos);
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
  aData[0].xDist = 133;
  aData[1].xDist = 220;
  aData[2].xDist = -50;
  aData[3].xDist = -220;
  aData[4].xDist = -110;

  hAnim = GUI_ANIM_Create(300, 25, &aData[4], NULL);
  for (i = 0; i < GUI_COUNTOF(_apWindows); i++) {
    GUI_ANIM_AddItem(hAnim, 0, 300, ANIM_ACCELDECEL, &aData[i], _AnimWindowIn);
  }
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
    GUI_SetColor(COLOR_TITLE_1);
    GUI_SetBkColor(GUI_BLACK);
    GUI_SetFont(FONT_LAZER_48);
    xDist = GUI_GetStringDistX(BANNER_STRING);
    xSize = WM_GetWindowSizeX(pMsg->hWin);
    GUI_DispStringAt(BANNER_STRING, (xSize / 2) - (xDist / 2) + 1, 0);
    GUI_SetColor(COLOR_TITLE_2);
    GUI_DispStringAt(BANNER_STRING, (xSize / 2) - (xDist / 2) - 2, 0);
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
      memcpy(acKmh, _acName, 11);
      if(acKmh[0] == '\0') {
        GUI_DispStringAt("PLAYER 1", 7, 3);
      } else {
        GUI_DispStringAt(acKmh, 7, 3);
      }
      GUI_SetColor(GUI_BLUE);
      GUI_SetFont(FONT_C64_14);
      i = pData->Meters;
      i = (i > TRACK_DIST_IN_M) ? TRACK_DIST_IN_M : i;
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
      sprintf(acKmh, "%dm", TRACK_DIST_IN_M);
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
      GUI_DispStringAt("DISTANCE RUN", STATWIN_BORDER + 2, 70);
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
    hAnim             = GUI_ANIM_Create(ANIM_PERIOD_STATS, 25, NULL, NULL);
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
*       _cbWin
*/
static void _cbWin(WM_MESSAGE * pMsg) {
         int             NCode;
         int             Id;
         int             i;
         int             xSize;
         int             yPos;
         int             TimeNow;
         WM_HWIN         hItem;
         int             Result;
         STATS         * pStatData;
  static int             Index;
  static int             Pos0, Pos1;
  static int             ProgPos;
  static WM_HTIMER       hTimerEnd;
  static WM_HTIMER       hTimer2;
  static WM_HTIMER       hTimer3;
  static WM_HTIMER       hTimer4;
  static WM_HTIMER       hTimerStats;
  static int             ProgTimerIsRunning;
  static int             Started;
  static int             TimeRunStart;
  static int             GoalReached;
  static STATS           Stats;
  static int             Exhaustion;
  static int           * pExh;
  static PROGWIN_DATA    Data;
  static PROGWIN_DATA  * pData;
  static int             Kmh;
  static int             KmhAvg;
  static GUI_ANIM_HANDLE hAnim;
  static ANIM_TITLE_DATA AnimData;
  static int             ButtonL;
  static int             ButtonR;
  static int             ButtonCntL;
  static int             ButtonCntR;
  
  switch (pMsg->MsgId) {
  case WM_CREATE:
    APP_CreateMenuButton(pMsg->hWin);
    //
    // Reset variables
    //
    Pos0               = 0;
    GoalReached        = 0;
    Started            = 0;
    ProgPos            = 0;
    Exhaustion         = MAX_STAMINA;
    ProgTimerIsRunning = 0;
    ButtonL            = 0;
    ButtonR            = 0;
    GUI_ZEROFILL(&Stats, sizeof(Stats));
    //
    // Create progress bar
    //
    _hProgWin     = WM_CreateWindowAsChild(-210, LCD_Y - 55, 200, 60, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbProgWin, sizeof(PROGWIN_DATA *));
    Data.pBarData = &ProgPos;
    Data.pKmh     = &Kmh;
    Data.pKmhAvg  = &KmhAvg;
    pData         = &Data;
    WM_SetUserData(_hProgWin, &pData, sizeof(PROGWIN_DATA *));
    //
    // Create Run buttons
    //
    _hRunButton1 = BUTTON_CreateEx(LCD_X, LCD_Y - RUN_BUTTON_SIZE - 10, RUN_BUTTON_SIZE, RUN_BUTTON_SIZE, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_RUN_LEFT);
    WM_SetCallback(_hRunButton1, _cbRunButton);
    WM_DisableWindow(_hRunButton1);
    _hRunButton2 = BUTTON_CreateEx(LCD_X, LCD_Y - RUN_BUTTON_SIZE - 10, RUN_BUTTON_SIZE, RUN_BUTTON_SIZE, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_RUN_RIGHT);
    WM_SetCallback(_hRunButton2, _cbRunButton);
    WM_DisableWindow(_hRunButton2);    
    //
    // Create stat window
    //
    _hStatWin = WM_CreateWindowAsChild(-125, 4, 125, 60, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbStatWin, sizeof(STATS *));
    pStatData = &Stats;
    WM_SetUserData(_hStatWin, &pStatData, sizeof(STATS *));
    //
    // Create stamina window
    //
    _hExhaustWin = WM_CreateWindowAsChild(LCD_X, 50, 40, 100, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbExhaustWin, sizeof(int *));
    pExh         = &Exhaustion;
    WM_SetUserData(_hExhaustWin, &pExh, sizeof(int *));
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
    break;
  case WM_PAINT:
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    xSize = LCD_X;
    yPos  = LCD_Y - 181;
    //
    // Draw track
    //
    GUI_SetColor(BK_COLOR_GREEN);
    GUI_FillRect(0, 91, xSize, LCD_Y);
    GUI_SetColor(BK_COLOR_BROWN);
    GUI_FillRect(0, 114, xSize, 204);
    GUI_SetColor(GUI_WHITE);
    for(i = 0; i < 3; i++) {
      GUI_FillRect(0, 111 + (i * 31), xSize, 113 + (i * 31));
    }
    GUI_FillRect(0, 205, xSize, 207);

    GUI_SetFont(FONT_C64_18);
    i = GUI_GetStringDistX(BANNER_STRING);
    //
    GUI_SetColor(BK_COLOR_GRAY);
    GUI_FillRect(0, bmAudience_new_144x57.YSize, xSize - 1, yPos - 1);
    //
    // Draw banner (logo and string)
    //
    GUI_SetColor(RENESAS_BLUE);
    GUI_DrawBitmap(&BM_RENESAS, xSize - ((Pos0 / 3) % (xSize + i)), 58);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetColor(GUI_BLACK);
    GUI_DispStringAt(BANNER_STRING, xSize - (((Pos0 / 3) - (i / 2) + xSize) % ((xSize + i))), 65);
    GUI_SetColor(GUI_BLACK);
    GUI_DrawLine(0, 90, LCD_X, 90);
    //
    // Draw meter marks
    //
    GUI_SetColor(COLOR_METER_MARK);
    GUI_DrawBitmap(&bmstartend_95x91, X_OFFSET_MARKS * 6 - (Pos0 * 2 % (xSize * 16 * X_OFFSET_MARKS)), 114);
    for(i = 0; i < 3; i++) {
      GUI_DrawBitmap(_apbmMeterMarks[i], ((xSize + X_OFFSET_MARKS) * (i + 1)) * 6 - (Pos0 * 2 % (xSize * 16 * X_OFFSET_MARKS)), 114);
    }
    GUI_DrawBitmap(&bmstartend_95x91, ((xSize + X_OFFSET_MARKS) * (TRACK_DIST_IN_M / 100)) * 6 - (Pos0 * 2 % (xSize * 16 * X_OFFSET_MARKS)), 114);
    //
    // Draw audience
    //
    for (i = 0; i < 5; i++) {
      GUI_DrawBitmap(&bmAudience_new_144x57, bmAudience_new_144x57.XSize * i - ((Pos0 / 3) % bmAudience_new_144x57.XSize), 0);
    }
    //
    // Draw player
    //
    if(Started == 0 && GoalReached == 0) {
      GUI_DrawBitmap(&bmStand_14x77, 40, 112);
    } else if(Started == 1 && GoalReached == 1) {
      GUI_DrawBitmap(APBM_RUNNER[Index], (Pos1 * 2 % (xSize)) + 15, 112);
    } else if(Started == 0 && GoalReached == 1) {
      GUI_DrawBitmap(&bmStand_14x77, (Pos1 * 2 % (xSize)) + 40, 112);
    } else {
      GUI_DrawBitmap(APBM_RUNNER[Index], 15, 112);
    }
    //
    // Update km/h and avg. km/h
    //
    Kmh = (Pos0 == 0) ? 0 : ((((float)ProgPos / 20.0F + 1) / M_IN_KMH) * 15.0F) * 3.6F;
    if(Kmh > Stats.KmhBest) {
      Stats.KmhBest = Kmh;
    }
    if(GoalReached == 0) {
      KmhAvg = GUI_GetTime() - TimeRunStart;
      KmhAvg = (int)floor(MS_TO_AVG_KMH((Pos0 / METER_MULTIPLIER), KmhAvg));
      KmhAvg = (KmhAvg < 0 || KmhAvg == 0X7FFFFFFF) ? 0 : KmhAvg;
    } else {
      KmhAvg = Stats.KmhAvg;
    }
    break;
  case WM_TIMER:
    Id = WM_GetTimerId(pMsg->Data.v);
    switch (Id) {
    case ID_TIMER_2:
      TimeNow = GUI_GetTime();
      if((Pos0 / METER_MULTIPLIER) >= TRACK_DIST_IN_M && GoalReached == 0) {
        GoalReached     = 1;
        Stats.FinalTime = GUI_GetTime() - TimeRunStart;
        Stats.KmhAvg    = floor(MS_TO_AVG_KMH(TRACK_DIST_IN_M, Stats.FinalTime));
        hTimerEnd       = WM_CreateTimer(pMsg->hWin, ID_TIMER_5, 0, 0);
        WM_DeleteTimer(hTimer3);
        hTimer3     = 0;
        hTimerStats = WM_CreateTimer(pMsg->hWin, ID_TIMER_6, 3000, 0);
        GUI_USE_PARA(hTimerStats);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_RUN_LEFT);
        WM_DisableWindow(hItem);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_RUN_RIGHT);
        WM_DisableWindow(hItem);
      }
      if(GoalReached == 0) {
        Pos0 += ProgPos / 15;
        Pos1  = Pos0;
        Index = (Pos0 / PIXEL_PER_IMAGE) % TOTAL_FRAMES;
      } else {
        Pos1 += ProgPos / 20;
        Index = (Pos1 / PIXEL_PER_IMAGE) % TOTAL_FRAMES;
      }
      Stats.Meters = Pos0 / METER_MULTIPLIER;
      if(hTimer2) {
        WM_RestartTimer(pMsg->Data.v, 50);
      }
      WM_InvalidateWindow(pMsg->hWin);
      WM_InvalidateWindow(_hStatWin);
      break;
    case ID_TIMER_3:
      TimeNow = (_SysTime0 > _SysTime1) ? _SysTime0 : _SysTime1;
      TimeNow = GUI_GetTime() - TimeNow;

      if (TimeNow < 200) {
        if (Exhaustion > 0 && ((ButtonL ^ ButtonR) == 0)) {
          ProgPos    += (200 - TimeNow) / 50;
          Exhaustion -= RAND(0, 1) + (ProgPos / 170);
        }
        if (Exhaustion <= 0 || (ButtonL ^ ButtonR)) {
          ProgPos -= 1;
        }
        if (ProgPos < MIN_PROGPOS) {
          ProgPos = MIN_PROGPOS;
        }
      } else {
        ProgPos    -= ProgPos / 80 + 1;
        Exhaustion += RAND(1, 2);
        ButtonL     = 0;
        ButtonR     = 0;
        if (Exhaustion >= MAX_STAMINA) {
          Exhaustion = MAX_STAMINA;
        }
        if(ProgPos < MIN_PROGPOS) {
          ProgPos = MIN_PROGPOS;
        }
      }
      if(ProgPos > 0) {
        WM_RestartTimer(hTimer3, 25);
        if(ProgPos >= MAX_PROGPOS) {
          ProgPos = MAX_PROGPOS;
        }
      } else {
        WM_DeleteTimer(hTimer3);
        hTimer3 = ProgPos = ProgTimerIsRunning = 0;
      }
      WM_InvalidateWindow(_hProgWin);
      WM_InvalidateWindow(_hExhaustWin);
      break;
    case ID_TIMER_4:
      if(ProgPos <= MIN_PROGPOS) {
        ProgPos += 2;
        WM_InvalidateWindow(_hProgWin);
        WM_RestartTimer(hTimer4, 10);
      } else {
        ProgPos = MIN_PROGPOS;
        WM_InvalidateWindow(_hProgWin);
        WM_DeleteTimer(hTimer4);
        hTimer4 = 0;
      }
      break;
    case ID_TIMER_5:
      if(ProgPos > 0) {
        ProgPos -= 5;
      } else {
        Started = 0;
        WM_DeleteTimer(hTimer2);
        hTimer2 = 0;
      }
      WM_InvalidateWindow(pMsg->hWin);
      WM_InvalidateWindow(_hProgWin);
      WM_RestartTimer(hTimerEnd, 25);
      break;
    case ID_TIMER_6:
      WM_SendMessageNoPara(_hStatWin, MSG_ANIMATE);
      break;
    }
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    if (NCode == WM_NOTIFICATION_CHILD_DELETED) {
      if (_hCountWin == pMsg->hWinSrc) {
        Started         = 1;
        hTimer4         = WM_CreateTimer(pMsg->hWin, ID_TIMER_4, 10, 0);
        hTimer2         = WM_CreateTimer(pMsg->hWin, ID_TIMER_2, 25, 0);
        _hCountWin      = 0;
        TimeRunStart    = GUI_GetTime();
        Stats.TimeStart = TimeRunStart;
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_RUN_LEFT);
        WM_EnableWindow(hItem);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_RUN_RIGHT);
        WM_EnableWindow(hItem);
      }
    }
    switch (Id) {
    case ID_BUTTON_RUN_LEFT:
      switch (NCode) {
      case WM_NOTIFICATION_CLICKED:
        _SysTime0 = GUI_GetTime();
        _SysTimeD = abs(_SysTime1 - _SysTime0);
        ButtonL   = 1;

        if(_SysTimeD < 500 && ProgTimerIsRunning == 0) {
          hTimer3 = WM_CreateTimer(pMsg->hWin, ID_TIMER_3, 50, 0);

          ProgTimerIsRunning = 1;
        }
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_RUN_RIGHT:
      switch (NCode) {
      case WM_NOTIFICATION_CLICKED:
        _SysTime1 = GUI_GetTime();
        _SysTimeD = abs(_SysTime1 - _SysTime0);
        ButtonR   = 1;

        if(_SysTimeD < 500 && ProgTimerIsRunning == 0) {
          hTimer3 = WM_CreateTimer(pMsg->hWin, ID_TIMER_3, 50, 0);
          ProgTimerIsRunning = 1;
        }
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      WM_InvalidateWindow(pMsg->hWin);
      break;
    case ID_BUTTON_MENU:
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        Result = Stats.FinalTime;
        if(Result) {
          AddHighscoreRunner(Result);
        }
        GUI_ANIM_DeleteAll();
        RemoveCurrentWindow();
        _DisplayStats = 0;
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
/*********************************************************************
*
*       CreateWindowRunning
*/
WM_HWIN CreateWindowRunning(char * acName) {
  WM_HWIN hWin;

  _acName = acName;

  hWin = WM_CreateWindowAsChild(0, 0, LCD_X, LCD_Y, WM_HBKWIN, WM_CF_SHOW, _cbWin, 0);
  BUTTON_SetReactOnTouch();
  return hWin;
}

/*************************** End of file ****************************/
