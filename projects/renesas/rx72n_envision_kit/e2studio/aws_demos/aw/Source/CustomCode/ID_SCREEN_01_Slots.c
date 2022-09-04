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
*       ID_SCREEN_01__APPW_NOTIFICATION_INITDIALOG__ID_WINDOW_01__APPW_JOB_SETVIS
*/
void ID_SCREEN_01__APPW_NOTIFICATION_INITDIALOG__ID_WINDOW_01__APPW_JOB_SETVIS(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);

  TASK_INFO *task_info = get_task_info();
  task_info->hWin_firmware_update_via_sd_card = hScreen;
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
}

/*********************************************************************
*
*       ID_SCREEN_01__ID_WINDOW_00__APPW_NOTIFICATION_FIXED__ID_TIMER_01__APPW_JOB_START
*/
void ID_SCREEN_01__ID_WINDOW_00__APPW_NOTIFICATION_FIXED__ID_TIMER_01__APPW_JOB_START(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
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

/*********************************************************************
*
*       ID_SCREEN_01__ID_TIMER_01__APPW_NOTIFICATION_TIMER
*/
void ID_SCREEN_01__ID_TIMER_01__APPW_NOTIFICATION_TIMER(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  WM_HWIN hItem;
  char string[256] = {0};

  /* this timer is one shot timer */

  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);

  TASK_INFO *task_info = get_task_info();

  /* update Hardware Spec info */
  hItem = WM_GetDialogItem(hScreen, ID_LISTVIEW_00);
  sprintf(string, "%s", task_info->hardware_info.cpu_name);
  LISTVIEW_SetItemText(hItem, 1, 0, string);
  sprintf(string, "%s", task_info->hardware_info.memory_size);
  LISTVIEW_SetItemText(hItem, 1, 1, string);
  sprintf(string, "%s", task_info->hardware_info.frequency);
  LISTVIEW_SetItemText(hItem, 1, 2, string);
  sprintf(string, "%s", task_info->hardware_info.crypto);
  LISTVIEW_SetItemText(hItem, 1, 3, string);
  sprintf(string, "%s", task_info->hardware_info.board_capability);
  LISTVIEW_SetItemText(hItem, 1, 4, string);
  sprintf(string, "0x");
  strcat(string, task_info->hardware_info.unique_id);
  string[2+UNIQUE_ID_LENGTH*2] = 0; /* 2 means "0x" */
  LISTVIEW_SetItemText(hItem, 1, 5, string);

  /* update Software Spec info */
  hItem = WM_GetDialogItem(hScreen, ID_LISTVIEW_01);
  sprintf(string, "%s", task_info->software_info.firmware_version);
  LISTVIEW_SetItemText(hItem, 1, 0, string);
  sprintf(string, "%s", task_info->software_info.amazon_freertos_version);
  LISTVIEW_SetItemText(hItem, 1, 1, string);
  sprintf(string, "%s", task_info->software_info.emwin_version);
  LISTVIEW_SetItemText(hItem, 1, 2, string);
  sprintf(string, "%s", task_info->software_info.compiled_time);
  LISTVIEW_SetItemText(hItem, 1, 3, string);

}

/*********************************************************************
*
*       ID_SCREEN_01__ID_BUTTON_00__WM_NOTIFICATION_CLICKED
*/
void ID_SCREEN_01__ID_BUTTON_00__WM_NOTIFICATION_CLICKED(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
}

/*********************************************************************
*
*       ID_SCREEN_01__ID_BUTTON_01__WM_NOTIFICATION_CLICKED
*/
void ID_SCREEN_01__ID_BUTTON_01__WM_NOTIFICATION_CLICKED(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
}

/*********************************************************************
*
*       ID_SCREEN_01__ID_BUTTON_01__WM_NOTIFICATION_CLICKED__ID_WINDOW_01__APPW_JOB_SETVIS
*/
void ID_SCREEN_01__ID_BUTTON_01__WM_NOTIFICATION_CLICKED__ID_WINDOW_01__APPW_JOB_SETVIS(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
}

/*********************************************************************
*
*       ID_SCREEN_01__ID_BUTTON_01__WM_NOTIFICATION_CLICKED__ID_WINDOW_00__APPW_JOB_SETVIS
*/
void ID_SCREEN_01__ID_BUTTON_01__WM_NOTIFICATION_CLICKED__ID_WINDOW_00__APPW_JOB_SETVIS(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
}

/*********************************************************************
*
*       ID_SCREEN_01__ID_BUTTON_00__WM_NOTIFICATION_CLICKED__ID_WINDOW_00__APPW_JOB_SETVIS
*/
void ID_SCREEN_01__ID_BUTTON_00__WM_NOTIFICATION_CLICKED__ID_WINDOW_00__APPW_JOB_SETVIS(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
}

/*********************************************************************
*
*       ID_SCREEN_01__ID_BUTTON_00__WM_NOTIFICATION_CLICKED__ID_WINDOW_01__APPW_JOB_SETVIS
*/
void ID_SCREEN_01__ID_BUTTON_00__WM_NOTIFICATION_CLICKED__ID_WINDOW_01__APPW_JOB_SETVIS(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
}

/*********************************************************************
*
*       ID_SCREEN_01__WM_NOTIFICATION_VALUE_CHANGED__ID_TEXT_00_TITLE__APPW_JOB_SETTEXT
*/
void ID_SCREEN_01__WM_NOTIFICATION_VALUE_CHANGED__ID_TEXT_00_TITLE__APPW_JOB_SETTEXT(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
}

/*********************************************************************
*
*       ID_SCREEN_01__WM_NOTIFICATION_VALUE_CHANGED__ID_TEXT_00_TITLE__APPW_JOB_SETTEXT_0
*/
void ID_SCREEN_01__WM_NOTIFICATION_VALUE_CHANGED__ID_TEXT_00_TITLE__APPW_JOB_SETTEXT_0(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
}

/*********************************************************************
*
*       ID_SCREEN_01__ID_BUTTON_03__WM_NOTIFICATION_CLICKED
*/
void ID_SCREEN_01__ID_BUTTON_03__WM_NOTIFICATION_CLICKED(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  WM_HWIN hItem;
  char selected_file_name[256+1];
  int select_id;

  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);

  TASK_INFO *task_info = get_task_info();
  hItem = WM_GetDialogItem(hScreen, ID_LISTBOX_00);

  select_id = LISTBOX_GetSel(hItem);
  if(select_id == -1)
  {
      firmware_update_log_string(task_info, "No file selected.\r\n");
      return;
  }
  LISTBOX_GetItemText(hItem, select_id, selected_file_name, 256);
  if(true != is_firmware_updating())
  {
      hItem = WM_GetDialogItem(hScreen, ID_MULTIEDIT_00);
      firmware_update_request(selected_file_name);
      firmware_update_log_string(task_info, "start firmware update.\r\n");
  }
  else
  {
      firmware_update_log_string(task_info, "cannot firmware update in this status.\r\n");
  }
}

/*********************************************************************
*
*       ID_SCREEN_01__ID_BUTTON_04__WM_NOTIFICATION_CLICKED
*/
void ID_SCREEN_01__ID_BUTTON_04__WM_NOTIFICATION_CLICKED(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);

  TASK_INFO *task_info = get_task_info();
  task_info->software_reset_requested_flag = 1;
}

/*************************** End of file ****************************/
