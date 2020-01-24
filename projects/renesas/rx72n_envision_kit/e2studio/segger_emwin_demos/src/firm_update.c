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
 * Copyright (C) 2014(2017) Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : firm_update.c
 * Description  :
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 08.06.2017 1.00 First Release
 *********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "platform.h"
#include "r_tfat_lib.h"
#include "r_flash_rx_if.h"

/******************************************************************************
Typedef definitions
*******************************************************************************/
#define UPDATE_FILE_NAME "userprog.rsu"

typedef struct _load_firmware_control_block{
	uint32_t status;
	uint32_t flash_buffer[FLASH_CF_MEDIUM_BLOCK_SIZE / 4];
	uint32_t offset;
	volatile uint32_t flash_write_in_progress_flag;
	volatile uint32_t flash_erase_in_progress_flag;
	uint32_t progress;
}LOAD_FIRMWARE_CONTROL_BLOCK;

/******************************************************************************
 External variables
 ******************************************************************************/

/******************************************************************************
 Private global variables
 ******************************************************************************/
static uint32_t g_ram_vector_table[256];      // RAM space to hold the vector table
static LOAD_FIRMWARE_CONTROL_BLOCK g_load_firmware_control_block;

/******************************************************************************
 External functions
 ******************************************************************************/
extern uint32_t base64_decode(uint8_t *source, uint8_t *result, uint32_t size);

/*******************************************************************************
 Private global variables and functions
********************************************************************************/
static int32_t firm_block_read(uint32_t *firmware, uint32_t offset);
static uint32_t *flash_copy_vector_table(void);
static uint32_t *firm_update_init(flash_interrupt_config_t *cb_func_info);
static void dummy_int(void);
static void flash_bank_swap_callback_function(void *event);
static void flash_load_firmware_callback_function(void *event);

void bank_swap(void);
uint32_t load_firmware_process(void);

uint32_t *firm_update_init(flash_interrupt_config_t *cb_func_info)
{
	flash_err_t flash_err;
	uint32_t *flash_vect;

	flash_vect = flash_copy_vector_table();
    flash_err = R_FLASH_Control(FLASH_CMD_SET_BGO_CALLBACK,(void *)cb_func_info);
    if (FLASH_SUCCESS != flash_err)
    {
    	while(1);
    }
    return flash_vect;
}

void firm_update_finalize(uint32_t *flash_vect)
{
    /* Set INTB to ram address */
#if __RENESAS_VERSION__ >= 0x01010000
    set_intb((void *)flash_vect);
#else
    set_intb((uint32_t)flash_vect);
#endif
}
/***********************************************************************************************************************
* Function Name: firm_block_read
* Description  : Does an example task. Making this longer just to see how it wraps. Making this long just to see how it
*                wraps.
* Arguments    : index -
*                    Where to start looking
*                p_output -
*                    Pointer of where to put the output data
* Return Value : count -
*                    How many entries were found
***********************************************************************************************************************/
static int32_t firm_block_read(uint32_t *firmware, uint32_t offset)
{
    FRESULT ret = TFAT_FR_OK;
    uint8_t buf[256] = {0};
    uint8_t arg1[256] = {0};
    uint8_t arg2[256] = {0};
    uint8_t arg3[256] = {0};
    uint16_t size = 0;
    FIL file = {0};
    uint32_t read_size = 0;
    static uint32_t upprogram[4 + 1] = {0};
    static uint32_t current_char_position = 0;
    static uint32_t current_file_position = 0;
    static uint32_t previous_file_position = 0;

    if (0 == offset)
    {
    	current_char_position = 0;
    	current_file_position = 0;
        previous_file_position = 0;
        memset(upprogram,0,sizeof(upprogram));
    }

    ret = R_tfat_f_open(&file, UPDATE_FILE_NAME, TFAT_FA_READ | TFAT_FA_OPEN_EXISTING);
    if (TFAT_RES_OK == ret)
    {
        current_char_position = 0;
        memset(buf, 0, sizeof(buf));

        R_tfat_f_lseek(&file, previous_file_position);
        if (TFAT_RES_OK == ret)
        {
			while(1)
			{
				ret = R_tfat_f_read(&file, &buf[current_char_position++], 1, &size);
				if (TFAT_RES_OK == ret)
				{
					if (0 == size)
					{
						break;
					}
					else
					{
						previous_file_position += size;

						/* received 1 line */
						if(strstr((char*)buf, "\r\n"))
						{
							sscanf((char*)buf, "%256s %256s %256s", (char*)arg1, (char*)arg2, (char*)arg3);
							if (!strcmp((char *)arg1, "upprogram"))
							{
								base64_decode(arg3, (uint8_t *)upprogram, strlen((char *)arg3));
								memcpy(&firmware[current_file_position], upprogram, 16);
								current_file_position += 4;
								read_size += 16;
							}
							if((current_file_position * 4) == FLASH_CF_MEDIUM_BLOCK_SIZE)
							{
								current_file_position = 0;
								break;
							}
							current_char_position = 0;
							memset(buf, 0, sizeof(buf));
						}
					}
				}
				else
				{
					goto firm_block_read_error;
					break;
				}
			}
        }
        else
        {
			goto firm_block_read_error;
        }
        if (TFAT_RES_OK == ret)
        {
            ret = R_tfat_f_close(&file);
            if (TFAT_RES_OK != ret)
            {
				goto firm_block_read_error;
            }
        }
    }
    else
    {
firm_block_read_error:
        return -1;
    }
    return 0;
}

/***********************************************************************************************************************
* Function Name: flash_copy_vector_table
* Description  : Moves the relocatable vector table to RAM. This is only needed if ROM operations are performed.
*                ROM cannot be accessed during operations. The vector table is located in ROM and will be accessed
*                if an interrupt occurs.
* Arguments    : none
* Return Value : uint32_t *flash_vect
***********************************************************************************************************************/
static uint32_t *flash_copy_vector_table(void)
{
    uint32_t *flash_vect;
    uint32_t   i;

    /* Get address of variable vector table in ROM */
    flash_vect = (uint32_t *)get_intb();

    /* Copy over variable vector table to RAM */
    for(i = 0; i < 256; i++ )
    {
    	g_ram_vector_table[i] = (uint32_t)dummy_int;      // Copy over entry
    }

    g_ram_vector_table[VECT_FCU_FIFERR] = flash_vect[VECT_FCU_FIFERR];
    g_ram_vector_table[VECT_FCU_FRDYI] = flash_vect[VECT_FCU_FRDYI];

    /* Set INTB to ram address */
#if __RENESAS_VERSION__ >= 0x01010000
    set_intb((void *)&g_ram_vector_table[0] );
#else
    set_intb( (uint32_t)&g_ram_vector_table[0] );
#endif
    return flash_vect;
}

#pragma section FRAM2

/******************************************************************************
 Function Name   : bank_swap()
 Description     :
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void bank_swap(void)
{
	flash_err_t flash_err;
	uint32_t level;
	uint32_t *flash_vect;
	flash_interrupt_config_t cb_func_info;

	cb_func_info.pcallback = flash_bank_swap_callback_function;
    cb_func_info.int_priority = 15;

    /* disable interrupt excluding flash */
	level = R_BSP_CpuInterruptLevelRead();
    R_BSP_CpuInterruptLevelWrite(14);
    flash_vect = firm_update_init(&cb_func_info);

	flash_err = R_FLASH_Control(FLASH_CMD_BANK_TOGGLE, NULL);
	if(FLASH_SUCCESS == flash_err)
	{
		while(1);	/* wait software reset */
	}

    /* restore interrupt level/vector */
    firm_update_finalize(flash_vect);
    R_BSP_CpuInterruptLevelWrite(level);
}

uint32_t load_firmware_process(void)
{
	uint32_t level;
	uint32_t *flash_vect;
	flash_interrupt_config_t cb_func_info;

	cb_func_info.pcallback = flash_load_firmware_callback_function;
    cb_func_info.int_priority = 15;

	switch(g_load_firmware_control_block.status)
	{
		case 0: /* initialize */
			g_load_firmware_control_block.progress = 0;
			g_load_firmware_control_block.status = 1;
			g_load_firmware_control_block.offset = 0;
			g_load_firmware_control_block.flash_write_in_progress_flag = 0;
			g_load_firmware_control_block.flash_erase_in_progress_flag = 0;
			break;
		case 1: /* erase bank0 (0xFFE00000-0xFFEF0000) */
		    /* disable interrupt excluding flash */
			level = R_BSP_CpuInterruptLevelRead();
		    R_BSP_CpuInterruptLevelWrite(14);
		    flash_vect = firm_update_init(&cb_func_info);

		    g_load_firmware_control_block.offset = 0;
		    R_FLASH_Erase((flash_block_address_t)FLASH_CF_BLOCK_38, FLASH_NUM_BLOCKS_CF);
			g_load_firmware_control_block.flash_erase_in_progress_flag = 1;
		    while(g_load_firmware_control_block.flash_erase_in_progress_flag);

		    /* restore interrupt level/vector */
		    firm_update_finalize(flash_vect);
		    R_BSP_CpuInterruptLevelWrite(level);

		    g_load_firmware_control_block.status = 2;
			break;
		case 2: /* read firmware 1block from external USB memory */
			if(!firm_block_read(g_load_firmware_control_block.flash_buffer, g_load_firmware_control_block.offset))
			{
				g_load_firmware_control_block.status = 3;
			}
			else
			{
				g_load_firmware_control_block.status = 4;
			}
			break;
		case 3: /* write firmware 1block to internal FLASH memory */
		    /* disable interrupt excluding flash */
			level = R_BSP_CpuInterruptLevelRead();
		    R_BSP_CpuInterruptLevelWrite(14);
		    flash_vect = firm_update_init(&cb_func_info);

		    R_FLASH_Write((uint32_t)g_load_firmware_control_block.flash_buffer, (uint32_t)FLASH_CF_LO_BANK_LO_ADDR + g_load_firmware_control_block.offset, sizeof(g_load_firmware_control_block.flash_buffer));
		    g_load_firmware_control_block.flash_write_in_progress_flag = 1;
		    while(g_load_firmware_control_block.flash_write_in_progress_flag);

		    /* restore interrupt level/vector */
		    firm_update_finalize(flash_vect);
		    R_BSP_CpuInterruptLevelWrite(level);

		    if(g_load_firmware_control_block.offset == (FLASH_CF_MEDIUM_BLOCK_SIZE * 30) + (FLASH_CF_SMALL_BLOCK_SIZE * 8))
		    {
		    	g_load_firmware_control_block.status = 4;
		    }
		    else
		    {
		    	g_load_firmware_control_block.progress = (uint32_t)(((float)(g_load_firmware_control_block.offset)/(float)((FLASH_CF_MEDIUM_BLOCK_SIZE * 30) + (FLASH_CF_SMALL_BLOCK_SIZE * 8))*100));
		    	g_load_firmware_control_block.status = 2;
		    }
		    break;
		case 4: /* finalize */
			g_load_firmware_control_block.status = 0;
			g_load_firmware_control_block.progress = 100;
			break;
	}
	return g_load_firmware_control_block.progress;
}

void flash_load_firmware_callback_function(void *event)
{
	uint32_t event_code;
	event_code = *((uint32_t*)event);

	switch(event_code)
	{
		case FLASH_INT_EVENT_ERASE_COMPLETE:
			g_load_firmware_control_block.flash_erase_in_progress_flag = 0;
			break;
		case FLASH_INT_EVENT_WRITE_COMPLETE:
			g_load_firmware_control_block.flash_write_in_progress_flag = 0;
			g_load_firmware_control_block.offset += FLASH_CF_MEDIUM_BLOCK_SIZE;
			break;
		default:
			break;
	}
}

void flash_bank_swap_callback_function(void *event)
{
	flash_err_t flash_err;
	uint32_t event_code;
	event_code = *((uint32_t*)event);

	switch(event_code)
	{
		case FLASH_INT_EVENT_BLANK:
			break;
		case FLASH_INT_EVENT_NOT_BLANK:
			flash_err = R_FLASH_Control(FLASH_CMD_BANK_TOGGLE, NULL);
			if(FLASH_SUCCESS != flash_err)
			{
				while(1);
			}
			break;
		case FLASH_INT_EVENT_TOGGLE_BANK:
	        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
	        SYSTEM.SWRR = 0xa501;
			while(1);	/* software reset */
			break;
		default:
			break;
	}
}

#pragma interrupt (dummy_int)
static void dummy_int(void)
{
	/* nothing to do */
}

/* end of file */
