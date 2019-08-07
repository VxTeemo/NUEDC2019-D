#ifndef __DRIVE_ADF4001_H
#define __DRIVE_ADF4001_H 

#include "User_header.h"




#define AD4001_CLK_PIN													GPIO_Pin_11
#define AD4001_CLK_GPIO_PORT										GPIOG
#define AD4001_CLK_GPIO_CLK											RCC_AHB1Periph_GPIOG 

#define AD4001_DAT_PIN													GPIO_Pin_12
#define AD4001_DAT_GPIO_PORT										GPIOG
#define AD4001_DAT_GPIO_CLK											RCC_AHB1Periph_GPIOG 

#define AD4001_LE_PIN														GPIO_Pin_13
#define AD4001_LE_GPIO_PORT											GPIOG
#define AD4001_LE_GPIO_CLK											RCC_AHB1Periph_GPIOG 


#define AD4001_CLK_1			AD4001_CLK_GPIO_PORT->BSRRL = AD4001_CLK_PIN
#define AD4001_CLK_0  		AD4001_CLK_GPIO_PORT->BSRRH = AD4001_CLK_PIN

#define AD4001_DAT_1	    AD4001_DAT_GPIO_PORT->BSRRL = AD4001_DAT_PIN
#define AD4001_DAT_0	    AD4001_DAT_GPIO_PORT->BSRRH = AD4001_DAT_PIN

#define AD4001_LE_1				AD4001_LE_GPIO_PORT->BSRRL = AD4001_LE_PIN	
#define AD4001_LE_0				AD4001_LE_GPIO_PORT->BSRRH = AD4001_LE_PIN	


//MUXOUT引脚输出状态
typedef enum
{
	THREE_STATE_OUTPUT=0,		//三态输出
	DIGITAL_LOCK_DETECT,		//数字锁定检测
	N_DIVIDER_OUTPUT,				//N分频输出
	AVDD_OUTPUT,						//模拟电源输出
	R_DIVIDER_OUTPUT,				//R分频输出
	OPENDRAIN_LOCK_DETECT,	//开漏锁定检测
	SERIAL_DATA_OUTPUT,			//串行数据输出
	DGND_OUTPUT							//数字地输出	
}MUXOUT_STATE;

void ADF4001_Init(void);

//设置锁定输出频率函数接口，有两种方式，
//如果定义了R_COUNTER则通过设置RCounter,NCounter的只取定频率
//如果定义了R_COUNTER则可以直接输入频率即可
 //#define R_COUNTER	120	
#ifndef R_COUNTER

void ADF4001_SetFreq(unsigned long int RCounter, unsigned long int NCounter,unsigned int Muxout);
#else
void ADF4001_SetFreq(unsigned long int freq,unsigned long int Muxout); 
#endif

#endif

