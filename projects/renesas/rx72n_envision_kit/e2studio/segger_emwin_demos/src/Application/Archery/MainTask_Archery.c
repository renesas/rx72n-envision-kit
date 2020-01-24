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
  int     xSize, ySize;
  int     xPos, yPos;
  WM_HWIN hWin;
  int     DistY;
  int     DistX;
} ANIM_WIN_DATA;

typedef struct {
  WM_HWIN hWin;
  int     xDest;
  int     xPos;
  int     xCenter;
  int     xOld;
  int     xSize;
} ANIM_WIND_DATA;

typedef struct {
  WM_HWIN hWin;
  int     Index;
  int     yOff;
  int     xStart;
  int     yStart;
  int     xDest;
  int     yDest;
  int     xPos;
  int     yPos;
} ANIM_ARROW_DATA;

typedef struct {
  WM_HWIN hWin;
  int     xStart;
  int     yStart;
  int     xDest;
  int     yDest;
  int     xPos;
  int     yPos;
} ANIM_PLANE_DATA;

typedef struct {
  int xPos;
  int yPos;
} AIM_POS;

typedef struct {
  int xPos;   // X coordinate where the arrow hit
  int yPos;   // Y coordinate where the arrow hit
  int Points; // Points scored
} SHOT;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define TARGET_SIZE            181

#define FONT_MED               FONT_BOXY_24
#define FONT_LARGE             FONT_BOXY_48

#define TOUCH_SIZE_X           170
#define TOUCH_SIZE_Y           120

#define STAT_MIN_OFFSET        -120

#define WIND_INDC_PADDING      5

#define TITLE_YSIZE            50
#define TITLE_YPOS             20

#define MSG_SET_POINTS         (WM_USER + 0x00)
#define MSG_ANIM_END           (WM_USER + 0x01)
#define MSG_SET_WIND           (WM_USER + 0x02)
#define MSG_NEW_ROUND          (WM_USER + 0x03)
#define MSG_STOP_ANIM          (WM_USER + 0x04)

#define MAX_ARROWS             3
#define MAX_ROUNDS             3
#define FORCE_BAR              170

#define MAX_YOFF_SHOT          100
#define ANIM_PERIOD_SHOT       500
#define ANIM_PERIOD_INTRO      300
#define ANIM_PERIOD_PLANE      25000

#define ID_TIMER_FORCE         1
#define ID_TIMER_STATS         2
#define ID_TIMER_PLANE         3

#define ID_BUTTON_ARCH_NEXT    0x876

#define BUFFER_SIZE            8

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static char            _aR[5];
static SHOT            _aShots[MAX_ARROWS * MAX_ROUNDS];
static const GUI_COLOR _aColor[] = { COLOR_TARGET_YELLOW, COLOR_TARGET_RED, COLOR_TARGET_BLUE, COLOR_TARGET_BLACK, GUI_WHITE };
static WM_HWIN         _hPoints;
static WM_HWIN         _hWind;
static WM_HWIN         _hTarget;
static WM_HWIN         _hForceBar;
static WM_HWIN         _hArrowWin;
static WM_HWIN         _hTouchArea;
static WM_HWIN         _hStatWin;
static WM_HWIN         _hBk;
static WM_HWIN         _hMenuButton;
static int             _ArrowsShot;
static int             _TitleAnimDone;
static int             _xAimPos, _yAimPos;
static int             _AnimArrow;
static int             _DisplayStats;
static int             _RoundCount;         // 1-based round index

static GUI_CONST_STORAGE GUI_BITMAP * _apbmShotArrow[] = {
  &bmShot_Arrow_0,
  &bmShot_Arrow_1,
  &bmShot_Arrow_2,
  &bmShot_Arrow_3,
  &bmShot_Arrow_4,
  &bmShot_Arrow_5,
  &bmShot_Arrow_6,
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _AnimWindowIn
*/
static void _AnimWindowIn(GUI_ANIM_INFO * pInfo, void * pVoid) {
  ANIM_WIN_DATA * pData;
  int             xPos;

  pData = (ANIM_WIN_DATA *)pVoid;
  xPos  = pData->xPos + (pData->DistX * pInfo->Pos) / GUI_ANIM_RANGE;
  WM_MoveTo(pData->hWin, xPos, pData->yPos);
}

/*********************************************************************
*
*       _cbButtonNext
*/
static void _cbButtonNext(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    if (BUTTON_IsPressed(pMsg->hWin)) {
      GUI_SetColor(COLOR_GREY_SECONDARY);
    }
    else {
      GUI_SetColor(COLOR_GREY_PRIMARY);
    }
    GUI_FillCircle(WM_GetXSize(pMsg->hWin) / 2, WM_GetYSize(pMsg->hWin) / 2, WM_GetXSize(pMsg->hWin) / 2 - 1);
    GUI_SetColor(GUI_BLACK);
    GUI_DrawBitmap(&bmNextButton_30x30, 0, 0);
    break;
  default:
    BUTTON_Callback(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbPoints
*/
static void _cbPoints(WM_MESSAGE * pMsg) {
         GUI_RECT        Rect;
  static GUI_ANIM_HANDLE hAnim;
  static ANIM_WIN_DATA   Data;
  static int             Points;
         char            acBuffer[BUFFER_SIZE];

  switch (pMsg->MsgId) {
  case WM_CREATE:
    Data.hWin  = pMsg->hWin;
    Data.xPos  = WM_GetWindowOrgX(pMsg->hWin);
    Data.yPos  = WM_GetWindowOrgY(pMsg->hWin);
    Data.xSize = WM_GetWindowSizeX(pMsg->hWin);
    Data.ySize = WM_GetWindowSizeY(pMsg->hWin);
    Data.DistX = -Data.xSize - 20;
    hAnim      = GUI_ANIM_Create(ANIM_PERIOD_INTRO, 25, NULL, NULL);
    GUI_ANIM_AddItem(hAnim, 0, ANIM_PERIOD_INTRO, ANIM_ACCELDECEL, &Data, _AnimWindowIn);
    GUI_ANIM_StartEx(hAnim, 1, NULL);
    Points = 0;
    break;
  case WM_PAINT:
    GUI_GetClientRect(&Rect);
    GUI_SetFont(FONT_MED);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetColor(COLOR_TARGET_YELLOW);
    GUI_SetBkColor(COLOR_TARGET_RED);
    GUI_DispStringInRect("Score:  ", &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
    Rect.x1 -= 5;
    sprintf(acBuffer, "%i", Points);
    GUI_DispStringInRect(acBuffer, &Rect, GUI_TA_RIGHT | GUI_TA_VCENTER);
    break;
  case MSG_SET_POINTS:
    Points += pMsg->Data.v;
    WM_InvalidateWindow(pMsg->hWin);
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _AnimStatWin
*/
static void _AnimStatWin(GUI_ANIM_INFO * pInfo, void * pVoid) {
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
*       _AnimShrinkStatsDelete
*/
static void _AnimShrinkStatsDelete(void * pVoid) {
  GUI_USE_PARA(pVoid);
  WM_EnableWindow(_hTouchArea);
  WM_SendMessageNoPara(_hTouchArea, MSG_NEW_ROUND);
}

/*********************************************************************
*
*       _cbStatWin
*/
static void _cbStatWin(WM_MESSAGE * pMsg) {
         GUI_RECT           Rect;
         GUI_RECT           CRect;
         int                i, j;
         char               acBuf[BUFFER_SIZE];
         int                CurrentPoints;
         int                TotalPoints;
         int                XCnt;
         int                NCode;
  static WM_HWIN            hButton;
  static int                yOffset;
  static int                MinOffset;
  static GUI_ANIM_HANDLE    hStatAnim;
  static STAT_WIN_ANIM_DATA AnimData;

  switch (pMsg->MsgId) {
  case WM_CREATE:
    hButton = BUTTON_CreateEx(300, 12, 30, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_ARCH_NEXT);
    WM_SetCallback(hButton, _cbButtonNext);
    if (_RoundCount <= 1) {
      yOffset = 0;
    }
    //
    // Automatic scrolling
    //
    if (_RoundCount > 3) {
      yOffset = -((_RoundCount - 2) * 20);
      yOffset = (yOffset < MinOffset) ? MinOffset : yOffset;
    }
    break;
  case WM_PAINT:
    WM_GetClientRect(&Rect);
    SHRINK_RECT(&Rect, 1);

    GUI_DrawGradientRoundedV(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 3, COLOR_STATWIN_1, COLOR_STATWIN_2);

    GUI_SetColor(GUI_BLACK);
    GUI_SetPenSize(2);
    GUI_AA_DrawRoundedRectEx(&Rect, 3);

    if (_DisplayStats) {
      //
      // Draw header
      //
      GUI_SetColor(GUI_BLACK);
      GUI_SetFont(FONT_C64_20);
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_SetTextAlign(GUI_TA_HCENTER);
      GUI_DispStringAt("SCORECARD", WM_GetWindowSizeX(pMsg->hWin) / 2, 15);

      //
      // Draw top of table
      //
      Rect.x0 += 20;
      Rect.x1  = Rect.x0 + 65;
      Rect.y0 += 50;
      Rect.y1  = Rect.y0 + 50;
      GUI_SetPenSize(1);
      GUI_SetColor(GUI_GRAY_C8);
      GUI_AA_FillRoundedRectEx(&Rect, 2);
      GUI_SetColor(GUI_BLACK);
      GUI_AA_DrawRoundedRectEx(&Rect, 2);
      GUI_SetFont(FONT_C64_14);
      GUI_DispStringInRect("Round", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);

      Rect.x0  = Rect.x1;
      Rect.x1 += 130;
      GUI_SetColor(GUI_GRAY_C8);
      GUI_AA_FillRoundedRectEx(&Rect, 2);
      GUI_SetColor(GUI_BLACK);
      GUI_AA_DrawRoundedRectEx(&Rect, 2);
      Rect.y1 -= 25;
      GUI_AA_DrawRoundedRectEx(&Rect, 2);
      GUI_DispStringInRect("Arrow", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);

      CRect     = Rect;
      CRect.y0 += 25;
      CRect.y1 += 25;
      CRect.x1 -= 43 * 2;
      GUI_AA_DrawRoundedRectEx(&CRect, 2);
      GUI_DispStringInRect("1", &CRect, GUI_TA_HCENTER | GUI_TA_VCENTER);
      CRect.x0 = CRect.x1;
      CRect.x1 += 43;
      GUI_AA_DrawRoundedRectEx(&CRect, 2);
      GUI_DispStringInRect("2", &CRect, GUI_TA_HCENTER | GUI_TA_VCENTER);
      CRect.x0 = CRect.x1;
      CRect.x1 += 44;
      GUI_DispStringInRect("3", &CRect, GUI_TA_HCENTER | GUI_TA_VCENTER);

      Rect.y1 += 25;
      Rect.x0  = Rect.x1;
      Rect.x1 += 70;
      GUI_SetColor(GUI_GRAY_C8);
      GUI_AA_FillRoundedRectEx(&Rect, 2);
      GUI_SetColor(GUI_BLACK);
      GUI_AA_DrawRoundedRectEx(&Rect, 2);
      GUI_DispStringInRectWrap("End Score", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER, GUI_WRAPMODE_WORD);

      Rect.x0 = Rect.x1;
      Rect.x1 = WM_GetXSize(pMsg->hWin) - 20;
      GUI_SetColor(GUI_GRAY_C8);
      GUI_AA_FillRoundedRectEx(&Rect, 2);
      GUI_SetColor(GUI_BLACK);
      GUI_AA_DrawRoundedRectEx(&Rect, 2);
      GUI_DispStringInRect("X's", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);

      //
      // Set clip rect
      //
      WM_GetClientRect(&CRect);
      CRect.y0  = 101;
      CRect.y1 -= 11;
      WM_SetUserClipRect(&CRect);

      //
      // Print stats into table
      //
      GUI_SetFont(FONT_C64_12);
      for (i = 0; i < MAX_ROUNDS; i++) {
        TotalPoints = XCnt = 0;
        //
        // 'Round' column
        //
        Rect.x0 = 21;
        Rect.y0 = 101 + (i * 20) + yOffset;
        Rect.y1 = Rect.y0 + 20;
        Rect.x1 = Rect.x0 + 65;
        GUI_AA_DrawRoundedRectEx(&Rect, 2);
        sprintf(acBuf, "%d", i + 1);
        if (_RoundCount - 1 == i) {
          GUI_SetColor(COLOR_HIGHLIGHTED_RED);
        }
        GUI_DispStringInRect(acBuf, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
        GUI_SetColor(GUI_BLACK);
        //
        // 'Arrow' columns
        //
        for (j = 0; j < MAX_ARROWS; j++) {
          Rect.x0  = Rect.x1;
          Rect.x1 += (j == 0) ? 44 : 43;
          GUI_AA_DrawRoundedRectEx(&Rect, 2);
          CurrentPoints = _aShots[i * MAX_ARROWS + j].Points;
          if (CurrentPoints == 10) {
            XCnt++;
          }
          if (CurrentPoints > -1) {
            sprintf(acBuf, "%d", CurrentPoints);
          }
          else {
            GUI_ZEROFILL(acBuf, BUFFER_SIZE);
          }
          if (CurrentPoints > -1) {
            TotalPoints += CurrentPoints;
          }
          if (_RoundCount - 1 == i) {
            GUI_SetColor(COLOR_HIGHLIGHTED_RED);
          }
          GUI_DispStringInRect(acBuf, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
          GUI_SetColor(GUI_BLACK);
        }
        //
        // 'End score' column
        //
        Rect.x0  = Rect.x1;
        Rect.x1 += 70;
        GUI_AA_DrawRoundedRectEx(&Rect, 2);
        if (_aShots[i * MAX_ARROWS].Points > -1) {
          sprintf(acBuf, "%d", TotalPoints);
        }
        else {
          GUI_ZEROFILL(acBuf, BUFFER_SIZE);
        }
        if (_RoundCount - 1 == i) {
          GUI_SetColor(COLOR_HIGHLIGHTED_RED);
        }
        GUI_DispStringInRect(acBuf, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
        GUI_SetColor(GUI_BLACK);
        //
        // 'X' column
        //
        Rect.x0 = Rect.x1;
        Rect.x1 = WM_GetXSize(pMsg->hWin) - 20;
        GUI_AA_DrawRoundedRectEx(&Rect, 2);
        if (_aShots[i * MAX_ARROWS].Points > -1) {
          sprintf(acBuf, "%d", XCnt);
        }
        else {
          GUI_ZEROFILL(acBuf, BUFFER_SIZE);
        }
        if (_RoundCount - 1 == i) {
          GUI_SetColor(COLOR_HIGHLIGHTED_RED);
        }
        GUI_DispStringInRect(acBuf, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
        GUI_SetColor(GUI_BLACK);
      }
      //
      // Total score column
      //
      Rect.x0  = 130;
      Rect.y0 += 20;
      Rect.y1  = Rect.y0 + 20;
      Rect.x1  = Rect.x0 + 43 * 2;
      GUI_AA_DrawRoundedRectEx(&Rect, 2);
      sprintf(acBuf, "Totals:");
      GUI_DispStringInRect(acBuf, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
      //
      // Total end score
      //
      Rect.x0 = Rect.x1;
      Rect.x1 = Rect.x0 + 70;
      GUI_AA_DrawRoundedRectEx(&Rect, 2);
      if (_RoundCount == MAX_ROUNDS) {
        TotalPoints = XCnt = 0;
        for (i = 0; i < GUI_COUNTOF(_aShots); i++) {
          TotalPoints += _aShots[i].Points;
          if (_aShots[i].Points == 10) {
            XCnt++;
          }
        }
        sprintf(acBuf, "%d", TotalPoints);
      }
      else {
        GUI_ZEROFILL(acBuf, BUFFER_SIZE);
      }
      GUI_DispStringInRect(acBuf, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
      //
      // Total X's
      //
      Rect.x0 = Rect.x1;
      Rect.x1 = WM_GetXSize(pMsg->hWin) - 20;
      GUI_AA_DrawRoundedRectEx(&Rect, 2);
      if (_RoundCount == MAX_ROUNDS) {
        sprintf(acBuf, "%d", XCnt);
      } else {
        GUI_ZEROFILL(acBuf, BUFFER_SIZE);
      }
      GUI_DispStringInRect(acBuf, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);

      WM_SetUserClipRect(NULL);
    }
    MinOffset = STAT_MIN_OFFSET;
    break;
  case WM_NOTIFY_PARENT:
    NCode = pMsg->Data.v;
    if (NCode == WM_NOTIFICATION_RELEASED) {
      if (_RoundCount != MAX_ROUNDS) {
        AnimData.hWin     = _hStatWin;
        AnimData.xPos     = WM_GetWindowOrgX(_hStatWin);
        AnimData.yPos     = WM_GetWindowOrgY(_hStatWin);
        AnimData.xSizeNew = 1;
        AnimData.ySizeNew = 1;
        AnimData.xSizeOrg = WM_GetWindowSizeX(_hStatWin);
        AnimData.ySizeOrg = WM_GetWindowSizeY(_hStatWin);
        AnimData.xDist    = ((LCD_X - (AnimData.xSizeNew)) / 2) - AnimData.xPos;
        AnimData.yDist    = ((LCD_Y - (AnimData.ySizeNew)) / 2) - AnimData.yPos;
        hStatAnim = GUI_ANIM_Create(600, 25, NULL, NULL);
        GUI_ANIM_AddItem(hStatAnim, 0, 600, ANIM_ACCELDECEL, &AnimData, _AnimStatWin);
        GUI_ANIM_StartEx(hStatAnim, 1, _AnimShrinkStatsDelete);
        _DisplayStats = 0;
        _ArrowsShot   = 0;
      } else {
        XCnt = 0;
        for (i = 0; i < GUI_COUNTOF(_aShots); i++) {
          XCnt += _aShots[i].Points;
        }
        WM_SendMessageNoPara(_hWind, MSG_STOP_ANIM);
        WM_SendMessageNoPara(_hTarget, MSG_STOP_ANIM);
        AddHighscoreArchery(XCnt);
        RemoveCurrentWindow();
        _DisplayStats = 0;
      }
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _AnimShotArrow
*/
static void _AnimShotArrow(GUI_ANIM_INFO * pInfo, void * pVoid) {
  ANIM_ARROW_DATA * pData;
  int               v;
  int               xSign;
  int               ySign;
  int               xDelta;
  int               yDelta;

  pData = (ANIM_ARROW_DATA *)pVoid;
  pData->Index = pInfo->Pos / (GUI_ANIM_RANGE / GUI_COUNTOF(_apbmShotArrow)) - 1;
  pData->Index = (pData->Index < 0) ? 0 : pData->Index;
  xDelta  = pData->xDest - pData->xStart;
  xSign   = (xDelta < 0) ? -1 : 1;
  xDelta *= xSign;
  pData->xPos  = pData->xStart + (((xDelta * pInfo->Pos) / GUI_ANIM_RANGE) * xSign);
  yDelta  = pData->yDest - pData->yStart;
  ySign   = (yDelta < 0) ? -1 : 1;
  yDelta *= ySign;
  pData->yPos  = pData->yStart + (((yDelta * pInfo->Pos) / GUI_ANIM_RANGE) * ySign);

  v = pInfo->Pos * 2;
  if (v > GUI_ANIM_RANGE) {
    v = GUI_ANIM_RANGE - (v - GUI_ANIM_RANGE);
  }
  pData->yOff = (v * MAX_YOFF_SHOT) / GUI_ANIM_RANGE;
  WM_InvalidateWindow(pData->hWin);
}

/*********************************************************************
*
*       _AnimPlane
*/
static void _AnimPlane(GUI_ANIM_INFO * pInfo, void * pVoid) {
  ANIM_PLANE_DATA * pData;

  pData = (ANIM_PLANE_DATA *)pVoid;
  pData->xPos = pData->xStart + ((pData->xDest - pData->xStart) * pInfo->Pos) / GUI_ANIM_RANGE;
  pData->yPos = pData->yStart + ((pData->yDest - pData->yStart) * pInfo->Pos) / GUI_ANIM_RANGE;
  WM_InvalidateWindow(pData->hWin);
}

/*********************************************************************
*
*       _AnimArrowDelete
*/
static void _AnimArrowDelete(void * pVoid) {
  GUI_USE_PARA(pVoid);

  _AnimArrow = 0;
  if (_ArrowsShot < MAX_ARROWS) {
    WM_EnableWindow(_hTouchArea);
  }
}

/*********************************************************************
*
*       _AnimStatsDelete
*/
static void _AnimStatsDelete(void * pVoid) {
  GUI_USE_PARA(pVoid);
  _DisplayStats = 1;
}

/*********************************************************************
*
*       _AnimWindDelete
*/
static void _AnimWindDelete(void * pVoid) {
  ANIM_WIN_DATA * pData;

  pData = (ANIM_WIN_DATA *)pVoid;
  if (pData) {
    WM_SendMessageNoPara(pData->hWin, MSG_ANIM_END);
  }
}

/*********************************************************************
*
*       _AnimWind
*/
static void _AnimWind(GUI_ANIM_INFO * pInfo, void * pVoid) {
  ANIM_WIND_DATA * pData;
  int              xDelta;
  WM_MESSAGE       Message;
  int              Sign;

  srand(GUI_GetTime());

  pData       = (ANIM_WIND_DATA *)pVoid;
  xDelta      = pData->xDest - pData->xOld;
  Sign    = (xDelta < 0) ? -1 : 1;
  xDelta *= Sign;
  pData->xPos = pData->xOld + (((xDelta * pInfo->Pos) / GUI_ANIM_RANGE) * Sign);
  if (pInfo->State == GUI_ANIM_END) {
    pData->xOld = pData->xPos;
    pData->xDest = rand() % pData->xSize;
  }
  WM_InvalidateWindow(pData->hWin);
  Message.hWinSrc = pData->hWin;
  Message.Data.v  = pData->xCenter - pData->xPos;
  Message.MsgId   = MSG_SET_WIND;
  WM_BroadcastMessage(&Message);
}

/*********************************************************************
*
*       _cbWindIndicator
*/
static void _cbWindIndicator(WM_MESSAGE * pMsg) {
         GUI_RECT        Rect;
         int             x0;
         int             y0;
         int             x1;
         int             y1;
  static GUI_ANIM_HANDLE hAnimWin;
  static GUI_ANIM_HANDLE hAnimWind;
  static ANIM_WIN_DATA   Data;
  static ANIM_WIND_DATA  WindData;
  static int             Points;

  switch (pMsg->MsgId) {
  case WM_CREATE:
    Data.hWin  = pMsg->hWin;
    Data.xPos  = WM_GetWindowOrgX(pMsg->hWin);
    Data.yPos  = WM_GetWindowOrgY(pMsg->hWin);
    Data.xSize = WM_GetWindowSizeX(pMsg->hWin);
    Data.ySize = WM_GetWindowSizeY(pMsg->hWin);
    Data.DistX = Data.xSize;
    hAnimWin   = GUI_ANIM_Create(ANIM_PERIOD_INTRO, 25, &Data, NULL);
    GUI_ANIM_AddItem(hAnimWin, 0, ANIM_PERIOD_INTRO, ANIM_ACCELDECEL, &Data, _AnimWindowIn);
    GUI_ANIM_StartEx(hAnimWin, 1, _AnimWindDelete);

    Points = 0;
    break;
  case MSG_ANIM_END:
    Data.hWin  = pMsg->hWin;
    GUI_SetFont(FONT_MED);
    srand(GUI_GetTime());

    WindData.hWin    = pMsg->hWin;
    WindData.xSize   = WM_GetWindowSizeX(pMsg->hWin) - (GUI_GetStringDistX("Wind: ") + WIND_INDC_PADDING);
    WindData.xDest   = rand() % WindData.xSize;
    WindData.xCenter = WindData.xSize / 2;
    WindData.xOld    = WindData.xSize / 2;
    hAnimWind = GUI_ANIM_Create(10000, 25, NULL, NULL);
    GUI_ANIM_AddItem(hAnimWind, 0, 5000, ANIM_LINEAR, &WindData, _AnimWind);
    GUI_ANIM_AddItem(hAnimWind, 7000, 8000, ANIM_LINEAR, &WindData, _AnimWind);
    GUI_ANIM_StartEx(hAnimWind, -1, NULL);
    WM_EnableWindow(_hMenuButton);
    break;
  case WM_PAINT:
    GUI_GetClientRect(&Rect);
    GUI_SetFont(FONT_MED);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetColor(COLOR_TARGET_YELLOW);
    GUI_SetBkColor(COLOR_TARGET_RED);
    Rect.x0 += 5;
    Rect.y1 += 2;
    GUI_DispStringInRect("Wind:", &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
    Rect.y1 -= 2;
    GUI_SetColor(GUI_BLACK);
    Rect.x0 = GUI_GetStringDistX("Wind: ") + WIND_INDC_PADDING;
    GUI_AA_DrawRoundedRectEx(&Rect, 3);
    Rect.x0 = WindData.xCenter + (GUI_GetStringDistX("Wind: ") + WIND_INDC_PADDING);
    Rect.y0 = 0;
    Rect.x1 = (GUI_GetStringDistX("Wind: ") + WIND_INDC_PADDING) + WindData.xPos;
    Rect.y1 = WM_GetWindowSizeY(pMsg->hWin) - 1;
    if (Rect.x1 < Rect.x0) {
      x0 = Rect.x1;
      Rect.x1 = Rect.x0;
      Rect.x0 = x0;
    }
    WM_SetUserClipRect(&Rect);
    x0 = GUI_GetStringDistX("Wind: ") + WIND_INDC_PADDING + 1;
    y0 = Rect.y0 + 1;
    x1 = WindData.xCenter + (GUI_GetStringDistX("Wind: ") + WIND_INDC_PADDING) - 1;
    y1 = Rect.y1 - 1;
    GUI_DrawGradientH(x0, y0, x1, y1, COLOR_TARGET_RED, COLOR_TARGET_YELLOW);
    x0 = WindData.xCenter + (GUI_GetStringDistX("Wind: ") + WIND_INDC_PADDING);
    x1 = WM_GetWindowSizeX(pMsg->hWin) - 1;
    GUI_DrawGradientH(x0, y0, x1, y1, COLOR_TARGET_YELLOW, COLOR_TARGET_RED);
    WM_SetUserClipRect(NULL);
    break;
  case MSG_STOP_ANIM:
    GUI_ANIM_Stop(hAnimWind);
    GUI_ANIM_Delete(hAnimWind);
    hAnimWind = 0;
    break;
  case MSG_SET_POINTS:
    Points += pMsg->Data.v;
    WM_InvalidateWindow(pMsg->hWin);
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbRoundIndicator
*/
static void _cbRoundIndicator(WM_MESSAGE * pMsg) {
  static GUI_ANIM_HANDLE hAnim;
  static ANIM_WIN_DATA   Data;
         GUI_RECT        Rect;
         char            acBuf[BUFFER_SIZE * 2];

  switch (pMsg->MsgId) {
  case WM_CREATE:
    Data.hWin  = pMsg->hWin;
    Data.xPos  = WM_GetWindowOrgX(pMsg->hWin);
    Data.yPos  = WM_GetWindowOrgY(pMsg->hWin);
    Data.xSize = WM_GetWindowSizeX(pMsg->hWin);
    Data.ySize = WM_GetWindowSizeY(pMsg->hWin);
    Data.DistX = Data.xSize;
    hAnim = GUI_ANIM_Create(ANIM_PERIOD_INTRO, 25, NULL, NULL);
    GUI_ANIM_AddItem(hAnim, 0, ANIM_PERIOD_INTRO, ANIM_ACCELDECEL, &Data, _AnimWindowIn);
    GUI_ANIM_StartEx(hAnim, 1, NULL);
    break;
  case WM_PAINT:
    GUI_GetClientRect(&Rect);
    GUI_SetFont(FONT_MED);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetColor(COLOR_TARGET_YELLOW);
    GUI_SetBkColor(COLOR_TARGET_RED);

    sprintf(acBuf, "Round: %d / %d", _RoundCount, MAX_ROUNDS);
    GUI_DispStringInRect(acBuf, &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbArrowIndicator
*/
static void _cbArrowIndicator(WM_MESSAGE * pMsg) {
         int             i;
  static GUI_ANIM_HANDLE hAnim;
  static ANIM_WIN_DATA   Data;

  switch (pMsg->MsgId){
  case WM_CREATE:
    //
    // Prepare intro animation
    //
    Data.hWin  = pMsg->hWin;
    Data.xPos  = WM_GetWindowOrgX(pMsg->hWin);
    Data.yPos  = WM_GetWindowOrgY(pMsg->hWin);
    Data.xSize = WM_GetWindowSizeX(pMsg->hWin);
    Data.ySize = WM_GetWindowSizeY(pMsg->hWin);
    Data.DistX = 115;
    hAnim      = GUI_ANIM_Create(2000 + ANIM_PERIOD_INTRO, 25, NULL, NULL);
    GUI_ANIM_AddItem(hAnim, 2000, 2000 + ANIM_PERIOD_INTRO, ANIM_ACCELDECEL, &Data, _AnimWindowIn);
    GUI_ANIM_StartEx(hAnim, 1, NULL);
    break;
  case WM_PAINT:
    GUI_DrawBitmap(&bmQuiver_37x110, 5, 40);

    for (i = MAX_ARROWS - _ArrowsShot; i > 0; i--) {
      GUI_DrawBitmap(&bmArrow_16x37, -5 + (i * 10), 3);
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _SliceInfo
*/
static void _SliceInfo(int State, void * pVoid) {
  GUI_USE_PARA(State);
  GUI_USE_PARA(pVoid);
  if (WM_IsWindow(_hArrowWin)) {
    WM_InvalidateWindow(_hArrowWin);
  }
}

/*********************************************************************
*
*       _AnimY
*/
static void _AnimY(GUI_ANIM_INFO * pInfo, void * pVoid) {
  ANIM_WIN_DATA * pData;
  I32             SinHQ;
  I32             Ang;
  int             yh;
  int             ya;

  pData = (ANIM_WIN_DATA *)pVoid;
  Ang = (pInfo->Pos * 360) / GUI_ANIM_RANGE;
  SinHQ = GUI__SinHQ(Ang * 1000);
  yh = pData->ySize / 2;
  ya = (yh * 3) / 4;
  pData->yPos = yh + ((SinHQ * ya) >> 16);
}

/*********************************************************************
*
*       _AnimXAB
*/
static void _AnimXAB(GUI_ANIM_INFO * pInfo, void * pVoid, int v) {
  ANIM_WIN_DATA * pData;
  int             xh, xa;

  pData = (ANIM_WIN_DATA *)pVoid;
  xh = pData->xSize / 2;
  xa = (xh * 3) / 4;
  pData->xPos = xh + v * xa - v * ((pInfo->Pos * xa * 2) / GUI_ANIM_RANGE);
}

/*********************************************************************
*
*       _AnimX01
*/
static void _AnimX01(GUI_ANIM_INFO * pInfo, void * pVoid, int v) {
  ANIM_WIN_DATA * pData;
  I32             CosHQ;
  I32             Ang;
  int             xh, xa;

  pData = (ANIM_WIN_DATA *)pVoid;
  Ang = (pInfo->Pos * 360) / GUI_ANIM_RANGE;
  CosHQ = GUI__CosHQ(Ang * 1000);
  xh = pData->xSize / 2;
  xa = (xh * 3) / 4;
  pData->xPos = xh + v * ((CosHQ * xa) >> 16);
}

/*********************************************************************
*
*       _AnimXA
*/
static void _AnimXA(GUI_ANIM_INFO * pInfo, void * pVoid) {
  _AnimXAB(pInfo, pVoid, +1);
}

/*********************************************************************
*
*       _AnimXB
*/
static void _AnimXB(GUI_ANIM_INFO * pInfo, void * pVoid) {
  _AnimXAB(pInfo, pVoid, -1);
}

/*********************************************************************
*
*       _AnimX0
*/
static void _AnimX0(GUI_ANIM_INFO * pInfo, void * pVoid) {
  _AnimX01(pInfo, pVoid, +1);
}

/*********************************************************************
*
*       _AnimX1
*/
static void _AnimX1(GUI_ANIM_INFO * pInfo, void * pVoid) {
  _AnimX01(pInfo, pVoid, -1);
}

/*********************************************************************
*
*       _StartAnim
*/
static void _StartAnim(ANIM_WIN_DATA * pData, GUI_ANIM_HANDLE * pAnim0, GUI_ANIM_HANDLE * pAnim1) {
  U32 Duration;

  Duration = 1500;
  *pAnim0  = GUI_ANIM_Create(Duration, 20, pData, _SliceInfo);
  GUI_ANIM_AddItem(*pAnim0, 0, Duration, ANIM_LINEAR, pData, _AnimY);
  GUI_ANIM_StartEx(*pAnim0, -1, NULL);
  Duration = 2000;
  *pAnim1  = GUI_ANIM_Create(Duration * 6, 20, pData, _SliceInfo);
  GUI_ANIM_AddItem(*pAnim1, Duration * 0, Duration * 1, ANIM_LINEAR, pData, _AnimX0);
  GUI_ANIM_AddItem(*pAnim1, Duration * 1, Duration * 2, ANIM_LINEAR, pData, _AnimX0);
  GUI_ANIM_AddItem(*pAnim1, Duration * 2, Duration * 3, ANIM_LINEAR, pData, _AnimXA);
  GUI_ANIM_AddItem(*pAnim1, Duration * 3, Duration * 4, ANIM_LINEAR, pData, _AnimX1);
  GUI_ANIM_AddItem(*pAnim1, Duration * 4, Duration * 5, ANIM_LINEAR, pData, _AnimX1);
  GUI_ANIM_AddItem(*pAnim1, Duration * 5, Duration * 6, ANIM_LINEAR, pData, _AnimXB);
  GUI_ANIM_StartEx(*pAnim1, -1, NULL);
}

/*********************************************************************
*
*       _AnimTitleIn
*/
static void _AnimTitleIn(GUI_ANIM_INFO * pInfo, void * pVoid) {
  ANIM_WIN_DATA * pData;
  int             yPos;

  pData = (ANIM_WIN_DATA *)pVoid;
  yPos  = pData->yPos + (pData->DistY * pInfo->Pos) / GUI_ANIM_RANGE;
  WM_MoveTo(pData->hWin, pData->xPos, yPos);
  if (pInfo->State == GUI_ANIM_END) {
    pData->yPos = TITLE_YPOS;
  }
}

/*********************************************************************
*
*       _AnimTitleOut
*/
static void _AnimTitleOut(GUI_ANIM_INFO * pInfo, void * pVoid) {
  ANIM_WIN_DATA * pData;
  int             xPos;

  pData = (ANIM_WIN_DATA *)pVoid;
  xPos  = pData->xPos - (pData->xSize * pInfo->Pos) / GUI_ANIM_RANGE;
  WM_MoveTo(pData->hWin, xPos, pData->yPos);
  if (pInfo->State == GUI_ANIM_START) {
    _TitleAnimDone = 1;
    _hPoints = WM_CreateWindowAsChild(LCD_X, TITLE_YPOS + 35, 180, 24, _hBk, WM_CF_SHOW | WM_CF_HASTRANS, _cbPoints, 0);
    _hWind   = WM_CreateWindowAsChild(-240,           TITLE_YPOS,      240, 24, _hBk, WM_CF_SHOW | WM_CF_HASTRANS, _cbWindIndicator, 0);
    WM_CreateWindowAsChild(-215,           TITLE_YPOS + 35, 220, 24, _hBk, WM_CF_SHOW | WM_CF_HASTRANS, _cbRoundIndicator, 0);
    WM_EnableWindow(_hTouchArea);
  }
}

/*********************************************************************
*
*       _cbForceBar
*/
static void _cbForceBar(WM_MESSAGE * pMsg) {
         GUI_RECT        Rect;
         GUI_RECT        ClipRect;
  static int           * pForce;
  static ANIM_WIN_DATA   Data;
  static GUI_ANIM_HANDLE hAnim;

  switch (pMsg->MsgId) {
  case WM_CREATE:
    //
    // Prepare intro animation
    //
    Data.hWin  = pMsg->hWin;
    Data.xPos  = WM_GetWindowOrgX(pMsg->hWin);
    Data.yPos  = WM_GetWindowOrgY(pMsg->hWin);
    Data.xSize = WM_GetWindowSizeX(pMsg->hWin);
    Data.ySize = WM_GetWindowSizeY(pMsg->hWin);
    Data.DistX = -200;
    hAnim      = GUI_ANIM_Create(2000 + ANIM_PERIOD_INTRO, 25, NULL, NULL);
    GUI_ANIM_AddItem(hAnim, 2000, 2000 + ANIM_PERIOD_INTRO, ANIM_ACCELDECEL, &Data, _AnimWindowIn);
    GUI_ANIM_StartEx(hAnim, 1, NULL);
    break;
  case WM_PAINT:
    WM_GetClientRect(&Rect);
    GUI_SetColor(GUI_BLACK);
    GUI_AA_DrawRoundedRectEx(&Rect, 3);

    SHRINK_RECT(&Rect, 1);

    ClipRect     = Rect;
    ClipRect.x1 -= *pForce;
    WM_SetUserClipRect(&ClipRect);
    
    GUI_DrawGradientRoundedH(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 1, GUI_YELLOW, GUI_RED);

    WM_SetUserClipRect(NULL);

    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetFont(FONT_C64_14);
    GUI_SetColor(GUI_WHITE);
    GUI_DispStringAt("FORCE", (WM_GetXSize(pMsg->hWin)) / 2 - (GUI_GetStringDistX("FORCE") / 2), (WM_GetYSize(pMsg->hWin) - GUI_GetFontSizeY()) / 2 - 1);
    break;
  case WM_USER_DATA:
    WM_GetUserData(pMsg->hWin, &pForce, sizeof(int *));
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbTitle
*/
static void _cbTitle(WM_MESSAGE * pMsg) {
         GUI_RECT        Rect;
  static GUI_ANIM_HANDLE hAnim;
  static ANIM_WIN_DATA   Data;

  switch (pMsg->MsgId) {
  case WM_CREATE:
    Data.hWin  = pMsg->hWin;
    Data.xPos  = WM_GetWindowOrgX(pMsg->hWin);
    Data.yPos  = WM_GetWindowOrgY(pMsg->hWin);
    Data.xSize = WM_GetWindowSizeX(pMsg->hWin);
    Data.ySize = WM_GetWindowSizeY(pMsg->hWin);
    Data.DistY = WM_GetWindowSizeY(pMsg->hWin) + TITLE_YPOS;
    hAnim      = GUI_ANIM_Create(2300, 25, NULL, NULL);
    GUI_ANIM_AddItem(hAnim, 0, 1000, ANIM_ACCEL,  &Data, _AnimTitleIn);
    GUI_ANIM_AddItem(hAnim, 2000, 2300, ANIM_ACCEL, &Data, _AnimTitleOut);
    GUI_ANIM_StartEx(hAnim, 1, NULL);
    break;
  case WM_PAINT:
    GUI_GetClientRect(&Rect);
    GUI_SetFont(FONT_LARGE);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetColor(COLOR_TARGET_YELLOW);
    GUI_SetBkColor(COLOR_TARGET_RED);
    GUI_DispStringInRect("A R C H E R Y", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _GetPoints
*/
static int _GetPoints(int x, int y) {
  int c, r, d, p;

  r = (TARGET_SIZE / 2); 
  d = r / 10;
  c = GUI__sqrt32(x * x + y * y);
  if (c > 90) {
    return 0;
  }
  c = (TARGET_SIZE / 2) - c;
  p = c / d + 1;
  p = (p > 10) ? 10 : (p < 0) ? 0 : p;
  return p;
}

/*********************************************************************
*
*       _cbArrowWin
*/
static void _cbArrowWin(WM_MESSAGE * pMsg) {
         int               i;
         int               xArrPos, yArrPos;
         SHOT            * pCurrentShot;
  static ANIM_ARROW_DATA * pData;

  switch (pMsg->MsgId) {
  case WM_PAINT:
    for(i = 0; i < _ArrowsShot; i++) {
      pCurrentShot = _aShots + ((_RoundCount - 1) * MAX_ARROWS) + i;
      xArrPos      = pCurrentShot->xPos + (TARGET_SIZE / 2) + 69;
      yArrPos      = pCurrentShot->yPos + (TARGET_SIZE / 2) + (WM_GetYSize(pMsg->hWin) - TARGET_SIZE);
      if(pCurrentShot->Points && i <= _ArrowsShot - 2) {
        GUI_DrawBitmap(_apbmShotArrow[6], xArrPos - ((*_apbmShotArrow[6]).XSize / 2), yArrPos - (*_apbmShotArrow[6]).YSize);
      } else if(pCurrentShot->Points && i == _ArrowsShot - 1 && _AnimArrow == 0) {
        GUI_DrawBitmap(_apbmShotArrow[6], xArrPos - ((*_apbmShotArrow[6]).XSize / 2), yArrPos - (*_apbmShotArrow[6]).YSize);
      }
    }
    
    _xAimPos += 69;
    _yAimPos += (WM_GetYSize(pMsg->hWin) - TARGET_SIZE);

    if(_AnimArrow) {
      if(pData->Index <= 4) {
        GUI_DrawBitmap(_apbmShotArrow[pData->Index], pData->xPos + (*_apbmShotArrow[pData->Index]).XSize / 2 + 10, pData->yPos - pData->yOff);
      } else {
        GUI_DrawBitmap(_apbmShotArrow[pData->Index], pData->xPos + (*_apbmShotArrow[pData->Index]).XSize / 2 + 10, pData->yPos - pData->yOff - (*_apbmShotArrow[pData->Index]).YSize);
      }
    }

    //
    // Draw aim point
    //
    if(_TitleAnimDone) {
      GUI_SetPenSize(1);
      GUI_SetColor(GUI_BLACK);
      GUI_AA_DrawCircle(_xAimPos, _yAimPos, 5);
      GUI_SetColor(GUI_WHITE);
      GUI_AA_DrawCircle(_xAimPos, _yAimPos, 6);
      GUI_SetColor(COLOR_AIM_RED);
      GUI_AA_FillCircle(_xAimPos, _yAimPos, 2);
    }
    break;
  case WM_USER_DATA:
    WM_GetUserData(pMsg->hWin, &pData, sizeof(ANIM_ARROW_DATA *));
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbTarget
*/
static void _cbTarget(WM_MESSAGE * pMsg) {
  static GUI_ANIM_HANDLE   hAnim0;
  static GUI_ANIM_HANDLE   hAnim1;
  static ANIM_WIN_DATA     Data;
  static int               Wind;
  static AIM_POS         * pPos;
  static int               xPos, yPos;
  static SHOT            * pShots;
         int               i;
         int               Points;
         int               NumItems;
         WM_MESSAGE        Message;

  switch (pMsg->MsgId) {
  case WM_CREATE:
    //
    // Set up animation
    //
    Data.xSize = TARGET_SIZE / 5;
    Data.ySize = TARGET_SIZE / 5;
    Data.hWin = pMsg->hWin;
    _StartAnim(&Data, &hAnim0, &hAnim1);
    NumItems = (int)GUI_COUNTOF(_aR) - 1;
    for (i = 0; i < (int)GUI_COUNTOF(_aR); i++) {
      _aR[NumItems - i] = ((TARGET_SIZE / 2) / GUI_COUNTOF(_aR)) * (GUI_COUNTOF(_aR) - i) - 1;
    }
    pShots = _aShots;

    Wind = 0;
    break;
  case WM_PAINT:
    xPos = TARGET_SIZE / 2;
    yPos = TARGET_SIZE / 2;
    NumItems = (int)GUI_COUNTOF(_aR) - 1;
    for (i = 0; i < (int)GUI_COUNTOF(_aR); i++) {
      GUI_SetColor(_aColor[NumItems - i]);
      GUI_AA_FillCircle(xPos, yPos, _aR[NumItems - i]);
      if(i == 1) {
        GUI_SetColor(GUI_WHITE);
      } else {
        GUI_SetColor(GUI_BLACK);
      }
      GUI_AA_DrawCircle(xPos, yPos, _aR[NumItems - i]);
      GUI_AA_DrawCircle(xPos, yPos, _aR[NumItems - i] - (((TARGET_SIZE / GUI_COUNTOF(_aR)) / 2) / 2));
    }
    GUI_SetPenSize(3);
    GUI_AA_DrawCircle(xPos, yPos, 0);
    xPos = _xAimPos = Data.xPos + pPos->xPos - Wind;
    yPos = _yAimPos = Data.yPos + pPos->yPos;

    xPos -= (TARGET_SIZE / 2);
    yPos -= (TARGET_SIZE / 2);
    break;
  case WM_USER_DATA:
    WM_GetUserData(pMsg->hWin, &pPos, sizeof(AIM_POS *));
    break;
  case MSG_SET_POINTS:
    Points = _GetPoints(xPos, yPos);
    pShots->Points = Points;
    pShots->xPos = xPos;
    pShots->yPos = yPos;
    pShots++;

    Message.Data.v = Points;
    Message.hWin = _hPoints;
    Message.hWinSrc = pMsg->hWin;
    Message.MsgId = MSG_SET_POINTS;
    WM_SendMessage(_hPoints, &Message);
    break;
  case MSG_SET_WIND:
    Wind = pMsg->Data.v;
    break;
  case MSG_STOP_ANIM:
    GUI_ANIM_Stop(hAnim0);
    GUI_ANIM_Delete(hAnim0);
    hAnim0 = 0;
    GUI_ANIM_Stop(hAnim1);
    GUI_ANIM_Delete(hAnim1);
    hAnim1 = 0;
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _ExecArrowShoot
*/
static void _ExecArrowShoot(ANIM_ARROW_DATA * pData, GUI_ANIM_HANDLE * pAnim, WM_HTIMER * pTimerForce, WM_HTIMER * pTimerStats, int * pForce, int * pArrowShot, WM_HWIN * pWin) {
  int Period;

  WM_ReleaseCapture();
  WM_DisableWindow(_hTouchArea);
  pData->hWin   = _hArrowWin;
  pData->xStart = pData->xPos = 160;
  pData->yStart = pData->yPos = WM_GetWindowSizeY(_hArrowWin);
  pData->xDest  = (_xAimPos - 35);
  pData->yDest  = _yAimPos;

  Period = ANIM_PERIOD_SHOT;
  *pAnim = GUI_ANIM_Create(Period, 25, NULL, NULL);
  GUI_ANIM_AddItem(*pAnim, 0, Period, ANIM_DECEL, pData, _AnimShotArrow);
  GUI_ANIM_StartEx(*pAnim, 1, _AnimArrowDelete);
  _AnimArrow = 1;

  WM_DeleteTimer(*pTimerForce);
  *pTimerForce = 0;
  *pForce      = FORCE_BAR;
  WM_SendMessageNoPara(_hTarget, MSG_SET_POINTS);
  _ArrowsShot++;
  *pArrowShot = 1;

  // Game is done
  if(_ArrowsShot == MAX_ARROWS) {
    WM_DisableWindow(_hTouchArea);
    *pTimerStats = WM_CreateTimer(*pWin, ID_TIMER_STATS, 800, 0);
  }
}

/*********************************************************************
*
*       _cbTouchArea
*/
static void _cbTouchArea(WM_MESSAGE * pMsg) {
  static GUI_ANIM_HANDLE      hAnim;
  static ANIM_ARROW_DATA      Data;
  static int                  Pressed;
  static AIM_POS              AimPos;
  static int                  Force;
  static WM_HTIMER            hTimerForce;
  static WM_HTIMER            hTimerStats;
  static int                  ArrowShot;
  static GUI_ANIM_HANDLE      hStatAnim;
  static ANIM_WIN_DATA        WinData;
  static STAT_WIN_ANIM_DATA   AnimData;
         GUI_RECT             Rect;
         GUI_PID_STATE      * pState;
         AIM_POS            * pPos;
         ANIM_ARROW_DATA    * pData;
         int                * pForce;
         int                  Id;

  switch (pMsg->MsgId) {
  case WM_CREATE:
    _hTarget = WM_CreateWindowAsChild((LCD_X - TARGET_SIZE) / 2 - 80, LCD_Y - TARGET_SIZE, TARGET_SIZE, TARGET_SIZE, _hBk, WM_CF_SHOW | WM_CF_HASTRANS, _cbTarget, sizeof(AIM_POS *));
    pPos     = &AimPos;
    WM_SetUserData(_hTarget, &pPos, sizeof(AIM_POS *));

    _hForceBar = WM_CreateWindowAsChild(LCD_X, 90, TOUCH_SIZE_X, 20, _hBk, WM_CF_SHOW | WM_CF_HASTRANS, _cbForceBar, sizeof(int *));
    pForce     = &Force;
    WM_SetUserData(_hForceBar, &pForce, sizeof(int *));

    _hArrowWin = WM_CreateWindowAsChild(0, TITLE_YPOS, LCD_X - 180, LCD_Y - TITLE_YPOS, _hBk, WM_CF_SHOW | WM_CF_HASTRANS, _cbArrowWin, sizeof(ANIM_ARROW_DATA *));
    pData      = &Data;
    WM_SetUserData(_hArrowWin, &pData, sizeof(ANIM_ARROW_DATA *));

    memset(_aShots, -1, sizeof(_aShots));
    //
    // Prepare intro animation
    //
    WinData.hWin  = pMsg->hWin;
    WinData.xPos  = WM_GetWindowOrgX(pMsg->hWin);
    WinData.yPos  = WM_GetWindowOrgY(pMsg->hWin);
    WinData.xSize = WM_GetWindowSizeX(pMsg->hWin);
    WinData.ySize = WM_GetWindowSizeY(pMsg->hWin);
    WinData.DistX = -200;
    hAnim         = GUI_ANIM_Create(2000 + ANIM_PERIOD_INTRO, 25, NULL, NULL);
    GUI_ANIM_AddItem(hAnim, 2000, 2000 + ANIM_PERIOD_INTRO, ANIM_ACCELDECEL, &WinData, _AnimWindowIn);
    GUI_ANIM_StartEx(hAnim, 1, NULL);
    //
    // Reset variables
    //
    _RoundCount = 1;
    AimPos.xPos = AimPos.yPos = TARGET_SIZE / 2;
    Force       = FORCE_BAR;
    ArrowShot   = 0;
    break;
  case WM_PAINT:
    GUI_DrawBitmap(&bmTouchArea_170x120, 0, 0);
    GUI_GetClientRect(&Rect);
    GUI_SetColor(GUI_BLACK);
    GUI_AA_DrawRoundedRectEx(&Rect, 5);
    break;
  case WM_TOUCH:
    pState = (GUI_PID_STATE *)pMsg->Data.p;
    WM_InvalidateWindow(_hTarget);
    WM_SetCapture(pMsg->hWin, 1);
    if (pState) {
      if (pState->Pressed) {
        if (Pressed == 0) {
          if (hTimerForce == 0) {
            hTimerForce = WM_CreateTimer(pMsg->hWin, ID_TIMER_FORCE, 0, 0);
            ArrowShot   = 0;
          }
        }
        AimPos.xPos = ((float)pState->x / TOUCH_SIZE_X) * TARGET_SIZE;
        AimPos.yPos = ((float)pState->y / TOUCH_SIZE_Y) * TARGET_SIZE;
        Pressed = 1;
      } else if(Pressed) {
        Pressed = 0;
        
        if(ArrowShot == 0) {
          _ExecArrowShoot(&Data, &hAnim, &hTimerForce, &hTimerStats, &Force, &ArrowShot, &pMsg->hWin);
        }
      }
    }
    break;
  case WM_TIMER:
    Id = WM_GetTimerId(pMsg->Data.v);
    switch (Id) {
    case ID_TIMER_FORCE:
      Force -= 3;
      WM_InvalidateWindow(_hForceBar);
      WM_RestartTimer(hTimerForce, 25);

      if (Force <= 0) {
        if (ArrowShot == 0) {
          _ExecArrowShoot(&Data, &hAnim, &hTimerForce, &hTimerStats, &Force, &ArrowShot, &pMsg->hWin);
        }
      }
      break;
    case ID_TIMER_STATS:
      WM_ReleaseCapture();
      _hStatWin = WM_CreateWindow(LCD_X / 2, LCD_Y / 2, 1, 1, WM_CF_SHOW, _cbStatWin, 0);
      WM_BringToTop(_hStatWin);
      AnimData.hWin     = _hStatWin;
      AnimData.xPos     = WM_GetWindowOrgX(_hStatWin);
      AnimData.yPos     = WM_GetWindowOrgY(_hStatWin);
      AnimData.xSizeNew = 350;
      AnimData.ySizeNew = 215;
      AnimData.xSizeOrg = WM_GetWindowSizeX(_hStatWin);
      AnimData.ySizeOrg = WM_GetWindowSizeY(_hStatWin);
      AnimData.xDist    = ((LCD_X - (AnimData.xSizeNew)) / 2) - AnimData.xPos;
      AnimData.yDist    = ((LCD_Y - (AnimData.ySizeNew)) / 2) - AnimData.yPos;
      hStatAnim = GUI_ANIM_Create(600, 25, NULL, NULL);
      GUI_ANIM_AddItem(hStatAnim, 0, 600, ANIM_ACCELDECEL, &AnimData, _AnimStatWin);
      GUI_ANIM_StartEx(hStatAnim, 1, _AnimStatsDelete);
      break;
    }
    break;
  case MSG_NEW_ROUND:
    AimPos.xPos = AimPos.yPos = TARGET_SIZE / 2;
    Force = FORCE_BAR;
    _RoundCount++;
    if (_hStatWin) {
      WM_DeleteWindow(_hStatWin);
      _hStatWin = 0;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbBk
*/
static void _cbBk(WM_MESSAGE * pMsg) {
         WM_HWIN         hItem;
         int             Id;
         int             NCode;
         int             Points;
         int             i;
  static GUI_ANIM_HANDLE hAnim;
  static ANIM_PLANE_DATA Data;

  switch (pMsg->MsgId) {
  case WM_CREATE:
	in_main_menu_flag = 0;
	//
    // Save background window and create menu button
    //
    _hBk         = pMsg->hWin;
    _hMenuButton = APP_CreateMenuButton(pMsg->hWin);
    //
    // Create title window
    //
    hItem = WM_CreateWindowAsChild(0, -TITLE_YSIZE, LCD_X, TITLE_YSIZE, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbTitle, 0);
    WM_SetUntouchable(hItem, 1);
    //
    // Create windows for touch area and arrow indicator
    //
    _hTouchArea = WM_CreateWindowAsChild(LCD_X, 130, TOUCH_SIZE_X, TOUCH_SIZE_Y, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbTouchArea, 0);
    WM_CreateWindowAsChild(-105, 105, 50, 150, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbArrowIndicator, 0);

    _ArrowsShot    = 0;
    _DisplayStats  = 0;
    _TitleAnimDone = 0;
    //
    // Set up plane animation
    //
    Data.hWin = pMsg->hWin;
    Data.xPos = -(bmAirplane_145x27.XSize);
    Data.yPos = 30;
    Data.xStart = Data.xPos;
    Data.yStart = Data.yPos;
    Data.xDest = LCD_X + bmAirplane_145x27.XSize;
    Data.yDest = 30;
    hAnim = GUI_ANIM_Create(ANIM_PERIOD_PLANE, 25, NULL, NULL);
    GUI_ANIM_AddItem(hAnim, 0, ANIM_PERIOD_PLANE, ANIM_LINEAR, &Data, _AnimPlane);
    WM_CreateTimer(pMsg->hWin, ID_TIMER_PLANE, ANIM_PERIOD_PLANE / 8, 0);
    break;
  case WM_PAINT:
    //
    // Draw background, sky and fuji
    //
    GUI_DrawGradientV(0, 0, LCD_X, LCD_Y, COLOR_SKY_1, COLOR_SKY_2);
    GUI_DrawBitmap(&bmFuji_bare_480x136, 0, LCD_Y - bmFuji_bare_480x136.YSize);
    //
    // Draw airplane
    //
    GUI_DrawBitmap(&bmAirplane_145x27, Data.xPos, Data.yPos);
    break;
  case WM_TIMER:
    GUI_ANIM_StartEx(hAnim, -1, NULL);
    break;
  case WM_NOTIFY_PARENT:
    Id = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    if (Id == ID_BUTTON_MENU) {
      switch (NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        if (_RoundCount == MAX_ROUNDS && _ArrowsShot == MAX_ARROWS) {
          Points = 0;
          for (i = 0; i < MAX_ROUNDS * MAX_ARROWS; i++) {
            Points += _aShots[i].Points;
          }

          AddHighscoreArchery(Points);
        }
        GUI_ANIM_DeleteAll();
        RemoveCurrentWindow();
        in_main_menu_flag = 1;
        break;
      }
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
*       CreateWindowArchery
*/
WM_HWIN CreateWindowArchery(char * acName) {
  WM_HWIN hWin;

  GUI_USE_PARA(acName);
  hWin = WM_CreateWindowAsChild(0, 0, LCD_X, LCD_Y, WM_HBKWIN, WM_CF_SHOW, _cbBk, 0);
  BUTTON_SetReactOnTouch();
  return hWin;
}

/*************************** End of file ****************************/
