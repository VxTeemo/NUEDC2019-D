#ifndef __DRIVER_GPIO_H
#define __DRIVER_GPIO_H 

#include "User_header.h"

#if SYSTEM_VERSION

#define LED1 PBout(7)
#define LED2 PBout(8)
#define LED3 PDout(2)
#define LED4 PBout(1) 

#else

#define LED1 PBout(7)
#define LED2 PBout(8)
#define LED3 PCout(6)
#define LED4 PCout(7) 

#endif

void LED_Init(void);
void GPIO_Key_Init(void);
void GPIO_POW_Init(void);
void GPIO_Data_Init(void);
void GPIO_ADF_Init(void);
#endif

