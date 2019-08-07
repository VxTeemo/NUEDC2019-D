/*******************************(C) COPYRIGHT 2017 Vitamin（   ）***************/
/**============================================================================
* @FileName    : main.c
* @Description : None
* @Date        : 2017/7/10
* @By          : Vitamin（   ）
* @Email       : 857248242@ qq.com
* @Platform    : Keil uVision5 5.20 (STM32F407ZG)
* @Explain     : None
*=============================================================================*/

/* 头文件包含 ----------------------------------------------------------------*/
#include "Drive_HMC542.h"

void HMC542_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF, ENABLE);	 //使能PF端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_4;				 //PF 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOF
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
 GPIO_Init(GPIOF, &GPIO_InitStructure);					 //根据设定参数初始化GPIOF
	
 GPIO_SetBits(GPIOE,GPIO_Pin_0);					
 GPIO_ResetBits(GPIOF,GPIO_Pin_1);	
 GPIO_SetBits(GPIOE,GPIO_Pin_4);
 GPIO_ResetBits(GPIOE,GPIO_Pin_2);		
}
 
void HMC542_SendByte(u16 Txdata)
{
	u8 i;
	u16 db_reg;
//	float db;
//	db_reg=0xfc;
//	
//	db=20.0f*log10(Txdata);
//	
//	if(db>=16)
//	{
//		db_reg=db_reg&(0x7c);
//	  db=db-16;
//	}
//	if(db>=8)
//	{
//		db_reg=db_reg&(0xbc);
//		db=db-8;
//	}
//	if(db>=4)
//	{
//		db_reg=db_reg&(0xdc);
//		db=db-4;
//	}
//	if(db>=2)
//	{
//		db_reg=db_reg&(0xec);
//		db=db-2;
//	}
//	if(db>=1)
//	{
//		db_reg=db_reg&(0xf4);
//		db=db-1;
//	}
//	if(db>=0.5)
//	{
//		db_reg=db_reg&(0xf8);
//		db=db-0.5;
//	}
	
	RESET_L;
	delay_us(80);
	RESET_H;
	
	CS_L;
  SCLK_L;
	
	for(i=8;i>0;i--)
	{	
		
		SCLK_L;
		 delay_us(20);
		if(Txdata&0x80)
			MOSI_H;
		else MOSI_L;
		Txdata<<=1;
		delay_us(20);
    	
    SCLK_H;
	  delay_us(40);				
		
	}
	SCLK_L;
	delay_us(10);	
	CS_H;
}


u16 db_calc(float db)
{
	u16 db_reg;
	db_reg=0xfc;
	if(db>=16)
	{
		db_reg=db_reg&(0x7c);
	  db=db-16;
	}
	if(db>=8)
	{
		db_reg=db_reg&(0xbc);
		db=db-8;
	}
	if(db>=4)
	{
		db_reg=db_reg&(0xdc);
		db=db-4;
	}
	if(db>=2)
	{
		db_reg=db_reg&(0xec);
		db=db-2;
	}
	if(db>=1)
	{
		db_reg=db_reg&(0xf4);
		db=db-1;
	}
	if(db>=0.5)
	{
		db_reg=db_reg&(0xf8);
		db=db-0.5;
	}
	
	return (db_reg);
}






