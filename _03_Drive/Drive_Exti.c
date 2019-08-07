/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_Exti.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : 外部中断配置程序
*********************************************************************************************************
*/
#include "Drive_Exti.h"

/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有（静态）函数声明 ------------------------------------------------------*/

/* 全局变量定义 --------------------------------------------------------------*/
 
/* 全局函数编写 --------------------------------------------------------------*/
/**----------------------------------------------------------------------------
* @FunctionName  : Exti0_Init()     
* @Description   : None 
* @Data          : 2016/7/11
* @Explain       : 超声波测距
------------------------------------------------------------------------------*/ 
void Exti0_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOA,GPIOE时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE0,2
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource0);//PE0 连接到中断线0  
  
	/* 配置EXTI_Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道 
	 
	NVIC_Init(&NVIC_InitStructure);//配置
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位    


	
}

/**----------------------------------------------------------------------------
* @FunctionName  : Exti1_Init()     
* @Description   : None 
* @Data          : 2016/7/11
* @Explain       : None
------------------------------------------------------------------------------*/ 
void Exti1_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOA,GPIOE时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE0,2
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);//PE0 连接到中断线0  
  
	/* 配置EXTI_Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道 
	 
	NVIC_Init(&NVIC_InitStructure);//配置
	EXTI_ClearITPendingBit(EXTI_Line1); //清除LINE0上的中断标志位    
	
}

/**----------------------------------------------------------------------------
* @FunctionName  : Exti2_Init()     
* @Description   : None 
* @Data          : 2016/7/11
* @Explain       : None
------------------------------------------------------------------------------*/ 
void Exti2_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOA,GPIOE时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE0,2
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource2);//PE0 连接到中断线0  
  
	/* 配置EXTI_Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道 
	 
	NVIC_Init(&NVIC_InitStructure);//配置
	EXTI_ClearITPendingBit(EXTI_Line2); //清除LINE0上的中断标志位    
	
}

/**----------------------------------------------------------------------------
* @FunctionName  : Exti3_Init()     
* @Description   : None 
* @Data          : 2016/7/11
* @Explain       : None
------------------------------------------------------------------------------*/ 
void Exti3_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOA,GPIOE时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE0,2
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource3);//PE0 连接到中断线0  
  
	/* 配置EXTI_Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道 
	 
	NVIC_Init(&NVIC_InitStructure);//配置
	EXTI_ClearITPendingBit(EXTI_Line3); //清除LINE0上的中断标志位    
	
}

/**----------------------------------------------------------------------------
* @FunctionName  : Exti4_Init()     
* @Description   : None
* @Data          : 2016/7/11
* @Explain       : None
------------------------------------------------------------------------------*/ 
void Exti4_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIO时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIO
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);// 连接到中断线 
  
	/* 配置EXTI_Line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道 
	 
	NVIC_Init(&NVIC_InitStructure);//配置
	EXTI_ClearITPendingBit(EXTI_Line4); //清除LINE上的中断标志位    
	
}

/**----------------------------------------------------------------------------
* @FunctionName  : Exti5_9_Init()     
* @Description   : None
* @Data          : 2016/7/11
* @Explain       : None
------------------------------------------------------------------------------*/ 
void Exti5_9_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIO时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource5);// 连接到中断线 
  
	/* 配置EXTI_Line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道 
	 
	NVIC_Init(&NVIC_InitStructure);//配置
	EXTI_ClearITPendingBit(EXTI_Line5); //清除LINE上的中断标志位    
	
}

/**----------------------------------------------------------------------------
* @FunctionName  : Exti10_15_Init()     
* @Description   : None
* @Data          : 2016/7/11
* @Explain       : None
------------------------------------------------------------------------------*/ 
void Exti10_15_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIO时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIO
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource10);// 连接到中断线 
  
	/* 配置EXTI_Line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道 
	 
	NVIC_Init(&NVIC_InitStructure);//配置
	EXTI_ClearITPendingBit(EXTI_Line10); //清除LINE上的中断标志位    
	
}


void Exti_Enable(u8 Exti_Num)
{
	EXTI_InitTypeDef   EXTI_InitStructure; 
	/* 配置EXTI_Line */  
	if(Exti_Num == 0) 			EXTI_InitStructure.EXTI_Line = EXTI_Line0; 
	else if(Exti_Num == 1) 		EXTI_InitStructure.EXTI_Line = EXTI_Line1; 
	else if(Exti_Num == 2) 		EXTI_InitStructure.EXTI_Line = EXTI_Line2; 
	else if(Exti_Num == 3) 		EXTI_InitStructure.EXTI_Line = EXTI_Line3; 
	else if(Exti_Num == 4) 		EXTI_InitStructure.EXTI_Line = EXTI_Line4; 
	else if(Exti_Num == 5)  	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	else if(Exti_Num == 6)  	EXTI_InitStructure.EXTI_Line = EXTI_Line6; 
	else if(Exti_Num == 7)  	EXTI_InitStructure.EXTI_Line = EXTI_Line7; 
	else if(Exti_Num == 8)  	EXTI_InitStructure.EXTI_Line = EXTI_Line8; 
	else if(Exti_Num == 9)  	EXTI_InitStructure.EXTI_Line = EXTI_Line9; 
	else if(Exti_Num == 10)  	EXTI_InitStructure.EXTI_Line = EXTI_Line10; 
	else if(Exti_Num == 11)  	EXTI_InitStructure.EXTI_Line = EXTI_Line11; 	
	else if(Exti_Num == 12)  	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	else if(Exti_Num == 13)  	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	else if(Exti_Num == 14)  	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	else if(Exti_Num == 15)  	EXTI_InitStructure.EXTI_Line = EXTI_Line15;	
	else return; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
	
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能 
	EXTI_Init(&EXTI_InitStructure);//配置
		
}


void Exti_Disable(u8 Exti_Num)
{ 
	EXTI_InitTypeDef   EXTI_InitStructure; 
	/* 配置EXTI_Line */
	if(Exti_Num == 0) 			EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	else if(Exti_Num == 1)		EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	else if(Exti_Num == 2) 		EXTI_InitStructure.EXTI_Line = EXTI_Line2;	
	else if(Exti_Num == 3) 		EXTI_InitStructure.EXTI_Line = EXTI_Line3;	
	else if(Exti_Num == 4) 		EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	else if(Exti_Num == 5)  	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	else if(Exti_Num == 6)  	EXTI_InitStructure.EXTI_Line = EXTI_Line6; 
	else if(Exti_Num == 7)  	EXTI_InitStructure.EXTI_Line = EXTI_Line7; 
	else if(Exti_Num == 8)  	EXTI_InitStructure.EXTI_Line = EXTI_Line8; 
	else if(Exti_Num == 9)  	EXTI_InitStructure.EXTI_Line = EXTI_Line9; 
	else if(Exti_Num == 10)  	EXTI_InitStructure.EXTI_Line = EXTI_Line10; 
	else if(Exti_Num == 11)  	EXTI_InitStructure.EXTI_Line = EXTI_Line11; 	
	else if(Exti_Num == 12)  	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	else if(Exti_Num == 13)  	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	else if(Exti_Num == 14)  	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	else if(Exti_Num == 15)  	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	else return; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
	
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;//中断线使能 
	EXTI_Init(&EXTI_InitStructure);//配置
}





