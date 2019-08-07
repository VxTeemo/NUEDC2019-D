#ifndef __APP_ADF4351_H
#define __APP_ADF4351_H 

#include "User_header.h"

extern u8 ADF4351_Sweep;
extern double Start_Freq,Stop_Freq;
extern u16 Sweep_Time;
extern double Step_Freq;   			//步进频率
extern u16 Wave_Vol;     					  	//输出幅度
extern double Out_Freq;    				 //输出频率
extern double Divisor;
void ADF4351_main(void);
void Touch_Handle(double data); 	 //按键处理
void Start_Meum(void);					  //开始菜单
void PID_Init(void);							//PID初始化

#endif 
