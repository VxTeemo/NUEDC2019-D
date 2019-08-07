/*
*********************************************************************************************************
*                                              _04_OS
* File			 : App_ADF4351_Sweep.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : ADF4351É¨ÆµÏß³Ì
*********************************************************************************************************
*/
#include "App_ADF4351_Sweep.h"



void ADF4351_Sweep_main()
{
	static u16    Start_flag=1;
	static double Freqs;
	while(1)
	{
		
     if(ADF4351_Sweep)
		 {
			  if(Start_flag)
				{
					  Freqs=Start_Freq;
				  	Start_flag=0;
				}
				
			 	ADF4351WriteFreq(Freqs);
				Freqs+=0.1f;
				
				if(Freqs>=Stop_Freq)
					ADF4351_Sweep=0;

		 }		
		 else
		 {
			  Start_flag=1;

		 }
		 
		 OSTimeDly((u16)(Sweep_Time*1000/(((u16)Stop_Freq-(u16)Start_Freq)*10)));
	}
}
			
