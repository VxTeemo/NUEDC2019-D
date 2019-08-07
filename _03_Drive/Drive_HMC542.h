#ifndef __DRIVE_HMC542_H
#define __DRIVE_HMC542_H 

/* 头文件包含 ----------------------------------------------------------------*/  
#include "User_header.h"
/* 全局宏定义 ----------------------------------------------------------------*/   

#define MOSI_H   GPIO_SetBits(GPIOF, GPIO_Pin_1)
#define MOSI_L   GPIO_ResetBits(GPIOF, GPIO_Pin_1)
#define CS_H     GPIO_SetBits(GPIOE, GPIO_Pin_4)
#define CS_L     GPIO_ResetBits(GPIOE, GPIO_Pin_4)
#define SCLK_H   GPIO_SetBits(GPIOE, GPIO_Pin_2)
#define SCLK_L   GPIO_ResetBits(GPIOE, GPIO_Pin_2)
#define RESET_H  GPIO_SetBits(GPIOE, GPIO_Pin_0)
#define RESET_L  GPIO_ResetBits(GPIOE, GPIO_Pin_0)

void HMC542_Init(void);
void HMC542_SendByte(u16 Txdata);
u16 db_calc(float db);

#endif
/*******************************(C) COPYRIGHT 2016*********************************/















