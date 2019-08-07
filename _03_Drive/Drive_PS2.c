/*
*********************************************************************************************************
*                                              _03_OS
* File			 : Drive_PS2.c
* By  			 : 
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : PS2键盘驱动程序
*********************************************************************************************************
*/
#include "Drive_PS2.h"

/* 私有宏 ------------------------------------------------------------------------------------*/
#define PS2_SCL 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)
#define PS2_SDA   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)

#define PS2_Count   10000
/* 私有变量 ----------------------------------------------------------------------------------*/
__IO uint32_t Count = PS2_Count;
/* 私有结构体 --------------------------------------------------------------------------------*/

/* 私有函数声明 ------------------------------------------------------------------------------*/
static void PS2_SCL_Set(uint8_t Mode);
static uint8_t PS2_SCL_Wait(uint8_t Mode);
static void PS2_GPIO_Init(void);
//static void TIM4_Configuration(void);
/* 私有函数模型 ------------------------------------------------------------------------------*/

/**********************************************************************************************
 *名    称：void PS2_Keyboard_Init(void)
 *
 *参    数：无
 *
 *返 回 值：无
 *
 *描    述：PS2键盘初始化
 *********************************************************************************************/
void PS2_Keyboard_Init(void)
{
	PS2_GPIO_Init(); 
//	TIM4_Configuration();
}
/**********************************************************************************************
 *名    称：void PS2_GPIO_Init(void)
 *
 *参    数：无
 *
 *返 回 值：无
 *
 *描    述：PS2 IO口初始化
 *********************************************************************************************/
static void PS2_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/**********************************************************************************************
 *名    称：static void PS2_SCL_Set(uint8_t Mode)
 *
 *参    数：uint8_t Mode    1：输出   0：输入
 *
 *返 回 值：无
 *
 *描    述：SCL IO端口状态设置
 *********************************************************************************************/
static void PS2_SCL_Set(uint8_t Mode)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	if(Mode)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	}
	else
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	}
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	if(Mode)
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);
}
/**********************************************************************************************
 *名    称：void PS2_ReadKeyCodon(volatile uint8_t* Key_Num)
 *
 *参    数：volatile uint8_t* Key_Num  键码接收缓冲区指针
 *
 *返 回 值：void
 *
 *描    述：PS2 键码读取函数
 *********************************************************************************************/
void PS2_ReadKeyCodon(volatile uint8_t* Key_Num)
{
	uint8_t i;
	PS2_SCL_Set(0);
	if(!PS2_SCL_Wait(0))
	{
		*Key_Num = 0;
		return;
	}
	if(!PS2_SCL_Wait(1))
	{
		*Key_Num = 0;
		return;
	}
	if(!PS2_SCL_Wait(0))
	{
		*Key_Num = 0;
		return;
	}
	for(i = 0;i < 8;i++)
	{
		*Key_Num >>= 1;
		if(PS2_SDA)
			*Key_Num |= 0x80;
		if(!PS2_SCL_Wait(1))
		{
			*Key_Num = 0;
			return;
		}
		if(!PS2_SCL_Wait(0))
		{
			*Key_Num = 0;
			return;
		}
	}
	if(!PS2_SCL_Wait(1))
	{
		*Key_Num = 0;
		return;
	}
	if(!PS2_SCL_Wait(0))
	{
		*Key_Num = 0;
		return;
	}
	if(!PS2_SCL_Wait(1))
	{
		*Key_Num = 0;
		return;
	}
	PS2_SCL_Set(1);
}

/**********************************************************************************************
 *名    称：static uint8_t PS2_SCL_Wait(uint8_t Mode)
 *
 *参    数：uint8_t Mode  1等待上升沿   0等待下降沿
 *
 *返 回 值：1等待成功  0等待超时
 *
 *描    述：PS2 SCL等待跳变
 *********************************************************************************************/
static uint8_t PS2_SCL_Wait(uint8_t Mode)
{
	if(Mode)
	{
		while((!PS2_SCL) && Count)
		Count--;
		if(!Count)
		{
			Count = PS2_Count;
			PS2_SCL_Set(1);
			return 0;
		}
		else
			return 1;
	}
	else
	{
		while(PS2_SCL && Count)
		Count--;
		if(!Count)
		{
			Count = PS2_Count;
			PS2_SCL_Set(1);
			return 0;
		}
		else
			return 1;
	}
}

/**********************************************************************************************
 *名    称：void TIM4_Configuration(void)
 *
 *参    数：无
 *
 *返 回 值：无
 *
 *描    述：TIM4配置
 *********************************************************************************************/
//static void TIM4_Configuration(void)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM4时钟
//	
//     TIM_TimeBaseInitStructure.TIM_Period = 1000-1; 	//自动重装载值
//	TIM_TimeBaseInitStructure.TIM_Prescaler=840-1;  //定时器分频
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	
//	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化TIM4
//	
//	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器4更新中断
//	TIM_Cmd(TIM4,ENABLE); //使能定时器4
//	
//	NVIC_InitStructure.NVIC_IRQChannel= TIM4_IRQn; //定时器4中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //抢占优先级3级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 1; //子优先级1级
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//}
/**********************************************************************************************
 *名    称：void TIM4_IRQHandler(void)
 *
 *参    数：无
 *
 *返 回 值：无
 *
 *描    述：TIM4中断服务函数
 *********************************************************************************************/
//void TIM4_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
//	{
//		
//		Key_StateSweep(&Key_FSM_PS2);  
//	}
//	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
//}
/* 文件结束 ---------------------------------------------------------------------------------*/
/**********************************************************************************************
 *   文 件 名：Key_State.c
 *
 *   调试平台：理工竞赛STM32开发板
 *
 *   作    者：唐 江
 *
 *   日    期：2015.7.17
 **********************************************************************************************/

/* 包含 --------------------------------------------------------------------------------------*/
/* 私有宏 ------------------------------------------------------------------------------------*/
#define OPEN_CODE         0xf0   //断码
#define LONG_KEY_COUNT    3      //长按键计数
/* 私有变量 ----------------------------------------------------------------------------------*/

/* 私有结构体 --------------------------------------------------------------------------------*/
__KEY_StateMachine  Key_FSM_PS2;
/* 私有函数声明 ------------------------------------------------------------------------------*/

/* 私有函数模型 ------------------------------------------------------------------------------*/

/**********************************************************************************************
 *名    称：void Key_StateSweep(__KEY_StateMachine *Key_FSM)
 *
 *参    数：__KEY_StateMachine *Key_FSM  状态机结构体指针
 *
 *返 回 值：void
 *
 *描    述：状态机扫描函数
 *********************************************************************************************/
void Key_StateSweep(__KEY_StateMachine *Key_FSM)
{
	PS2_ReadKeyCodon(&Key_FSM ->Key_Code_Pre);
	if(Key_FSM ->Key_Code_Pre == 0)
	{
		Key_FSM ->Key_Filt++;
		if(Key_FSM ->Key_Filt > 50)
		{
			Key_FSM ->Key_Filt = 0;
			Key_FSM ->State_Flag = 0;
		}
		return;
	}
	else
		Key_FSM ->Key_Filt = 0;
	switch(Key_FSM ->State_Flag)
	{
		case 0:
			if((Key_FSM ->Key_Code_Pre != 0) && (Key_FSM ->Key_Code_Pre != OPEN_CODE))
			{
				Key_FSM ->Key_Code_Bef = Key_FSM ->Key_Code_Pre;
				Key_FSM ->State_Flag = 1;
				Key_FSM ->Key_Mode = 1;
			}
			else
			{
				Key_FSM ->Key_Code_Bef = 0;
				Key_FSM ->Key_Mode = 0;
				Key_FSM ->State_Flag = 0;
			}
			break;
			
		case 1:
			if(Key_FSM ->Key_Code_Bef == Key_FSM ->Key_Code_Pre)
			{
				Key_FSM ->State_Flag = 1;   //停留在第二级计数
				Key_FSM ->Key_Count++;
				if(Key_FSM ->Key_Count == LONG_KEY_COUNT)
				{
					Key_FSM ->State_Flag = 2;
					Key_FSM ->Key_Count = 0;
					Key_FSM ->Key_Mode = 2;  //独立长按键
				}
			}
			else if(Key_FSM ->Key_Code_Pre == OPEN_CODE)
			{
				Key_FSM ->Key_Code_Bef = Key_FSM ->Key_Code_Pre;
				Key_FSM ->State_Flag = 1;
			}
			else
			{
				Key_FSM ->Key_Count = 0;
				Key_FSM ->State_Flag = 0;
				Key_FSM ->Key_Code_Bef = 0;
			}
			break;
			
		case 2:
			if(Key_FSM ->Key_Code_Bef == Key_FSM ->Key_Code_Pre)
			{
					Key_FSM ->Key_Mode = 3;   //独立持续按键
			}
			else if(Key_FSM ->Key_Code_Pre == OPEN_CODE)
			{
				Key_FSM ->Key_Code_Bef = Key_FSM ->Key_Code_Pre;
				Key_FSM ->State_Flag = 2;
			}
			else
			{
				Key_FSM ->Key_Mode = 0;
				Key_FSM ->State_Flag = 0;
			}
			break;
	}
}

