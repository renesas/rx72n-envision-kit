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
File        : Resource.h
Purpose     : Generated file do NOT edit!
---------------------------END-OF-HEADER------------------------------
*/

#ifndef RESOURCE_H
#define RESOURCE_H

#include "AppWizard.h"

/*********************************************************************
*
*       Text
*/
#define ID_RTEXT_0 0
#define ID_RTEXT_1 1
#define ID_RTEXT_2 2
#define ID_RTEXT_3 3
#define ID_RTEXT_4 4
#define ID_RTEXT_5 5
#define ID_RTEXT_6 6
#define ID_RTEXT_7 7
#define ID_RTEXT_8 8
#define ID_RTEXT_9 9
#define ID_RTEXT_10 10
#define ID_RTEXT_11 11
#define ID_RTEXT_12 12
#define ID_RTEXT_13 13

#define APPW_MANAGE_TEXT APPW_MANAGE_TEXT_EXT
extern const unsigned char acAPPW_Language_0[];

/*********************************************************************
*
*       Fonts
*/
extern const unsigned char acRoboto_24_Normal_EXT_AA4[];
extern const unsigned char acRoboto_16_Normal_EXT_AA4[];

/*********************************************************************
*
*       Images
*/
extern const unsigned char acmcu[];
extern const unsigned char actitle_renesas[];
extern const unsigned char acDARK_Button_Up_100x30[];
extern const unsigned char acDARK_Button_Down_100x30[];
extern const unsigned char acDARK_Progbar_Tile_H_Blue_1x16[];
extern const unsigned char acDARK_Progbar_Tile_H_Gray_1x16[];

/*********************************************************************
*
*       Variables
*/
#define ID_VAR_00 (GUI_ID_USER + 2048)
#define ID_VAR_01 (GUI_ID_USER + 2049)
#define ID_VAR_02_WINDOW_INDEX (GUI_ID_USER + 2050)

/*********************************************************************
*
*       Content
*/
extern const APPW_CONTENT ID_CONTENT_000_Data;
extern const APPW_CONTENT ID_CONTENT_001_Data;

/*********************************************************************
*
*       Screens
*/
#define ID_SCREEN_00 (GUI_ID_USER + 4096)
#define ID_SCREEN_01 (GUI_ID_USER + 4097)

extern APPW_ROOT_INFO ID_SCREEN_00_RootInfo;
extern APPW_ROOT_INFO ID_SCREEN_01_RootInfo;

#define APPW_INITIAL_SCREEN &ID_SCREEN_00_RootInfo

/*********************************************************************
*
*       Project path
*/
#define APPW_PROJECT_PATH "C:/git/rx72n-envision-kit/projects/renesas/rx72n_envision_kit/e2studio/aws_demos/aw"

#endif  // RESOURCE_H

/*************************** End of file ****************************/
