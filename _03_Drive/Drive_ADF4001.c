/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_ADF4001.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : ADF4001模块驱动
*********************************************************************************************************
*/
#include "Drive_ADF4001.h"

/*
****************************************************************************************
*	函数名	: 	ADF4001_Init
*	输  入	: 	void
*	输  出	: 	void
*	功  能	: 	ADF4001初始化
*	说  明	:	  ADF4001_CLK = PF7	ADF4001_DAT = PF8	ADF4001_LE = PF9
****************************************************************************************
*/

void ADF4001_Init(void)
{
	
	GPIO_InitTypeDef   GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	/* Configure PG0 pin as ouput */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

	AD4001_CLK_0;
	AD4001_DAT_0;
	AD4001_LE_0;
}

/*
****************************************************************************************
*	函数名	: 	ADF4001_InputData
*	输  入	：	data:	ADF4001锁存器数据	
*	输  出	:		void
*	功  能	:		模拟SPI总线，向ADF4001写入数据
****************************************************************************************
*/
static void ADF4001_InputData(unsigned long int data)
{ 
	int i;
	long int temp=0x800000;
	
	AD4001_CLK_0;
	AD4001_LE_0;

	for(i=0;i<24;i++)
	{
		if(data&temp)
			AD4001_DAT_1;
		else 
			AD4001_DAT_0;

		AD4001_CLK_1;
		Delay_ms(2);
		AD4001_CLK_0;
		Delay_ms(2);
		temp = temp>>1;
	}
	AD4001_LE_1;
	Delay_ms(10);
}

#ifndef R_COUNTER
/*
****************************************************************************************
*	函数名	: 	ADF4001_SetFreq
*	输  入	：	RCounter:	R锁存器中R COUNTER的值
							NCounter:	N锁存器中N COUNTER的值
							Muxout:		MUXOUT引脚输出状态
*	输  出	:		void
*	功  能	:		设置ADF4001的锁定输出频率及MUXOUT引脚的输出状态
*	说  明	：	ADF4001的锁定输出频率 Fout=REF*NCounter/RCounter
****************************************************************************************
*/
void ADF4001_SetFreq(unsigned long int RCounter, unsigned long int NCounter,unsigned int Muxout)
{
	unsigned long int Rdata,Ndata,Idata;
	
	Rdata = (RCounter<<2) | 0x000000;
	Ndata = (NCounter<<8) | 0x000001;
	Idata = (Muxout<<4)   | 0x0D8883;

	ADF4001_InputData(Idata);//Init Latch															  
	ADF4001_InputData(Rdata);//R
	ADF4001_InputData(Ndata);//N	 	
}
#else
/*
****************************************************************************************
*	函数名	:		ADF4001_SetFreq
*	输  入	：	freq:		ADF4001锁定输出频率值
							Muxout:		MUXOUT引脚输出状态
*	输  出	:		void
*	功  能	:		设置ADF4001的锁定输出频率及MUXOUT引脚的输出状态
*	说  明	：	ADF4001的锁定输出频率 Fout=REF*NCounter/RCounter
****************************************************************************************
*/
void ADF4001_SetFreq(unsigned long int freq,unsigned long int Muxout)
{
	unsigned long int step;
	unsigned long int Rdata,Ndata,Idata;	

	step  = 24000000/R_COUNTER;
	Rdata = (R_COUNTER<<2) | 0x000000;
	Ndata = ((freq/step)<<8) | 0x000001;
	Idata = (Muxout<<4) | 0x0D8883;
	
	ADF4001_InputData(Idata);//Init Latch											  
	ADF4001_InputData(Rdata);//R Latch
	ADF4001_InputData(Ndata);//N Latch	 	
}
#endif

