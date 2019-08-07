#ifndef __DRIVE_ADL5336_H
#define __DRIVE_ADL5336_H

#include "User_header.h"

//#define ADL5336_SDO  PGout(0) 
#define ADL5336_Data PFout(15) 
#define ADL5336_CLK  PFout(13)
#define ADL5336_LE 	 PFout(11)

#define ADL5336_SDO0  PFout(12) 
#define ADL5336_Data0 PFout(10) 
#define ADL5336_CLK0  PFout(8)
#define ADL5336_LE0   PFout(6)


void ADL5336_Init(void);
void ADL5336_Writer(u16 SetD);
void ADL5336_Read(void);
void ADL5336_Init2(void);
void ADL5336_Writer2(u16 SetD);
void ADL5336_Read2(void);


#endif
