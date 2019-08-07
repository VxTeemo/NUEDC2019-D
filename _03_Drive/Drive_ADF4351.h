#ifndef DRIVE_ADF4351_H_
#define DRIVE_ADF4351_H_

#include "User_header.h"

#define ADF4351_CLK PFout(9)
#define ADF4351_OUTPUT_DATA PFout(7)
#define ADF4351_LE PFout(5)
#define ADF4351_CE PFout(3)

#define ADF4351_INPUT_DATA PFout(13)//æ≤“Ù


void ADF4351Init(void);
void ReadToADF4351(u8 count, u8 *buf);
void WriteToADF4351(u8 count, u8 *buf);
void WriteOneRegToADF4351(u32 Regster);
void ADF4351_Init_some(void);
void ADF4351WriteFreq(float Fre);		//	(xx.x) M Hz

#endif

