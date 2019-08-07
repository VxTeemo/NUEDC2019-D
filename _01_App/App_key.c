/*
*********************************************************************************************************
*                                              _04_OS
* File			 : App_key.c
* By  			 : Wind
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : 按键扫描任务
*********************************************************************************************************
*/

#include "App_key.h"


Type_Key Key = {0};

void Key_main(void)
{

	
		while(1)
		{
			
			if(GPIO_WK_UP == 0)	{ Key.CountNow[0]++;Key.CountLast[0]++;}
			else Key.CountNow[0]= 0;
			
			if(GPIO_KEY1 == 0)	{ Key.CountNow[1]++;Key.CountLast[1]++;}
			else Key.CountNow[1]= 0;
			
			if(GPIO_KEY2 == 0)	{ Key.CountNow[2]++;Key.CountLast[2]++;}
			else Key.CountNow[2]= 0;
			
			if(GPIO_KEY3 == 0)  { Key.CountNow[3]++;Key.CountLast[3]++;}
			else Key.CountNow[3]= 0; 
		
		  if(!KEY_PRESS) //没有按键按下
			{		 
				if(Key.CountLast[0] > KEY_MODE_SHORT && Key.CountLast[0] < KEY_COUNT_LONG)  
					Key.ResultShort |= WK_UP;
				
				if(Key.CountLast[1] > KEY_MODE_SHORT && Key.CountLast[1] < KEY_COUNT_LONG)
					Key.ResultShort |= KEY1; 
				
				if(Key.CountLast[2] > KEY_MODE_SHORT && Key.CountLast[2] < KEY_COUNT_LONG)
					Key.ResultShort |= KEY2; 
				
				if(Key.CountLast[3] > KEY_MODE_SHORT && Key.CountLast[3] < KEY_COUNT_LONG)
					Key.ResultShort |= KEY3;  
				 
				if(Key.CountLast[0] > KEY_COUNT_LONG && Key.CountLast[0] < KEY_COUNT_LIMIT)  
					Key.ResultLong |= WK_UP;
				
				if(Key.CountLast[1] > KEY_COUNT_LONG && Key.CountLast[1] < KEY_COUNT_LIMIT)
					Key.ResultLong |= KEY1; 
				
				if(Key.CountLast[2] > KEY_COUNT_LONG && Key.CountLast[2] < KEY_COUNT_LIMIT)
					Key.ResultLong |= KEY2; 
				
				if(Key.CountLast[3] > KEY_COUNT_LONG && Key.CountLast[3] < KEY_COUNT_LIMIT)
					Key.ResultLong |= KEY3;  
				 
				Key.CountLast[0] = 0;
				Key.CountLast[1] = 0;
				Key.CountLast[2] = 0;
				Key.CountLast[3] = 0; 
			}		
			else {Key.ResultShort = 0; Key.ResultLong = 0;}
			
			OSTimeDly(400);
		}
}
						
/**----------------------------------------------------------------------------
* @FunctionName  : Key_Now_Get()     
* @Description   : 按键现状获取函数 
* @Data          : 2016/7/19
* @Explain       
-------------------------------------------------------------------------------		
 @param  	KeyNum: WK_UP|KEY1|KEY2|KEY3	
			Mode: KEY_MODE_SHORT|KEY_MODE_LONG						
 @return 	0（无效）,1（有效）							
------------------------------------------------------------------------------*/ 
u8 Key_Now_Get(u8 KeyNum,u8 Mode)
{
	u8 Result = 0;  
	if(Mode & KEY_MODE_SHORT)
	{
		if(KeyNum&WK_UP)
		{
			if(Key.CountNow[0] > KEY_MODE_SHORT)
				Result |= WK_UP;
		}
		if(KeyNum&KEY1)
		{
			if(Key.CountNow[1] > KEY_MODE_SHORT)
				Result |= KEY1;
		}
		if(KeyNum&KEY2)
		{
			if(Key.CountNow[2] > KEY_MODE_SHORT)
				Result |= KEY2;
		}
		if(KeyNum&KEY3)
		{
			if(Key.CountNow[3] > KEY_MODE_SHORT)
				Result |= KEY3;
		}
	}
	else if(Mode & KEY_MODE_LONG)
	{
		if(KeyNum&WK_UP)
		{
			if(Key.CountNow[0] > KEY_COUNT_LONG)
				Result |= WK_UP;
		}
		if(KeyNum&KEY1)
		{
			if(Key.CountNow[1] > KEY_COUNT_LONG)
				Result |= KEY1;
		}
		if(KeyNum&KEY2)
		{
			if(Key.CountNow[2] > KEY_COUNT_LONG)
				Result |= KEY2;
		}
		if(KeyNum&KEY3)
		{
			if(Key.CountNow[3] > KEY_COUNT_LONG)
				Result |= KEY3;
		}
		
	}
		
	if(Result == KeyNum) return 1;
	else return 0;  
}

/**----------------------------------------------------------------------------
* @FunctionName  : Key_Get()     
* @Description   : 按键获取函数 
* @Data          : 2016/7/19
* @Explain       ：可用 | 连接个参数
 -------------------------------------------------------------------------------		
 @param  	KeyNum: WK_UP|KEY1|KEY2|KEY3	
					Mode: KEY_MODE_SHORT|KEY_MODE_LONG
 @return 	0（无效）,1（有效）							
------------------------------------------------------------------------------*/ 
u8 Key_Get(u8 KeyNum,u8 Mode)
{ 
	if(Mode & KEY_MODE_SHORT)
	{ 
		if(Key.ResultShort == KeyNum) return 1;
		else return 0; 
	}
	if(Mode & KEY_MODE_LONG)
	{ 
		if(Key.ResultLong == KeyNum) return 1;
		else return 0; 
	}
	return 0;	
}
 

/**----------------------------------------------------------------------------
* @FunctionName  : Key_Clear()     
* @Description   : 按键清除函数 
* @Data          : 2016/7/19
* @Explain       ：可用 | 连接个参数  
-------------------------------------------------------------------------------		
 与Key_Get()配套使用
 @param Mode: KEY_MODE_SHORT|KEY_MODE_LONG
------------------------------------------------------------------------------*/
void Key_Clear(u8 Mode)
{ 
	if(Mode & KEY_MODE_SHORT) Key.ResultShort = 0;
	if(Mode & KEY_MODE_LONG)  Key.ResultLong = 0;
}

u8 KEY_Scan(u8 mode)
{
		static u8 key_up=1;//按键按松开标志
	
		if(mode)
			key_up=1;  //支持连按		  
		
		if(key_up&&(GPIO_KEY1==0||GPIO_KEY2==0||GPIO_KEY3==0||GPIO_WK_UP==0))
		{
				Delay_ms(10);//去抖动 
				key_up=0;
			
				if(GPIO_KEY1==0)
					return 1;
				else if(GPIO_KEY2==0)
					return 2;
				else if(GPIO_KEY3==0)
					return 3;
				else if(GPIO_WK_UP==0)
					return 4;
		}
		else if(GPIO_KEY1==1 && GPIO_KEY2==1 && GPIO_KEY3==1 && GPIO_WK_UP==1)
			key_up=1; 	
		
		return 0;// 无按键按下
}
