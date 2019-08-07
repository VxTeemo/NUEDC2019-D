/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_ADC.c
* By  			 : 
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : ADC配置程序
*********************************************************************************************************
*/
#include "Drive_ADC.h" 

/* 全局函数编写 --------------------------------------------------------------*/
/**----------------------------------------------------------------------------
* @FunctionName  : ADC1_Init()
* @Description   : None
* @Data          : 2016/7/11
* @Explain       : None
------------------------------------------------------------------------------*/
void ADC1_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef       ADC_InitStructure;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
	
    //先初始化ADC1通道1 IO口
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//PA1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);	//ADC1复位
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);	//复位结束
	
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //关闭DMA使能
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//预分频2分频。//ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
    ADC_CommonInit(&ADC_CommonInitStructure);//初始化
		
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式	
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
    ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1
    ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
 
    ADC_Cmd(ADC1, ENABLE);//开启AD转换器	

}

/**----------------------------------------------------------------------------
* @FunctionName  : ADC2_Init()
* @Description   : ADC2初始化程序
* @Data          : 2016/7/24
* @Explain       : 用于音频采集
------------------------------------------------------------------------------*/
//void ADC2_Init(void)
//{
//		GPIO_InitTypeDef  GPIO_InitStructure;
//    ADC_CommonInitTypeDef ADC_CommonInitStructure;
//    ADC_InitTypeDef       ADC_InitStructure;
//	
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); //使能ADC1时钟
//	
//    //先初始化ADC1通道1 IO口
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PA
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
//    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
//	
//    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, ENABLE);	//ADC1复位
//    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, DISABLE);	//复位结束
//	
//    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
//    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
//    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //关闭DMA使能
//    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;//预分频2分频。//ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
//    ADC_CommonInit(&ADC_CommonInitStructure);//初始化
//		
//    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式	
//    ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式
//    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
//    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
//    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
//    ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1
//    ADC_Init(ADC2, &ADC_InitStructure);//ADC初始化
// 
//    ADC_Cmd(ADC2, ENABLE);//开启AD转换器	

//}

void ADC2_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef       ADC_InitStructure;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); //使能ADC2时钟
	
    //先初始化ADC2通道1 IO口
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PA2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, ENABLE);	//ADC1复位
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, DISABLE);	//复位结束
	
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //关闭DMA使能
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//预分频2分频。//ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
    ADC_CommonInit(&ADC_CommonInitStructure);//初始化
		
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式	
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
    ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1
    ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
 
    ADC_Cmd(ADC1, ENABLE);//开启AD转换器	

}
 
/**----------------------------------------------------------------------------
* @FunctionName  : ADC_Result_Get()
* @Description   : 获得ADC值
* @Data          : 2016/7/24
* @Explain       : 
------------------------------------------------------------------------------- 
Channel:  ADC_Channel_0~ADC_Channel_18 (填0~18也行) 
Speed  :  0~7(越小越快)
返回值:转换结果
------------------------------------------------------------------------------*/
u16 ADC_Result_Get(u8 Channel,u8 Speed)
{  
		ADC_TypeDef* ADCx = ADC1;
		if(Channel == 2) 
				ADCx = ADC2;
		else if(Channel == 3) 
				ADCx = ADC3;	 
		if(Speed > 7) 
				Speed = 7; 
    ADC_RegularChannelConfig(ADCx,Channel, 1, Speed );	 
    ADC_SoftwareStartConv(ADCx);		                    //使能指定的ADC1的软件转换启动功能 
    while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC ));     //等待转换结束 
    return ADC_GetConversionValue(ADCx);	              //返回最近一次ADC1规则组的转换结果
}

/**----------------------------------------------------------------------------
* @FunctionName  : Get_ADC_Average()
* @Description   : 求多次ADC采集的平均值
* @Data          : 2016/7/24
* @Explain       :
-------------------------------------------------------------------------------
获取通道ch的转换值，取times次,然后平均
Channel:通道编号
Times:获取次数
返回值:通道ch的times次转换结果平均值
------------------------------------------------------------------------------*/
u16 ADC_Average_Get(u8 Channel, u8 Times)
{
    u32 Result = 0; 
		while(Times--)
    {
        Result += ADC_Result_Get(Channel,3);
		    Result/=2;
        //delay_ms(1);
    }
    return Result;
}

/**----------------------------------------------------------------------------
* @FunctionName  : ADC_Waveform_Get()     	
* @Description   : 波形提取
* @Data          : 2016/8/17	
* @Explain       : None	
-------------------------------------------------------------------------------
SourceBuff :源数据数组
SourceLenth：源数据长度
ResultBuff：输出据数组
ResultLenth：输出数据长度
return:0失败   其他：取样点数
------------------------------------------------------------------------------*/
u16 ADC_Waveform_Get(u16 *SourceBuff,u16 SourceLenth,double *ResultBuff,u16 ResultLenth)
{
		double ZoomX = 0.0f,SourceX = 0.0f,SourceDecimX = 0.0f;
		u16 i = 0,StartX = 0,EndX = 0;
		u16 Max = 0,Min = 0xffff,Vpp = 0;   
	 
		/* 取个周期 -------------------------------------*/
		for(i = 0;i< SourceLenth;i++)
		{
				if(SourceBuff[i] > 4096) 
						return 0;              //DMA传输溢出  			 
				if(Max < SourceBuff[i]) 
					  Max = SourceBuff[i];
				if(Min > SourceBuff[i]) Min = SourceBuff[i]; 
		}	  
		Vpp = Max - Min; 
		i = SourceLenth/100;//去掉前面1%的不稳定数据
		for(;i< SourceLenth;i++)
				if(SourceBuff[i] + Min > Vpp - Vpp/10) break;
		for(;i< SourceLenth;i++)
				if(SourceBuff[i] + Min  < Vpp/2) break;
		StartX = i; 
		for(;i< SourceLenth;i++)
				if(SourceBuff[i] + Min  > Vpp - Vpp/10) break;
		for(;i< SourceLenth;i++)
				if(SourceBuff[i] + Min  < Vpp/2) break;
		EndX = i;		   
		ZoomX = (double)(EndX - StartX)/ResultLenth;
	
		for(i = 0;i < ResultLenth;i ++)
		{ 
				SourceX = (double)StartX+(i*ZoomX);
				SourceDecimX = SourceX - (u16)SourceX;//取小数	 
				if(SourceX < SourceLenth-1)
				{
						ResultBuff[i]=(SourceBuff[(u16)SourceX]+((double)SourceBuff[(u16)SourceX + 1]-(double)SourceBuff[(u16)SourceX])*SourceDecimX)*3.3f/4096.0f;//连续化 
				} 	
		}   
	return EndX - StartX;
}

void  Adc_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟

  //先初始化ADC1通道5 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//PA5 通道5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE); //使能ADC1时钟 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);  //复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
	
 
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	

}				  
//获得ADC值
//ch: @ref ADC_channels 
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
u16 Get_Adc(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}
//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 








