#ifndef __PS2_H
#define __PS2_H

#include "User_header.h"


void PS2_Keyboard_Init(void);
void PS2_ReadKeyCodon(volatile uint8_t* Key_Num);


/* 私有宏 ------------------------------------------------------------------------------------*/
#define Key_Num_0  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_0) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Num_1  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_1) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Num_2  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_2) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Num_3  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_3) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Num_4  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_4) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Num_5  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_5) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Num_6  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_6) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Num_7  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_7) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Num_8  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_8) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Num_9  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_9) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Enter  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_Enter) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Add    			((Key_FSM_PS2.Key_Code_Bef == Key_Code_Add) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Minus       ((Key_FSM_PS2.Key_Code_Bef == Key_Code_Minus) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Ride       	((Key_FSM_PS2.Key_Code_Bef == Key_Code_Ride) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Div    			((Key_FSM_PS2.Key_Code_Bef == Key_Code_Div) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_NumLock			((Key_FSM_PS2.Key_Code_Bef == Key_Code_NumLock) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Point       ((Key_FSM_PS2.Key_Code_Bef == Key_Code_Point) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Back        ((Key_FSM_PS2.Key_Code_Bef == Key_Code_Back) && (Key_FSM_PS2.Key_Mode == 1))


#define Key_Code_0  				0x70
#define Key_Code_1  				0x69
#define Key_Code_2  				0x72
#define Key_Code_3  				0x7a
#define Key_Code_4  				0x6b
#define Key_Code_5  				0x73
#define Key_Code_6  				0x74
#define Key_Code_7  				0x6c
#define Key_Code_8  				0x75
#define Key_Code_9  				0x7d
#define Key_Code_Enter  		0xE0
#define Key_Code_Add        0x79
#define Key_Code_Minus      0x7b
#define Key_Code_Ride       0x7c
#define Key_Code_Div        0x4a
#define Key_Code_NumLock    0x77
#define Key_Code_Point      0x71
#define Key_Code_Back       0x66

/* 私有结构体 --------------------------------------------------------------------------------*/

/**********************************************************************************************
 *结构体名：__KEY_StateMachine	
 *
 *成		员：
 *					volatile uint16_t State_Flag  	记录按键状态标志位
 *					volatile uint16_t Key_Mode    	回传按键模式 
 *说    明：	“0” 无按键   “1”独立短按键   “2”独立长按键     “3”独立持续按键		
 *					volatile uint8_t KeyCode_Pre		记录当前按键IO状态
 *  				volatile uint8_t KeyCode_Bef		记录前一次按键IO口状态
 *					volatile uint16_t Key_Count     长按键、持续按键计数					
 *
 *说		明：本状态机支持16个独立按键任意按键组合使用，只需自行定义按键状态即可
 *********************************************************************************************/
typedef struct 
{
	volatile uint16_t State_Flag;       //状态机状态标志
	volatile uint8_t  Key_Code_Pre;     //当前键码
	volatile uint8_t  Key_Code_Bef;     //前一次键码
	volatile uint16_t Key_Mode;         //按键模式
	volatile uint16_t Key_Count;        //长按键计数
	volatile uint16_t Key_Filt;         //PS2数据滤波
}__KEY_StateMachine;
extern __KEY_StateMachine  Key_FSM_PS2;
/* 函数声明 ----------------------------------------------------------------------------------*/
void Key_StateSweep(__KEY_StateMachine *Key_FSM);


/* 文件结束 ----------------------------------------------------------------------------------*/




#endif

