/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_Interruput.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : 中断服务函数配置程序
*********************************************************************************************************
*/
#include "Drive_Interrupt.h"

/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有（静态）函数声明 -------------------------------------------------------*/

/* 全局变量定义 --------------------------------------------------------------*/
 
/* 全局函数编写 --------------------------------------------------------------*/

u32 com;
u32 counter=0;
u8 num=0,i=0;
u8 start_flag=0;
u16 DA=0;
u16 T_data=0;
u16 temperature=19;
u32 irdata[14];							// 接收数据位电平时间用于判断01编码
//unsigned char buf[10];

/**----------------------------------------------------------------------------
* @FunctionName  : TIM1_UP_TIM10_IRQHandler()     
* @Description   : 自己根据需要配置中断逻辑 
* @Data          : 2018/7/16
* @Explain       : 
------------------------------------------------------------------------------*/ 
void TIM1_UP_TIM10_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update); //清除中断标志位		 
	}
}

/**----------------------------------------------------------------------------
* @FunctionName  : TIM2_IRQHandler()     
* @Description   : ADF4351扫频切换判断
* @Data          : 2018/7/16
* @Explain       : 
------------------------------------------------------------------------------*/ 
//=========================================================================
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{

		TIM_ClearITPendingBit(TIM2,TIM_IT_Update); //清除中断标志位		 
	}
		
}	


/**----------------------------------------------------------------------------
* @FunctionName  : TIM3_IRQHandler()     
* @Description   : 自己根据需要配置中断逻辑 
* @Data          : 2018/7/16
* @Explain       : 
------------------------------------------------------------------------------*/ 
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update); //清除中断标志位		 
	}
}

/**----------------------------------------------------------------------------
* @FunctionName  : TIM4_IRQHandler()     
* @Description   : 自己根据需要配置中断逻辑 
* @Data          : 2018/7/16
* @Explain       : 通信编码
------------------------------------------------------------------------------*/ 
void TIM4_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET)
	{
		#if SCIRXST
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update); //清除中断标志位		
		
		#else 
		
//		u16 AdData;
		
//		AdData =  ADC_Result_Get(2,5);
		
	//	Code_Data(AdData);
		
	//	DataSend();
		
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update); //清除中断标志位		

		//Send_Flag=0;
		#endif

		
	}
	
	
	//Send_Flag=0;
	
}

/**----------------------------------------------------------------------------
* @FunctionName  : TIM5_IRQHandler()     
* @Description   : 自己根据需要配置中断逻辑 
* @Data          : 2018/7/16
* @Explain       : 通信接收
------------------------------------------------------------------------------*/ 
void TIM5_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM5,TIM_IT_CC3) != RESET)
	{
		#if SCIRXST
		
		static u32 daaaa[25],de=0;
		int i;
		u16 DA_data;
				

		 counter= TIM_GetCounter(TIM5);
		 	TIM5->CNT=0; 
	// 	daaaa[de]=counter;
		de++;
		if(de==24)
			de=0;

		if((counter >=15) && (counter <= 80))
		{
			start_flag = 1;
			T_data=0;
			num=0;
		}
		
		if(1 == start_flag)
		{
			if((counter >= 112) && (counter <=115))
			{
				irdata[num]=1;
				num ++;
			}
			else if((counter >=116) && (counter <=120))
			{	
				irdata[num]=0;	
				num ++;
				
			}			
			if(num >=8)
			{
				start_flag = 0;
				num = 0;
				
				for(i=7;i>=0;i--)
				{
					DA = DA <<1;
					DA = DA +	irdata[i];
				}
				DA = DA <<4;
		//		ADPCM_Decode(&DA,&DA_data,1);
				DAC1_Vol_Set(DA);
				
				T_data=(T_data<<1)|irdata[7];			// 温度信号处理输出
					
				if((T_data>>6)==0x38)
				{
						Get_Data=T_data;
						Get_Flag=0;
				}
				
			}
		
		}
		#endif
		TIM_ClearITPendingBit(TIM5,TIM_IT_CC3|TIM_IT_Update); //清除中断标志位		 
	}
	else if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC3|TIM_IT_Update); //清除中断标志位
	}
	

}

/**----------------------------------------------------------------------------
* @FunctionName  : TIM6_DAC_IRQHandler()     
* @Description   : 自己根据需要配置中断逻辑 
* @Data          : 2018/7/16
* @Explain       :
------------------------------------------------------------------------------*/ 
void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update); //清除中断标志位		 
	}
}

/**----------------------------------------------------------------------------
* @FunctionName  : TIM7_IRQHandler()     
* @Description   : 自己根据需要配置中断逻辑 
* @Data          : 2018/7/16
* @Explain       : 
------------------------------------------------------------------------------*/ 
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update); //清除中断标志位		 
	}
}

/**----------------------------------------------------------------------------
* @FunctionName  : TIM8_UP_TIM13_IRQHandler()     
* @Description   : 自己根据需要配置中断逻辑 
* @Data          : 2018/7/16
* @Explain       : 超声波测距
------------------------------------------------------------------------------*/ 
void TIM8_UP_TIM13_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM8,TIM_IT_Update) != RESET)
	{
//			HC_count++;  
//			if(HC_count==1)
//			{		
//				
//				TIM_SetCompare1(TIM1,0);
//				TIM_SetCompare2(TIM1,4200);
////				GPIO_ResetBits(GPIOD ,GPIO_Pin_11);
//				Exti0_Init();
//				
//			}
	
		TIM_ClearITPendingBit(TIM8,TIM_IT_Update); //清除中断标志位		 
	}

}

/**----------------------------------------------------------------------------
* @FunctionName  : TIM1_BRK_TIM9_IRQHandler()     
* @Description   : 自己根据需要配置中断逻辑
* @Data          : 2018/7/16
* @Explain       : 
------------------------------------------------------------------------------*/ 
void TIM1_BRK_TIM9_IRQHandler(void)   
{
	if(TIM_GetITStatus(TIM9,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM9,TIM_IT_Update); //清除中断标志位		 
	}
}


/**----------------------------------------------------------------------------
* @FunctionName  : TIM1_TRG_COM_TIM11_IRQHandler()     
* @Description   : 自己根据需要配置中断逻辑
* @Data          : 2018/7/16
* @Explain       : 
------------------------------------------------------------------------------*/ 
void TIM1_TRG_COM_TIM11_IRQHandler(void)   
{
	if(TIM_GetITStatus(TIM11,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM11,TIM_IT_Update); //清除中断标志位		 
	}
}

/**----------------------------------------------------------------------------
* @FunctionName  : TIM8_BRK_TIM12_IRQHandler()     
* @Description   : 自己根据需要配置中断逻辑
* @Data          : 2018/7/16
* @Explain       : 
------------------------------------------------------------------------------*/ 
void TIM8_BRK_TIM12_IRQHandler(void)   
{
	if(TIM_GetITStatus(TIM12,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM12,TIM_IT_Update); //清除中断标志位		 
	}
}

/**----------------------------------------------------------------------------
* @FunctionName  : TIM8_TRG_COM_TIM14_IRQHandler()     
* @Description   : 自己根据需要配置中断逻辑
* @Data          : 2018/7/16
* @Explain       : 
------------------------------------------------------------------------------*/ 
void TIM8_TRG_COM_TIM14_IRQHandler(void)   
{
	if(TIM_GetITStatus(TIM14,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM14,TIM_IT_Update); //清除中断标志位		 
	}
}

/**----------------------------------------------------------------------------
* @FunctionName  : EXTI0_IRQHandler()     
* @Description   : 外部中断0服务程序 
* @Data          : 2018/7/16
* @Explain       : 超声波测距
------------------------------------------------------------------------------*/ 
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{ 	
//		Exti0=0;
//		EXTI_DeInit();
//		EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位 

	}

}

/**----------------------------------------------------------------------------
* @FunctionName  : EXTI1_IRQHandler()     
* @Description   : 外部中断1服务程序 
* @Data          : 2018/7/16
* @Explain       : 
------------------------------------------------------------------------------*/ 
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
	{  
		EXTI_ClearITPendingBit(EXTI_Line1); //清除LINE1上的中断标志位 
	}
}
 
/**----------------------------------------------------------------------------
* @FunctionName  : EXTI2_IRQHandler()     
* @Description   : 外部中断2服务程序 
* @Data          : 2018/7/16
* @Explain       : 
------------------------------------------------------------------------------*/ 
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)!=RESET)
	{ 
				 
		EXTI_ClearITPendingBit(EXTI_Line2); //清除LINE2上的中断标志位 
	}
}

/**----------------------------------------------------------------------------
* @FunctionName  : EXTI3_IRQHandler()     
* @Description   : 外部中断3服务程序 
* @Data          : 2018/7/16
* @Explain       : 自己根据需要配置中断逻辑
------------------------------------------------------------------------------*/ 
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)	
	{ 
		EXTI_ClearITPendingBit(EXTI_Line3); //清除LINE3上的中断标志位 
	}
}

/**----------------------------------------------------------------------------
* @FunctionName  : EXTI4_IRQHandler()     
* @Description   : 外部中断4服务程序 
* @Data          : 2018/7/16
* @Explain       : 自己根据需要配置中断逻辑
------------------------------------------------------------------------------*/ 
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)	
	{ 

		EXTI_ClearITPendingBit(EXTI_Line4); //清除LINE3上的中断标志位 
	}
}

/**----------------------------------------------------------------------------
* @FunctionName  : EXTI9_5_IRQHandler()     
* @Description   : 外部中断5-9服务程序，自己根据需要配置中断线
* @Data          : 2018/7/16
* @Explain       : 自己根据需要配置中断逻辑
------------------------------------------------------------------------------*/ 
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5)!=RESET)	
	{ 
					
	 	EXTI_ClearITPendingBit(EXTI_Line5); //清除LINE5上的中断标志位 
	}
}

/**----------------------------------------------------------------------------
* @FunctionName  : EXTI15_10_IRQHandler()     
* @Description   : 外部中断10-15服务程序，自己根据需要配置中断线
* @Data          : 2018/7/16
* @Explain       : 自己根据需要配置中断逻辑
------------------------------------------------------------------------------*/ 
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line10)!=RESET)	
	{ 
		EXTI_ClearITPendingBit(EXTI_Line10); //清除LINE10上的中断标志位 
	}
}















