
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

/* mbedtls */
#include "mbedtls/sha256.h"	/* this will change to Renesas SHA256 library to improve more easy to integrate by using FIT mechanism. */

/* C runtime includes. */
#include <stdio.h>
#include <string.h>

/* Renesas RX platform includes */
#include "platform.h"
#include "r_flash_rx_if.h"
#include "r_simple_filesystem_on_dataflash_if.h"

/* define the word "Simple Filesystem on Dataflash" as "SFD". */
#define SFD_DATA_STATUS_EMPTY 0
#define SFD_DATA_STATUS_REGISTERED 1
#define SFD_DATA_STATUS_HIT 2

#define SFD_HANDLES_LABEL_MAX_LENGTH 40
#define SFD_OBJECT_HANDLES_NUM 5
#define SFD_SHA256_LENGTH 32

#define MAX_CHECK_DATAFLASH_AREA_RETRY_COUNT 3

typedef struct _sfd_descriptor
{
    uint8_t label[SFD_HANDLES_LABEL_MAX_LENGTH];
    uint32_t label_length;
    uint32_t local_storage_index;
    uint32_t data_length;
    uint32_t status;
    SFD_HANDLE xHandle;
} SFD_DESCRIPTOR;

#if defined (BSP_MCU_RX72N)
#define SFD_CONTROL_BLOCK_INITIAL_DATA \
    {\
        /* uint8_t local_storage[((FLASH_DF_BLOCK_SIZE * FLASH_NUM_BLOCKS_DF) / 4) - (sizeof(SFD_DATA) * SFD_OBJECT_HANDLES_NUM) - SFD_SHA256_LENGTH]; */\
        {0x00},\
        /* SFD_DATA sfd_data[SFD_OBJECT_HANDLES_NUM]; */\
        {0x00},\
    },\
    /* uint8_t hash_sha256[SFD_SHA256_LENGTH]; */\
    {0x5f, 0x51, 0xd9, 0xe5, 0x7c, 0x05, 0x1c, 0xc5, 0x16, 0x38, 0x83, 0x46, 0xd8, 0x60, 0x97, 0x6b, 0xff, 0x35, 0x31, 0xda, 0x95, 0x61, 0xf6, 0x57, 0x0c, 0x19, 0xb2, 0xca, 0x2d, 0xa4, 0x8e, 0xab},
#else
#error "Simple Filesystem on Dataflash (SFD) does not support your MCU"
#endif

#define SFD_DEBUG_PRINT( X )  /* printf( X ) */

#define SFD_DATA_FLASH_UPDATE_STATE_INITIALIZE 0
#define SFD_DATA_FLASH_UPDATE_STATE_ERASE 1
#define SFD_DATA_FLASH_UPDATE_STATE_ERASE_WAIT_COMPLETE 2
#define SFD_DATA_FLASH_UPDATE_STATE_WRITE 3
#define SFD_DATA_FLASH_UPDATE_STATE_WRITE_WAIT_COMPLETE 4
#define SFD_DATA_FLASH_UPDATE_STATE_FINALIZE 5
#define SFD_DATA_FLASH_UPDATE_STATE_FINALIZE_COMPLETED 6
#define SFD_DATA_FLASH_UPDATE_STATE_ERROR 103

typedef struct _pkcs_storage_control_block_sub
{
    uint8_t local_storage[SFD_LOCAL_STORAGE_SIZE - (sizeof(SFD_DESCRIPTOR) * SFD_OBJECT_HANDLES_NUM) - SFD_SHA256_LENGTH];
    SFD_DESCRIPTOR sfd_data[SFD_OBJECT_HANDLES_NUM];
} SFD_STORAGE_CONTROL_BLOCK_SUB;

typedef struct _SFD_CONTROL_BLOCK
{
    SFD_STORAGE_CONTROL_BLOCK_SUB data;
    uint8_t hash_sha256[SFD_SHA256_LENGTH];
} SFD_CONTROL_BLOCK;

static SFD_CONTROL_BLOCK sfd_control_block_data_image;        /* RX72N case: 1KB  */

R_BSP_ATTRIB_SECTION_CHANGE(C, _SYSTEM_CONFIG , 1)
static const SFD_CONTROL_BLOCK sfd_control_block_data = {SFD_CONTROL_BLOCK_INITIAL_DATA};
R_BSP_ATTRIB_SECTION_CHANGE_END

R_BSP_ATTRIB_SECTION_CHANGE(C, _SYSTEM_CONFIG_MIRROR , 1)
static const SFD_CONTROL_BLOCK sfd_control_block_data_mirror = {SFD_CONTROL_BLOCK_INITIAL_DATA};
R_BSP_ATTRIB_SECTION_CHANGE_END

static void update_dataflash_data_from_image(void);
static void update_dataflash_data_mirror_from_image(void);
static void check_dataflash_area(uint32_t retry_counter);

static void data_flash_update_status_initialize(void);
static void update_data_flash_callback_function(void *event);

typedef struct _update_data_flash_control_block {
	uint32_t status;
}UPDATA_DATA_FLASH_CONTROL_BLOCK;

/******************************************************************************
 Private global variables
 ******************************************************************************/
static UPDATA_DATA_FLASH_CONTROL_BLOCK update_data_flash_control_block;

sfd_err_t R_SFD_Open(void)
{
	sfd_err_t sfd_err = SFD_FATAL_ERROR;

	if(FLASH_CFG_DATA_FLASH_BGO == 1)
	{
		R_FLASH_Open();
		/* check the hash */
		check_dataflash_area(0);
		/* copy data from storage to ram */
		memcpy(&sfd_control_block_data_image, (void *)&sfd_control_block_data, sizeof(sfd_control_block_data_image));
		R_FLASH_Close();
		sfd_err = SFD_SUCCESS;
	}
	else
	{
		SFD_DEBUG_PRINT("Please set FLASH_CFG_DATA_FLASH_BGO to 1 in r_flash_config_rx.h.\r\n");
		sfd_err = SFD_FATAL_ERROR;
	}
    return sfd_err;
}

SFD_HANDLE R_SFD_SaveObject(uint8_t *label, uint32_t label_length, uint8_t *data, uint32_t data_length)
{
    uint32_t i;
    uint8_t hash_sha256[SFD_SHA256_LENGTH];
    SFD_HANDLE xHandle = SFD_HANDLE_INVALID;
    mbedtls_sha256_context ctx;

    mbedtls_sha256_init(&ctx);
    R_FLASH_Open();

    /* check the hash */
    check_dataflash_area(0);

    /* copy data from storage to ram */
    memcpy(&sfd_control_block_data_image, (void *)&sfd_control_block_data, sizeof(sfd_control_block_data_image));

    /* search specified label value from object_handle_dictionary */
    for (i = 0; i < SFD_OBJECT_HANDLES_NUM; i++)
    {
        if (!strncmp((char * )&sfd_control_block_data_image.data.sfd_data[i].label, (char * )label, label_length))
        {
            xHandle = i;
        }
    }

    /* pre-calculate -> total stored data size : pre-calculate phase */
    uint32_t total_stored_data_size = 0;

    /* specified label found: overwrite specified data into registered label.  */
    if (xHandle != SFD_HANDLE_INVALID)
    {
        for (int i = 0; i < SFD_OBJECT_HANDLES_NUM; i++)
        {
            if (sfd_control_block_data_image.data.sfd_data[i].status == SFD_DATA_STATUS_REGISTERED)
            {
                total_stored_data_size += sfd_control_block_data_image.data.sfd_data[i].data_length;
            }
        }

        /* remove current xHandle from sfd_data */
        if (sfd_control_block_data_image.data.sfd_data[xHandle].status == SFD_DATA_STATUS_REGISTERED)
        {
            uint32_t move_target_xHandle = 0, move_target_index = 0;

            uint32_t delete_target_index = sfd_control_block_data_image.data.sfd_data[xHandle].local_storage_index;
            uint32_t delete_target_data_size = sfd_control_block_data_image.data.sfd_data[xHandle].data_length;

            /* Search move target index and handle  */
            for (int i = 0; i < SFD_OBJECT_HANDLES_NUM; i++)
            {
                if ((sfd_control_block_data_image.data.sfd_data[i].status == SFD_DATA_STATUS_REGISTERED)
                        && (sfd_control_block_data_image.data.sfd_data[i].local_storage_index == (delete_target_index + delete_target_data_size)))
                {
                    move_target_xHandle = i;
                    move_target_index = sfd_control_block_data_image.data.sfd_data[i].local_storage_index;
                    break;
                }
            }

            if (move_target_xHandle != 0)
            {
                /* Move target index to delete target index */
                memmove(
                    (void * )&sfd_control_block_data_image.data.local_storage[delete_target_index],
                    (void * )&sfd_control_block_data_image.data.local_storage[move_target_index],
                    (size_t )total_stored_data_size - move_target_index);

                /* Fix index of all moved data  */
                for (int i = 0; i < SFD_OBJECT_HANDLES_NUM; i++)
                {
                    if (sfd_control_block_data_image.data.sfd_data[i].local_storage_index > delete_target_index)
                    {
                        sfd_control_block_data_image.data.sfd_data[i].local_storage_index -= delete_target_data_size;
                    }
                }
            }

            /* Recalculate the end of data storage  */
            total_stored_data_size -= delete_target_data_size;

            sfd_control_block_data_image.data.sfd_data[xHandle].local_storage_index = 0;
            sfd_control_block_data_image.data.sfd_data[xHandle].data_length = 0;

        }
    }
	/* specified label not found: add specified data as new label. */
	else
	{
		/* search target blank entry */
        for (int i = 0; i < SFD_OBJECT_HANDLES_NUM; i++)
        {
            if (sfd_control_block_data_image.data.sfd_data[i].status == SFD_DATA_STATUS_EMPTY)
            {
                xHandle = i;
            	break;
            }
            else if (sfd_control_block_data_image.data.sfd_data[i].status == SFD_DATA_STATUS_REGISTERED)
            {
                total_stored_data_size += sfd_control_block_data_image.data.sfd_data[i].data_length;
            }
        }
	}

	strncpy((char * )sfd_control_block_data_image.data.sfd_data[xHandle].label, (char * )label, SFD_HANDLES_LABEL_MAX_LENGTH);
	sfd_control_block_data_image.data.sfd_data[xHandle].label_length = label_length;
	sfd_control_block_data_image.data.sfd_data[xHandle].local_storage_index = total_stored_data_size;
	sfd_control_block_data_image.data.sfd_data[xHandle].data_length = data_length;
	sfd_control_block_data_image.data.sfd_data[xHandle].status = SFD_DATA_STATUS_REGISTERED;
	sfd_control_block_data_image.data.sfd_data[xHandle].xHandle = xHandle;
	memcpy(&sfd_control_block_data_image.data.local_storage[total_stored_data_size], data, data_length);

	/* update the hash */
    mbedtls_sha256_starts_ret(&ctx, 0); /* 0 means SHA256 context */
    mbedtls_sha256_update_ret(&ctx, (unsigned char *)&sfd_control_block_data_image.data, sizeof(sfd_control_block_data.data));
    mbedtls_sha256_finish_ret(&ctx, hash_sha256);
	memcpy(sfd_control_block_data_image.hash_sha256, hash_sha256, sizeof(hash_sha256));

	/* update data from ram to storage */
	data_flash_update_status_initialize();
	while ( update_data_flash_control_block.status < SFD_DATA_FLASH_UPDATE_STATE_FINALIZE_COMPLETED )
	{
		update_dataflash_data_from_image();
	}
	if (update_data_flash_control_block.status == SFD_DATA_FLASH_UPDATE_STATE_ERROR)
	{
		SFD_DEBUG_PRINT(("ERROR: Update data flash data from image\r\n"));
		while(1);
	}
	data_flash_update_status_initialize();
	while ( update_data_flash_control_block.status < SFD_DATA_FLASH_UPDATE_STATE_FINALIZE_COMPLETED )
	{
		update_dataflash_data_mirror_from_image();
	}
	if (update_data_flash_control_block.status == SFD_DATA_FLASH_UPDATE_STATE_ERROR)
	{
		SFD_DEBUG_PRINT(("ERROR: Update data flash data mirror from image\r\n"));
		while(1);
	}
    R_FLASH_Close();

    return xHandle;

}

sfd_err_t R_SFD_Close(void)
{
	/* nothing to do */
    return SFD_SUCCESS;
}

SFD_HANDLE R_SFD_FindObject( uint8_t *label, uint8_t label_length )
{
	SFD_HANDLE xHandle = SFD_HANDLE_INVALID;
    int i;

    for(i = 0; i < SFD_OBJECT_HANDLES_NUM; i++)
    {
        if(!strncmp((char *)&sfd_control_block_data_image.data.sfd_data[i].label, (char *)label, label_length))
        {
            if(sfd_control_block_data_image.data.sfd_data[i].status == SFD_DATA_STATUS_REGISTERED)
            {
                xHandle = i;
            }
        }
    }
    return xHandle;
}

sfd_err_t R_SFD_GetObjectValue( SFD_HANDLE xHandle,
                                 uint8_t **data,
                                 uint32_t *data_size)
{
	sfd_err_t xReturn = SFD_FATAL_ERROR;

    if (xHandle != SFD_HANDLE_INVALID)
    {
        *data = &sfd_control_block_data_image.data.local_storage[sfd_control_block_data_image.data.sfd_data[xHandle].local_storage_index];
        *data_size = sfd_control_block_data_image.data.sfd_data[xHandle].data_length;
    }
    return xReturn;
}

static void update_dataflash_data_from_image(void)
{
	flash_interrupt_config_t cb_func_info;
    static uint32_t required_dataflash_block_num;
    flash_err_t flash_error_code = FLASH_SUCCESS;

    switch(update_data_flash_control_block.status)
    {
        case SFD_DATA_FLASH_UPDATE_STATE_INITIALIZE: /* initialize */
        	cb_func_info.pcallback = update_data_flash_callback_function;
        	cb_func_info.int_priority = 15;
        	R_FLASH_Control(FLASH_CMD_SET_BGO_CALLBACK, (void *)&cb_func_info);
            required_dataflash_block_num = sizeof(SFD_CONTROL_BLOCK) / FLASH_DF_BLOCK_SIZE;
            if(sizeof(SFD_CONTROL_BLOCK) % FLASH_DF_BLOCK_SIZE)
            {
                required_dataflash_block_num++;
            }
            update_data_flash_control_block.status = SFD_DATA_FLASH_UPDATE_STATE_ERASE;
            break;
        case SFD_DATA_FLASH_UPDATE_STATE_ERASE:
            SFD_DEBUG_PRINT(("erase dataflash(main)..."));
            R_BSP_InterruptsDisable();
            flash_error_code = R_FLASH_Erase((flash_block_address_t)&sfd_control_block_data, required_dataflash_block_num);
            R_BSP_InterruptsEnable();
            if(FLASH_SUCCESS == flash_error_code)
            {
                 SFD_DEBUG_PRINT(("OK\r\n"));
            }
            else
            {
                SFD_DEBUG_PRINT(("NG\r\n"));
                SFD_DEBUG_PRINT(("R_FLASH_Erase() returns error code = %d.\r\n", flash_error_code));
            }
            update_data_flash_control_block.status = SFD_DATA_FLASH_UPDATE_STATE_ERASE_WAIT_COMPLETE;
            break;
        case SFD_DATA_FLASH_UPDATE_STATE_ERASE_WAIT_COMPLETE:
            /* this state will be changed by callback routine */
            break;
        case SFD_DATA_FLASH_UPDATE_STATE_WRITE:
            SFD_DEBUG_PRINT(("write dataflash(main)..."));
            R_BSP_InterruptsDisable();
            flash_error_code = R_FLASH_Write((flash_block_address_t)&sfd_control_block_data_image, (flash_block_address_t)&sfd_control_block_data, FLASH_DF_BLOCK_SIZE * required_dataflash_block_num);
            R_BSP_InterruptsEnable();
            if(FLASH_SUCCESS == flash_error_code)
            {
                SFD_DEBUG_PRINT(("OK\r\n"));
            }
            else
            {
            	SFD_DEBUG_PRINT(("NG\r\n"));
            	SFD_DEBUG_PRINT(("R_FLASH_Write() returns error code = %d.\r\n", flash_error_code));
            	return;
            }
            update_data_flash_control_block.status = SFD_DATA_FLASH_UPDATE_STATE_WRITE_WAIT_COMPLETE;
            break;
        case SFD_DATA_FLASH_UPDATE_STATE_WRITE_WAIT_COMPLETE:
            /* this state will be changed by callback routine */
            break;
        case SFD_DATA_FLASH_UPDATE_STATE_FINALIZE: /* finalize */
            update_data_flash_control_block.status = SFD_DATA_FLASH_UPDATE_STATE_FINALIZE_COMPLETED;
            break;
        default:
            break;
    }
    return;
}

static void update_dataflash_data_mirror_from_image(void)
{
	flash_interrupt_config_t cb_func_info;
    static uint32_t required_dataflash_block_num;
    flash_err_t flash_error_code = FLASH_SUCCESS;

    switch(update_data_flash_control_block.status)
    {
    	case SFD_DATA_FLASH_UPDATE_STATE_INITIALIZE: /* initialize */
    		cb_func_info.pcallback = update_data_flash_callback_function;
    		cb_func_info.int_priority = 15;
    		R_FLASH_Control(FLASH_CMD_SET_BGO_CALLBACK, (void *)&cb_func_info);
            required_dataflash_block_num = sizeof(SFD_CONTROL_BLOCK) / FLASH_DF_BLOCK_SIZE;
            if(sizeof(SFD_CONTROL_BLOCK) % FLASH_DF_BLOCK_SIZE)
            {
                required_dataflash_block_num++;
            }

            update_data_flash_control_block.status = SFD_DATA_FLASH_UPDATE_STATE_ERASE;
            break;
        case SFD_DATA_FLASH_UPDATE_STATE_ERASE:
            SFD_DEBUG_PRINT(("erase dataflash(mirror)..."));
            R_BSP_InterruptsDisable();
            flash_error_code = R_FLASH_Erase((flash_block_address_t)&sfd_control_block_data_mirror, required_dataflash_block_num);
            R_BSP_InterruptsEnable();
            if(FLASH_SUCCESS == flash_error_code)
            {
                SFD_DEBUG_PRINT(("OK\r\n"));
            }
            else
            {
                SFD_DEBUG_PRINT(("NG\r\n"));
                SFD_DEBUG_PRINT(("R_FLASH_Erase() returns error code = %d.\r\n", flash_error_code));
                return;
            }

            update_data_flash_control_block.status = SFD_DATA_FLASH_UPDATE_STATE_ERASE_WAIT_COMPLETE;
            break;
        case SFD_DATA_FLASH_UPDATE_STATE_ERASE_WAIT_COMPLETE:
            /* this state will be changed by callback routine */
            break;
        case SFD_DATA_FLASH_UPDATE_STATE_WRITE:
            SFD_DEBUG_PRINT(("write dataflash(mirror)..."));
            R_BSP_InterruptsDisable();
            flash_error_code = R_FLASH_Write((flash_block_address_t)&sfd_control_block_data_image, (flash_block_address_t)&sfd_control_block_data_mirror, FLASH_DF_BLOCK_SIZE * required_dataflash_block_num);
            R_BSP_InterruptsEnable();
            if(FLASH_SUCCESS == flash_error_code)
            {
                SFD_DEBUG_PRINT(("OK\r\n"));
            }
            else
            {
                SFD_DEBUG_PRINT(("NG\r\n"));
                SFD_DEBUG_PRINT(("R_FLASH_Write() returns error code = %d.\r\n", flash_error_code));
                return;
            }
            update_data_flash_control_block.status = SFD_DATA_FLASH_UPDATE_STATE_WRITE_WAIT_COMPLETE;
            break;
        case SFD_DATA_FLASH_UPDATE_STATE_WRITE_WAIT_COMPLETE:
            /* this state will be changed by callback routine */
            break;
        case SFD_DATA_FLASH_UPDATE_STATE_FINALIZE: /* finalize */

            if(!memcmp(&sfd_control_block_data, &sfd_control_block_data_mirror, sizeof(SFD_CONTROL_BLOCK)))
            {
                SFD_DEBUG_PRINT(("data flash setting OK.\r\n"));
            }
            else
            {
                SFD_DEBUG_PRINT(("data flash setting NG.\r\n"));
                return;
            }
            update_data_flash_control_block.status = SFD_DATA_FLASH_UPDATE_STATE_FINALIZE_COMPLETED;
            break;
        default:
    	    break;
    }
    return;
}

static void check_dataflash_area(uint32_t retry_counter)
{
    uint8_t hash_sha256[SFD_SHA256_LENGTH];
    mbedtls_sha256_context ctx;

    mbedtls_sha256_init(&ctx);

    if(retry_counter)
    {
        SFD_DEBUG_PRINT(("recover retry count = %d.\r\n", retry_counter));
        if(retry_counter == MAX_CHECK_DATAFLASH_AREA_RETRY_COUNT)
        {
            SFD_DEBUG_PRINT(("retry over the limit.\r\n"));
            while(1);
        }
    }
    SFD_DEBUG_PRINT(("data flash(main) hash check..."));
    mbedtls_sha256_starts_ret(&ctx, 0); /* 0 means SHA256 context */
    mbedtls_sha256_update_ret(&ctx, (unsigned char *)&sfd_control_block_data.data, sizeof(sfd_control_block_data.data));
    mbedtls_sha256_finish_ret(&ctx, hash_sha256);
    if(!memcmp(sfd_control_block_data.hash_sha256, hash_sha256, sizeof(hash_sha256)))
    {
        SFD_DEBUG_PRINT(("OK\r\n"));
        SFD_DEBUG_PRINT(("data flash(mirror) hash check..."));
        mbedtls_sha256_starts_ret(&ctx, 0); /* 0 means SHA256 context */
        mbedtls_sha256_update_ret(&ctx, (unsigned char *)&sfd_control_block_data_mirror.data, sizeof(sfd_control_block_data_mirror.data));
        mbedtls_sha256_finish_ret(&ctx, hash_sha256);
        if(!memcmp(sfd_control_block_data_mirror.hash_sha256, hash_sha256, sizeof(hash_sha256)))
        {
            SFD_DEBUG_PRINT(("OK\r\n"));
        }
        else
        {
            SFD_DEBUG_PRINT(("NG\r\n"));
            SFD_DEBUG_PRINT(("recover mirror from main.\r\n"));
            memcpy(&sfd_control_block_data_image, (void *)&sfd_control_block_data, sizeof(sfd_control_block_data));

            data_flash_update_status_initialize();
            while ( update_data_flash_control_block.status < SFD_DATA_FLASH_UPDATE_STATE_FINALIZE_COMPLETED )
            {
                update_dataflash_data_mirror_from_image();
            }
            if (update_data_flash_control_block.status == SFD_DATA_FLASH_UPDATE_STATE_ERROR)
            {
                SFD_DEBUG_PRINT(("ERROR: Update data flash data mirror from image\r\n"));
                while(1);
            }
            check_dataflash_area(retry_counter + 1);
        }
    }
    else
    {
        SFD_DEBUG_PRINT(("NG\r\n"));
        SFD_DEBUG_PRINT(("data flash(mirror) hash check..."));
        mbedtls_sha256_starts_ret(&ctx, 0); /* 0 means SHA256 context */
        mbedtls_sha256_update_ret(&ctx, (unsigned char *)&sfd_control_block_data_mirror.data, sizeof(sfd_control_block_data_mirror.data));
        mbedtls_sha256_finish_ret(&ctx, hash_sha256);
        if(!memcmp(sfd_control_block_data_mirror.hash_sha256, hash_sha256, sizeof(hash_sha256)))
        {
            SFD_DEBUG_PRINT(("OK\r\n"));
            SFD_DEBUG_PRINT(("recover main from mirror.\r\n"));
            memcpy(&sfd_control_block_data_image, (void *)&sfd_control_block_data_mirror, sizeof(sfd_control_block_data_mirror));
            data_flash_update_status_initialize();
            while ( update_data_flash_control_block.status < SFD_DATA_FLASH_UPDATE_STATE_FINALIZE_COMPLETED )
            {
                update_dataflash_data_from_image();
            }
            if (update_data_flash_control_block.status == SFD_DATA_FLASH_UPDATE_STATE_ERROR)
            {
                SFD_DEBUG_PRINT(("ERROR: Update data flash data from image\r\n"));
                while(1);
            }
            check_dataflash_area(retry_counter + 1);
        }
        else
        {
            SFD_DEBUG_PRINT(("NG\r\n"));
            while(1)
            {
            	R_BSP_SoftwareDelay(3, BSP_DELAY_SECS);
                SFD_DEBUG_PRINT(("------------------------------------------------\r\n"));
                SFD_DEBUG_PRINT(("Data flash is completely broken.\r\n"));
                SFD_DEBUG_PRINT(("Please erase all code flash.\r\n"));
                SFD_DEBUG_PRINT(("And, write initial firmware using debugger/rom writer.\r\n"));
                SFD_DEBUG_PRINT(("------------------------------------------------\r\n"));
            }
        }
    }
}

static void data_flash_update_status_initialize(void)
{
	update_data_flash_control_block.status = SFD_DATA_FLASH_UPDATE_STATE_INITIALIZE;
}

void update_data_flash_callback_function(void *event)
{
	uint32_t event_code;
	event_code = *((uint32_t*)event);

	switch(event_code)
	{
		case FLASH_INT_EVENT_ERASE_COMPLETE:
			if(SFD_DATA_FLASH_UPDATE_STATE_ERASE_WAIT_COMPLETE == update_data_flash_control_block.status)
			{
				update_data_flash_control_block.status = SFD_DATA_FLASH_UPDATE_STATE_WRITE;
			}
			else
			{
				update_data_flash_control_block.status = SFD_DATA_FLASH_UPDATE_STATE_ERROR;
			}
			break;
		case FLASH_INT_EVENT_WRITE_COMPLETE:
			if(SFD_DATA_FLASH_UPDATE_STATE_WRITE_WAIT_COMPLETE == update_data_flash_control_block.status)
			{

				update_data_flash_control_block.status = SFD_DATA_FLASH_UPDATE_STATE_FINALIZE;
			}
			else
			{
				update_data_flash_control_block.status = SFD_DATA_FLASH_UPDATE_STATE_ERROR;
			}
			break;
		default:
			break;
	}
}
