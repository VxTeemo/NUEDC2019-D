#ifndef __DRIVER_RELAY_H
#define __DRIVER_RELAY_H 

#include "User_header.h"

typedef enum {
	Relay_ON  = 1,
	Relay_OFF = 0
} Relay_State;

typedef enum {
	Relay_631HSLS   = 1,//G1 J4 关0高速 开1低速
	Relay_MESS_0    = 2,//G3 J2 暂时没用 一直关
	Relay_9851      = 3,//G5 J1 9851的衰减 暂时不用 关
	Relay_LOAD      = 4,//G7 J3 4k负载的开关 0关不接 1接
	Relay_7K        = 5,//G9 J5 0不接 1接
	Relay_ALL       = 6
} Relay_Select;


void Relay_Init(void);
void Relay_Control(Relay_Select Relay_Num, Relay_State relay_state);


#endif

