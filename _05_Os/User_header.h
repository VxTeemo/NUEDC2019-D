/*
*********************************************************************************************************
*                                              _04_OS
* File			 : User_header.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
*********************************************************************************************************
*/
#ifndef __USER_HEADER_H
#define __USER_HEADER_H 


#define SYSTEM_VERSION 				1				//0为2016系统板  1为2018系统板
#include <math.h>
/* OS 文件*/
#include "Os_cpu.h"
#include "Os_UI.h"
#include "Os_malloc.h"

/* 系统文件*/
#include "stdio.h"
#include <stdlib.h>
#include "arm_math.h"	
#include "math.h"
#include "stm32f4xx.h" 							
#include "stm32f4xx_exti.h"					
#include "stm32f4xx_rcc.h"	
#include "stm32f4xx_can.h"	
#include "stm32f4xx_adc.h"						
#include "stm32f4xx_dma.h"					
#include "stm32f4xx_usart.h"				
#include "stm32f4xx_dma.h"				
#include "stm32f4xx_adc.h"	 
#include "stm32f4xx_gpio.h"	 
#include "stm32f4xx_syscfg.h"
#include "sys.h"
#include "usart.h"


/*应用文件*/


//#include "TEST.h"

//#include "App_Frequency.h"
//#include "App_OSC.h"
#include "App_Touch.h"
//#include "App_AFG.h"
#include "App_LED.h"
#include "App_key.h"
#include "App_Switch.h"
//#include "App_Wave.h"
//#include "App_RFA.h"
//#include "App_ADF4351.h"
//#include "App_ADF4351_Sweep.h"
#include "App_FreqAna.h"
//#include "App_ADF4001.h"
//#include "App_Wave.h"
//#include "App_Code.h"
//#include "App_ADF4360.h"
//#include "App_CAN.h"
//#include "App_analysis.h"

#include "App_NIT.h"

/*驱动文件*/

#include "TFT_LCD.h"
#include "W25Q64.h" 
#include "fontupd.h" 



#include "Drive_Touch.h" 
#include "Drive_TouchKey.h" 
#include "Drive_GPIO.h"
#include "Drive_ADC.h"
#include "Drive_DAC.h"
#include "Drive_PWM.h"
#include "Drive_DMA.h"
#include "Drive_Interrupt.h"
#include "Drive_Flash.h"
#include "Drive_Timer.h"
#include "Drive_Exti.h" 
#include "Drive_IIC.h"
//#include "Drive_FFT.h"
//#include "Drive_PID.h"
//#include "Drive_TMP275.h"
//#include "Drive_PGA2310.h"
//#include "Drive_ds18b20.h"
//#include "Drive_ADF4001.h"
//#include "Drive_CDCE937.h"
//#include "Drive_ADF4360.h"
//#include "Drive_CAN.h"
//#include "Drive_ADPCM.h"
//#include "Drive_ADF4351.h"
#include "Drive_TouchKey.h"
//#include "Drive_HMC542.h"
//#include "Drive_ADL5336.h"
#include "Drive_ADS1256.h"
#include "Drive_Communication.h"

#endif


