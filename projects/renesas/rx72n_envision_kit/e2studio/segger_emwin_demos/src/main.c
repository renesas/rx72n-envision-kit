/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

***** emWin - Graphical user interface for embedded applications *****
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.

----------------------------------------------------------------------
File        : main.c
Purpose     : Starts emWin application
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "r_flash_rx_if.h"
#include "r_sys_time_rx_if.h"

void MainTask(void);

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       main
*/
void main(void) {
	flash_err_t flash_err;

	/* workaround for FLASH API */
    uint8_t * p_rom_section;    // ROM source location
    uint8_t * p_ram_section;    // RAM copy destination
    uint32_t  bytes_copied;

    R_SYS_TIME_Open();

    p_ram_section = (uint8_t *)__sectop("RPFRAM");
    p_rom_section = (uint8_t *)__sectop("PFRAM");

    /* Copy code from ROM to RAM. */
    for (bytes_copied = 0; bytes_copied < __secsize("PFRAM"); bytes_copied++)
    {
        p_ram_section[bytes_copied] = p_rom_section[bytes_copied];
    }

	flash_err = R_FLASH_Open();
    if (FLASH_SUCCESS == flash_err)
    {
    	/* nothing to do */
    }
    else
    {
        while(1);	/* fatal error */
    }
    MainTask();
	while (1);
}

/*************************** End of file ****************************/
