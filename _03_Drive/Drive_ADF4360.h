#ifndef __DRIVE_ADF4360_H
#define __DRIVE_ADF4360_H 


#include "User_header.h"

/*************************单个模块控制*******************************/
#define AD4360_CLK_PIN													GPIO_Pin_11
#define AD4360_CLK_GPIO_PORT										GPIOG

#define AD4360_DAT_PIN													GPIO_Pin_12
#define AD4360_DAT_GPIO_PORT										GPIOG

#define AD4360_LE_PIN														GPIO_Pin_13
#define AD4360_LE_GPIO_PORT											GPIOG


#define AD4360_CLK_1			AD4360_CLK_GPIO_PORT->BSRRL = AD4360_CLK_PIN
#define AD4360_CLK_0  		AD4360_CLK_GPIO_PORT->BSRRH = AD4360_CLK_PIN

#define AD4360_DAT_1	    AD4360_DAT_GPIO_PORT->BSRRL = AD4360_DAT_PIN
#define AD4360_DAT_0	    AD4360_DAT_GPIO_PORT->BSRRH = AD4360_DAT_PIN

#define AD4360_LE_1				AD4360_LE_GPIO_PORT->BSRRL = AD4360_LE_PIN	
#define AD4360_LE_0				AD4360_LE_GPIO_PORT->BSRRH = AD4360_LE_PIN	

/************************多个模块控制*************************************/
#define AD4360_2_CLK_PIN													GPIO_Pin_7
#define AD4360_2_CLK_GPIO_PORT										GPIOF

#define AD4360_2_DAT_PIN													GPIO_Pin_8
#define AD4360_2_DAT_GPIO_PORT										GPIOF

#define AD4360_2_LE_PIN														GPIO_Pin_9
#define AD4360_2_LE_GPIO_PORT											GPIOF


#define AD4360_2_CLK_1			AD4360_2_CLK_GPIO_PORT->BSRRL = AD4360_2_CLK_PIN
#define AD4360_2_CLK_0  		AD4360_2_CLK_GPIO_PORT->BSRRH = AD4360_2_CLK_PIN

#define AD4360_2_DAT_1	    AD4360_2_DAT_GPIO_PORT->BSRRL = AD4360_2_DAT_PIN
#define AD4360_2_DAT_0	    AD4360_2_DAT_GPIO_PORT->BSRRH = AD4360_2_DAT_PIN

#define AD4360_2_LE_1				AD4360_2_LE_GPIO_PORT->BSRRL = AD4360_2_LE_PIN	
#define AD4360_2_LE_0				AD4360_2_LE_GPIO_PORT->BSRRH = AD4360_2_LE_PIN	

//MUXOUT引脚输出状态
typedef enum
{
	DVDD=0,		
	DIGITAL_LOCK,		
	N_OUTPUT,				
	DVDD2,						
	R_OUTPUT,			
	A_CNTR_2_OUT,	
	A_CNTR_OUT,			
	DGND							
}MUXOUT;

void ADF4360_Init(void);

void ADF4360_2_Init(void);

void ADF4360_SetFreq(unsigned long int Freq,unsigned int Muxout);

void ADF4360_2_SetFreq(unsigned long int Freq,unsigned int Muxout);

#endif


