/**********************************************************************************************************************
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
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : firm_update.c
 * Description  :
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 31.01.2020 1.00 First Release
 *********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "tinycrypt/sha256.h"
#include "tinycrypt/ecc.h"
#include "tinycrypt/ecc_dsa.h"
#include "platform.h"
#include "r_tfat_lib.h"
#include "r_flash_rx_if.h"
#include "r_cryptogram.h"
#include "code_signer_public_key.h"

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
#define LIFECYCLE_STATE_BLANK                   (0xff)
#define LIFECYCLE_STATE_TESTING                 (0xfe)
#define LIFECYCLE_STATE_INITIAL_FIRM_INSTALLED  (0xfc)
#define LIFECYCLE_STATE_VALID                   (0xf8)
#define LIFECYCLE_STATE_INVALID                 (0xf0)

#define FIRMWARE_UPDATE_STATE_INITIALIZE 0
#define FIRMWARE_UPDATE_STATE_ERASE 1
#define FIRMWARE_UPDATE_STATE_ERASE_WAIT_COMPLETE 2
#define FIRMWARE_UPDATE_STATE_READ_WAIT_COMPLETE 3
#define FIRMWARE_UPDATE_STATE_READ_START 4
#define FIRMWARE_UPDATE_STATE_WRITE_WAIT_COMPLETE 5
#define FIRMWARE_UPDATE_STATE_FINALIZE 6
#define FIRMWARE_UPDATE_STATE_FINALIZE_WAIT_ERASE_DATA_FLASH 7
#define FIRMWARE_UPDATE_STATE_FINALIZE_WAIT_WRITE_DATA_FLASH 8
#define FIRMWARE_UPDATE_STATE_FINALIZE_COMPLETED 9

#define FIRMWARE_UPDATE_STATE_CAN_SWAP_BANK 100
#define FIRMWARE_UPDATE_STATE_WAIT_START 101
#define FIRMWARE_UPDATE_STATE_COMPLETED 102
#define FIRMWARE_UPDATE_STATE_ERROR 103

/*------------------------------------------ firmware update configuration (start) --------------------------------------------*/
#define FIRMWARE_LOW_ADDRESS FLASH_CF_BLOCK_13
#define FIRMWARE_TEMPORARY_AREA_HIGH_ADDRESS FLASH_CF_BLOCK_84
#define FIRMWARE_MIRROR_BLOCK_NUM_FOR_SMALL 8
#define FIRMWARE_MIRROR_BLOCK_NUM_FOR_MEDIUM 6
#define FIRMWARE_USER_FIRMWARE_HEADER_LENGTH 0x200
/*------------------------------------------ firmware update configuration (end) --------------------------------------------*/

#define INTEGRITY_CHECK_SCHEME_HASH_SHA256_STANDALONE "hash-sha256"
#define INTEGRITY_CHECK_SCHEME_SIG_SHA256_ECDSA_STANDALONE "sig-sha256-ecdsa"
#define FIRMWARE_TARGET_BLOCK_NUMBER (FLASH_NUM_BLOCKS_CF - FIRMWARE_MIRROR_BLOCK_NUM_FOR_SMALL - FIRMWARE_MIRROR_BLOCK_NUM_FOR_MEDIUM)
#define FIRMWARE_TEMPORARY_AREA_LOW_ADDRESS FLASH_CF_LO_BANK_LO_ADDR
#define USER_RESET_VECTOR_ADDRESS (FIRMWARE_LOW_ADDRESS - 4)

#define FIRMWARE_UPDATE_BLOCK_LENGTH (512)
#define FILE_PARAMETER_FIRMWARE_VERIFICATION_TYPE_OFFSET (0x00000008)
#define FILE_PARAMETER_SIGNATURE_SIZE_OFFSET             (0x00000028)
#define FILE_PARAMETER_SIGNATURE_OFFSET                  (0x0000002c)
#define FILE_PARAMETER_FIRMWARE_VERIFICATION_TYPE_LENGTH (32)
#define FILE_PARAMETER_SIGNATURE_SIZE_LENGTH             (4)
#define FILE_PARAMETER_SIGNATURE_LENGTH                  (256)

#define TC_SHA256_BLOCK_SIZE (64)
#define TC_SHA256_DIGEST_SIZE (32)
#define TC_SHA256_STATE_BLOCKS (TC_SHA256_DIGEST_SIZE/4)

#define FLASH_INTERRUPT_PRIORITY 1 /* 0(low) - 15(high) */

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/
typedef struct _firmware_update_control_block
{
    uint8_t magic_code[7];
    uint8_t image_flag;
    uint8_t signature_type[32];
    uint32_t signature_size;
    uint8_t signature[256];
    uint32_t dataflash_flag;
    uint32_t dataflash_start_address;
    uint32_t dataflash_end_address;
    uint8_t reserved1[200];
    uint32_t sequence_number;
    uint32_t start_address;
    uint32_t end_address;
    uint32_t execution_address;
    uint32_t hardware_id;
    uint8_t reserved2[236];
}FIRMWARE_UPDATE_CONTROL_BLOCK;

typedef struct _load_firmware_control_block {
    uint32_t status;
    uint8_t file_name[256];
    uint32_t flash_buffer[FIRMWARE_UPDATE_BLOCK_LENGTH / sizeof(uint32_t)];
    uint32_t offset;
    uint32_t progress;
volatile    uint32_t firmware_length;
volatile    uint32_t firmware_read_length;
}LOAD_FIRMWARE_CONTROL_BLOCK;

/***********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/
void firmware_update_request(char *string);
bool is_firmupdating(void);
bool is_firmupdatewaitstart(void);
uint32_t firmware_update(void);
uint32_t get_update_data_size(void);
void flash_bank_swap_callback_function(void *event);
void load_firmware_status(uint32_t *now_status, uint32_t *finish_status);
void software_reset(void);

const uint8_t code_signer_public_key[] = CODE_SIGNER_PUBLIC_KEY_PEM;
const uint32_t code_signer_public_key_length = sizeof(code_signer_public_key);

/***********************************************************************************************************************
 Private global variables and functions
 ***********************************************************************************************************************/
static LOAD_FIRMWARE_CONTROL_BLOCK load_firmware_control_block;
static FIRMWARE_UPDATE_CONTROL_BLOCK *firmware_update_control_block_bank1 = (FIRMWARE_UPDATE_CONTROL_BLOCK*)FIRMWARE_TEMPORARY_AREA_LOW_ADDRESS;
static uint8_t buf[FIRMWARE_UPDATE_BLOCK_LENGTH];
static uint32_t buf_dummy[FIRMWARE_UPDATE_BLOCK_LENGTH];
static uint8_t block[FIRMWARE_USER_FIRMWARE_HEADER_LENGTH];

static int32_t firm_block_read(uint32_t *firmware, uint32_t offset);
static int32_t extract_update_file_parameters(uint8_t *string, uint8_t *type, uint32_t *size, uint8_t *sig);
static void flash_load_firmware_callback_function(void *event);
static int32_t firmware_verification_sha256_ecdsa(const uint8_t * pucData, uint32_t ulSize, const uint8_t * pucSignature);
static FIRMWARE_UPDATE_CONTROL_BLOCK * p_block_header;

uint32_t firmware_update(void)
{
    flash_interrupt_config_t cb_func_info;
    uint32_t required_dataflash_block_num;
    int32_t flash_error_code;
    int32_t verification_result = -1;
    static uint32_t next_offset = 0;

    p_block_header = (FIRMWARE_UPDATE_CONTROL_BLOCK *)block;
    switch (load_firmware_control_block.status)
    {
        case FIRMWARE_UPDATE_STATE_INITIALIZE: /* initialize */
            load_firmware_control_block.progress = 0;
            load_firmware_control_block.offset = 0;
            cb_func_info.pcallback = flash_load_firmware_callback_function;
            cb_func_info.int_priority = FLASH_INTERRUPT_PRIORITY;
            flash_error_code = R_FLASH_Control(FLASH_CMD_SET_BGO_CALLBACK, (void *)&cb_func_info);
            if (FLASH_SUCCESS != flash_error_code)
            {
                printf("R_FLASH_Control() returns error. %d.\r\n", flash_error_code);
                printf("system error.\r\n");
                load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_ERROR;
                break;
            }
            load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_WAIT_START;
            break;
        case FIRMWARE_UPDATE_STATE_WAIT_START: /* wait start */
            /* this state will be changed by other process request using load_firmware_control_block.status */
            break;
        case FIRMWARE_UPDATE_STATE_ERASE: /* erase bank1 user program area */
            extract_update_file_parameters(load_firmware_control_block.file_name, p_block_header->signature_type, &p_block_header->signature_size, p_block_header->signature);
            load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_ERASE_WAIT_COMPLETE;
            flash_error_code = R_FLASH_Erase((flash_block_address_t)FIRMWARE_TEMPORARY_AREA_HIGH_ADDRESS, FIRMWARE_TARGET_BLOCK_NUMBER);
            if (FLASH_SUCCESS != flash_error_code)
            {
                printf("R_FLASH_Erase() returns error. %d.\r\n", flash_error_code);
                printf("system error.\r\n");
                load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_ERROR;
                break;
            }
            break;
        case FIRMWARE_UPDATE_STATE_ERASE_WAIT_COMPLETE:
            /* this state will be changed by callback routine */
            break;
        case FIRMWARE_UPDATE_STATE_READ_START:
            if (!firm_block_read(buf_dummy, load_firmware_control_block.offset))
            {
                next_offset = FIRMWARE_UPDATE_BLOCK_LENGTH;
                load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_READ_WAIT_COMPLETE;
            }
            else
            {
                printf("File system read error.\r\n");
                load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_ERROR;
            }
            break;
        case FIRMWARE_UPDATE_STATE_READ_WAIT_COMPLETE:
            load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_WRITE_WAIT_COMPLETE;
            memcpy(load_firmware_control_block.flash_buffer, buf_dummy, FIRMWARE_UPDATE_BLOCK_LENGTH);
            flash_error_code = R_FLASH_Write((uint32_t)load_firmware_control_block.flash_buffer, (uint32_t)FLASH_CF_LO_BANK_LO_ADDR + load_firmware_control_block.offset, sizeof(load_firmware_control_block.flash_buffer));
            if (FLASH_SUCCESS != flash_error_code)
            {
                printf("R_FLASH_Write() returns error. %d.\r\n", flash_error_code);
                printf("system error.\r\n");
                load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_ERROR;
                break;
            }
            if ((FLASH_CF_MEDIUM_BLOCK_SIZE * FIRMWARE_TARGET_BLOCK_NUMBER) > next_offset)
            {
                if (!firm_block_read(buf_dummy, next_offset))
                {
                    next_offset += FIRMWARE_UPDATE_BLOCK_LENGTH;
                }
                else
                {
                    load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_ERROR;
                }
            }
            break;
        case FIRMWARE_UPDATE_STATE_WRITE_WAIT_COMPLETE:
            /* this state will be changed by callback routine */
            break;
        case FIRMWARE_UPDATE_STATE_FINALIZE: /* finalize */
            firmware_update_control_block_bank1 = (FIRMWARE_UPDATE_CONTROL_BLOCK*)FIRMWARE_TEMPORARY_AREA_LOW_ADDRESS;
            if (!strcmp((const char *)p_block_header->signature_type, INTEGRITY_CHECK_SCHEME_HASH_SHA256_STANDALONE))
            {
                uint8_t hash_sha256[32];
                /* Hash message */
                struct tc_sha256_state_struct xCtx;
                tc_sha256_init(&xCtx);
                tc_sha256_update(&xCtx,
                        (uint8_t*)FIRMWARE_TEMPORARY_AREA_LOW_ADDRESS + FIRMWARE_USER_FIRMWARE_HEADER_LENGTH,
                        (FLASH_CF_MEDIUM_BLOCK_SIZE * FIRMWARE_TARGET_BLOCK_NUMBER) - FIRMWARE_USER_FIRMWARE_HEADER_LENGTH);
                tc_sha256_final(hash_sha256, &xCtx);
                verification_result = memcmp(p_block_header->signature, hash_sha256, sizeof(hash_sha256));
            }
            else if (!strcmp((const char *)p_block_header->signature_type, INTEGRITY_CHECK_SCHEME_SIG_SHA256_ECDSA_STANDALONE))
            {
                verification_result = firmware_verification_sha256_ecdsa(
                                                    (const uint8_t *)FIRMWARE_TEMPORARY_AREA_LOW_ADDRESS + FIRMWARE_USER_FIRMWARE_HEADER_LENGTH,
                                                    (FLASH_CF_MEDIUM_BLOCK_SIZE * FIRMWARE_TARGET_BLOCK_NUMBER) - FIRMWARE_USER_FIRMWARE_HEADER_LENGTH,
                                                    firmware_update_control_block_bank1->signature);
            }
            else
            {
                verification_result = -1;
            }

            if (0 == verification_result)
            {
                load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_FINALIZE_COMPLETED;
            }
            else
            {
                load_firmware_control_block.status= FIRMWARE_UPDATE_STATE_ERROR;
            }
            break;
        case FIRMWARE_UPDATE_STATE_FINALIZE_WAIT_ERASE_DATA_FLASH:
            /* this state will be changed by callback routine */
            break;
        case FIRMWARE_UPDATE_STATE_FINALIZE_WAIT_WRITE_DATA_FLASH:
            /* this state will be changed by callback routine */
            break;
        case FIRMWARE_UPDATE_STATE_FINALIZE_COMPLETED:
            load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_COMPLETED;
            break;
        case FIRMWARE_UPDATE_STATE_COMPLETED:
            break;
        case FIRMWARE_UPDATE_STATE_ERROR:
            load_firmware_control_block.progress = 100;
            break;
        default:
            break;
    }
    return load_firmware_control_block.progress;
}

static int32_t firm_block_read(uint32_t *firmware, uint32_t offset)
{
    FRESULT ret = TFAT_FR_OK;
    uint16_t size = 0;
    FIL file = {0};
    static uint32_t previous_file_position;

    if (0 == offset)
    {
        previous_file_position = 0;
    }

    ret = R_tfat_f_open(&file, (char*)load_firmware_control_block.file_name, TFAT_FA_READ | TFAT_FA_OPEN_EXISTING);
    if (TFAT_RES_OK == ret)
    {
        memset(buf, 0xff, sizeof(buf));
        ret = R_tfat_f_lseek(&file, previous_file_position);
        if (TFAT_RES_OK == ret)
        {
            ret = R_tfat_f_read(&file, buf, FIRMWARE_UPDATE_BLOCK_LENGTH, &size);
            if (TFAT_RES_OK == ret)
            {
                if (0 == size)
                {
                    R_tfat_f_close(&file);
                    return -1;
                }
                else
                {
                    previous_file_position += size;
                    memcpy(&firmware[0], buf, size);
                }
            }
            else
            {
                R_tfat_f_close(&file);
                return -1;
            }
            load_firmware_control_block.firmware_read_length = previous_file_position;
        }
        else
        {
            R_tfat_f_close(&file);
            return -1;
        }
        ret = R_tfat_f_close(&file);
        if (TFAT_RES_OK != ret)
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
    return 0;
}

static int32_t extract_update_file_parameters(uint8_t *string, uint8_t *type, uint32_t *sig_size, uint8_t *sig)
{
    FRESULT ret = TFAT_FR_OK;
    uint16_t size = 0;
    FIL file = {0};
    uint8_t *p_buf = 0;

    ret = R_tfat_f_open(&file, (const char *)string, TFAT_FA_READ | TFAT_FA_OPEN_EXISTING);
    if (TFAT_RES_OK == ret)
    {
        ret = R_tfat_f_read(&file, buf, FIRMWARE_UPDATE_BLOCK_LENGTH, &size);
        if (TFAT_RES_OK == ret)
        {
            /* Register a signature type */
            memcpy(type, &buf[FILE_PARAMETER_FIRMWARE_VERIFICATION_TYPE_OFFSET], FILE_PARAMETER_FIRMWARE_VERIFICATION_TYPE_LENGTH);
            /* Register a signature size */
            p_buf = (uint8_t *)sig_size;
            *p_buf++ = buf[FILE_PARAMETER_SIGNATURE_SIZE_OFFSET];
            *p_buf++ = buf[FILE_PARAMETER_SIGNATURE_SIZE_OFFSET+1];
            *p_buf++ = buf[FILE_PARAMETER_SIGNATURE_SIZE_OFFSET+2];
            *p_buf   = buf[FILE_PARAMETER_SIGNATURE_SIZE_OFFSET+3];
            /* Register signature value */
            memcpy(sig, &buf[FILE_PARAMETER_SIGNATURE_OFFSET], FILE_PARAMETER_SIGNATURE_LENGTH);
        }
        else
        {
            R_tfat_f_close(&file);
            return -1;
        }
        ret = R_tfat_f_close(&file);
        if (TFAT_RES_OK != ret)
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
    return 0;
}


void flash_load_firmware_callback_function(void *event)
{
    uint32_t event_code;
    event_code = *((uint32_t*)event);

    switch (event_code)
    {
        case FLASH_INT_EVENT_ERASE_COMPLETE:
            if (FIRMWARE_UPDATE_STATE_ERASE_WAIT_COMPLETE == load_firmware_control_block.status)
            {
                load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_READ_START;
            }
            else if (FIRMWARE_UPDATE_STATE_FINALIZE_WAIT_ERASE_DATA_FLASH == load_firmware_control_block.status)
            {
                load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_FINALIZE_COMPLETED;
            }
            else
            {
                load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_ERROR;
            }
            break;
        case FLASH_INT_EVENT_WRITE_COMPLETE:
            if (FIRMWARE_UPDATE_STATE_WRITE_WAIT_COMPLETE == load_firmware_control_block.status)
            {
                load_firmware_control_block.offset += FIRMWARE_UPDATE_BLOCK_LENGTH;
                load_firmware_control_block.progress = (uint32_t)(((float)(load_firmware_control_block.offset)/(float)((FLASH_CF_MEDIUM_BLOCK_SIZE * FIRMWARE_TARGET_BLOCK_NUMBER))*100));
                load_firmware_control_block.firmware_length = load_firmware_control_block.firmware_read_length;
                if (load_firmware_control_block.offset == (FLASH_CF_MEDIUM_BLOCK_SIZE * FIRMWARE_TARGET_BLOCK_NUMBER))
                {
                    load_firmware_control_block.progress = 100;
                    load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_FINALIZE;
                }
                else
                {
                    load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_READ_WAIT_COMPLETE;
                }
            }
            else if (FIRMWARE_UPDATE_STATE_FINALIZE_WAIT_WRITE_DATA_FLASH == load_firmware_control_block.status)
            {
                load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_FINALIZE_COMPLETED;
            }
            else
            {
                load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_ERROR;
            }
            break;
        default:
            load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_ERROR;
            break;
    }
}

void firmware_update_request(char *string)
{
    if (FIRMWARE_UPDATE_STATE_COMPLETED == load_firmware_control_block.status)
    {
        load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_INITIALIZE;
    }
    if (FIRMWARE_UPDATE_STATE_INITIALIZE == load_firmware_control_block.status)
    {
        firmware_update();
    }
    if (FIRMWARE_UPDATE_STATE_CAN_SWAP_BANK < load_firmware_control_block.status)
    {
        strcpy((char *)load_firmware_control_block.file_name, string);
        load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_ERASE;
    }
}

void firmware_update_status_initialize(void)
{
    load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_INITIALIZE;
}
uint32_t get_update_data_size(void)
{
    return (load_firmware_control_block.firmware_length);
}

bool is_firmupdating(void)
{
    if (FIRMWARE_UPDATE_STATE_CAN_SWAP_BANK < load_firmware_control_block.status)
    {
        return false;
    }
    return true;
}

bool is_firmupdatewaitstart(void)
{
    if (FIRMWARE_UPDATE_STATE_WAIT_START != load_firmware_control_block.status)
    {
        return false;
    }
    return true;
}

void load_firmware_status(uint32_t *now_status, uint32_t *finish_status)
{
    *now_status    = load_firmware_control_block.status;
    *finish_status = FIRMWARE_UPDATE_STATE_COMPLETED;
}

static int32_t firmware_verification_sha256_ecdsa(const uint8_t * pucData, uint32_t ulSize, const uint8_t * pucSignature)
{
    int32_t xResult = -1;
    uint8_t pucHash[TC_SHA256_DIGEST_SIZE];
    uint8_t data_length;
    uint8_t public_key[64];
    uint8_t binary[256];
    uint8_t *head_pointer, *current_pointer, *tail_pointer;;

    /* Hash message */
    struct tc_sha256_state_struct xCtx;
    tc_sha256_init(&xCtx);
    tc_sha256_update(&xCtx, pucData, ulSize);
    tc_sha256_final(pucHash, &xCtx);

    /* extract public key from code_signer_public_key (pem format) */
    head_pointer = (uint8_t*)strstr((char *)code_signer_public_key, "-----BEGIN PUBLIC KEY-----");
    if (head_pointer)
    {
        head_pointer += strlen("-----BEGIN PUBLIC KEY-----");
        tail_pointer = (uint8_t*)strstr((char *)code_signer_public_key, "-----END PUBLIC KEY-----");
        base64_decode(head_pointer, binary, tail_pointer - head_pointer);
        current_pointer = binary;
        data_length = *(current_pointer + 1);
        while(1)
        {
            switch (*current_pointer)
            {
                case 0x30: /* found "SEQUENCE" */
                    current_pointer += 2;
                    break;
                case 0x03: /* found BIT STRING (maybe public key) */
                    if (*(current_pointer + 1) == 0x42)
                    {
                        memcpy(public_key, current_pointer + 4, 64);
                        /* Verify signature */
                        if (uECC_verify(public_key, pucHash, TC_SHA256_DIGEST_SIZE, pucSignature, uECC_secp256r1()))
                        {
                            xResult = 0;
                        }
                    }
                    current_pointer += *(current_pointer + 1) + 2;
                    break;
                default:
                    current_pointer += *(current_pointer + 1) + 2;
                    break;
            }
            if ((current_pointer - binary) > data_length)
            {
                /* parsing error */
                break;
            }
        }
    }
    return xResult;
}

void software_reset(void)
{
    /* stop all interrupt completely */
    set_psw(0);
    R_BSP_InterruptsDisable();
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
    SYSTEM.SWRR = 0xa501;
    while(1);   /* software reset */
}

/* end of file */
