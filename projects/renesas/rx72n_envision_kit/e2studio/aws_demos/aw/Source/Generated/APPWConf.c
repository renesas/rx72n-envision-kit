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
File        : APPWConf.c
Purpose     : Generated file do NOT edit!
---------------------------END-OF-HEADER------------------------------
*/

#include "AppWizard.h"
#include "Resource.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define XSIZE_PHYS 480
#define YSIZE_PHYS 272
#define COLOR_CONVERSION GUICC_M565
#define DISPLAY_DRIVER GUIDRV_WIN32
#define NUM_BUFFERS   2
#define _appDrawing NULL
#define _NumDrawings 0

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       _apRootList
*/
static APPW_ROOT_INFO * _apRootList[] = {
  &ID_SCREEN_00_RootInfo,
  &ID_SCREEN_01_RootInfo,
};

/*********************************************************************
*
*       _NumScreens
*/
static unsigned _NumScreens = GUI_COUNTOF(_apRootList);

/*********************************************************************
*
*       _aVarList
*/
static APPW_VAR_OBJECT _aVarList[] = {
  { ID_VAR_00, 0, 0, NULL },
  { ID_VAR_01, 0, 0, NULL },
  { ID_VAR_02_WINDOW_INDEX, 0, 0, NULL },
};

/*********************************************************************
*
*       _NumVars
*/
static unsigned _NumVars = GUI_COUNTOF(_aVarList);

/*********************************************************************
*
*       ID_CONTENT_000
*/
static const char * ID_CONTENT_000_Col_0[] = {
  "CPU",
  "ROM/RAM size",
  "Frequency",
  "Crypto",
  "On Board Capability",
  "CPU Unique ID[16 bytes]",
};

static const char * ID_CONTENT_000_Col_1[] = {
  "",
  "",
  "",
  "",
  "",
  "",
};

static const char ** ID_CONTENT_000_Text[] = {
  ID_CONTENT_000_Col_0,
  ID_CONTENT_000_Col_1,
};

static const char * ID_CONTENT_000_Head[] = {
  "Hardware Spec",
  "Information",
};

static const U16 ID_CONTENT_000_Size[] = {
  237, 237
};

static const U8 ID_CONTENT_000_HeadAlign[] = {
  12, 12
};

static const U8 ID_CONTENT_000_TextAlign[] = {
  12, 12
};

const APPW_CONTENT ID_CONTENT_000_Data = {
  ID_CONTENT_000_Text,
  6,
  2,
  ID_CONTENT_000_Head,
  ID_CONTENT_000_Size,
  ID_CONTENT_000_HeadAlign,
  ID_CONTENT_000_TextAlign
};

/*********************************************************************
*
*       ID_CONTENT_001
*/
static const char * ID_CONTENT_001_Col_0[] = {
  "Firmware Version",
  "Amazon FreeRTOS Version",
  "emWin (Segger) Version",
  "Compiled at",
};

static const char * ID_CONTENT_001_Col_1[] = {
  "",
  "",
  "",
  "",
};

static const char ** ID_CONTENT_001_Text[] = {
  ID_CONTENT_001_Col_0,
  ID_CONTENT_001_Col_1,
};

static const char * ID_CONTENT_001_Head[] = {
  "Software Spec",
  "Information",
};

static const U16 ID_CONTENT_001_Size[] = {
  237, 237
};

static const U8 ID_CONTENT_001_HeadAlign[] = {
  12, 12
};

static const U8 ID_CONTENT_001_TextAlign[] = {
  12, 12
};

const APPW_CONTENT ID_CONTENT_001_Data = {
  ID_CONTENT_001_Text,
  4,
  2,
  ID_CONTENT_001_Head,
  ID_CONTENT_001_Size,
  ID_CONTENT_001_HeadAlign,
  ID_CONTENT_001_TextAlign
};

/*********************************************************************
*
*       Multibuffering
*/
static U8 _MultibufEnable = 1;

/*********************************************************************
*
*       _ShowMissingCharacters
*/
static U8 _ShowMissingCharacters = 1;

/*********************************************************************
*
*       _apLang
*/
static const char * _apLang[] = {
  (const char *)acAPPW_Language_0,
};

/*********************************************************************
*
*       _TextInit
*/
static const APPW_TEXT_INIT _TextInit = {
  _apLang,
  GUI_COUNTOF(_apLang),
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _InitText
*/
static void _InitText(void) {
  APPW_TextInitMem(&_TextInit);
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       APPW_X_Setup
*/
void APPW_X_Setup(void) {
  APPW_SetpfInitText(_InitText);
  APPW_X_FS_Init();
  APPW_MULTIBUF_Enable(_MultibufEnable);
  APPW_SetData(_apRootList, _NumScreens, _aVarList, _NumVars, (APPW_DRAWING_ITEM **)_appDrawing, _NumDrawings);
  GUI_ShowMissingCharacters(_ShowMissingCharacters);
}

/*********************************************************************
*
*       APPW_X_Config
*/
#ifdef WIN32
void APPW_X_Config(void) {
  GUI_MULTIBUF_Config(NUM_BUFFERS);
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  if (LCD_GetSwapXY()) {
    LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
    LCD_SetVSizeEx(0, YSIZE_PHYS, XSIZE_PHYS);
  } else {
    LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
    LCD_SetVSizeEx(0, XSIZE_PHYS, YSIZE_PHYS);
  }
}
#endif

/*************************** End of file ****************************/
