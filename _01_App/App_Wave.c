/*
*********************************************************************************************************
*                                              _03_OS
* File			 : App_Wave.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/20
* function 	 : 超声波测距
*********************************************************************************************************
*/

#include "App_Wave.h"  

#define DEFAULT_OSC_SHIFTX 			(500)				//值越大越往左移
#define DEFAULT_OSC_SHIFTY 			(100)  				//值越大越往上移
#define DEFAULT_OSC_ZOOMX 			(100)			
#define DEFAULT_OSC_ZOOMY			(100)		
#define DEFAULT_OSC_VPP 			0
#define DEFAULT_OSC_T 				0
#define DEFAULT_OSC_MAXVALUE 		0
#define DEFAULT_OSC_MINVALUE		0xffff
Type_OSC Struct_OSC = {DEFAULT_OSC_SHIFTX,DEFAULT_OSC_SHIFTY,DEFAULT_OSC_ZOOMX,DEFAULT_OSC_ZOOMY, \
					   DEFAULT_OSC_VPP,DEFAULT_OSC_T,DEFAULT_OSC_MAXVALUE,DEFAULT_OSC_MINVALUE };

void Data1_Show(void);	
void Data2_Show(void);	
void frequent_show(void);
void touch_show();
u8 OSC_Flag=0;
float OUTV_ADC,OUTV_ADC1,OUTV_ADC2;				
PID_TypeDef	adc1_PID,adc2_PID; 	
static float AD8361_V=170.0,dac1,dac2;		
static float target_dac1=170.0,target_dac2=660.0;	
static u16 V1=0,V2=0;				 
static float Fre_flag=250.0,FRE=0.0;	
void Wave_main(void)		
{     
	static float last1_vol=0,last2_vol=0,adc1_vol,adc2_vol;

	TouchKey_Draw();
	while(1)
	{		
		if(OSC_Flag==0)
		{
			OSC_Flag=1;
			Struct_OSC.BuffFormat = OSC_FORMAT_U16;  	
			Struct_OSC.BuffLenth = 8;				
			Struct_OSC.Buff_u16 = ADC1_DMA2_Buff;
			Struct_OSC.Buff_u16 = ADC3_DMA2_Buff;
			OS_TextColor_Set(Black);
			OS_BackColor_Set(White);
			
		}

		if(DMA_GetFlagStatus(DMA2_Stream0,DMA_FLAG_TCIF0) == SET) //adc1的返回值
		{   
			Data1_Show();		
			OUTV_ADC1=V1/4.8;
		}	
		
		if(DMA_GetFlagStatus(DMA2_Stream1,DMA_FLAG_TCIF0) == SET) //adc3的返回值
		{     
			Data2_Show();			
			OUTV_ADC2=V2;
		}	
		touch_show();
		if(OUTV_ADC1!= (target_dac1*1.24))
		{
			adc1_vol=OUTV_ADC1;
			last1_vol=adc1_vol*0.5f + last1_vol*0.5f;
			dac1=pid_calc( &adc1_PID,target_dac1*1.24, last1_vol);  //PID算出要设置的值
			Dac1_Set_Vol(dac1);
		}
		if(OUTV_ADC2!= (target_dac2*1.24))
		{
			adc2_vol=OUTV_ADC2;
			last2_vol=adc2_vol*0.5f + last2_vol*0.5f;
			dac2=pid_calc( &adc2_PID,target_dac2*1.24, last2_vol);  //PID算出要设置的值
			Dac2_Set_Vol(dac2);
		}
		
		
		
		OSTimeDly(100);
	}									
}	

void Data1_Show(void)
{ 
	u16 sum1=0.0;
	int i=0;
	//FindMaxMin(&Vmax,&Vmin);
	for(i=0;i<8;i++)
	{
		sum1=sum1+ADC1_DMA2_Buff[i];
	}
	V1=sum1/8.0;
	OS_Num_Show(100,200,32,1, V1/1240.0,"V1pp:%.2f        ");

}		

void Data2_Show(void)
{ 
	double sum2=0.0;
	int i=0;
	//FindMaxMin(&Vmax,&Vmin);
	for(i=0;i<8;i++)
	{
		sum2=sum2+ADC3_DMA2_Buff[i];
	}
	V2=sum2/8.0;
	OS_Num_Show(100,250,32,1, V2/1240.0,"V2pp:%.2f        ");

}		




void touch_show()
{
	AD8361_V=TouchKey_Scan();
	if(DAC1==1)
	{
		DAC1=0;
		target_dac1=AD8361_V;
	}		
	OS_Num_Show(100,100,32,1,target_dac1,"DAC1:%.0f");
	if(DAC2==1)
	{
		DAC2=0;
		target_dac2=AD8361_V;
	}
	OS_Num_Show(100,150,32,1,target_dac2,"DAC2:%.0f");
	if(ADF4351_flag==1)
	{
		ADF4351_flag=0;
		FRE=AD8361_V;
		ADF4351Init((u16)(FRE));

	}	 
 	OS_Num_Show(100,50,32,1,FRE,"FRE:%.1f");
			
}


