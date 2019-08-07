/***************************************************************************************************
  * 文 件 名：W25Q64.c
  * 作    者：陈弟斌
  *	日    期：2015.7.4
  * 版    本：Stm32F407ZG
  * 开 发 板：stm32f407ZG--2015竞赛板
  * 修改时间：无 
  * 编 译 器: keil.5
  * 库 版 本：V1.4
  *************************************************************************************************/ 
/* -------防止递归包含的定义-----------------------------------------------------------------------*/
#ifndef __W25Q64_H
#define __W25Q64_H	
/*--------包含头文件------------------------------------------------------------------------------*/
#include "delay.h"
/*----------------芯片列表宏定义-------------------------------------------------------------------*/	
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17

//extern uint16_t W25QXX_TYPE; //定义芯片型号
/*----------------片选信号------------------------------------------------------------------*/	
#define	W25Q64_CS_H 	GPIO_SetBits(GPIOB,GPIO_Pin_9); 



#define	W25Q64_CS_L 	GPIO_ResetBits(GPIOB,GPIO_Pin_9);  
						
						
/*----------------指令表------------------------------------------------------------------*/	
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData		    0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 
/*---------------------出口函数模型---------------------------------------------------------------*/  
void W25Q64_Init(void);
uint16_t  W25Q64_ReadID(void);  	    		//读取FLASH ID
uint8_t	 W25Q64_ReadSR(void);        		//读取状态寄存器 
void W25Q64_Write_SR(uint8_t sr);  			//写状态寄存器
void W25Q64_Write_Enable(void);  		//写使能 
void W25Q64_Write_Disable(void);		//写保护
void W25Q64_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void W25Q64_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);   //读取flash
void W25Q64_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);//写入flash
void W25Q64_Erase_Chip(void);    	  	//整片擦除
void W25Q64_Erase_Sector(uint32_t Dst_Addr);	//扇区擦除
void W25Q64_Wait_Busy(void);           	//等待空闲
void W25Q64_PowerDown(void);        	//进入掉电模式
void W25Q64_WAKEUP(void);				//唤醒
#endif
















