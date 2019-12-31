/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH                         *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.50                          *
*        Compiled Jun 11 2019, 16:51:25                              *
*        (c) 2019 Segger Microcontroller GmbH                        *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
#include <string.h>
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_MULTIEDIT_0    (GUI_ID_USER + 0x01)


// USER START (Optionally insert additional defines)
#define MULTIEDIT_MAX_NUM_CHARS 2048
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "SystemLogWindow", ID_WINDOW_0, 5, 23, 467, 228, 0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect, "Multiedit", ID_MULTIEDIT_0, 0, 0, 467, 228, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Multiedit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
    MULTIEDIT_SetText(hItem, "");
    // USER START (Optionally insert additional code for further widget initialization)
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
    MULTIEDIT_SetReadOnly(hItem, 1);
	MULTIEDIT_SetAutoScrollV(hItem, 1);
	MULTIEDIT_SetTextColor(hItem, MULTIEDIT_CI_READONLY, GUI_MAKE_COLOR(0x0000FF00));
    MULTIEDIT_SetBkColor(hItem, MULTIEDIT_CI_READONLY, GUI_MAKE_COLOR(0x00000000));
    MULTIEDIT_SetMaxNumChars(hItem, MULTIEDIT_MAX_NUM_CHARS);
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_MULTIEDIT_0: // Notifications sent by 'Multiedit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
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
*       CreateSystemLogWindow
*/
WM_HWIN CreateSystemLogWindow(void);
WM_HWIN CreateSystemLogWindow(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
void display_syslog_putstring(WM_HWIN hWin, char *string)
{
	  WM_HWIN hItem;

	  hItem = WM_GetDialogItem(hWin, ID_MULTIEDIT_0);
	  if((MULTIEDIT_GetTextSize(hItem) + strlen(string)) > MULTIEDIT_MAX_NUM_CHARS)
	  {
		  MULTIEDIT_SetText(hItem, "");
	  }
	  MULTIEDIT_AddText(hItem, string);
	  MULTIEDIT_SetCursorOffset(hItem, MULTIEDIT_GetTextSize(hItem));
}
// USER END

/*************************** End of file ****************************/
