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
File        : ID_SCREEN_00.c
Purpose     : Generated file do NOT edit!
---------------------------END-OF-HEADER------------------------------
*/

#include "Resource.h"
#include "ID_SCREEN_00.h"

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
    ID_SCREEN_00, 0,
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
    ID_BOX_00, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
      },
      0, 0, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_IMAGE_Create,
    ID_IMAGE_00, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 180, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 75, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      120, 121, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_IMAGE_Create,
    ID_IMAGE_01, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 39, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 9, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      402, 61, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_00, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 39, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 203, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      402, 32, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_01, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 131, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 226, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      218, 32, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TIMER_Create,
    ID_TIMER_00, ID_SCREEN_00,
    { { { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
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
  { ID_BOX_00,    APPW_SET_PROP_COLOR,        { ARG_V(GUI_WHITE) } },
  { ID_IMAGE_00,  APPW_SET_PROP_TILE,         { ARG_V(0) } },
  { ID_IMAGE_00,  APPW_SET_PROP_SBITMAP,      { ARG_VP(0, acmcu),
                                                ARG_V(6386), } },
  { ID_IMAGE_01,  APPW_SET_PROP_TILE,         { ARG_V(0) } },
  { ID_IMAGE_01,  APPW_SET_PROP_SBITMAP,      { ARG_VP(0, actitle_renesas),
                                                ARG_V(13480), } },
  { ID_TEXT_00,   APPW_SET_PROP_COLOR,        { ARG_V(GUI_BLACK) } },
  { ID_TEXT_00,   APPW_SET_PROP_ALIGNTEXT,    { ARG_V(GUI_ALIGN_HCENTER | GUI_ALIGN_VCENTER),
                                                ARG_V(0),
                                                ARG_V(0) } },
  { ID_TEXT_00,   APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_0) } },
  { ID_TEXT_00,   APPW_SET_PROP_FONT,         { ARG_VP(0, acRoboto_24_Normal_EXT_AA4) } },
  { ID_TEXT_01,   APPW_SET_PROP_COLOR,        { ARG_V(GUI_BLACK) } },
  { ID_TEXT_01,   APPW_SET_PROP_ALIGNTEXT,    { ARG_V(GUI_ALIGN_HCENTER | GUI_ALIGN_VCENTER),
                                                ARG_V(0),
                                                ARG_V(0) } },
  { ID_TEXT_01,   APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_1) } },
  { ID_TEXT_01,   APPW_SET_PROP_FONT,         { ARG_VP(0, acRoboto_16_Normal_EXT_AA4) } },
  { ID_TIMER_00,  APPW_SET_PROP_PERIOD,       { ARG_V(500) } },
  { ID_TIMER_00,  APPW_SET_PROP_AUTORESTART,  { ARG_V(0) } },
};

/*********************************************************************
*
*       Comparison(s)
*/
static APPW_COND_COMP _aComparison_04[] = {
  { { { APPW_IS_VAR, ID_VAR_00 }, { APPW_IS_VAL, 1 } }, APPW__CompareIsEqual },
  { { { APPW_IS_VAR, ID_VAR_01 }, { APPW_IS_VAL, 2 } }, APPW__CompareIsEqual },
};

static APPW_COND_COMP _aComparison_05[] = {
  { { { APPW_IS_VAR, ID_VAR_00 }, { APPW_IS_VAL, 1 } }, APPW__CompareIsEqual },
  { { { APPW_IS_VAR, ID_VAR_01 }, { APPW_IS_VAL, 2 } }, APPW__CompareIsEqual },
};

/*********************************************************************
*
*       Condition(s)
*/
static const APPW_COND _Condition_04 = { "A&B", _aComparison_04, GUI_COUNTOF(_aComparison_04) };
static const APPW_COND _Condition_05 = { "A&B", _aComparison_05, GUI_COUNTOF(_aComparison_05) };

/*********************************************************************
*
*       _aAction
*/
static const APPW_ACTION_ITEM _aAction[] = {
  { ID_SCREEN_00, APPW_NOTIFICATION_INITDIALOG,     ID_TIMER_00,  APPW_JOB_START,          ID_SCREEN_00__APPW_NOTIFICATION_INITDIALOG__ID_TIMER_00__APPW_JOB_START,
  },
  { ID_SCREEN_00, APPW_NOTIFICATION_INITDIALOG,     ID_VAR_00,    APPW_JOB_SETVALUE,       ID_SCREEN_00__APPW_NOTIFICATION_INITDIALOG,
    { ARG_V(1),
    }, 0, NULL
  },
  { ID_TIMER_00,  APPW_NOTIFICATION_TIMER,          ID_TEXT_01,   APPW_JOB_SETVIS,         ID_SCREEN_00__ID_TIMER_00__APPW_NOTIFICATION_TIMER__ID_TEXT_01__APPW_JOB_SETVIS,
    { ARG_V(APPW_SET_TOGGLE),
    }, 0, NULL
  },
  { ID_SCREEN_00, APPW_NOTIFICATION_PIDPRESSED,     ID_VAR_01,    APPW_JOB_ADDVALUE,       ID_SCREEN_00__APPW_NOTIFICATION_PIDPRESSED_1,
    { ARG_V(1),
    }, 0, NULL
  },
  { ID_SCREEN_00, APPW_NOTIFICATION_PIDPRESSED,     0,            APPW_JOB_SHOWSCREEN,     ID_SCREEN_00__APPW_NOTIFICATION_PIDPRESSED_0,
    { ARG_V(ID_SCREEN_01),
    }, 0, &_Condition_04
  },
  { ID_SCREEN_00, APPW_NOTIFICATION_PIDPRESSED,     0,            APPW_JOB_CLOSESCREEN,    ID_SCREEN_00__APPW_NOTIFICATION_PIDPRESSED,
    { ARG_V(ID_SCREEN_00),
    }, 0, &_Condition_05
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
*       ID_SCREEN_00_RootInfo
*/
APPW_ROOT_INFO ID_SCREEN_00_RootInfo = {
  ID_SCREEN_00,
  _aCreate, GUI_COUNTOF(_aCreate),
  _aSetup,  GUI_COUNTOF(_aSetup),
  _aAction, GUI_COUNTOF(_aAction),
  cbID_SCREEN_00,
  0
};

/*************************** End of file ****************************/
