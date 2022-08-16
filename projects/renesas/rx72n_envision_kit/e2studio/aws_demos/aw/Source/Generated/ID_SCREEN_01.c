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
File        : ID_SCREEN_01.c
Purpose     : Generated file do NOT edit!
---------------------------END-OF-HEADER------------------------------
*/

#include "Resource.h"
#include "ID_SCREEN_01.h"

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       _aCreate
*/
static APPW_CREATE_ITEM _aCreate[] = {
  { WM_OBJECT_WINDOW_Create,
    ID_SCREEN_01, 0,
    { { { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
      },
      0, 0, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_BOX_Create,
    ID_BOX_00, ID_SCREEN_01,
    { { { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
      },
      0, 0, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_00_TITLE, ID_SCREEN_01,
    { { { DISPOSE_MODE_REL_PARENT, 6, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, -6, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      113, 32, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_01_CPU_LOAD, ID_SCREEN_01,
    { { { DISPOSE_MODE_REL_PARENT, 128, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, -6, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      100, 32, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_02_IP, ID_SCREEN_01,
    { { { DISPOSE_MODE_REL_PARENT, 142, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 245, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      98, 32, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_03_SD, ID_SCREEN_01,
    { { { DISPOSE_MODE_REL_PARENT, 68, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 245, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      67, 32, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_04_TIME, ID_SCREEN_01,
    { { { DISPOSE_MODE_REL_PARENT, 314, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 245, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      162, 32, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_05_USB, ID_SCREEN_01,
    { { { DISPOSE_MODE_REL_PARENT, 6, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 245, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      62, 32, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_06_FPS, ID_SCREEN_01,
    { { { DISPOSE_MODE_REL_PARENT, 222, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, -6, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      75, 32, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_BUTTON_Create,
    ID_BUTTON_01, ID_SCREEN_01,
    { { { DISPOSE_MODE_REL_PARENT, 306, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 2, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      80, 16, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_BUTTON_Create,
    ID_BUTTON_00, ID_SCREEN_01,
    { { { DISPOSE_MODE_REL_PARENT, 395, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 2, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      80, 16, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TIMER_Create,
    ID_TIMER_00, ID_SCREEN_01,
    { { { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      0, 0, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_WINDOW_Create,
    ID_WINDOW_00, ID_SCREEN_01,
    { { { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 20, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 20, 0, 0 },
      },
      0, 0, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_BOX_Create,
    ID_BOX_01, ID_WINDOW_00,
    { { { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
      },
      0, 0, 0, 0, 0, 0
    },
    { 0, 0 }
  },
};

/*********************************************************************
*
*       _aSetup
*/
static const APPW_SETUP_ITEM _aSetup[] = {
  { ID_BOX_00,           APPW_SET_PROP_COLOR,        { ARG_V(GUI_BLACK) } },
  { ID_TEXT_00_TITLE,    APPW_SET_PROP_COLOR,        { ARG_V(GUI_WHITE) } },
  { ID_TEXT_00_TITLE,    APPW_SET_PROP_ALIGNTEXT,    { ARG_V(GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER),
                                                       ARG_V(0),
                                                       ARG_V(0) } },
  { ID_TEXT_00_TITLE,    APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_2) } },
  { ID_TEXT_00_TITLE,    APPW_SET_PROP_FONT,         { ARG_VP(0, acRoboto_16_Normal_EXT_AA4) } },
  { ID_TEXT_01_CPU_LOAD, APPW_SET_PROP_COLOR,        { ARG_V(GUI_WHITE) } },
  { ID_TEXT_01_CPU_LOAD, APPW_SET_PROP_ALIGNTEXT,    { ARG_V(GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER),
                                                       ARG_V(0),
                                                       ARG_V(0) } },
  { ID_TEXT_01_CPU_LOAD, APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_3) } },
  { ID_TEXT_01_CPU_LOAD, APPW_SET_PROP_FONT,         { ARG_VP(0, acRoboto_16_Normal_EXT_AA4) } },
  { ID_TEXT_02_IP,       APPW_SET_PROP_COLOR,        { ARG_V(GUI_WHITE) } },
  { ID_TEXT_02_IP,       APPW_SET_PROP_ALIGNTEXT,    { ARG_V(GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER),
                                                       ARG_V(0),
                                                       ARG_V(0) } },
  { ID_TEXT_02_IP,       APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_6) } },
  { ID_TEXT_02_IP,       APPW_SET_PROP_FONT,         { ARG_VP(0, acRoboto_16_Normal_EXT_AA4) } },
  { ID_TEXT_03_SD,       APPW_SET_PROP_COLOR,        { ARG_V(GUI_WHITE) } },
  { ID_TEXT_03_SD,       APPW_SET_PROP_ALIGNTEXT,    { ARG_V(GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER),
                                                       ARG_V(0),
                                                       ARG_V(0) } },
  { ID_TEXT_03_SD,       APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_7) } },
  { ID_TEXT_03_SD,       APPW_SET_PROP_FONT,         { ARG_VP(0, acRoboto_16_Normal_EXT_AA4) } },
  { ID_TEXT_04_TIME,     APPW_SET_PROP_COLOR,        { ARG_V(GUI_WHITE) } },
  { ID_TEXT_04_TIME,     APPW_SET_PROP_ALIGNTEXT,    { ARG_V(GUI_ALIGN_RIGHT | GUI_ALIGN_VCENTER),
                                                       ARG_V(0),
                                                       ARG_V(0) } },
  { ID_TEXT_04_TIME,     APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_9) } },
  { ID_TEXT_04_TIME,     APPW_SET_PROP_FONT,         { ARG_VP(0, acRoboto_16_Normal_EXT_AA4) } },
  { ID_TEXT_05_USB,      APPW_SET_PROP_COLOR,        { ARG_V(GUI_WHITE) } },
  { ID_TEXT_05_USB,      APPW_SET_PROP_ALIGNTEXT,    { ARG_V(GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER),
                                                       ARG_V(0),
                                                       ARG_V(0) } },
  { ID_TEXT_05_USB,      APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_8) } },
  { ID_TEXT_05_USB,      APPW_SET_PROP_FONT,         { ARG_VP(0, acRoboto_16_Normal_EXT_AA4) } },
  { ID_TEXT_06_FPS,      APPW_SET_PROP_COLOR,        { ARG_V(GUI_WHITE) } },
  { ID_TEXT_06_FPS,      APPW_SET_PROP_ALIGNTEXT,    { ARG_V(GUI_ALIGN_RIGHT | GUI_ALIGN_VCENTER),
                                                       ARG_V(0),
                                                       ARG_V(0) } },
  { ID_TEXT_06_FPS,      APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_10) } },
  { ID_TEXT_06_FPS,      APPW_SET_PROP_FONT,         { ARG_VP(0, acRoboto_16_Normal_EXT_AA4) } },
  { ID_BUTTON_01,        APPW_SET_PROP_SBITMAPS,     { ARG_VP(0, acDARK_Button_Up_100x30),
                                                       ARG_VP(0, acDARK_Button_Down_100x30), } },
  { ID_BUTTON_01,        APPW_SET_PROP_COLORS,       { ARG_V(GUI_WHITE),
                                                       ARG_V(0xff646464),
                                                       ARG_V(GUI_INVALID_COLOR) } },
  { ID_BUTTON_01,        APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_5) } },
  { ID_BUTTON_00,        APPW_SET_PROP_SBITMAPS,     { ARG_VP(0, acDARK_Button_Up_100x30),
                                                       ARG_VP(0, acDARK_Button_Down_100x30), } },
  { ID_BUTTON_00,        APPW_SET_PROP_COLORS,       { ARG_V(GUI_WHITE),
                                                       ARG_V(0xff646464),
                                                       ARG_V(GUI_INVALID_COLOR) } },
  { ID_BUTTON_00,        APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_4) } },
  { ID_TIMER_00,         APPW_SET_PROP_PERIOD,       { ARG_V(1000) } },
  { ID_TIMER_00,         APPW_SET_PROP_AUTORESTART,  { ARG_V(0) } },
  { ID_BOX_01,           APPW_SET_PROP_COLOR,        { ARG_V(GUI_WHITE) } },
};

/*********************************************************************
*
*       _aAction
*/
static const APPW_ACTION_ITEM _aAction[] = {
  { ID_SCREEN_01,        APPW_NOTIFICATION_INITDIALOG,     ID_TIMER_00,         APPW_JOB_START,          ID_SCREEN_01__APPW_NOTIFICATION_INITDIALOG__ID_TIMER_00__APPW_JOB_START,
  },
  { ID_TIMER_00,         APPW_NOTIFICATION_TIMER,          0,                   APPW_JOB_NULL,           ID_SCREEN_01__ID_TIMER_00__APPW_NOTIFICATION_TIMER,
  },
};

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/
/*********************************************************************
*
*       ID_SCREEN_01_RootInfo
*/
APPW_ROOT_INFO ID_SCREEN_01_RootInfo = {
  ID_SCREEN_01,
  _aCreate, GUI_COUNTOF(_aCreate),
  _aSetup,  GUI_COUNTOF(_aSetup),
  _aAction, GUI_COUNTOF(_aAction),
  cbID_SCREEN_01,
  0
};

/*************************** End of file ****************************/
