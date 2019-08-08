#ifndef __DRIVER_RELAY_H
#define __DRIVER_RELAY_H 

#include "User_header.h"

typedef enum {
	Relay_ON = 0,
	Relay_OFF = 1
} Relay_State;

typedef enum {
	Relay_IN   = 1,
	Relay_LOOP = 2,
	Relay_LOAD = 3,
	Relay_OUT  = 4
} Relay_Select;

#ifndef Relay_AllOFF
#define Relay_AllOFF GPIO_SetBits(GPIOG, GPIO_Pin_1 | GPIO_Pin_3| GPIO_Pin_5| GPIO_Pin_7)
#endif

void Relay_Init(void);
void Relay_Control(Relay_Select Relay_Num, Relay_State relay_state);


#endif

