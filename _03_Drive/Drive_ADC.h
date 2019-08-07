#ifndef __DRIVE_ADC_H
#define __DRIVE_ADC_H 

/* 头文件包含 ----------------------------------------------------------------*/  
#include "User_header.h"

/* 全局宏定义 ----------------------------------------------------------------*/

/* 结构体声明 ----------------------------------------------------------------*/ 

/* 全局变量声明 --------------------------------------------------------------*/ 
 
/* 全局函数声明 --------------------------------------------------------------*/  
u16 ADC_Result_Get(u8 Channel,u8 Speed);
u16 ADC_Average_Get(u8 Channel, u8 Times);
u16 ADC_Waveform_Get(u16 *SourceBuff,u16 SourceLenth,double *ResultBuff,u16 ResultLenth);
void ADC1_Init(void);
void ADC2_Init(void);
void  Adc_Init(void);
u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);
#endif
/*******************************(C) COPYRIGHT 2016 Wind（谢玉伸）*********************************/






