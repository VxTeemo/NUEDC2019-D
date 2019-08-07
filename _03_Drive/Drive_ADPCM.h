#ifndef __Drive_ADPCM_H
#define __Drive_ADPCM_H 

#include "User_header.h"



void ADPCM_Encode(volatile u16 *Data_in,volatile u8 *Data_out,u16 size);

void ADPCM_Decode(volatile u8 *Data_in,volatile u16 *Data_out,u16 size);

#endif

