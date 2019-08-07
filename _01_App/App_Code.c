/*
*********************************************************************************************************
*                                              _03_OS
* File			 : App_Code.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : 数据编码
*********************************************************************************************************
*/
#include "App_Code.h"

#if SCIRXST

u16 Get_Data=1;
u8  Get_Flag=0;

#else
u16 Send_Flag=0;
u8	Send_Data=0;
u8 dat_buf[14];

#endif


void Code_main(void)
{
	float temper=1.564;
	static u16 daa=100;
	while(1)
	{
		printf("%f",temper);
		#if SCIRXST
		if(Get_Flag==0)
		{
			Get_Flag=1;
			
			Show_Data();
			
		}
		#else 
		if(Send_Flag == 0)
		{
			Send_Flag=1;
			//TMP275ReadTemprature(&temper);
			if(Key_Now_Get(WK_UP,KEY_MODE_SHORT))
			{
				daa+=10;
			}
			if(Key_Now_Get(KEY2,KEY_MODE_SHORT))
			{
				daa-=10;
			}
			
			Send_Data=daa;//temper*5;
			//Code_Data();
			
			Show_Send();
		}
		#endif
		OSTimeDly(333);
	}
}

#if SCIRXST
void Show_Data(void)
{
		OS_Num_Show(320,80,24,1, 1,"白光通信接收端	    ");
		OS_Num_Show(300,140,24, 1,Get_Data/5.0f,"接收数据为：%.1f℃    ");
}

#else 
void DataSend(void)
{
	int i = 0;

	for(i = 0; i < 8; i ++)
	{
		
		if(0== dat_buf[i])
		{
			// 输出编码0
			SendOut = 1;
			delay_us(2);
			SendOut = 0;
			delay_us(3);
		}
		else
		{
			// 输出编码1
			SendOut = 1;
			delay_us(2);
			SendOut = 0;
			delay_us(8);
		}	
	}
	// 产生一个边沿，以免丢失最后一个数据
	SendOut = 1;
	delay_us(2);
	SendOut = 0;
}

void Code_Data(u16 data)
{
		u8 cnt=0;
//		u8 PCM_data;
		static u8 Flag=0;
		
		data=data>>5;
	//	ADPCM_Encode(&data,&PCM_data,1);
	
		for(cnt=0;cnt<7;cnt++)
		{
				dat_buf[7-cnt] =(data & 0x01);
				data= data>>1;
		}
		
		if(Flag>=8)
		{
			Flag=0;
			
			Send_Flag=0;		
		}
		
		dat_buf[7]=Send_Data>>Flag;
		
		
		Flag++;
	

	
}
void Show_Send()
{
	OS_Num_Show(320,80,24,1, 1,"白光通信发送端	    ");
	OS_Num_Show(300,140,24, 1,Send_Data/5.0f,"发送数据为：%.1f℃    ");
}
#endif

