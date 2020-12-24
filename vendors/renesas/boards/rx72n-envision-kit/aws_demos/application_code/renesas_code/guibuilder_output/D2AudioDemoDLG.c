/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH                         *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 6.14                          *
*        Compiled Jun  2 2020, 15:49:31                              *
*        (c) 2019 Segger Microcontroller GmbH                        *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
#include <stdio.h>
#include <string.h>
#include "platform.h"
#include "r_flash_rx_if.h"
#include "r_tfat_lib.h"
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0   (GUI_ID_USER + 0x00)
#define ID_TEXT_0   (GUI_ID_USER + 0x01)
#define ID_BUTTON_0   (GUI_ID_USER + 0x02)
#define ID_LISTBOX_0   (GUI_ID_USER + 0x03)
#define ID_TEXT_1   (GUI_ID_USER + 0x04)
#define ID_MULTIEDIT_0   (GUI_ID_USER + 0x05)
#define ID_TEXT_2   (GUI_ID_USER + 0x06)
#define ID_TEXT_3   (GUI_ID_USER + 0x07)
#define ID_BUTTON_1   (GUI_ID_USER + 0x08)
#define ID_BUTTON_2   (GUI_ID_USER + 0x09)


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
char selected_file_name2[256+1];
static const char reboot_string2[] = "Reboot";
char buff2[350];
char text2[13];
TEXT_Handle hText2;
FILINFO filinfo2;
DIR dir2;
FIL g_file = {0};
uint8_t g_riff_data[44];
uint32_t g_chunk_size;
uint16_t g_bit_per_sample = 0;

// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "D2AudioDemo", ID_WINDOW_0, 0, 21, 480, 228, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "D2 Audio Record and Play", ID_TEXT_0, 6, 0, 158, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "STOP", ID_BUTTON_0, 396, 201, 80, 20, 0, 0x0, 0 },
  { LISTBOX_CreateIndirect, "Listbox", ID_LISTBOX_0, 7, 33, 129, 162, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "File List", ID_TEXT_1, 7, 18, 105, 20, 0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect, "Multiedit", ID_MULTIEDIT_0, 142, 33, 334, 162, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "File Size", ID_TEXT_2, 8, 204, 51, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "", ID_TEXT_3, 52, 204, 84, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "PLAY", ID_BUTTON_1, 312, 201, 80, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "RECORD", ID_BUTTON_2, 228, 201, 80, 20, 0, 0x0, 0 },
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
extern void bank_swap_with_software_reset(void);
extern void software_reset(void);
extern void firmware_update_request(char *string);
extern bool is_firmupdating(void);
extern void SetSwbankchangeRebootBotton(void);
extern void firmware_update_request(char *string);
extern void d2audio_play_start(void);
extern void d2audio_play_stop(void);
extern void d2audio_record_start(void);
extern void d2audio_record_stop(void);

void d2audio_log_string(char *pstring);
void d2audio_file_search(void);
int32_t d2audio_get_riff_data(char *filename);
void d2audio_get_chunck_size(uint32_t *get_chunk_size);
void d2audio_get_bit_per_sample(uint16_t *bit_per_sample);
int32_t d2audio_read_data(uint8_t *read_buff, uint16_t request_read_size, uint16_t *read_size);

WM_HWIN hWinD2AudioRecordAndPlayWindow;
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
  WM_HWIN hWin;
  int select_id;
  int ret_code;
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'D2AudioDemo'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
    //
    // Initialization of 'Listbox'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTBOX_0);
    LISTBOX_AddString(hItem, "Item 0");
    LISTBOX_AddString(hItem, "Item 1");
    LISTBOX_AddString(hItem, "Item 2");
    // USER START (Optionally insert additional code for further widget initialization)
    hText2 = TEXT_CreateEx(52, 204, 84, 20, pMsg->hWin, WM_CF_SHOW, TEXT_CF_LEFT, ID_TEXT_3, text2);
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'STOP'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        d2audio_play_stop();
        d2audio_record_stop();
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_LISTBOX_0: // Notifications sent by 'Listbox'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
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
    case ID_BUTTON_1: // Notifications sent by 'PLAY'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
          hWin = hWinD2AudioRecordAndPlayWindow;
          hItem = WM_GetDialogItem(hWin, ID_LISTBOX_0);
          select_id = LISTBOX_GetSel(hItem);
          if(select_id == -1)
          {
        	  d2audio_log_string("No file selected.\r\n");
              return;
          }
          LISTBOX_GetItemText(hItem, select_id, selected_file_name2, 256);
          hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
          ret_code = d2audio_get_riff_data(selected_file_name2);
          if (ret_code != 0)
          {
              d2audio_log_string("D2 Audio wav File Format Error!!\r\n");
          }
          else
          {
              d2audio_log_string("D2 Audio wav File Play Start\r\n");
              d2audio_play_start();
          }
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'RECORD'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        d2audio_record_start();
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
*       CreateD2AudioDemo
*/
WM_HWIN CreateD2AudioDemo(void);
WM_HWIN CreateD2AudioDemo(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  hWinD2AudioRecordAndPlayWindow = hWin;
  return hWin;
}

// USER START (Optionally insert additional public code)
void d2audio_log_string(char *pstring)
{
    WM_HWIN hItem;
    WM_HWIN hWin;

    hWin = hWinD2AudioRecordAndPlayWindow;
    hItem = WM_GetDialogItem(hWin, ID_MULTIEDIT_0);
    MULTIEDIT_AddText(hItem, pstring);
    MULTIEDIT_SetCursorOffset(hItem, MULTIEDIT_GetTextSize(hItem));
}

void d2audio_list_add(char *pstring)
{
    WM_HWIN hItem;
    WM_HWIN hWin;

    hWin = hWinD2AudioRecordAndPlayWindow;
    hItem = WM_GetDialogItem(hWin, ID_LISTBOX_0);

    LISTBOX_AddString(hItem, pstring);
}

void d2audio_list_clear(void)
{
    WM_HWIN hItem;
    WM_HWIN hWin;
    U32 list_num;
    I32 i;

    hWin = hWinD2AudioRecordAndPlayWindow;
    hItem = WM_GetDialogItem(hWin, ID_LISTBOX_0);

    list_num = LISTBOX_GetNumItems(hItem);
    for(i = list_num-1;i>=0;i--)
    {
        LISTBOX_DeleteItem(hItem, i);
    }
}

void d2audio_file_search(void)
{
    FRESULT tfat_ret;
    int32_t i;

    d2audio_list_clear();
    tfat_ret = R_tfat_f_opendir (&dir2,"0:");
    if(tfat_ret == TFAT_FR_OK)
    {
        while(1)
        {
            tfat_ret = R_tfat_f_readdir (&dir2,  &filinfo2 );
            if(tfat_ret == TFAT_FR_OK)
            {
                if(filinfo2.fname[0] == '\0')
                {
                    break;
                }
                if(TFAT_AM_DIR == (filinfo2.fattrib & TFAT_AM_DIR) )
                {
                    continue;
                }
                for(i= 0;i<sizeof(filinfo2.fname);i++)
                {
                    if('A' <=  filinfo2.fname[i] && filinfo2.fname[i] <= 'Z' )
                    {
                    	filinfo2.fname[i] += 0x20;
                    }
                }
                if(0 != strstr(filinfo2.fname,".wav"))
                {
                	d2audio_list_add(filinfo2.fname);
                }
            }
            else
            {
                break;
            }
            vTaskDelay(1);
        }
    }
}

int32_t d2audio_get_riff_data(char *filename)
{
    FRESULT ret = TFAT_FR_OK;
    int32_t ret_code;
    uint16_t read_size = 0;
    static uint32_t previous_file_position;
	uint8_t chunk_id[] = "data";
	uint8_t i;
	uint8_t red_data;
	uint8_t get_chunk_size = 0;
	uint32_t w_chunk = 0;
	uint16_t work = 0;

    ret = R_tfat_f_open(&g_file, filename, TFAT_FA_READ | TFAT_FA_OPEN_EXISTING);
    if (TFAT_RES_OK == ret)
    {
        memset(g_riff_data, 0x00, sizeof(g_riff_data));
        ret_code = d2audio_read_data(g_riff_data, sizeof(g_riff_data), &read_size);
        /* get bit per Sample */
        work =  g_riff_data[34];
        work = work & 0x00FF;
        g_bit_per_sample = work;
        work =  g_riff_data[35];
        work = work & 0x00FF;
        work = work << 8;
        work = work & 0xFF00;
        g_bit_per_sample = work | g_bit_per_sample;

    	for (i=0; i<4; i++)
    	{
    		red_data = *(g_riff_data + 36 + i);
    		if (red_data != chunk_id[i])
    		{
    			ret_code = -1;
    			break;
    		}
    	}
		get_chunk_size = *(g_riff_data + 40);
		w_chunk = (w_chunk | get_chunk_size);
		w_chunk = w_chunk & 0x000000FF;
		g_chunk_size = g_chunk_size | w_chunk;

		get_chunk_size = *(g_riff_data + 41);
		w_chunk = 0;
		w_chunk = (w_chunk | get_chunk_size);
		w_chunk = w_chunk << 8;
		w_chunk = w_chunk & 0x0000FF00;
		g_chunk_size = g_chunk_size | w_chunk;

		get_chunk_size = *(g_riff_data + 42);
		w_chunk = 0;
		w_chunk = (w_chunk | get_chunk_size);
		w_chunk = w_chunk << 16;
		w_chunk = w_chunk & 0x00FF0000;
		g_chunk_size = g_chunk_size | w_chunk;

		get_chunk_size = *(g_riff_data + 43);
		w_chunk = 0;
		w_chunk = (w_chunk | get_chunk_size);
		w_chunk = w_chunk << 24;
		w_chunk = w_chunk & 0xFF000000;
		g_chunk_size = g_chunk_size | w_chunk;
    }
    else
    {
    	ret_code = -1;
    }
    return ret_code;
}

int32_t d2audio_read_data(uint8_t *read_buff, uint16_t request_read_size, uint16_t *read_size)
{
    FRESULT ret = TFAT_FR_OK;
	int32_t ret_code = 0;

    ret = R_tfat_f_read(&g_file, read_buff, request_read_size, read_size);
    if (ret != TFAT_RES_OK || *read_size < request_read_size)
    {
        R_tfat_f_close(&g_file);
        ret_code = -1;
    }
	return ret_code;
}

void d2audio_get_chunck_size(uint32_t *get_chunk_size)
{
	*get_chunk_size = g_chunk_size;
}

void d2audio_get_bit_per_sample(uint16_t *g_pcm_read_size)
{
	*g_pcm_read_size = g_bit_per_sample;
}
// USER END

/*************************** End of file ****************************/
