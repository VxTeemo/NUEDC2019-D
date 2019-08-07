/*
*********************************************************************************************************
*                                              _03_OS
* File			 : PID.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : 此套PID适用于电源控制
*********************************************************************************************************
*/

#include "Drive_PID.h"


/**
 **********************************************************
 * 函数：void PID_DeInit( PID_TypeDef	*p)
 * 描述：PID数据复位
 * 输入：*p：定义的PID数据
 * 返回：无 
 **********************************************************
 */
void pid_reset( PID_TypeDef	*p)
{
	p->a0			= 0;
	p->a1			= 0;
	p->a2			= 0;

	p->Ek_0 	= 0;     //  Error[k]
	p->Ek_1		= 0;     //  Error[k-1]
	p->Ek_2		= 0;     //  Error[k-2]

	p->T			= 0;
	p->Kp			= 0;
	p->Ti			= 0;
	p->Td			= 0;
		
	p->L_Output	= 0;
	p->N_Output	= 0;

	p->Inc			= 0;

	p->OutMax		= 0;
	p->OutMin		= 0; 
}

/**
 **********************************************************
 * 函数：void PID_init ( PID_TypeDef *p)
 * 描述：PID参数计算
 * 输入：*p：定义的PID数据
 * 返回：无 
 **********************************************************
 */
void pid_init ( PID_TypeDef *p)
{   
	
	if(p->Ti == 0)
	
	p->Ti		= p->Ti == 0 ? 0xFFFFFFFF: p->Ti;
	p->a0		= p->Kp * (1 + 1.0f * p->T / p->Ti + 1.0f * p->Td / p->T);
	p->a1		= p->Kp * (1 + 2.0f * p->Td / p->T );
	p->a2		= 1.0f * p->Kp * p->Td / p->T;
}

/**
 **********************************************************
 * 函数：float PID_Calc( PID_TypeDef *p)
 * 描述：PID增量式结算	 
 * 输入：*p：PID结构体
 * 返回：PID运算结果	
 **********************************************************
 */
float pid_calc( PID_TypeDef *p, float ref, float fb)
{
	p->Ek_0		= ref - fb;	//计算误差

	p->Inc		= (  p->a0 * p->Ek_0	- p->a1 * p->Ek_1  + p->a2 * p->Ek_2 	);				//PID计算

 	p->N_Output = p->L_Output + p->Inc; 

	if(p->N_Output > p->OutMax)p->N_Output   =	p->OutMax;
	if(p->N_Output < p->OutMin)p->N_Output   =	p->OutMin;
	
	p->Ek_2        = p->Ek_1;
	p->Ek_1        = p->Ek_0;		
	p->L_Output    = p->N_Output;
	
	return p->N_Output;

}


/**
 **********************************************************
 * 函数：void PID_Clean( PID_TypeDef	*p)
 * 描述：清楚PID计算中的缓存
 * 输入：*p：定义的PID数据
 * 返回：无 
 **********************************************************
 */
void pid_clc( PID_TypeDef	*p)
{
	p->Ek_0 			= 0;     //  Error[k]
	p->Ek_1				= 0;     //  Error[k-1]
	p->Ek_2				= 0;     //  Error[k-2]
	p->Inc				= 0;
	p->L_Output 	= 0;
	p->N_Output 	= 0;
}





