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
File        : ID_SCREEN_01_Slots.c
Purpose     : AppWizard managed file, function content could be changed
---------------------------END-OF-HEADER------------------------------
*/

#include "Application.h"
#include "../Generated/Resource.h"
#include "../Generated/ID_SCREEN_01.h"

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
*       cbID_SCREEN_01
*/
void cbID_SCREEN_01(WM_MESSAGE * pMsg) {
  GUI_USE_PARA(pMsg);
}

/*********************************************************************
*
*       ID_SCREEN_01__APPW_NOTIFICATION_INITDIALOG__ID_TIMER_00__APPW_JOB_START
*/
void ID_SCREEN_01__APPW_NOTIFICATION_INITDIALOG__ID_TIMER_00__APPW_JOB_START(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);

  TASK_INFO *task_info = get_task_info();
}

/*********************************************************************
*
*       ID_SCREEN_01__ID_TIMER_00__APPW_NOTIFICATION_TIMER
*/
void ID_SCREEN_01__ID_TIMER_00__APPW_NOTIFICATION_TIMER(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  WM_HWIN hItem;
  char string[256] = {0};

  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);

  TASK_INFO *task_info = get_task_info();

  /* update CPU load */
  hItem = WM_GetDialogItem(hScreen, ID_TEXT_01_CPU_LOAD);
  sprintf(string, "CPU Load %3d%%", task_info->cpu_load);
  TEXT_SetText(hItem, string);

  /* update IP address */
  hItem = WM_GetDialogItem(hScreen, ID_TEXT_02_IP);
  sprintf(string, "IP: %d.%d.%d.%d", task_info->ip_address[0], task_info->ip_address[1], task_info->ip_address[2], task_info->ip_address[3]);
  TEXT_SetText(hItem, string);

  /* update sd status info */
  hItem = WM_GetDialogItem(hScreen, ID_TEXT_03_SD);
  if(task_info->sd_status == 1)
  {
	  sprintf(string, "SD: attach");

  }
  else if(task_info->sd_status == 0)
  {
	  sprintf(string, "SD: detach");
  }
  TEXT_SetText(hItem, string);

  /* update time info */
  hItem = WM_GetDialogItem(hScreen, ID_TEXT_04_TIME);
  sprintf(string, "Time: %02d/%02d/%04d %02d:%02d:%02d", task_info->sys_time.month, task_info->sys_time.day, task_info->sys_time.year, task_info->sys_time.hour, task_info->sys_time.min, task_info->sys_time.sec);
  TEXT_SetText(hItem, string);

  /* update fps info */
  hItem = WM_GetDialogItem(hScreen, ID_TEXT_06_FPS);
  sprintf(string, "%3.02f fps", task_info->average_fps);
  TEXT_SetText(hItem, string);
}

/*************************** End of file ****************************/
