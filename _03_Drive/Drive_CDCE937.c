/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_CDCE937.c
* By  			 : 
* platform   : STM32F407ZG
*	Data   		 : 2018/7/20
* function 	 : CDCE937驱动程序
*********************************************************************************************************
*/
#include "Drive_CDCE937.h"


/* 私有定义宏-------------------------------------------------------------------------------------*/
#define I2C_Speed              	50000	//I2C通信速度	
#define I2C_SLAVE_ADDRESS7     	0x3A	  //STM32作为从机的时候的地址
/* 私有变量 --------------------------------------------------------------------------------------*/
//CDCE937寄存器配置值，应用TI ClockPro仿真软件可快速得出
const uint8_t  Generic_register[7]={0x81,0x1,0xb4,0x8,0x2,0x50,0x81};
const uint8_t  PLL1_register[16]={0x0,0x0,0x0,0x0,0x6d,0x2,0x8,0x8,0x6a,0x4a,0xa3,0x4c,0x0,0x40,0x2,0x8};
const uint8_t  PLL2_register[16]={0x0,0x0,0x0,0x0,0x6d,0x2,0x8,0x8,0x6a,0x4a,0xa3,0x4c,0x0,0x40,0x2,0x8};
const uint8_t  PLL3_register[16]={0x0,0x0,0x0,0x0,0x6d,0x2,0x8,0x8,0x6a,0x4a,0xa3,0x4c,0x0,0x40,0x2,0x8};
/* 私有函数模型 ----------------------------------------------------------------------------------*/
static void GPIO_Configuration(void);
static void I2C1_Configuration(void);
/* 私有宏-----------------------------------------------------------------------------------------*/
/***************************************************************************************************
  * 描述   : CDCE937通信初始化
  * 参数   : 无
  * 返回   : 无
  * 说明   :
  *************************************************************************************************/
void CDCE937_Init(void)
{
	GPIO_Configuration();
	I2C1_Configuration();
}
/***************************************************************************************************
  * 描述   : 根据配置参数启动CDCE937
  * 参数   : 无
  * 返回   : 无
  * 说明   :  
  *************************************************************************************************/
void CDCE937_Start(void)
{
	CECE937_WriteRegister((uint8_t*)PLL1_register,0x10,16);
	CECE937_WriteRegister((uint8_t*)PLL2_register,0x20,16);
	CECE937_WriteRegister((uint8_t*)PLL3_register,0x30,16);
	CECE937_WriteRegister((uint8_t*)Generic_register,0x00,7);
}
/***************************************************************************************************
  * 描述   : 写寄存器
  * 参数   : pBuffer->写字节缓冲区  WriteAddr->写起始地址 NumByteToWrite->写字节个数  
  * 返回   : 无
  * 说明   : 向CDCE937寄存器写入数据 
  *************************************************************************************************/
void CECE937_WriteRegister(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite)
{ 
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));//等待总线空闲
	
	I2C_GenerateSTART(I2C1, ENABLE);//发送起始条件
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));//起始条件已发送并应答
	
	I2C_Send7bitAddress(I2C1, CDCE937_ADDRESS, I2C_Direction_Transmitter);//发送地址选择从设备
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//发送结束并应答  
	
	I2C_SendData(I2C1, WriteAddr);//发送写地址
	
	while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));//地址已发送并应答
	
	I2C_SendData(I2C1, NumByteToWrite);//发送块传输长度 
	/* Test on EV8 and clear it */
	while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	while(NumByteToWrite--)  
	{
		I2C_SendData(I2C1, *pBuffer); 	
		pBuffer++; 
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}
	I2C_GenerateSTOP(I2C1, ENABLE);
}
/***************************************************************************************************
  * 描述   : I2C1配置
  * 参数   : 无
  * 返回   : 无
  * 说明   :  
  *************************************************************************************************/
static void I2C1_Configuration(void)
{ 
	I2C_InitTypeDef  I2C_InitStructure; 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
// 	/* Reset I2C1 IP */
//   RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
//   /* Release reset signal of I2C1 IP */
//   RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
	//配置I2C
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;//STM32做为从设备的地址
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
	
	I2C_Cmd(I2C1, ENABLE);				//使能I2C
	I2C_Init(I2C1, &I2C_InitStructure);	//I2C初始化
}
/***************************************************************************************************
  * 描述   : I/0配置
  * 参数   : 无
  * 返回   : 无
  * 说明   : CDCE937通信接口
  *************************************************************************************************/
static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
	/*Configure I2C SCL SDA pin */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}

