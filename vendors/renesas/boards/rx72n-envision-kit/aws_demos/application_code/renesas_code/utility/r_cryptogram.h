/* Copyright,2006 RENESAS SOLUTIONS CORPORATION ALL RIGHT RESERVED */

/* $Id: //depot/cryptogram/include/r_cryptogram.h#2 $ */
/* $Date: 2010/02/28 $ */

#ifndef __r_cryptogram_h__
#define __r_cryptogram_h__

#if defined(MACOS) || defined(RX)
#include <stdint.h>
#else
#include "r_stdint.h"
#endif

#if !defined(M16C) && !defined(M32C)
#define near
#endif

void R_Sha1_Hash(uint8_t near *, uint8_t near *, uint32_t near); 

void R_Des_Keysch(uint8_t near *,  uint32_t near *);
void R_Des_Ecbenc(uint8_t near *, uint8_t near *, uint32_t near *, uint32_t near);
void R_Des_Ecbdec(uint8_t near *, uint8_t near *, uint32_t near *, uint32_t near);


#if defined(M16C) || defined(M32C)
#pragma PARAMETER R_Sha1_Hash(r1, r2, r3)
#endif

#endif