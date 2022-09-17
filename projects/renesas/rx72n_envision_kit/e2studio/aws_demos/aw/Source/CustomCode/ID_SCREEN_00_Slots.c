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
File        : ID_SCREEN_00_Slots.c
Purpose     : AppWizard managed file, function content could be changed
---------------------------END-OF-HEADER------------------------------
*/

#include "Application.h"
#include "../Generated/Resource.h"
#include "../Generated/ID_SCREEN_00.h"

/*** Begin of user code area ***/
/*** End of user code area ***/

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       cbID_SCREEN_00
*/
void cbID_SCREEN_00(WM_MESSAGE * pMsg) {
  GUI_USE_PARA(pMsg);
}

/*********************************************************************
*
*       ID_SCREEN_00__APPW_NOTIFICATION_INITDIALOG__ID_TIMER_00__APPW_JOB_START
*/
void ID_SCREEN_00__APPW_NOTIFICATION_INITDIALOG__ID_TIMER_00__APPW_JOB_START(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
}

/*********************************************************************
*
*       ID_SCREEN_00__APPW_NOTIFICATION_INITDIALOG
*/
void ID_SCREEN_00__APPW_NOTIFICATION_INITDIALOG(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
}

/*********************************************************************
*
*       ID_SCREEN_00__ID_TIMER_00__APPW_NOTIFICATION_TIMER__ID_TEXT_01__APPW_JOB_SETVIS
*/
void ID_SCREEN_00__ID_TIMER_00__APPW_NOTIFICATION_TIMER__ID_TEXT_01__APPW_JOB_SETVIS(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
}

/*********************************************************************
*
*       ID_SCREEN_00__APPW_NOTIFICATION_PIDPRESSED_1
*/
void ID_SCREEN_00__APPW_NOTIFICATION_PIDPRESSED_1(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
}

/*********************************************************************
*
*       ID_SCREEN_00__APPW_NOTIFICATION_PIDPRESSED_0
*/
void ID_SCREEN_00__APPW_NOTIFICATION_PIDPRESSED_0(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
}

/*********************************************************************
*
*       ID_SCREEN_00__APPW_NOTIFICATION_PIDPRESSED
*/
void ID_SCREEN_00__APPW_NOTIFICATION_PIDPRESSED(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);

  TASK_INFO *task_info = get_task_info();
  task_info->gui_initialize_complete_flag = 1;

  /* notify completing GUI initialization and first touch to each tasks */
  xTaskNotifyGive(task_info->serial_terminal_task_handle);
  xTaskNotifyGive(task_info->sdcard_task_handle);
  xTaskNotifyGive(task_info->task_manager_task_handle);
  xTaskNotifyGive(task_info->serial_flash_task_handle);
}

/*************************** End of file ****************************/
