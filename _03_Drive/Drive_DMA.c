/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_DMA.c
* By  			 : 
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : DMA≈‰÷√≥Ã–Ú
*********************************************************************************************************
*/
#include "Drive_DMA.h"   
#define  ADC_PRECISION 			ADC_Resolution_12b 

/* ÀΩ”–∫Í∂®“Â ----------------------------------------------------------------*/ 

/* ÀΩ”–£®æ≤Ã¨£©∫Ø ˝…˘√˜ ------------------------------------------------------*/
 
/* »´æ÷±‰¡ø∂®“Â --------------------------------------------------------------*/ 
u16  ADC1_DMA2_Buff[ADC1_DMA2_LENTH] = {0};		 
u16  ADC2_DMA2_Buff[ADC2_DMA2_LENTH] = {0};		
u16  ADC3_DMA2_Buff[ADC3_DMA2_LENTH] = {0};	
u16  DAC1_DMA1_Buff[DAC1_DMA1_LENTH] = {0};		

/* »´æ÷∫Ø ˝±‡–¥ --------------------------------------------------------------*/
/**----------------------------------------------------------------------------
* @FunctionName  : ADC1_DMA2_ReLoad()    	 
* @Description   : None 	
* @Data          : 2016/5/24	
* @Explain       : Speed: 0 ~ 7 π≤8∏ˆµµŒª£¨ ˝÷µ‘Ω–°≤…ºØÀŸ∂»‘ΩøÏ	
------------------------------------------------------------------------------*/ 	
void ADC1_DMA2_Reload()
{ 
	DMA_InitTypeDef 	  DMA_InitStructure;  
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	GPIO_InitTypeDef  	  GPIO_InitStructure;
	
	/* DMA≈‰÷√ ------------------------------------*/ 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2 ±÷” πƒ‹ 
	DMA_DeInit(DMA2_Stream0);
	while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}//µ»¥˝DMAø…≈‰÷√ 
		 
	/* ≈‰÷√ DMA Stream */
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;  //Õ®µ¿—°‘Ò
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;//DMAÕ‚…Ëµÿ÷∑
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC1_DMA2_Buff;//DMA ¥Ê¥¢∆˜0µÿ÷∑
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//Õ‚…ËµΩ¥Ê¥¢∆˜ƒ£ Ω
	DMA_InitStructure.DMA_BufferSize = ADC1_DMA2_LENTH;// ˝æ›¥´ ‰¡ø 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//Õ‚…Ë∑«‘ˆ¡øƒ£ Ω
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//¥Ê¥¢∆˜‘ˆ¡øƒ£ Ω
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//Õ‚…Ë ˝æ›≥§∂»:16Œª
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//¥Ê¥¢∆˜ ˝æ›≥§∂»:16Œª
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//  π”√—≠ª∑ƒ£ Ω DMA_Mode_Normal DMA_Mode_Circular
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//÷–µ»”≈œ»º∂
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//¥Ê¥¢∆˜Õª∑¢µ•¥Œ¥´ ‰
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//Õ‚…ËÕª∑¢µ•¥Œ¥´ ‰
	DMA_Init(DMA2_Stream0,&DMA_InitStructure);//≥ı ºªØDMA Stream
	  
	DMA_SetCurrDataCounter(DMA2_Stream0,ADC1_DMA2_LENTH);// ˝æ›¥´ ‰¡ø   
	DMA_Cmd(DMA2_Stream0, ENABLE);                      //ø™∆ÙDMA¥´ ‰ 	
	
	/* GPIO≈‰÷√ -----------------------------------*/  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);// πƒ‹GPIOA ±÷”  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//PA1 Õ®µ¿1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ƒ£ƒ‚ ‰»Î
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//≤ª¥¯…œœ¬¿≠
	GPIO_Init(GPIOA, &GPIO_InitStructure);//≥ı ºªØ 
	
	/* ADC≈‰÷√ ------------------------------------*/ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE); // πƒ‹ADC1 ±÷” 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1∏¥Œª
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);  //∏¥ŒªΩ· ¯	 
  
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//∂¿¡¢ƒ£ Ω
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;//¡Ω∏ˆ≤…—˘Ω◊∂Œ÷Æº‰µƒ—”≥Ÿ5∏ˆ ±÷”
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA 
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//ADC_Prescaler_Div8;//‘§∑÷∆µ4∑÷∆µ°£ADCCLK=PCLK2/4=84/4=21Mhz,ADC ±÷”◊Ó∫√≤ª“™≥¨π˝36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);//≥ı ºªØ
		
	ADC_InitStructure.ADC_Resolution = ADC_PRECISION;//12Œªƒ£ Ω
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//…®√Ëƒ£ Ω	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//¡¨–¯◊™ªª
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//Ω˚÷π¥•∑¢ºÏ≤‚£¨ π”√»Ìº˛¥•∑¢
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//”“∂‘∆Î	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1∏ˆ◊™ªª‘⁄πÊ‘Ú–Ú¡–÷– “≤æÕ «÷ª◊™ªªπÊ‘Ú–Ú¡–1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC≥ı ºªØ 
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_3Cycles);  
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE); 
	ADC_DMACmd(ADC1,ENABLE);	
	ADC_Cmd(ADC1,ENABLE);//ø™∆ÙAD◊™ªª∆˜	
	ADC_SoftwareStartConv(ADC1); //»Ìº˛∆Ù∂Ø≤…ºØ   
	
}	

/**----------------------------------------------------------------------------
* @FunctionName  : ADC3_DMA2_Init()    	 
* @Description   : None 	
* @Data          : 2016/8/16	
* @Explain       : Speed: 0 ~ 7 π≤8∏ˆµµŒª£¨ ˝÷µ‘Ω–°≤…ºØÀŸ∂»‘ΩøÏ	
------------------------------------------------------------------------------*/ 	

void ADC3_DMA2_Init(void)
{ 
	uint32_t ADC_SampleTime = ADC_SampleTime_3Cycles;
	DMA_InitTypeDef 	  DMA_InitStructure;  
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	GPIO_InitTypeDef  	  GPIO_InitStructure;
  
	/* DMA≈‰÷√ ------------------------------------*/ 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2 ±÷” πƒ‹ 
	DMA_DeInit(DMA2_Stream1);
	while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}//µ»¥˝DMAø…≈‰÷√ 
		 
	/* ≈‰÷√ DMA Stream */
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;  //Õ®µ¿—°‘Ò
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC3->DR;//DMAÕ‚…Ëµÿ÷∑
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC3_DMA2_Buff;//DMA ¥Ê¥¢∆˜0µÿ÷∑
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//Õ‚…ËµΩ¥Ê¥¢∆˜ƒ£ Ω
	DMA_InitStructure.DMA_BufferSize = ADC3_DMA2_LENTH;// ˝æ›¥´ ‰¡ø 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//Õ‚…Ë∑«‘ˆ¡øƒ£ Ω
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//¥Ê¥¢∆˜‘ˆ¡øƒ£ Ω
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//Õ‚…Ë ˝æ›≥§∂»:16Œª
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//¥Ê¥¢∆˜ ˝æ›≥§∂»:16Œª
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//  π”√—≠ª∑ƒ£ Ω DMA_Mode_Normal DMA_Mode_Circular
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//÷–µ»”≈œ»º∂
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//¥Ê¥¢∆˜Õª∑¢µ•¥Œ¥´ ‰
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//Õ‚…ËÕª∑¢µ•¥Œ¥´ ‰
	DMA_Init(DMA2_Stream1,&DMA_InitStructure);//≥ı ºªØDMA Stream
	  
	DMA_SetCurrDataCounter(DMA2_Stream1,ADC3_DMA2_LENTH);// ˝æ›¥´ ‰¡ø   
	DMA_Cmd(DMA2_Stream1, ENABLE);                      //ø™∆ÙDMA¥´ ‰ 	
	
	/* GPIO≈‰÷√ -----------------------------------*/  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);// πƒ‹GPIOA ±÷”  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//Õ®µ¿3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ƒ£ƒ‚ ‰»Î
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//≤ª¥¯…œœ¬¿≠
	GPIO_Init(GPIOA, &GPIO_InitStructure);//≥ı ºªØ 
	
	/* ADC≈‰÷√ ------------------------------------*/ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE); // πƒ‹ADC ±÷” 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	  //ADC∏¥Œª
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);  //∏¥ŒªΩ· ¯	 
  
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//∂¿¡¢ƒ£ Ω
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//¡Ω∏ˆ≤…—˘Ω◊∂Œ÷Æº‰µƒ—”≥Ÿ5∏ˆ ±÷”
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //_DMA 
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//‘§∑÷∆µ4∑÷∆µ°£ADCCLK=PCLK2/4=84/4=21Mhz,ADC ±÷”◊Ó∫√≤ª“™≥¨π˝36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);//≥ı ºªØ
		
	ADC_InitStructure.ADC_Resolution = ADC_PRECISION;//12Œªƒ£ Ω
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//∑«…®√Ëƒ£ Ω	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//¡¨–¯◊™ªª
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//Ω˚÷π¥•∑¢ºÏ≤‚£¨ π”√»Ìº˛¥•∑¢
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//”“∂‘∆Î	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1∏ˆ◊™ªª‘⁄πÊ‘Ú–Ú¡–÷– “≤æÕ «÷ª◊™ªªπÊ‘Ú–Ú¡–1 
	ADC_Init(ADC3, &ADC_InitStructure);//ADC≥ı ºªØ 
	
	ADC_RegularChannelConfig(ADC3,ADC_Channel_3,1,ADC_SampleTime);  
	ADC_DMARequestAfterLastTransferCmd(ADC3,ENABLE); 
	ADC_DMACmd(ADC3,ENABLE);	
	ADC_Cmd(ADC3,ENABLE);//ø™∆ÙAD◊™ªª∆˜	
	ADC_SoftwareStartConv(ADC3); //»Ìº˛∆Ù∂Ø≤…ºØ   
	
}	
//void ADC3_DMA2_Init(void)
//{

//  DMA_InitTypeDef 	    DMA_InitStructure;  
//	ADC_CommonInitTypeDef ADC_CommonInitStructure;
//	ADC_InitTypeDef       ADC_InitStructure;
//	GPIO_InitTypeDef  	  GPIO_InitStructure;
//  
//	/* DMA≈‰÷√ ------------------------------------*/ 
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2 ±÷” πƒ‹ 
//	DMA_DeInit(DMA2_Stream1);
//	while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}//µ»¥˝DMAø…≈‰÷√ 
//		 
//	/* ≈‰÷√ DMA Stream */
//	DMA_InitStructure.DMA_Channel = DMA_Channel_2;  //Õ®µ¿—°‘Ò
//	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC3->DR;//DMAÕ‚…Ëµÿ÷∑
//	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC3_DMA2_Buff;//DMA ¥Ê¥¢∆˜0µÿ÷∑
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//Õ‚…ËµΩ¥Ê¥¢∆˜ƒ£ Ω
//	DMA_InitStructure.DMA_BufferSize =ADC3_DMA2_LENTH;// ˝æ›¥´ ‰¡ø 
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//Õ‚…Ë∑«‘ˆ¡øƒ£ Ω
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//¥Ê¥¢∆˜‘ˆ¡øƒ£ Ω
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//Õ‚…Ë ˝æ›≥§∂»:16Œª
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//¥Ê¥¢∆˜ ˝æ›≥§∂»:16Œª
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//  π”√—≠ª∑ƒ£ Ω DMA_Mode_Normal DMA_Mode_Circular
//	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//÷–µ»”≈œ»º∂
//	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
//	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
//	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//¥Ê¥¢∆˜Õª∑¢µ•¥Œ¥´ ‰
//	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//Õ‚…ËÕª∑¢µ•¥Œ¥´ ‰
//	DMA_Init(DMA2_Stream1,&DMA_InitStructure);//≥ı ºªØDMA Stream
//	  
//	DMA_SetCurrDataCounter(DMA2_Stream1,ADC3_DMA2_LENTH);// ˝æ›¥´ ‰¡ø  ø   
//	DMA_Cmd(DMA2_Stream1, ENABLE);                      //ø™∆ÙDMA¥´ ‰ 	
//	
//	/* GPIO≈‰÷√ -----------------------------------*/  
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOF,ENABLE);// πƒ‹GPIOA ±÷”  
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//Õ®µ¿3
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ƒ£ƒ‚ ‰»Î
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//≤ª¥¯…œœ¬¿≠
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//≥ı ºªØ 
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//Õ®µ¿3
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ƒ£ƒ‚ ‰»Î
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//≤ª¥¯…œœ¬¿≠
//	GPIO_Init(GPIOF, &GPIO_InitStructure);//≥ı ºªØ 
//	
//	/* ADC≈‰÷√ ------------------------------------*/ 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE); // πƒ‹ADC ±÷” 
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	  //ADC∏¥Œª
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);  //∏¥ŒªΩ· ¯	 
//  
//	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//∂¿¡¢ƒ£ Ω
//	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//¡Ω∏ˆ≤…—˘Ω◊∂Œ÷Æº‰µƒ—”≥Ÿ5∏ˆ ±÷”
//	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //_DMA 
//	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//‘§∑÷∆µ4∑÷∆µ°£ADCCLK=PCLK2/4=84/4=21Mhz,ADC ±÷”◊Ó∫√≤ª“™≥¨π˝36Mhz 
//	ADC_CommonInit(&ADC_CommonInitStructure);//≥ı ºªØ
//		
//	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12Œªƒ£ Ω
//	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//…®√Ëƒ£ Ω	
//	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//¡¨–¯◊™ªª
//	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//Ω˚÷π¥•∑¢ºÏ≤‚£¨ π”√»Ìº˛¥•∑¢
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//”“∂‘∆Î	
//	ADC_InitStructure.ADC_NbrOfConversion = 1;//2∏ˆ◊™ªª‘⁄πÊ‘Ú–Ú¡–÷– “≤æÕ «÷ª◊™ªªπÊ‘Ú–Ú¡–1 
//	ADC_Init(ADC3, &ADC_InitStructure);//ADC≥ı ºªØ 
//	
//	ADC_RegularChannelConfig(ADC3,ADC_Channel_3,1,ADC_SampleTime_3Cycles);  
////	ADC_RegularChannelConfig(ADC3,ADC_Channel_9,2,ADC_SampleTime_3Cycles);  
//	
//	ADC_DMARequestAfterLastTransferCmd(ADC3,ENABLE); 
//	ADC_DMACmd(ADC3,ENABLE);	
//	ADC_Cmd(ADC3,ENABLE);//ø™∆ÙAD◊™ªª∆˜	
//	ADC_SoftwareStartConv(ADC3); //»Ìº˛∆Ù∂Ø≤…ºØ      

//}


/**----------------------------------------------------------------------------
* @FunctionName  : ADC3_DMA2_Reload()    	 
* @Description   : None 	
* @Data          : 2016/8/16	
* @Explain       : Speed: 0 ~ 7 π≤8∏ˆµµŒª£¨ ˝÷µ‘Ω–°≤…ºØÀŸ∂»‘ΩøÏ	
------------------------------------------------------------------------------*/ 	
void ADC3_DMA2_Reload(u8 Speed)
{ 
	uint32_t ADC_SampleTime = ADC_SampleTime_3Cycles;
	DMA_InitTypeDef 	  DMA_InitStructure;  
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure; 
	switch(Speed)
	{
		case 0:	ADC_SampleTime = ADC_SampleTime_3Cycles;break;
		case 1:	ADC_SampleTime = ADC_SampleTime_15Cycles;break;
		case 2:	ADC_SampleTime = ADC_SampleTime_28Cycles;break;
		case 3:	ADC_SampleTime = ADC_SampleTime_56Cycles;break;
		case 4:	ADC_SampleTime = ADC_SampleTime_84Cycles;break;
		case 5:	ADC_SampleTime = ADC_SampleTime_112Cycles;break;
		case 6:	ADC_SampleTime = ADC_SampleTime_144Cycles;break;
		case 7:	ADC_SampleTime = ADC_SampleTime_480Cycles;break;
		default : ADC_SampleTime = ADC_SampleTime_3Cycles;break;;
	}
	
	/* DMA≈‰÷√ ------------------------------------*/ 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2 ±÷” πƒ‹ 
	DMA_DeInit(DMA2_Stream1);
	while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}//µ»¥˝DMAø…≈‰÷√ 
		 
	/* ≈‰÷√ DMA Stream */
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;  //Õ®µ¿—°‘Ò
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC3->DR;//DMAÕ‚…Ëµÿ÷∑
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC3_DMA2_Buff;//DMA ¥Ê¥¢∆˜0µÿ÷∑
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//Õ‚…ËµΩ¥Ê¥¢∆˜ƒ£ Ω
	DMA_InitStructure.DMA_BufferSize = ADC3_DMA2_LENTH;// ˝æ›¥´ ‰¡ø 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//Õ‚…Ë∑«‘ˆ¡øƒ£ Ω
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//¥Ê¥¢∆˜‘ˆ¡øƒ£ Ω
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//Õ‚…Ë ˝æ›≥§∂»:16Œª
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//¥Ê¥¢∆˜ ˝æ›≥§∂»:16Œª
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//  π”√—≠ª∑ƒ£ Ω DMA_Mode_Normal DMA_Mode_Circular
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//÷–µ»”≈œ»º∂
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//¥Ê¥¢∆˜Õª∑¢µ•¥Œ¥´ ‰
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//Õ‚…ËÕª∑¢µ•¥Œ¥´ ‰
	DMA_Init(DMA2_Stream1,&DMA_InitStructure);//≥ı ºªØDMA Stream
	  
	DMA_SetCurrDataCounter(DMA2_Stream1,ADC3_DMA2_LENTH);// ˝æ›¥´ ‰¡ø   
	DMA_Cmd(DMA2_Stream1, ENABLE);                      //ø™∆ÙDMA¥´ ‰ 	
	
//	/* GPIO≈‰÷√ -----------------------------------*/  
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);// πƒ‹GPIOA ±÷”  
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//Õ®µ¿3
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ƒ£ƒ‚ ‰»Î
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//≤ª¥¯…œœ¬¿≠
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//≥ı ºªØ 
	
	/* ADC≈‰÷√ ------------------------------------*/ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE); // πƒ‹ADC ±÷” 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	  //ADC∏¥Œª
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);  //∏¥ŒªΩ· ¯	 
  
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//∂¿¡¢ƒ£ Ω
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//¡Ω∏ˆ≤…—˘Ω◊∂Œ÷Æº‰µƒ—”≥Ÿ5∏ˆ ±÷”
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA 
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//‘§∑÷∆µ4∑÷∆µ°£ADCCLK=PCLK2/4=84/4=21Mhz,ADC ±÷”◊Ó∫√≤ª“™≥¨π˝36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);//≥ı ºªØ
		
	ADC_InitStructure.ADC_Resolution = ADC_PRECISION;//12Œªƒ£ Ω
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//∑«…®√Ëƒ£ Ω	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//¡¨–¯◊™ªª
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//Ω˚÷π¥•∑¢ºÏ≤‚£¨ π”√»Ìº˛¥•∑¢
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//”“∂‘∆Î	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1∏ˆ◊™ªª‘⁄πÊ‘Ú–Ú¡–÷– “≤æÕ «÷ª◊™ªªπÊ‘Ú–Ú¡–1 
	ADC_Init(ADC3, &ADC_InitStructure);//ADC≥ı ºªØ 
	
	ADC_RegularChannelConfig(ADC3,ADC_Channel_3,1,ADC_SampleTime);  
	ADC_DMARequestAfterLastTransferCmd(ADC3,ENABLE); 
	ADC_DMACmd(ADC3,ENABLE);	
	ADC_Cmd(ADC3,ENABLE);//ø™∆ÙAD◊™ªª∆˜	
	ADC_SoftwareStartConv(ADC3); //»Ìº˛∆Ù∂Ø≤…ºØ   
	
}	

/**----------------------------------------------------------------------------
* @FunctionName  : DMA1_Init()    	 
* @Description   : None 	
* @Data          : 2016/7/15	
* @Explain       : None	
------------------------------------------------------------------------------*/ 						
void DAC1_DMA1_Init(void)
{
	DMA_InitTypeDef 	  DMA_InitStructure;   
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
	/* DMA≈‰÷√ ------------------------------------*/ 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA2 ±÷” πƒ‹ 
	DMA_DeInit(DMA1_Stream5);
	while (DMA_GetCmdStatus(DMA1_Stream5) != DISABLE){}//µ»¥˝DMAø…≈‰÷√ 
		
	/* ≈‰÷√ DMA Stream */
	DMA_InitStructure.DMA_Channel = DMA_Channel_7;  //Õ®µ¿—°‘Ò 
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DAC ->DHR12R1;//DMAÕ‚…Ëµÿ÷∑  DAC1
		
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)DAC1_DMA1_Buff;//DMA ¥Ê¥¢∆˜0µÿ÷∑
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;// 
	DMA_InitStructure.DMA_BufferSize = DAC1_DMA1_LENTH;// ˝æ›¥´ ‰¡ø 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//Õ‚…Ë∑«‘ˆ¡øƒ£ Ω
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//¥Ê¥¢∆˜‘ˆ¡øƒ£ Ω
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//Õ‚…Ë ˝æ›≥§∂»:16Œª
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//¥Ê¥¢∆˜ ˝æ›≥§∂»:16Œª
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//  π”√—≠ª∑ƒ£ Ω DMA_Mode_Normal DMA_Mode_Circular
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//÷–µ»”≈œ»º∂
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//¥Ê¥¢∆˜Õª∑¢µ•¥Œ¥´ ‰
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//Õ‚…ËÕª∑¢µ•¥Œ¥´ ‰
	DMA_Init(DMA1_Stream5,&DMA_InitStructure);//≥ı ºªØDMA Stream 
	DMA_SetCurrDataCounter(DMA1_Stream5,DAC1_DMA1_LENTH);// ˝æ›¥´ ‰¡ø   
	DMA_Cmd(DMA1_Stream5, ENABLE);//ø™∆ÙDMA¥´ ‰ 	  
	
	/* DAC≈‰÷√ ------------------------------------*/ 

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//¢Ÿ πƒ‹ PA  ±÷”
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//¢⁄ πƒ‹ DAC  ±÷”
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ƒ£ƒ‚ ‰»Î
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//œ¬¿≠
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//¢Ÿ≥ı ºªØ GPIO
	DAC_InitType.DAC_Trigger=DAC_Trigger_T6_TRGO; //≤ª π”√¥•∑¢π¶ƒ‹ TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//≤ª π”√≤®–Œ∑¢…˙
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0; //∆¡±Œ°¢∑˘÷µ…Ë÷√
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;// ‰≥ˆª∫¥Êπÿ±’
	DAC_Init(DAC_Channel_1,&DAC_InitType); //¢€≥ı ºªØ DAC Õ®µ¿ 1
	
	DAC_SetChannel1Data(DAC_Align_12b_R ,0); //¢›12 Œª”“∂‘∆Î ˝æ›∏Ò Ω 
	
	DAC_DMACmd(DAC_Channel_1,ENABLE);  
	DAC_Cmd(DAC_Channel_1, ENABLE); //¢‹ πƒ‹ DAC Õ®µ¿ 1
	DAC1_DMA1_Reload(100);
   
}	

void DAC1_DMA1_Reload(u32 speed)
{	 
	u16 speed0 = 1;
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure; 
	if(speed == 0) return; 
	if(speed > 0xffff)  
	{
		speed0 = speed/0xffff;
		speed = speed%0xffff; 
	}
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
	TIM_TimeBaseStructure.TIM_Period = speed;          
	TIM_TimeBaseStructure.TIM_Prescaler = speed0;       
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); 
	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);  
	TIM_Cmd(TIM6, ENABLE);
  
	 
}
	
 
/*******************************(C) COPYRIGHT 2016 Wind£®–ª”Ò…Ï£©*********************************/





