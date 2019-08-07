#ifndef __DRIVE_EXTI_H
#define __DRIVE_EXTI_H 

/* 头文件包含 ----------------------------------------------------------------*/  
#include "User_header.h" 

/* 全局宏定义 ----------------------------------------------------------------*/

/* 结构体声明 ----------------------------------------------------------------*/ 

/* 全局变量声明 --------------------------------------------------------------*/ 

/* 全局函数声明 --------------------------------------------------------------*/  
void Exti0_Init(void);
void Exti1_Init(void);
void Exti2_Init(void);
void Exti3_Init(void);
void Exti4_Init(void);
void Exti5_9_Init(void);
void Exti10_15_Init(void);

void Exti_Enable(u8 Exti_Num);	
void Exti_Disable(u8 Exti_Num);

#endif



