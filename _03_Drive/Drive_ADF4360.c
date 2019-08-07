/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_header.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : ADF4360驱动程序
*********************************************************************************************************
*/
#include "Drive_ADF4360.h"



/*
*************************************************************************
*
*	功能	：	初始化ADF4360
*	参数	：	
*
**************************************************************************
*/
void ADF4360_Init(void)
{
	
	GPIO_InitTypeDef   GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	/* Configure PG0 pin as ouput */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

	AD4360_CLK_0;
	AD4360_DAT_0;
	AD4360_LE_0;
}

void ADF4360_2_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	/* Configure PG0 pin as ouput */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

	AD4360_2_CLK_0;
	AD4360_2_DAT_0;
	AD4360_2_LE_0;
}


/*
*************************************************************************
*
*	功能	：	模拟SPI数据写入
*	参数	：	1：写入数据
*
**************************************************************************
*/
static void ADF4360_InputData(unsigned long int data)
{ 
	int i;
	unsigned long int temp=0x800000;
	

	AD4360_LE_0;
	
	for(i=0;i<24;i++)
	{
		if(data&temp)
			AD4360_DAT_1;
		else 
			AD4360_DAT_0;
		
	
		AD4360_CLK_0;
			Delay_ms(2);
		AD4360_CLK_1;
		Delay_ms(2);
		
		temp = temp>>1;
	}
	AD4360_LE_1;
	Delay_ms(10);
}

static void ADF4360_2_InputData(unsigned long int data)
{ 
	int i;
	unsigned long int temp=0x800000;
	

	AD4360_2_LE_0;
	
	for(i=0;i<24;i++)
	{
		if(data&temp)
			AD4360_2_DAT_1;
		else 
			AD4360_2_DAT_0;
		
	
		AD4360_2_CLK_0;
			Delay_ms(2);
		AD4360_2_CLK_1;
		Delay_ms(2);
		
		temp = temp>>1;
	}
	AD4360_2_LE_1;
	Delay_ms(10);
}

/*
*************************************************************************
*
*	功能	：	设置频率输出
*	参数	：	1：设置的输出频率
						2：MUXOUT脚输出设置
*
**************************************************************************
*/

void ADF4360_SetFreq(unsigned long int Freq,unsigned int Muxout)
{
	unsigned long int Rdata,Ndata,Cdata,NCounter;
	
	NCounter=Freq/200000;
	
	Rdata =0x3001E1;											//(RCounter<<2)  | 0x300001;
	Cdata =(Muxout<<5)   | 0x0FF104;			//0x0DB124;
	Ndata =(NCounter<<8) | 0x000002;			//0x15402;

													  

	ADF4360_InputData(Rdata);//R
	
  ADF4360_InputData(Cdata);//Init Latch		

	ADF4360_InputData(Ndata);//N	 	
	
}

void ADF4360_2_SetFreq(unsigned long int Freq,unsigned int Muxout)
{
	unsigned long int Rdata,Ndata,Cdata,NCounter;
	
	NCounter=Freq/200000;
	
	Rdata =0x3001E1;											//(RCounter<<2)  | 0x300001;
	Cdata =(Muxout<<5)   | 0x0FF104;			//0x0DB124;
	Ndata =(NCounter<<8) | 0x000002;			//0x15402;

													  

	ADF4360_2_InputData(Rdata);//R
	
  ADF4360_2_InputData(Cdata);//Init Latch		

	ADF4360_2_InputData(Ndata);//N	 	
}



