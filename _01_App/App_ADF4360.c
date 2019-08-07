/*
*********************************************************************************************************
*                                              _04_OS
* File			 : App_ADF4360.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : ÎÞÏß»°Í²
*********************************************************************************************************
*/
#include "App_ADF4360.h"



void ADF4360_main()
{
	static u32 Set_Freq=88000000;
	static u8 Flag=0,Count=0;
	TouchKey_Draw();
	
	while(1)
	{
		if(Key_Now_Get(KEY1,KEY_MODE_SHORT))
		{
				Set_Freq+=200000;
				Count=1;
		}
		else if(Key_Now_Get(KEY2,KEY_MODE_SHORT))
		{
				Set_Freq-=200000;
				Count=1;
		}
		else if(Key_Now_Get(KEY3,KEY_MODE_SHORT))
		{
				Flag=!Flag;
				Count=1;
		}
		
		TouchKey_Scan();
		
		if(Touch_Keys.Flag==1)
		{
				Set_Freq=Touch_Keys.Result;
				Touch_Keys.Flag=0;
				Count=1;
		}
		
		if(Count == 1)
		{
			OS_Num_Show(40,50,32,1,Set_Freq,"Set_Freq:%0.0f     ");
			OS_Num_Show(40,90,32,1,Flag,"Mode:%0.0f     ");
		}
			
		if(Flag==0&&Count==1)
		{
			Count=0;
			ADF4360_SetFreq(Set_Freq,1);
			
		}
		else if(Flag==1&&Count==1)
		{
			Count=0;
			ADF4360_2_SetFreq(Set_Freq,1);
		}
	
	
		
		OSTimeDly(120);
		
	}
}



