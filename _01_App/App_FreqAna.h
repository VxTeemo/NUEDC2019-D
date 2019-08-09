#ifndef __APP_FREQANA_H
#define __APP_FREQANA_H 

#include "User_header.h"

typedef enum{
	Fault_Type_Normal,
	Fault_Type_Error,
	Fault_Type_R1Open,
	Fault_Type_R1Short,
	Fault_Type_R2Open,
	Fault_Type_R2Short,
	Fault_Type_R3Open,
	Fault_Type_R3Short,
	Fault_Type_R4Open,
	Fault_Type_R4Short,
	Fault_Type_C1Open,
	Fault_Type_C2Open,
	Fault_Type_C3Open,
	Fault_Type_C1Double,
	Fault_Type_C2Double,
	Fault_Type_C3Double,
	Fault_Type_Error1
} Fault_Type;


void FreqAna_main(void);
void GridData_Init(void);
void AD9851_Sweep(void);

#endif
