#ifndef __DRIVE_PID_H
#define __DRIVE_PID_H 

#include "User_header.h"

typedef struct  _PID{ 
	
	float	T;  		//输入:周期
	float	Kp; 		//输入:比例
	float	Ti; 		//输入:积分
	float	Td; 		//输入:微分

	float	a0; 		//计算值:a0 = Kp(1 + T/Ti + Td/T)
	float	a1; 		//计算值:a1 = Kp(1 + 2Td/T)
	float	a2; 		//计算值:a2 = Kp*Td/T

	float	Ek_0;   //计算值:Error[k],当前误差
	float	Ek_1; 	//计算值:Error[k-1]
	float	Ek_2; 	//计算值:Error[k-2]

	float	Inc;				//增量值(输出PID值与上一时刻的PID值之差)
	float	N_Output;		//计算值:PID输出值
	float	L_Output;		//计算值:上次输出值

	float	OutMax;			//输入:PID输出最大值
	float	OutMin;			//输入:PID输出最小值
	
}PID_TypeDef;

extern PID_TypeDef	adc1_PID,adc2_PID; 

void pid_reset( PID_TypeDef	*p);
void pid_init ( PID_TypeDef *p);
float pid_calc( PID_TypeDef *p, float ref, float fb);

//void ADC1_PID_Init(void);
//void ADC2_PID_Init(void);


#endif


