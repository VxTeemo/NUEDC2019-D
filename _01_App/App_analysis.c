/*
*********************************************************************************************************
*                                              _04_OS
* File			 : App_analysis.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : 语音频谱分析
*********************************************************************************************************
*/
#include "App_analysis.h"

void Show_FFT();
float FFT_VOL[50];

void analysis_main(void)
{
	u16 FFT_DATA[8192];

	TIM3_Init(1,10254);//8192HZ采样率51 201
	ADC1_DMA2_Reload(0,ADC_Prescaler_Div2);
	while(1)
	{
		FFT_Exchange(ADC1_DMA2_Buff,FFT_DATA,8192);  	//ADC1_DMA2_Buff  
		FFT_Harmonic(FFT_VOL,FFT_DATA,8192);
		Show_FFT();
	//	ADC1_DMA2_Reload(0,ADC_Prescaler_Div2);
		OSTimeDly(423);
	}
}

void Show_FFT()
{
		u16 i,j=0;
		OS_String_Show(350,100,24,1,"当前频率成分有：");
		for(i=1;i<25;i++)
		{
			if(FFT_VOL[i*2]!=0)
			{
				OS_Num_Show(350,140+40*j,24,1, FFT_VOL[i*2+1],"freq:%.1fHz            ");
				j++;
			}
		}
}

