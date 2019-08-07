#include "User_header.h"

#ifndef DS18B20_H_  
#define DS18B20_H_

/* 头文件包含 ----------------------------------------------------------------*/  
#include "User_header.h"

/* 全局宏定义 ----------------------------------------------------------------*/

#define MAXNUM 4    //定义总线上的DS18B20的最大个数MAXNUM

/* 结构体声明 ----------------------------------------------------------------*/

/* 全局变量声明 --------------------------------------------------------------*/ 
 
/* 全局函数声明 --------------------------------------------------------------*/ 

//DS18B20指令
typedef enum   
{  
    SEARCH_ROM          =   0xf0,   //搜索ROM指令  
    READ_ROM            =   0x33,   //读取ROM指令
    MATH_ROM            =   0x55,   //匹配ROM指令
    SKIP_ROM            =   0xcc,   //跳过ROM指令
    ALARM_SEARCH        =   0xec,   //报警搜索指令 
    CONVERT_T           =   0x44,   //温度转换指令
    WRITE_SCRATCHPAD    =   0x4e,   //写暂存器指令
    READ_SCRATCHPAD     =   0xbe,   //读取转存器指令
    COPY_SCRATCHPAD     =   0x48,   //拷贝暂存器指令  
    RECALL_E2           =   0xb8,   //召回EEPROM指令
    READ_POWER_SUPPLY   =   0xb4,   //读取电源模式指令  
	
} DS18B20_CMD;  
  
  
//DS18B20 ROM编码 
typedef struct  
{  
    u8  DS18B20_CODE;   //DS18B20单总线编码:0x19  
    u8  SN_1;           //序列号第1字节  
    u8  SN_2;           //序列号第2字节
    u8  SN_3;           //序列号第3字节
    u8  SN_4;           //序列号第4字节  
    u8  SN_5;           //序列号第5字节 
    u8  SN_6;           //序列号第6字节
    u8  crc8;           //CRC8校验    
	
} DS18B20_ROM_CODE;  


  
#define TRUE 0xff
#define FALSE 0X00

//IO方向设置
#define Set18b20IOin()  {GPIOG->MODER&=~(3<<(9*2));GPIOG->MODER|=0<<9*2;}
#define Set18b20IOout() {GPIOG->MODER&=~(3<<(9*2));GPIOG->MODER|=1<<9*2;}

////IO操作函数											   
#define	DS18B20_OUT PGout(9) //数据端口	PG9
#define	DS18B20_IN  PGin(9)  //数据端口	PG9

//函数使用申明

int DS18B20_Reset(void);
u8 DS18B20_Init(void);
u8 DS18B20_ReadBit(void);
u8 DS18B20_ReadData(void) ;
void DS18B20_WriteBit(u8 bit);
void DS18B20_WriteData(u8 data);
s16 DS18B20_ReadTemper(void);
u8 DS18B20_Read2Bit(void);
u8 DS18B20_SearchROM(u8 (*pID)[8],u8 Num);
s16 DS18B20_ReadDesignateTemper(u8 pID[8]);

#endif 

   	
















