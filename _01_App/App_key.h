#ifndef __APP_KEY_H
#define __APP_KEY_H 

#include "User_header.h"

/* 全局宏定义 ----------------------------------------------------------------*/ 
#define KEY_MODE_SHORT		(0x01<<0)	
#define KEY_MODE_LONG		(0x01<<1)	
  
#define KEY_COUNT_SHORT		20
#define KEY_COUNT_LONG		100
#define KEY_COUNT_LIMIT		500


#define GPIO_WK_UP 	PAin(0)	
#define GPIO_KEY1 	PCin(1)	
#define GPIO_KEY2 	PCin(2)	
#define GPIO_KEY3 	PCin(3)	

#define KEY_PRESS 	(GPIO_WK_UP == 0||GPIO_KEY1 == 0||GPIO_KEY2 == 0||GPIO_KEY3 == 0)

#define WK_UP		(0x01<<0)	
#define KEY1		(0x01<<1)													
#define KEY2		(0x01<<2)	
#define KEY3		(0x01<<3)	 
#define KEY_ALL		 0xff


/* 结构体声明 ----------------------------------------------------------------*/ 
typedef struct
{ 
	u16	CountNow[4];//计数
	u16	CountLast[4];//上一次繁荣计数停留
	u8 ResultShort;//短按按键组合结果
	u8 ResultLong;//长按按键组合结果
	
}Type_Key;

/* 全局变量声明 --------------------------------------------------------------*/  
extern Type_Key Key;

/* 全局函数声明 --------------------------------------------------------------*/  				
u8 Key_Now_Get(u8 KeyNum,u8 Mode);
u8 Key_Get(u8 KeyNum,u8 Mode);
void Key_Clear(u8 Mode);
 void Key_main(void);
#endif

