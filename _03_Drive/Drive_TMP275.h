#ifndef __DRIVE_TMP275_H
#define __DRIVE_TMP275_H

#include "User_header.h"


//---------- TPM 275 相关定义 ----------------------------------------	 
#define TMP275_IIC_ADDR	0x90		//7位地址 + 0(read)
// 温度与ADC转换值得关系
#define FTMP275_TEMP_AD_STEP		0.0625
	 
//----------- GPIO 定义 ----------------------------------------------
#define TMP275_SDA_CLK     RCC_AHBPeriph_GPIOB      
#define TMP275_SDA_Pin     GPIO_Pin_11
#define TMP275_SDA_GPIO    GPIOB
	 
#define TMP275_SCL_CLK     RCC_AHBPeriph_GPIOB       
#define TMP275_SCL_Pin     GPIO_Pin_12
#define TMP275_SCL_GPIO	   GPIOB
	 
	 
void TMP275Init(void);

unsigned char TMP275ReadTemprature(float *p);



#endif


