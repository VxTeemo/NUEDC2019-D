/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_PWM.c
* By  			 : 
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : PWM配置程序
*********************************************************************************************************
*/
#include "Drive_PWM.h"

/* 私有宏定义 ----------------------------------------------------------------*/
#define PWM1_FREQUENCY 	10000//PWM1输出的频率，单位Hz
#define PWM1_PERIOD 	(105000.0f/PWM1_FREQUENCY)

/* 私有（静态）函数声明 ------------------------------------------------------*/

/* 全局变量定义 --------------------------------------------------------------*/
 
/* 全局函数编写 --------------------------------------------------------------*/
/**----------------------------------------------------------------------------
* @FunctionName  : PWM1_Init()     
* @Description   : None 
* @Data          : 2016/7/11
* @Explain       : 超声波测距
------------------------------------------------------------------------------*/  
void PWM1_Init(u16 arr,u16 psc)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1时钟使能    	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//PWM端口复用   

		GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); 
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1);//复用定时器
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;        
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       //复用功能
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度100MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //上拉
		GPIO_Init(GPIOA,&GPIO_InitStructure);    
		GPIO_SetBits(GPIOA,GPIO_Pin_8); 
		GPIO_SetBits(GPIOA,GPIO_Pin_9); 
		
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //死区控制用。
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数器方向
		TIM_TimeBaseStructure.TIM_Prescaler =0;   //预分频Timer clock = sysclock /(TIM_Prescaler+1) = 168M
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseStructure.TIM_Period = arr;    //转载值Period = (TIM counter clock / TIM output clock) - 1 = 20K
		TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);

	
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;     //BUCK  OC1
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;                   //比较值一定要为零
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;     //BOOST  OC2
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_OC2Init(TIM1, &TIM_OCInitStructure);
		
		TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
		TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR2上的预装载寄存器
		
		TIM_ARRPreloadConfig(TIM1,ENABLE);
	
		
		TIM_Cmd(TIM1, ENABLE);  //使能TIM1
		
		TIM_CtrlPWMOutputs(TIM1,ENABLE);  
}
	
/**----------------------------------------------------------------------------
* @FunctionName  : PWM1_CCR_Set()     
* @Description   : 设置PWM1占空比	
* @Data          : 2016/7/11
* @Explain       : None
------------------------------------------------------------------------------*/  	 
void PWM1_CCR_Set(double xccr1)
{	
	xccr1 = xccr1*PWM1_PERIOD;	 			
	assert_param(IS_TIM_LIST1_PERIPH(TIM14));		
	TIM14->CCR1 = (u16)xccr1; 		
}	
	
	
	
	
/**----------------------------------------------------------------------------
* @FunctionName  : PWM2_Init()     
* @Description   : PWM通道2
* @Data          : 2016/7/11
* @Explain       : PWM_Cap2捕获     PA1复用位定时器5
------------------------------------------------------------------------------*/  	 
void PWM2_Init(void)
{
	
	TIM_ICInitTypeDef  TIM5_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //GPIOA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA1

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); //PA1复用位定时器5
  
	  
	TIM_TimeBaseStructure.TIM_Prescaler=168-1;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=1000000-1;  //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	

	//初始化TIM5输入捕获参数
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC2映射到TI1上
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC2,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
	TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5

 
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}




