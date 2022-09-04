/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2022  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File        : ID_SCREEN_01.h
Purpose     : Generated file do NOT edit!
---------------------------END-OF-HEADER------------------------------
*/

#ifndef ID_SCREEN_01_H
#define ID_SCREEN_01_H

#include "AppWizard.h"

/*********************************************************************
*
*       Objects
*/
#define ID_BOX_00           (GUI_ID_USER + 1)
#define ID_TEXT_00_TITLE    (GUI_ID_USER + 3)
#define ID_TEXT_01_CPU_LOAD (GUI_ID_USER + 5)
#define ID_TEXT_02_IP       (GUI_ID_USER + 9)
#define ID_TEXT_03_SD       (GUI_ID_USER + 10)
#define ID_TEXT_04_TIME     (GUI_ID_USER + 11)
#define ID_TEXT_05_USB      (GUI_ID_USER + 12)
#define ID_TEXT_06_FPS      (GUI_ID_USER + 13)
#define ID_BUTTON_00        (GUI_ID_USER + 8)
#define ID_BUTTON_01        (GUI_ID_USER + 7)
#define ID_TIMER_00         (GUI_ID_USER + 6)
#define ID_WINDOW_00        (GUI_ID_USER + 2)
#define ID_WINDOW_01        (GUI_ID_USER + 17)
#define ID_BOX_01           (GUI_ID_USER + 4)
#define ID_LISTVIEW_00      (GUI_ID_USER + 14)
#define ID_LISTVIEW_01      (GUI_ID_USER + 15)
#define ID_TIMER_01         (GUI_ID_USER + 16)
#define ID_BOX_02           (GUI_ID_USER + 18)
#define ID_LISTBOX_00       (GUI_ID_USER + 19)
#define ID_MULTIEDIT_00     (GUI_ID_USER + 20)
#define ID_PROGBAR_00       (GUI_ID_USER + 22)
#define ID_BUTTON_03        (GUI_ID_USER + 23)
#define ID_TEXT_00          (GUI_ID_USER + 25)
#define ID_BUTTON_04        (GUI_ID_USER + 21)

/*********************************************************************
*
*       Slots
*/
void ID_SCREEN_01__APPW_NOTIFICATION_INITDIALOG__ID_WINDOW_01__APPW_JOB_SETVIS         (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_01__APPW_NOTIFICATION_INITDIALOG__ID_TIMER_00__APPW_JOB_START           (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_01__ID_WINDOW_00__APPW_NOTIFICATION_FIXED__ID_TIMER_01__APPW_JOB_START  (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_01__ID_TIMER_00__APPW_NOTIFICATION_TIMER                                (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_01__ID_TIMER_01__APPW_NOTIFICATION_TIMER                                (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_01__ID_BUTTON_00__WM_NOTIFICATION_CLICKED                               (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_01__ID_BUTTON_01__WM_NOTIFICATION_CLICKED                               (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_01__ID_BUTTON_01__WM_NOTIFICATION_CLICKED__ID_WINDOW_01__APPW_JOB_SETVIS(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_01__ID_BUTTON_01__WM_NOTIFICATION_CLICKED__ID_WINDOW_00__APPW_JOB_SETVIS(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_01__ID_BUTTON_00__WM_NOTIFICATION_CLICKED__ID_WINDOW_00__APPW_JOB_SETVIS(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_01__ID_BUTTON_00__WM_NOTIFICATION_CLICKED__ID_WINDOW_01__APPW_JOB_SETVIS(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_01__WM_NOTIFICATION_VALUE_CHANGED__ID_TEXT_00_TITLE__APPW_JOB_SETTEXT   (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_01__WM_NOTIFICATION_VALUE_CHANGED__ID_TEXT_00_TITLE__APPW_JOB_SETTEXT_0 (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_01__ID_BUTTON_03__WM_NOTIFICATION_CLICKED                               (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_01__ID_BUTTON_04__WM_NOTIFICATION_CLICKED                               (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);

/*********************************************************************
*
*       Callback
*/
void cbID_SCREEN_01(WM_MESSAGE * pMsg);

#endif  // ID_SCREEN_01_H

/*************************** End of file ****************************/
