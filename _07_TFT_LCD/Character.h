/***************************************************************************************************
  * 文 件 名：Character.h
  * 作    者：陈弟斌
  *	日    期：2015.6.29
  * 版    本：Stm32F407ZG
  * 开 发 板：stm32f407ZG--2015竞赛板
  * 修改时间：无 
  * 编 译 器: keil.5
  * 库 版 本：V1.4
  *************************************************************************************************/  

/* -------防止递归包含的定义-----------------------------------------------------------------------*/
#ifndef __CHARACTER_H
#define __CHARACTER_H
/*--------包含头文件------------------------------------------------------------------------------*/
#include "TFT_LCD.h"
/*---------------------出口函数模型---------------------------------------------------------------*/  
void LCD_GB4848(uint16_t y,uint16_t x,uint8_t font[2],uint8_t size);
void Show_Str48(uint16_t y,uint16_t x,uint8_t *str,uint8_t size);
void LCD_GB3232(uint16_t y,uint16_t x,uint8_t font[2],uint8_t size);
void Show_Str32(uint16_t y,uint16_t x,uint8_t*str,uint8_t size);
void LCD_GB1616(uint16_t y,uint16_t x,uint8_t font[2],uint8_t size);
void Show_Str(uint16_t y,uint16_t x,uint8_t *str,uint8_t size);

#endif
