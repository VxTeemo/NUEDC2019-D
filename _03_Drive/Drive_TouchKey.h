#ifndef __DRIVE_TOUCHKEY_H
#define __DRIVE_TOUCHKEY_H 

#include "User_header.h"



typedef struct Touch_Key
{
		u32 Result;			//最后结果
		u32	Nums;				//中间变量
		u8 	Flag;				//输入完成标志
}Touch_Key;


extern Touch_Key Touch_Keys;

void TouchKey_Draw(void);
void Clear_Show(void);
void Show_Result(float data);
void Clear_Result(void);
float TouchKey_Scan(void);

extern char ADF4351_flag;
extern char DAC1;
extern char DAC2;
#endif

