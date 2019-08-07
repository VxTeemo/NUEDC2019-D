/*
*********************************************************************************************************
*                                              _04_OS
* File			 : App_OSC.c
* By  			 : Wind
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : 触屏扫描任务
*********************************************************************************************************
*/

#include "App_Touch.h"


/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有（静态）函数声明 ------------------------------------------------------*/

/* 全局变量定义 --------------------------------------------------------------*/
Type_Touch TouchStruct = {0};

/* 全局变量声明 --------------------------------------------------------------*/
  
/* 全局函数编写 --------------------------------------------------------------*/


void Touch_main(void)
{
		
	while(1)
	{
			static u16 LastX,LastY;
			TouchRead(&TouchStruct.NowX,&TouchStruct.NowY); 
				
			if(TouchStruct.NowX == 0xffff||TouchStruct.NowY == 0xffff)
			{
				if(TouchStruct.state == TOUCH_PRESS)
				{
					TouchStruct.EndX = LastX;
					TouchStruct.EndY = LastY; 
				}
				TouchStruct.state = TOUCH_FREE; 
			}
			else 
			{
				if(TouchStruct.state == TOUCH_FREE)
				{
					TouchStruct.StartX = TouchStruct.NowX;
					TouchStruct.StartY = TouchStruct.NowY;  
				}
				TouchStruct.state = TOUCH_PRESS; 
			}
			LastX = TouchStruct.NowX;
			LastY =	TouchStruct.NowY;
			
			
			OSTimeDly(9);
			
	}
}
 						

								

/**----------------------------------------------------------------------------
* @FunctionName  : Touch_Judge()     
* @Description   : None 
* @Data          : 2016/7/10
* @Explain       : 判断触屏的状态
------------------------------------------------------------------------------*/						
u8 Touch_Judge(u16 x1,u16 y1,u16 x2,u16 y2)	
{
	u8 result = TOUCH_INVALID;
	 
	if(TouchStruct.StartX >x1 && TouchStruct.StartX  <x2)
	{
		if(TouchStruct.StartY >y1 && TouchStruct.StartY <y2)
		{
			result = TOUCH_VALID;
			if(TouchStruct.EndX >x1 && TouchStruct.EndX <x2)  
			if(TouchStruct.EndY >y1 && TouchStruct.EndY <y2) 
			{
				result = TOUCH_VALID_FULL; 
			}
			
			if(TouchStruct.state == TOUCH_FREE)
				Touch_Clear();  
		} 
	}
	 
	return result;	
	
}

/**----------------------------------------------------------------------------
* @FunctionName  : TouchStart_Judge()     
* @Description   : None 
* @Data          : 2016/7/10
* @Explain       : 判断触屏的起始位置
------------------------------------------------------------------------------*/
u8 TouchStart_Judge(u16 x1,u16 y1,u16 x2,u16 y2)	
{
	u8 result = TOUCH_INVALID;
	 
	if(TouchStruct.StartX >x1 && TouchStruct.StartX  <x2)
	{
		if(TouchStruct.StartY >y1 && TouchStruct.StartY <y2)
		{
			  result = TOUCH_VALID;
		} 
	}
	if(TouchStruct.state == TOUCH_FREE) result = TOUCH_INVALID;
		
	return result;	
	
}


void Touch_Clear(void)
{
	TouchStruct.EndX = 0xffff;
	TouchStruct.StartX = 0xffff;
				
	TouchStruct.EndY = 0xffff;
	TouchStruct.StartY = 0xffff;  
}


