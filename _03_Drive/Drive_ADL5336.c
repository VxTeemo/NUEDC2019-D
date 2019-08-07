/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_ADL5335.c
* By  			 : 
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : 可变增益放大器(VGA)ADL5336驱动程序
*********************************************************************************************************
*/

#include "Drive_ADL5336.h"




void ADL5336_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOA,GPIOE时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_15; 
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉 
	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOE0,2
	
	//ADL5336_CLK=1;
	ADL5336_LE=1;
	//ADL5336_LE0=1;
	delay_us(100);
	
}

void ADL5336_Init2(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOA,GPIOE时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_10|GPIO_Pin_8|GPIO_Pin_6; 
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉 
	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOE0,2
	
	//ADL5336_CLK=1;
	//ADL5336_LE=1;
	ADL5336_LE0=1;
	delay_us(100);
	
}

void ADL5336_Writer(u16 SetD)
{
	 u16 i,k=0x01;
	 ADL5336_LE=1; 
	 ADL5336_CLK=1;
	 delay_us(10);
	 ADL5336_LE=0; 
	 ADL5336_CLK=0;
	 ADL5336_Data=1;
	 delay_us(10);
	 ADL5336_CLK=1;
	 delay_us(10);

	 for(i=0;i<11;i++)
	 {
				ADL5336_CLK=0;
				delay_us(10);
				if((SetD&k))
					ADL5336_Data=1;
				else
					ADL5336_Data=0;
				k<<=1;
				ADL5336_CLK=1;
				delay_us(10);
	 }
	 ADL5336_LE=1;
	 ADL5336_CLK=0;
}

void ADL5336_Read()
{
	 u16 i;
	 ADL5336_LE=1; 
	 ADL5336_CLK=1;
	 delay_us(10);
	 ADL5336_LE=0; 
	 ADL5336_CLK=0;
	 ADL5336_Data=0;
	 delay_us(10);
	 ADL5336_CLK=1;
	 delay_us(10);
	
	 for(i=0;i<11;i++)
	 {
				ADL5336_CLK=0;
				delay_us(10);
				
				ADL5336_Data=0;
	
				ADL5336_CLK=1;
				delay_us(10);
	 }
	 ADL5336_LE=1;
	  ADL5336_CLK=0;
}

void ADL5336_Writer2(u16 SetD)
{
	 u16 i,k=0x01;
	 ADL5336_LE0=1; 
	 ADL5336_CLK0=1;
	 delay_us(10);
	 ADL5336_LE0=0; 
	 ADL5336_CLK0=0;
	 ADL5336_Data0=1;
	 delay_us(10);
	 ADL5336_CLK0=1;
	 delay_us(10);

	 for(i=0;i<11;i++)
	 {
				ADL5336_CLK0=0;
				delay_us(10);
				if((SetD&k))
					ADL5336_Data0=1;
				else
					ADL5336_Data0=0;
				k<<=1;
				ADL5336_CLK0=1;
				delay_us(10);
	 }
	 ADL5336_LE0=1;
	 ADL5336_CLK0=0;
}

void ADL5336_Read2()
{
	 u16 i;
	 ADL5336_LE0=1; 
	 ADL5336_CLK0=1;
	 delay_us(10);
	 ADL5336_LE0=0; 
	 ADL5336_CLK0=0;
	 ADL5336_Data0=0;
	 delay_us(10);
	 ADL5336_CLK0=1;
	 delay_us(10);
	
	 for(i=0;i<11;i++)
	 {
				ADL5336_CLK0=0;
				delay_us(10);
				
				ADL5336_Data0=0;
	
				ADL5336_CLK0=1;
				delay_us(10);
	 }
	 ADL5336_LE0=1;
	  ADL5336_CLK0=0;
}
