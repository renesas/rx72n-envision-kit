/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2003 - 2015  SEGGER Microcontroller GmbH & Co. KG        *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File        : Resource.h
Purpose     : Resources
---------------------------END-OF-HEADER------------------------------
*/
#ifndef RESOURCE_H
#define RESOURCE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*********************************************************************
*
*       Externals
*
**********************************************************************
*/
//
// Fonts
//
extern GUI_CONST_STORAGE GUI_FONT GUI_FontCommodore64Angled8;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontCommodore64Angled12;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontCommodore64Angled14;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontCommodore64Angled18;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontCommodore64Angled20;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontCommodore64Angled24;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontCommodore64Angled32;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontCommodore64Angled40;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontOctinSportsRg40;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSFSportsNight48;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontlazer8448;

// Archery
extern GUI_CONST_STORAGE GUI_FONT GUI_FontFont_24;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontFont_48;

//
// Main Menu Background
//
extern GUI_CONST_STORAGE GUI_BITMAP bmArchery_200x113;
extern GUI_CONST_STORAGE GUI_BITMAP bmRunning_200x113;
extern GUI_CONST_STORAGE GUI_BITMAP bmLifting_200x113;
extern GUI_CONST_STORAGE GUI_BITMAP bmCycling_200x113;

// Buttons
extern GUI_CONST_STORAGE GUI_BITMAP bmHelpButton_36x36;
extern GUI_CONST_STORAGE GUI_BITMAP bmExitButton_36x36;
extern GUI_CONST_STORAGE GUI_BITMAP bmClickLayer_204x117;
extern GUI_CONST_STORAGE GUI_BITMAP bmPointingArrow_21x30;
extern GUI_CONST_STORAGE GUI_BITMAP bmPointingArrow_15x22;

//
// Archery
//
extern GUI_CONST_STORAGE GUI_BITMAP bmNextButton_30x30;
extern GUI_CONST_STORAGE GUI_BITMAP bmArrow_16x37;
extern GUI_CONST_STORAGE GUI_BITMAP bmQuiver_37x110;
extern GUI_CONST_STORAGE GUI_BITMAP bmAirplane_145x27;
extern GUI_CONST_STORAGE GUI_BITMAP bmTouchArea_170x120;
extern GUI_CONST_STORAGE GUI_BITMAP bmFuji_bare_480x136;

// Shot Arrows
extern GUI_CONST_STORAGE GUI_BITMAP bmShot_Arrow_0;
extern GUI_CONST_STORAGE GUI_BITMAP bmShot_Arrow_1;
extern GUI_CONST_STORAGE GUI_BITMAP bmShot_Arrow_2;
extern GUI_CONST_STORAGE GUI_BITMAP bmShot_Arrow_3;
extern GUI_CONST_STORAGE GUI_BITMAP bmShot_Arrow_4;
extern GUI_CONST_STORAGE GUI_BITMAP bmShot_Arrow_5;
extern GUI_CONST_STORAGE GUI_BITMAP bmShot_Arrow_6;

//
// Running
//
extern const GUI_BITMAP * apbmRunner1_Small[6];

extern GUI_CONST_STORAGE GUI_BITMAP bmStand_14x77;
extern GUI_CONST_STORAGE GUI_BITMAP bmAudience_new_144x57;
extern GUI_CONST_STORAGE GUI_BITMAP bmrenesas_logo_196x32;
extern GUI_CONST_STORAGE GUI_BITMAP bmShoe1_100x100;
extern GUI_CONST_STORAGE GUI_BITMAP bmShoe2_100x100;
extern GUI_CONST_STORAGE GUI_BITMAP bmEnergy_18x19;

// Meter marks
extern GUI_CONST_STORAGE GUI_BITMAP bm100m_95x91;
extern GUI_CONST_STORAGE GUI_BITMAP bm200m_95x91;
extern GUI_CONST_STORAGE GUI_BITMAP bm300m_95x91;
extern GUI_CONST_STORAGE GUI_BITMAP bmstartend_95x91;

extern GUI_CONST_STORAGE GUI_BITMAP bmRenesas_small;

//
// Weightlifting
//
extern const GUI_BITMAP * apbmLifter_146x148[13];
extern const unsigned     aDelayLifter_146x148[13];

extern GUI_CONST_STORAGE GUI_BITMAP bmFloor_480x65;

// Buttons
extern GUI_CONST_STORAGE GUI_BITMAP bmActionButton_Unpressed_130x80;
extern GUI_CONST_STORAGE GUI_BITMAP bmActionButton_Pressed_130x80;

// Weights
extern GUI_CONST_STORAGE GUI_BITMAP bmWeight_small_6x50;
extern GUI_CONST_STORAGE GUI_BITMAP bmWeight_med_6x50;
extern GUI_CONST_STORAGE GUI_BITMAP bmWeight_big_6x50;

//
// Cycling
//
extern GUI_CONST_STORAGE GUI_BITMAP bmBarrier_224x56;
extern GUI_CONST_STORAGE GUI_BITMAP bmCrowd_130x90;
extern GUI_CONST_STORAGE GUI_BITMAP bmBuildings_358x132;
extern GUI_CONST_STORAGE GUI_BITMAP bmStartFinish_Left_26x160;
extern GUI_CONST_STORAGE GUI_BITMAP bmStartFinish_Right_125x264;

extern GUI_CONST_STORAGE GUI_BITMAP bmKnob_150x150;

extern const GUI_BITMAP * apbmBicycleRider_15F_134x122[15];

extern int in_main_menu_flag;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// Util
//
#define RAND(min, max)            RandomNumber(min, max)
#define MAGNIFY_RECT(r, f)        GUI_AddRect(r, r, f)
#define SHRINK_RECT(r, f)         GUI_AddRect(r, r, -f)
#define TIME_TO_MS(i)             (i % 1000) / 10
#define TIME_TO_SEC(i)            (i / 1000) % 60
#define TIME_TO_MIN(i)            (i / 1000) / 60
#define MS_TO_AVG_KMH(m, t)       ((m / (t / 1000)) * 3.6)
#define PRINT_TIME(c, t)          sprintf(c, "%02d:%02d:%02d", TIME_TO_MIN(t), TIME_TO_SEC(t), TIME_TO_MS(t))
#define PRINT_POINTS(c, p)        sprintf(c, "%d", p)
#define LCD_X                     LCD_GetXSize()
#define LCD_Y                     LCD_GetYSize()

//
// Bitmap Macros
//
#define BM_RENESAS                bmrenesas_logo_196x32
#define BM_POINT_ARROW            bmPointingArrow_15x22

//
// Fonts Macros
//
#define FONT_C64_8                &GUI_FontCommodore64Angled8
#define FONT_C64_12               &GUI_FontCommodore64Angled12
#define FONT_C64_14               &GUI_FontCommodore64Angled14
#define FONT_C64_18               &GUI_FontCommodore64Angled18
#define FONT_C64_20               &GUI_FontCommodore64Angled20
#define FONT_C64_24               &GUI_FontCommodore64Angled24
#define FONT_C64_32               &GUI_FontCommodore64Angled32
#define FONT_C64_40               &GUI_FontCommodore64Angled40
                                  
#define FONT_BOXY_24              &GUI_FontFont_24
#define FONT_BOXY_48              &GUI_FontFont_48
#define FONT_SPORTS_48            &GUI_FontSFSportsNight48
#define FONT_OCTIN_48             &GUI_FontOctinSportsRg40
#define FONT_LAZER_48             &GUI_Fontlazer8448

//
// Keypad
//
#define COLOR_BACK0               GUI_MAKE_COLOR(0x00FF3333)
#define COLOR_BACK1               GUI_MAKE_COLOR(0x00550000)
#define COLOR_BORDER              GUI_MAKE_COLOR(0x00444444)
#define COLOR_KEYPAD0             GUI_MAKE_COLOR(0x00AAAAAA)
#define COLOR_KEYPAD1             GUI_MAKE_COLOR(0x00555555)
#define BUTTON_COLOR0             GUI_MAKE_COLOR(0x00EEEEEE)
#define BUTTON_COLOR1             GUI_MAKE_COLOR(0x00CCCCCC)
#define BUTTON_COLOR2             GUI_MAKE_COLOR(0x00CCCCCC)
#define BUTTON_COLOR3             GUI_MAKE_COLOR(0x00AAAAAA)
#define COLOR_BLUE                GUI_MAKE_COLOR(0x009C5B2C)
#define COLOR_BUTTON_PRESSED      GUI_MAKE_COLOR(0x00FF5423)
#define COLOR_BUTTON_UNPRESSED    GUI_MAKE_COLOR(0x00FFA000)
#define COLOR_GRAY                GUI_MAKE_COLOR(0x00CCCCCC)
#define BUTTON_SKINFLEX_RADIUS    4
#define ID_BUTTON                 (GUI_ID_USER + 0)
#define ANIMATION_TIME            300
#define BM_BACKBUTTON             bmBackButton_53x16
#define SIZE_Y_KEYPAD             194

#define APP_INIT_LOWERCASE        (WM_USER + 0x00)
#define MSG_ANIMATE               (WM_USER + 0x01)
#define MSG_DO_TRACE              (WM_USER + 0x02)

//
// Colors
//
#define RENESAS_BLUE              GUI_MAKE_COLOR(0x009B3F2D)
#define COLOR_METER_MARK          GUI_MAKE_COLOR(0x001DACD0)

#define COLOR_GRAY_A9             GUI_MAKE_COLOR(0x00A9A9A9)

#define COLOR_GREY_PRIMARY        GUI_MAKE_COLOR(0x00A9A9A9)
#define COLOR_GREY_SECONDARY      GUI_MAKE_COLOR(0x00808080)
#define COLOR_ORANGE_PRIMARY      GUI_MAKE_COLOR(0x004763FF)
#define COLOR_ORANGE_SECONDARY    GUI_MAKE_COLOR(0x000045FF)
#define COLOR_RED_EXIT            GUI_MAKE_COLOR(0x004D4DFF)

#define COLOR_STATWIN_1           GUI_MAKE_COLOR(0x00DFD9CF)
#define COLOR_STATWIN_2           GUI_MAKE_COLOR(0x00F0EBE2)

#define COLOR_SKY_1               GUI_MAKE_COLOR(0x00FFBA00)
#define COLOR_SKY_2               GUI_MAKE_COLOR(0x00FAE4B4)

#define COLOR_MENU_BK_1           GUI_MAKE_COLOR(0x00FBE9C2)
#define COLOR_MENU_BK_2           GUI_MAKE_COLOR(0x00FDC4A1)

#define COLOR_EXHAUST             GUI_MAKE_COLOR(0x00FF8000)

// Target colors
#define COLOR_TARGET_BLACK        GUI_MAKE_COLOR(0x00231F20)
#define COLOR_TARGET_RED          GUI_MAKE_COLOR(0x003840F1)
#define COLOR_TARGET_BLUE         GUI_MAKE_COLOR(0x00E5A401)
#define COLOR_TARGET_YELLOW       GUI_MAKE_COLOR(0x0000F0FF)
#define COLOR_AIM_RED             GUI_MAKE_COLOR(0x002222B2)

#define COLOR_HIGHLIGHTED_RED     GUI_MAKE_COLOR(0x000000CC)

//
// General
//
#define STATWIN_BORDER            20
#define ANIM_PERIOD_STATS         300
#define DEMO_TITLE                "SPORTS GAMES"

//
// GUI IDs
//

// Main Menu
#define ID_BUTTON_MENU_PLAY       (GUI_ID_USER + 0x02)
#define ID_BUTTON_MENU_HIGHSCORE  (GUI_ID_USER + 0x03)
#define ID_BUTTON_MENU_SETTINGS   (GUI_ID_USER + 0x04)
#define ID_BUTTON_MENU_HELP       (GUI_ID_USER + 0x05)
#define ID_BUTTON_MENU_EXIT       (GUI_ID_USER + 0x06)
#define ID_BUTTON_MENU_PLAYGAME0  (GUI_ID_USER + 0x07)
#define ID_BUTTON_MENU_PLAYGAME1  (GUI_ID_USER + 0x08)
#define ID_BUTTON_MENU_PLAYGAME2  (GUI_ID_USER + 0x09)
#define ID_BUTTON_MENU_PLAYGAME3  (GUI_ID_USER + 0x0A)

#define ID_BUTTON_MENU_DEMO1      (GUI_ID_USER + 0x30)
#define ID_BUTTON_MENU_DEMO2      (GUI_ID_USER + 0x31)
#define ID_BUTTON_MENU_DEMO3      (GUI_ID_USER + 0x32)
#define ID_BUTTON_MENU_DEMO4      (GUI_ID_USER + 0x33)

#define ID_WINDOW_MENU            (GUI_ID_USER + 0x00)
#define ID_WINDOW_MENU_SELECT     (GUI_ID_USER + 0x10)
#define ID_WINDOW_MENU_CIRCLE     (GUI_ID_USER + 0x20)

#define ID_TEXT_MENU_HEADER       (GUI_ID_USER + 0x01)
#define ID_TEXT_MENU_DEMO1        (GUI_ID_USER + 0x11)
#define ID_TEXT_MENU_DEMO2        (GUI_ID_USER + 0x12)
#define ID_TEXT_MENU_DEMO3        (GUI_ID_USER + 0x13)
#define ID_TEXT_MENU_DEMO4        (GUI_ID_USER + 0x14)

// Settings
#define ID_BUTTON_SETTINGS_OK     (GUI_ID_USER + 0x40)
#define ID_EDIT_SETTINGS          GUI_ID_EDIT0
#define ID_TEXT_SETTINGS_NAME     (GUI_ID_USER + 0x16)

// Highscore
#define ID_TEXT_HIGHSCORE_HEADER  (GUI_ID_USER + 0x40)

// General
#define ID_BUTTON_MENU            (GUI_ID_USER + 0x07) // Back button

// Runner
#define ID_BUTTON_RUN_LEFT        GUI_ID_BUTTON0
#define ID_BUTTON_RUN_RIGHT       GUI_ID_BUTTON1

// Weightlifting
#define ID_BUTTON_WEIGHT_LESS     (GUI_ID_USER + 0x50)
#define ID_BUTTON_WEIGHT_MORE     (GUI_ID_USER + 0x51)
#define ID_BUTTON_WEIGHT_ACTION   (GUI_ID_USER + 0x52)
#define ID_TEXT_WEIGHT_KG         (GUI_ID_USER + 0x53)
#define ID_TEXT_WEIGHT_ACTION     (GUI_ID_USER + 0x54)
#define ID_TEXT_WEIGHT_RECORD     (GUI_ID_USER + 0x55)
#define ID_TEXT_WEIGHT_TRIES_T    (GUI_ID_USER + 0x56)
#define ID_TEXT_WEIGHT_TRIES      (GUI_ID_USER + 0x57)

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
//
// Keypad
//
typedef struct {
  int          xPos;
  int          yPos;
  int          xSize;
  int          ySize;
  const char * acLabel;
  void      (* pfDraw)(WM_HWIN hWin);
  int          HasLowerCase;
} BUTTON_DATA;

typedef struct {
  BUTTON_SKINFLEX_PROPS * pProp;
  int                     Index;
  BUTTON_SKINFLEX_PROPS   PropOld;
} BUTTON_PROP;

typedef struct {
  WM_HWIN hWin;
  WM_HWIN hBackButton;
  int     Dir;
} ANIM_DATA;

typedef struct {
  WM_HWIN hWin;
  int     xSize;
  int     xPos;
  int     yPos;
} ANIM_TITLE_DATA;

//
// Main Menu
//
typedef struct {
  WM_HWIN hWin;
  int     xDist;
  int     xStart;
  int     xPos;
  int     Dir;
} ANIM_DATA_MENU;

typedef WM_HWIN (*DEMO_FUNC)(char * acName);  // Prototype

typedef struct {
  int  Data;
  char Name[11];
} HIGHSCORE;

//
// Running & Cycling
//

typedef struct {
  int Meters;
  int TimeStart;
  int FinalTime;
  int KmhAvg;
  int KmhBest;
} STATS;

typedef struct {
  WM_HWIN hWin;
  int     xPos;
  int     yPos;
  int     xDist;
  int     yDist;
  int     xSizeOrg;
  int     ySizeOrg;
  int     xSizeNew;
  int     ySizeNew;
} STAT_WIN_ANIM_DATA;

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/
//
// Create demo games
//
WM_HWIN CreateWindowArchery      (char * acName);
WM_HWIN CreateWindowRunning      (char * acName);
WM_HWIN CreateWindowWeightlifting(char * acName);
WM_HWIN CreateWindowCycling      (char * acName);
//
// Remove game
//
void RemoveCurrentWindow(void);
//
// Create widgets
//
WM_HWIN CreateMainMenu      (void);
WM_HWIN APP_CreateMenuButton(WM_HWIN hParent);
WM_HWIN GUI_CreateKeyPad    (WM_HWIN hParent);
//
// Highscore functions
//
void AddHighscoreArchery(int Points);
void AddHighscoreRunner (int Time);
void AddHighscoreWeight (int Kg);
void AddHighscoreCycling(int Time);
//
// Helper functions
//
int RandomNumber(int min, int max);

extern void SwitchButtonInit(void);
extern void CheckPressedState(void);
extern void sdcard_init(void);
extern void sdcard_task( void * pvParameters );

#endif /* RESOURCE_H */
