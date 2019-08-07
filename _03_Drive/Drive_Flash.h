#ifndef __DRIVE_FLASH_H
#define __DRIVE_FLASH_H 

/* 头文件包含 ----------------------------------------------------------------*/  
#include "User_header.h" 

/* 全局宏定义 ----------------------------------------------------------------*/ 
/* FLASH起始地址 ---------------------------*/
#define STM32_FLASH_BASE ADDR_FLASH_SECTOR_9 	//STM32 FLASH的起始地址
 
/* FLASH 扇区的起始地址 --------------------*/ 
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//扇区0起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//扇区1起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//扇区2起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//扇区3起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//扇区4起始地址, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//扇区5起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//扇区6起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//扇区7起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//扇区8起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//扇区9起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//扇区10起始地址,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//扇区11起始地址,128 Kbytes  

 
/* 结构体声明 ----------------------------------------------------------------*/ 

/* 全局变量声明 --------------------------------------------------------------*/ 

/* 全局函数声明 --------------------------------------------------------------*/  
u32 InFLASH_ReadWord(u32 addr);
u16 InFLASH_GetFlashSector(u32 addr);
void InFLASH_Write(u32 Addr,u32 *pBuffer,u32 DataLenth);
void InFLASH_Read(u32 Addr,u32 *pBuffer,u32 DataLenth);


#endif



