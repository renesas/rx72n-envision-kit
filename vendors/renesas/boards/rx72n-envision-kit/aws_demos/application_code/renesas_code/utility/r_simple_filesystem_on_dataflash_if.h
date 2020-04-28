
/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/* Return error codes */
typedef enum sfd_err_t
{
    SFD_SUCCESS = 0,
    SFD_FATAL_ERROR
} sfd_err_t;

typedef uint32_t SFD_HANDLE;

#define SFD_CONTROL_BLOCK_SIZE 2048

#define SFD_HANDLE_INVALID 0xffffffff

/******************************************************************************
 Interface funcsions
 ******************************************************************************/
sfd_err_t R_SFD_Open(void);
SFD_HANDLE R_SFD_SaveObject(uint8_t *label, uint32_t label_length, uint8_t *data, uint32_t data_length);
SFD_HANDLE R_SFD_FindObject( uint8_t *label, uint8_t label_length );
sfd_err_t R_SFD_GetObjectValue(SFD_HANDLE xHandle, uint8_t **data, uint32_t *data_size);
uint32_t R_SFD_ReadPysicalSize(void);
uint32_t R_SFD_ReadAllocatedStorageSize(void);
uint32_t R_SFD_ReadFreeSize(void);
sfd_err_t R_SFD_Close(void);
