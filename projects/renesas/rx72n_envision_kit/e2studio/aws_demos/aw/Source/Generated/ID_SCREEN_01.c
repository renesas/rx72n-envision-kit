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
    ID_BUTTON_00, ID_SCREEN_01,
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
    ID_BUTTON_01, ID_SCREEN_01,
    { { { DISPOSE_MODE_REL_PARENT, 394, 0, 0 },
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
  { WM_OBJECT_WINDOW_Create,
    ID_WINDOW_01, ID_SCREEN_01,
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
  { WM_OBJECT_LISTVIEW_Create,
    ID_LISTVIEW_00, ID_WINDOW_00,
    { { { DISPOSE_MODE_REL_PARENT, 2, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 4, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      476, 108, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_LISTVIEW_Create,
    ID_LISTVIEW_01, ID_WINDOW_00,
    { { { DISPOSE_MODE_REL_PARENT, 2, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 118, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      476, 108, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TIMER_Create,
    ID_TIMER_01, ID_WINDOW_00,
    { { { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      0, 0, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_BOX_Create,
    ID_BOX_02, ID_WINDOW_01,
    { { { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
      },
      0, 0, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_LISTBOX_Create,
    ID_LISTBOX_00, ID_WINDOW_01,
    { { { DISPOSE_MODE_REL_PARENT, 2, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 4, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      150, 200, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_MULTIEDIT_Create,
    ID_MULTIEDIT_00, ID_WINDOW_01,
    { { { DISPOSE_MODE_REL_PARENT, 156, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 4, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      318, 200, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_PROGBAR_Create,
    ID_PROGBAR_00, ID_WINDOW_01,
    { { { DISPOSE_MODE_REL_PARENT, 156, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 210, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      140, 16, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_BUTTON_Create,
    ID_BUTTON_03, ID_WINDOW_01,
    { { { DISPOSE_MODE_REL_PARENT, 306, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 210, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      80, 16, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_00, ID_WINDOW_01,
    { { { DISPOSE_MODE_REL_PARENT, 6, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 203, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      146, 32, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_BUTTON_Create,
    ID_BUTTON_04, ID_WINDOW_01,
    { { { DISPOSE_MODE_REL_PARENT, 394, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 210, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      80, 16, 0, 0, 0, 0
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
  { ID_BUTTON_00,        APPW_SET_PROP_SBITMAPS,     { ARG_VP(0, acDARK_Button_Up_100x30),
                                                       ARG_VP(0, acDARK_Button_Down_100x30), } },
  { ID_BUTTON_00,        APPW_SET_PROP_COLORS,       { ARG_V(GUI_WHITE),
                                                       ARG_V(0xff646464),
                                                       ARG_V(GUI_INVALID_COLOR) } },
  { ID_BUTTON_00,        APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_5) } },
  { ID_BUTTON_01,        APPW_SET_PROP_SBITMAPS,     { ARG_VP(0, acDARK_Button_Up_100x30),
                                                       ARG_VP(0, acDARK_Button_Down_100x30), } },
  { ID_BUTTON_01,        APPW_SET_PROP_COLORS,       { ARG_V(GUI_WHITE),
                                                       ARG_V(0xff646464),
                                                       ARG_V(GUI_INVALID_COLOR) } },
  { ID_BUTTON_01,        APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_4) } },
  { ID_TIMER_00,         APPW_SET_PROP_PERIOD,       { ARG_V(1000) } },
  { ID_TIMER_00,         APPW_SET_PROP_AUTORESTART,  { ARG_V(0) } },
  { ID_BOX_01,           APPW_SET_PROP_COLOR,        { ARG_V(GUI_WHITE) } },
  { ID_LISTVIEW_00,      APPW_SET_PROP_HEIGHT,       { ARG_V(20) } },
  { ID_LISTVIEW_00,      APPW_SET_PROP_HEIGHT2,      { ARG_V(20) } },
  { ID_LISTVIEW_00,      APPW_SET_PROP_BORDER,       { ARG_V(3) } },
  { ID_LISTVIEW_00,      APPW_SET_PROP_COLORS2,      { ARG_V(GUI_WHITE),
                                                       ARG_V(0xff2c2c30),
                                                       ARG_V(GUI_BLACK) } },
  { ID_LISTVIEW_00,      APPW_SET_PROP_FRAME,        { ARG_V(2) } },
  { ID_LISTVIEW_00,      APPW_SET_PROP_COLORS3,      { ARG_V(0xffbebebe),
                                                       ARG_V(GUI_INVALID_COLOR),
                                                       ARG_V(0xff2c2c30) } },
  { ID_LISTVIEW_00,      APPW_SET_PROP_BKCOLORS,     { ARG_V(GUI_WHITE),
                                                       ARG_V(0xff2777b3),
                                                       ARG_V(0xff2777b3),
                                                       ARG_V(GUI_INVALID_COLOR),
                                                       ARG_V(GUI_INVALID_COLOR) } },
  { ID_LISTVIEW_00,      APPW_SET_PROP_COLORS,       { ARG_V(0xff2c2c30),
                                                       ARG_V(GUI_WHITE),
                                                       ARG_V(GUI_WHITE) } },
  { ID_LISTVIEW_00,      APPW_SET_PROP_VISIBLE2,     { ARG_V(0) } },
  { ID_LISTVIEW_00,      APPW_SET_PROP_VISIBLE3,     { ARG_V(0) } },
  { ID_LISTVIEW_00,      APPW_SET_PROP_CONTENT,      { ARG_P(&ID_CONTENT_000_Data) } },
  { ID_LISTVIEW_00,      APPW_SET_PROP_VISIBLE,      { ARG_V(0) } },
  { ID_LISTVIEW_00,      APPW_SET_PROP_FIXED,        { ARG_V(2) } },
  { ID_LISTVIEW_01,      APPW_SET_PROP_HEIGHT,       { ARG_V(20) } },
  { ID_LISTVIEW_01,      APPW_SET_PROP_HEIGHT2,      { ARG_V(20) } },
  { ID_LISTVIEW_01,      APPW_SET_PROP_BORDER,       { ARG_V(3) } },
  { ID_LISTVIEW_01,      APPW_SET_PROP_COLORS2,      { ARG_V(GUI_WHITE),
                                                       ARG_V(0xff2c2c30),
                                                       ARG_V(GUI_BLACK) } },
  { ID_LISTVIEW_01,      APPW_SET_PROP_FRAME,        { ARG_V(2) } },
  { ID_LISTVIEW_01,      APPW_SET_PROP_VISIBLE,      { ARG_V(0) } },
  { ID_LISTVIEW_01,      APPW_SET_PROP_COLORS3,      { ARG_V(0xffbebebe),
                                                       ARG_V(GUI_INVALID_COLOR),
                                                       ARG_V(0xff2c2c30) } },
  { ID_LISTVIEW_01,      APPW_SET_PROP_BKCOLORS,     { ARG_V(GUI_WHITE),
                                                       ARG_V(0xff2777b3),
                                                       ARG_V(0xff2777b3),
                                                       ARG_V(GUI_INVALID_COLOR),
                                                       ARG_V(GUI_INVALID_COLOR) } },
  { ID_LISTVIEW_01,      APPW_SET_PROP_COLORS,       { ARG_V(0xff2c2c30),
                                                       ARG_V(GUI_WHITE),
                                                       ARG_V(GUI_WHITE) } },
  { ID_LISTVIEW_01,      APPW_SET_PROP_VISIBLE2,     { ARG_V(0) } },
  { ID_LISTVIEW_01,      APPW_SET_PROP_VISIBLE3,     { ARG_V(0) } },
  { ID_LISTVIEW_01,      APPW_SET_PROP_CONTENT,      { ARG_P(&ID_CONTENT_001_Data) } },
  { ID_LISTVIEW_01,      APPW_SET_PROP_FIXED,        { ARG_V(2) } },
  { ID_TIMER_01,         APPW_SET_PROP_PERIOD,       { ARG_V(1000) } },
  { ID_BOX_02,           APPW_SET_PROP_COLOR,        { ARG_V(GUI_WHITE) } },
  { ID_LISTBOX_00,       APPW_SET_PROP_BKCOLORS,     { ARG_V(GUI_WHITE),
                                                       ARG_V(0xff2777b3),
                                                       ARG_V(0xff2777b3) } },
  { ID_LISTBOX_00,       APPW_SET_PROP_COLORS,       { ARG_V(0xff2c2c30),
                                                       ARG_V(GUI_WHITE),
                                                       ARG_V(GUI_WHITE) } },
  { ID_LISTBOX_00,       APPW_SET_PROP_FRAME,        { ARG_V(2) } },
  { ID_LISTBOX_00,       APPW_SET_PROP_COLOR,        { ARG_V(0xff2c2c30) } },
  { ID_MULTIEDIT_00,     APPW_SET_PROP_BKCOLORS,     { ARG_V(GUI_BLACK),
                                                       ARG_V(GUI_INVALID_COLOR),
                                                       ARG_V(GUI_BLACK) } },
  { ID_MULTIEDIT_00,     APPW_SET_PROP_COLORS,       { ARG_V(GUI_GREEN),
                                                       ARG_V(GUI_INVALID_COLOR),
                                                       ARG_V(GUI_WHITE) } },
  { ID_MULTIEDIT_00,     APPW_SET_PROP_ALIGN,        { ARG_V(GUI_ALIGN_LEFT | GUI_ALIGN_TOP) } },
  { ID_MULTIEDIT_00,     APPW_SET_PROP_WRAP,         { ARG_V(0) } },
  { ID_MULTIEDIT_00,     APPW_SET_PROP_PERIOD,       { ARG_V(300) } },
  { ID_MULTIEDIT_00,     APPW_SET_PROP_MOTION,       { ARG_V(GUI_VERTICAL) } },
  { ID_MULTIEDIT_00,     APPW_SET_PROP_INVERT,       { ARG_V(0) } },
  { ID_PROGBAR_00,       APPW_SET_PROP_RADIUS,       { ARG_V(5) } },
  { ID_PROGBAR_00,       APPW_SET_PROP_FRAME,        { ARG_V(2) } },
  { ID_PROGBAR_00,       APPW_SET_PROP_COLOR,        { ARG_V(0xff2c2c30) } },
  { ID_PROGBAR_00,       APPW_SET_PROP_VALUE,        { ARG_V(0) } },
  { ID_PROGBAR_00,       APPW_SET_PROP_SBITMAPS,     { ARG_VP(0, acDARK_Progbar_Tile_H_Blue_1x16),
                                                       ARG_VP(0, acDARK_Progbar_Tile_H_Gray_1x16), } },
  { ID_PROGBAR_00,       APPW_SET_PROP_TILE,         { ARG_V(0) } },
  { ID_BUTTON_03,        APPW_SET_PROP_SBITMAPS,     { ARG_VP(0, acDARK_Button_Up_100x30),
                                                       ARG_VP(0, acDARK_Button_Down_100x30), } },
  { ID_BUTTON_03,        APPW_SET_PROP_COLORS,       { ARG_V(GUI_WHITE),
                                                       ARG_V(0xff646464),
                                                       ARG_V(GUI_INVALID_COLOR) } },
  { ID_BUTTON_03,        APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_11) } },
  { ID_TEXT_00,          APPW_SET_PROP_COLOR,        { ARG_V(GUI_BLACK) } },
  { ID_TEXT_00,          APPW_SET_PROP_ALIGNTEXT,    { ARG_V(GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER),
                                                       ARG_V(0),
                                                       ARG_V(0) } },
  { ID_TEXT_00,          APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_13) } },
  { ID_BUTTON_04,        APPW_SET_PROP_SBITMAPS,     { ARG_VP(0, acDARK_Button_Up_100x30),
                                                       ARG_VP(0, acDARK_Button_Down_100x30), } },
  { ID_BUTTON_04,        APPW_SET_PROP_COLORS,       { ARG_V(GUI_WHITE),
                                                       ARG_V(0xff646464),
                                                       ARG_V(GUI_INVALID_COLOR) } },
  { ID_BUTTON_04,        APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_12) } },
};

/*********************************************************************
*
*       Comparison(s)
*/
static APPW_COND_COMP _aComparison_05[] = {
  { { { APPW_IS_VAR, ID_VAR_02_WINDOW_INDEX }, { APPW_IS_VAL, 0 } }, APPW__CompareIsGreater },
};

static APPW_COND_COMP _aComparison_06[] = {
  { { { APPW_IS_VAR, ID_VAR_02_WINDOW_INDEX }, { APPW_IS_VAL, 1 } }, APPW__CompareIsLess },
};

static APPW_COND_COMP _aComparison_07[] = {
  { { { APPW_IS_VAR, ID_VAR_02_WINDOW_INDEX }, { APPW_IS_VAL, 1 } }, APPW__CompareIsEqual },
};

static APPW_COND_COMP _aComparison_08[] = {
  { { { APPW_IS_VAR, ID_VAR_02_WINDOW_INDEX }, { APPW_IS_VAL, 1 } }, APPW__CompareIsEqual },
};

static APPW_COND_COMP _aComparison_09[] = {
  { { { APPW_IS_VAR, ID_VAR_02_WINDOW_INDEX }, { APPW_IS_VAL, 0 } }, APPW__CompareIsEqual },
};

static APPW_COND_COMP _aComparison_0a[] = {
  { { { APPW_IS_VAR, ID_VAR_02_WINDOW_INDEX }, { APPW_IS_VAL, 0 } }, APPW__CompareIsEqual },
};

static APPW_COND_COMP _aComparison_0b[] = {
  { { { APPW_IS_VAR, ID_VAR_02_WINDOW_INDEX }, { APPW_IS_VAL, 0 } }, APPW__CompareIsEqual },
};

static APPW_COND_COMP _aComparison_0c[] = {
  { { { APPW_IS_VAR, ID_VAR_02_WINDOW_INDEX }, { APPW_IS_VAL, 1 } }, APPW__CompareIsEqual },
};

/*********************************************************************
*
*       Condition(s)
*/
static const APPW_COND _Condition_05 = { "A", _aComparison_05, GUI_COUNTOF(_aComparison_05) };
static const APPW_COND _Condition_06 = { "A", _aComparison_06, GUI_COUNTOF(_aComparison_06) };
static const APPW_COND _Condition_07 = { "A", _aComparison_07, GUI_COUNTOF(_aComparison_07) };
static const APPW_COND _Condition_08 = { "A", _aComparison_08, GUI_COUNTOF(_aComparison_08) };
static const APPW_COND _Condition_09 = { "A", _aComparison_09, GUI_COUNTOF(_aComparison_09) };
static const APPW_COND _Condition_0a = { "A", _aComparison_0a, GUI_COUNTOF(_aComparison_0a) };
static const APPW_COND _Condition_0b = { "A", _aComparison_0b, GUI_COUNTOF(_aComparison_0b) };
static const APPW_COND _Condition_0c = { "A", _aComparison_0c, GUI_COUNTOF(_aComparison_0c) };

/*********************************************************************
*
*       _aAction
*/
static const APPW_ACTION_ITEM _aAction[] = {
  { ID_SCREEN_01,        APPW_NOTIFICATION_INITDIALOG,     ID_WINDOW_01,        APPW_JOB_SETVIS,         ID_SCREEN_01__APPW_NOTIFICATION_INITDIALOG__ID_WINDOW_01__APPW_JOB_SETVIS,
    { ARG_V(APPW_SET_OFF),
    }, 0, NULL
  },
  { ID_SCREEN_01,        APPW_NOTIFICATION_INITDIALOG,     ID_TIMER_00,         APPW_JOB_START,          ID_SCREEN_01__APPW_NOTIFICATION_INITDIALOG__ID_TIMER_00__APPW_JOB_START,
  },
  { ID_SCREEN_01,        APPW_NOTIFICATION_INITDIALOG,     ID_TIMER_01,         APPW_JOB_START,          ID_SCREEN_01__ID_WINDOW_00__APPW_NOTIFICATION_FIXED__ID_TIMER_01__APPW_JOB_START,
  },
  { ID_TIMER_00,         APPW_NOTIFICATION_TIMER,          0,                   APPW_JOB_NULL,           ID_SCREEN_01__ID_TIMER_00__APPW_NOTIFICATION_TIMER,
  },
  { ID_TIMER_01,         APPW_NOTIFICATION_TIMER,          0,                   APPW_JOB_NULL,           ID_SCREEN_01__ID_TIMER_01__APPW_NOTIFICATION_TIMER,
  },
  { ID_BUTTON_00,        WM_NOTIFICATION_CLICKED,          ID_VAR_02_WINDOW_INDEX, APPW_JOB_ADDVALUE,       ID_SCREEN_01__ID_BUTTON_00__WM_NOTIFICATION_CLICKED,
    { ARG_V(-1),
    }, 0, &_Condition_05
  },
  { ID_BUTTON_01,        WM_NOTIFICATION_CLICKED,          ID_VAR_02_WINDOW_INDEX, APPW_JOB_ADDVALUE,       ID_SCREEN_01__ID_BUTTON_01__WM_NOTIFICATION_CLICKED,
    { ARG_V(1),
    }, 0, &_Condition_06
  },
  { ID_BUTTON_01,        WM_NOTIFICATION_CLICKED,          ID_WINDOW_01,        APPW_JOB_SETVIS,         ID_SCREEN_01__ID_BUTTON_01__WM_NOTIFICATION_CLICKED__ID_WINDOW_01__APPW_JOB_SETVIS,
    { ARG_V(APPW_SET_ON),
    }, 0, &_Condition_07
  },
  { ID_BUTTON_01,        WM_NOTIFICATION_CLICKED,          ID_WINDOW_00,        APPW_JOB_SETVIS,         ID_SCREEN_01__ID_BUTTON_01__WM_NOTIFICATION_CLICKED__ID_WINDOW_00__APPW_JOB_SETVIS,
    { ARG_V(APPW_SET_OFF),
    }, 0, &_Condition_08
  },
  { ID_BUTTON_00,        WM_NOTIFICATION_CLICKED,          ID_WINDOW_00,        APPW_JOB_SETVIS,         ID_SCREEN_01__ID_BUTTON_00__WM_NOTIFICATION_CLICKED__ID_WINDOW_00__APPW_JOB_SETVIS,
    { ARG_V(APPW_SET_ON),
    }, 0, &_Condition_09
  },
  { ID_BUTTON_00,        WM_NOTIFICATION_CLICKED,          ID_WINDOW_01,        APPW_JOB_SETVIS,         ID_SCREEN_01__ID_BUTTON_00__WM_NOTIFICATION_CLICKED__ID_WINDOW_01__APPW_JOB_SETVIS,
    { ARG_V(APPW_SET_OFF),
    }, 0, &_Condition_0a
  },
  { ID_VAR_02_WINDOW_INDEX, WM_NOTIFICATION_VALUE_CHANGED,    ID_TEXT_00_TITLE,    APPW_JOB_SETTEXT,        ID_SCREEN_01__WM_NOTIFICATION_VALUE_CHANGED__ID_TEXT_00_TITLE__APPW_JOB_SETTEXT,
    { ARG_V(ID_RTEXT_2),
    }, 65536, &_Condition_0b
  },
  { ID_VAR_02_WINDOW_INDEX, WM_NOTIFICATION_VALUE_CHANGED,    ID_TEXT_00_TITLE,    APPW_JOB_SETTEXT,        ID_SCREEN_01__WM_NOTIFICATION_VALUE_CHANGED__ID_TEXT_00_TITLE__APPW_JOB_SETTEXT_0,
    { ARG_V(ID_RTEXT_14),
    }, 65536, &_Condition_0c
  },
  { ID_BUTTON_03,        WM_NOTIFICATION_CLICKED,          0,                   APPW_JOB_NULL,           ID_SCREEN_01__ID_BUTTON_03__WM_NOTIFICATION_CLICKED,
  },
  { ID_BUTTON_04,        WM_NOTIFICATION_CLICKED,          0,                   APPW_JOB_NULL,           ID_SCREEN_01__ID_BUTTON_04__WM_NOTIFICATION_CLICKED,
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
