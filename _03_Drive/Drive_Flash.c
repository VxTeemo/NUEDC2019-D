/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_Flash.c
* By  			 : 
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : Flash驱动程序
*********************************************************************************************************
*/
#include "Drive_Flash.h"		
			
/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有（静态）函数声明 ------------------------------------------------------*/

/* 全局变量定义 --------------------------------------------------------------*/
 
/* 全局函数编写 --------------------------------------------------------------*/
/**----------------------------------------------------------------------------
* @FunctionName  : FLASH_ReadWord()    	 
* @Description   : None 	
* @Data          : 2016/4/18	
* @Explain       : 			
*----------------------------------------------------
读取指定地址的半字(16位数据) 	
faddr:读地址 	
返回值:对应数据.	
------------------------------------------------------------------------------*/
u32 InFLASH_ReadWord(u32 addr)
{
	return *(vu32*)addr; 
}
 

/**----------------------------------------------------------------------------  
* @FunctionName  : InFLASH_Write()     
* @Description   : None 
* @Data          : 2016/4/18
* @Explain       : 
*----------------------------------------------------
从指定地址开始写入指定长度的数据   
特别注意:因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数  
写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以  
写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里  
没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写.   
该函数对OTP区域也有效!可以用来写OTP区!  
OTP区域地址范围:0X1FFF7800~0X1FFF7A0F  

Addr:起始地址(此地址必须为4的倍数!!) 4*8 = 32Bit   
pBuffer:数据指针
DataLenth:字(32位)数(就是要写入的32位数据的个数.) 
------------------------------------------------------------------------------*/
void InFLASH_Write(u32 Addr,u32 *pBuffer,u32 DataLenth)	
{ 
	FLASH_Status status = FLASH_COMPLETE;
	u32 addrx=0;
	u32 endaddr=0;	
	if(Addr<STM32_FLASH_BASE||Addr%4)return;	//非法地址  	
	FLASH_Unlock();		//解锁 		
	FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存  
 		
	addrx=Addr;	   //写入的起始地址
	endaddr=Addr+DataLenth*4;	//写入的结束地址 
	if(addrx<0X1FFF0000)			//只有主存储区,才需要执行擦除操作!! 
	{
		while(addrx<endaddr) //扫清一切障碍.(对非FFFFFFFF的地方,先擦除)  
		{	
			if(InFLASH_ReadWord(addrx)!=0xffffffff)//有非0XFFFFFFFF的地方,要擦除这个扇区			
			{   
				status=FLASH_EraseSector(InFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V之间!!		
				if(status!=FLASH_COMPLETE) break;	//发生错误了  		
			}else addrx+=4;		
		} 					
	}						
	if(status==FLASH_COMPLETE) 				
	{				
		while(Addr<endaddr) //写数据				
		{	
			if(FLASH_ProgramWord(Addr,*pBuffer)!=FLASH_COMPLETE)//写入数据 			
			{ 		
				break;	//写入异常						
			}						
			Addr+=4;				
			pBuffer++;			
		} 		
	}			
	FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存			
	FLASH_Lock();//上锁		
} 			
	


/**----------------------------------------------------------------------------
* @FunctionName  : InFLASH_Read()     
* @Description   : None 
* @Data          : 2016/4/18
* @Explain       : 
*----------------------------------------------------
从指定地址开始读出指定长度的数据		
ReadAddr:起始地址		
pBuffer:数据指针		
DataLenth:字(4位)数	
------------------------------------------------------------------------------*/
void InFLASH_Read(u32 Addr,u32 *pBuffer,u32 DataLenth)   	
{
	u32 i;
	for(i=0;i<DataLenth;i++)
	{
		pBuffer[i]=InFLASH_ReadWord(Addr);//读取4个字节 = 32Bit		
		Addr+=4;//偏移4个字节.	
	}
}
		
		
			
/**----------------------------------------------------------------------------
* @FunctionName  : FLASH_ReadWord()     
* @Description   : None 
* @Data          : 2016/4/18
* @Explain       : 
*----------------------------------------------------
获取某个地址所在的flash扇区
addr:flash地址
返回值:0~11,即addr所在的扇区
------------------------------------------------------------------------------*/
u16 InFLASH_GetFlashSector(u32 addr)
{	
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;	
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;	
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;		
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;	
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;	
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;	
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;	
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;	
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;	
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;	
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 	
	return FLASH_Sector_11;	
																					
}		
		
		
		
/*******************************(C) COPYRIGHT 2016 Wind（谢玉伸）*********************************/





