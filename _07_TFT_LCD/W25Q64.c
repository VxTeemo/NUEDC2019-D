 
/*--------包含头文件------------------------------------------------------------------------------*/
#include "W25Q64.h" 
#include "spi.h"  
#include "TFT_LCD.h"
#include "stm32f4xx_spi.h" 

/*--------变量申明-----------------------------------------------------------------------------*/
uint16_t W25Q64_TYPE=W25Q64;	


/************************************************************************************************
 * 描述   : 初始化SPI Flash的IO口
 * 参数   : 无
 * 返回   : 无
 * 说明   : 无
 *************************************************************************************************/
void W25Q64_Init(void)
{ 
  GPIO_InitTypeDef  GPIO_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟

	  //GPIOB14
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PB14
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PG7
	GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化
 
	GPIO_SetBits(GPIOG,GPIO_Pin_7);//PG7输出1,防止NRF干扰SPI FLASH的通信 
	W25Q64_CS_H;			//SPI FLASH不选中
	SPI1_Init();		   			//初始化SPI
	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);		//设置为42M时钟,高速模式 
	W25Q64_TYPE=W25Q64_ReadID();	//读取FLASH ID.
}  

/************************************************************************************************
 * 描述   : 读取W25Q64状态
 * 参数   : 
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
uint8_t W25Q64_ReadSR(void)   
{  
	uint8_t byte=0;   
	W25Q64_CS_L;                            //使能器件   
	SPI1_ReadWriteByte(W25X_ReadStatusReg);    //发送读取状态寄存器命令    
	byte=SPI1_ReadWriteByte(0Xff);             //读取一个字节  
	W25Q64_CS_H;                            //取消片选     
	return byte;   
}
/************************************************************************************************
 * 描述   : 写W25QXX状态寄存器
 * 参数   : 无
 * 返回   : 无
 * 说明   :只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
 *************************************************************************************************/
void W25Q64_Write_SR(u8 sr)   
{   
	W25Q64_CS_L;                            //使能器件   
	SPI1_ReadWriteByte(W25X_WriteStatusReg);   //发送写取状态寄存器命令    
	SPI1_ReadWriteByte(sr);               //写入一个字节  
	W25Q64_CS_H;                            //取消片选     	      
}   
/************************************************************************************************
 * 描述   : W25Q64写使能
 * 参数   : 无
 * 返回   : 无
 * 说明   : 无
 *************************************************************************************************/  
void W25Q64_Write_Enable(void)   
{
	W25Q64_CS_L;                            //使能器件   
	SPI1_ReadWriteByte(W25X_WriteEnable);      //发送写使能  
	W25Q64_CS_H;                            //取消片选     	      
} 
/************************************************************************************************
 * 描述   : W25Q64写禁止
 * 参数   : 无
 * 返回   : 无
 * 说明   : 无
 *************************************************************************************************/  
void W25Q64_Write_Disable(void)   
{  
	W25Q64_CS_L;                            //使能器件   
	SPI1_ReadWriteByte(W25X_WriteDisable);  //发送写禁止指令    
	W25Q64_CS_H;                            //取消片选     	      
} 		
/************************************************************************************************
 * 描述   : 读取芯片ID
 * 参数   : 无
 * 返回   : 芯片ID对应值
 * 说明   : 无
 *************************************************************************************************/   
u16 W25Q64_ReadID(void)
{
	uint16_t Temp = 0;	  
	W25Q64_CS_L;				    
	SPI1_ReadWriteByte(0x90);//发送读取ID命令	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI1_ReadWriteByte(0xFF);	 
	W25Q64_CS_H;				    
	return Temp;
}  
/************************************************************************************************
 * 描述   : 读取SPI Flash
 * 参数   : 数据存储区 开始读取地址 要读取的最大字节数
 * 返回   : 无
 * 说明   : 无
 *************************************************************************************************/  
void W25Q64_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	u16 i;   										    
	W25Q64_CS_L;                            //使能器件   
	SPI1_ReadWriteByte(W25X_ReadData);         //发送读取命令   
	SPI1_ReadWriteByte((u8)((ReadAddr)>>16));  //发送24bit地址    
	SPI1_ReadWriteByte((u8)((ReadAddr)>>8));   
	SPI1_ReadWriteByte((u8)ReadAddr);   
	for(i=0;i<NumByteToRead;i++)
  { 
			pBuffer[i]=SPI1_ReadWriteByte(0XFF);   //循环读数  
	}
	W25Q64_CS_H;  				    	      
}
/************************************************************************************************
 * 描述   : SPI在一页内写入256个字节的数据
 * 参数   : 数据存储区 开始读取地址 要写入的最大字节数
 * 返回   : 无
 * 说明   : 要写入的最大字节数不能超过该页剩余的字节数
 *************************************************************************************************/  	 
void W25Q64_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
	W25Q64_Write_Enable();                  //SET WEL 
	W25Q64_CS_L;                            //使能器件   
	SPI1_ReadWriteByte(W25X_PageProgram);      //发送写页命令   
	SPI1_ReadWriteByte((u8)((WriteAddr)>>16)); //发送24bit地址    
	SPI1_ReadWriteByte((u8)((WriteAddr)>>8));   
	SPI1_ReadWriteByte((u8)WriteAddr);   
	for(i=0;i<NumByteToWrite;i++)SPI1_ReadWriteByte(pBuffer[i]);//循环写数  
	W25Q64_CS_H;                            //取消片选 
	W25Q64_Wait_Busy();					   //等待写入结束
} 
/************************************************************************************************
 * 描述   : 无检验写SPI Flash
 * 参数   : 数据存储区  开始写入的地址  要写入的最大数
 * 返回   : 无
 * 说明   : 无
 *************************************************************************************************/ 
void W25Q64_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		W25Q64_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 
/************************************************************************************************
 * 描述   : 写SPI Flash
 * 参数   : 数据存储区 开始写入的地址  要写的最大字节数
 * 返回   : 无
 * 说明   : 无
 *************************************************************************************************/ 
uint8_t W25Q64_BUFFER[4096];		 
void W25Q64_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t * W25Q64_BUF;	  
  W25Q64_BUF=W25Q64_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		W25Q64_Read(W25Q64_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25Q64_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			W25Q64_Erase_Sector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				W25Q64_BUF[i+secoff]=pBuffer[i];	  
			}
			W25Q64_Write_NoCheck(W25Q64_BUF,secpos*4096,4096);//写入整个扇区  

		}else W25Q64_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 
			pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		  NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	};	 
}
/************************************************************************************************
 * 描述   : 擦除整个芯片
 * 参数   : 无
 * 返回   : 无
 * 说明   : 无
 *************************************************************************************************/ 
void W25Q64_Erase_Chip(void)   
{      	
  W25Q64_Write_Enable();                  //SET WEL 
	W25Q64_Wait_Busy();   
	W25Q64_CS_L;                            //使能器件   
	SPI1_ReadWriteByte(W25X_ChipErase);        //发送片擦除命令  
	W25Q64_CS_H;                            //取消片选     	      
	W25Q64_Wait_Busy();   				   //等待芯片擦除结束
}  
/************************************************************************************************
 * 描述   : 擦出一个扇区
 * 参数   : 扇区地址（根据实际容量设置）
 * 返回   : 无
 * 说明   : 擦出一个扇区最少时间150ms
 *************************************************************************************************/ 
void W25Q64_Erase_Sector(uint32_t Dst_Addr)   
{  
	//监视falsh擦除情况,测试用   
 //	printf("fe:%x\r\n",Dst_Addr); 
 	Dst_Addr*=4096;
	W25Q64_Write_Enable();                  //SET WEL 	 
	W25Q64_Wait_Busy();   
	W25Q64_CS_L;                            //使能器件   
	SPI1_ReadWriteByte(W25X_SectorErase);      //发送扇区擦除指令 
	SPI1_ReadWriteByte((u8)((Dst_Addr)>>16));  //发送24bit地址    
	SPI1_ReadWriteByte((u8)((Dst_Addr)>>8));   
	SPI1_ReadWriteByte((u8)Dst_Addr);  
	W25Q64_CS_H;                            //取消片选     	      
	W25Q64_Wait_Busy();   				   //等待擦除完成
	  
}  
/************************************************************************************************
 * 描述   : 等待空闲
 * 参数   : 无
 * 返回   : 无
 * 说明   : 无
 *************************************************************************************************/ 
void W25Q64_Wait_Busy(void)   
{   
	while((W25Q64_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
}  
/************************************************************************************************
 * 描述   : 进入掉电模式
 * 参数   : 无
 * 返回   : 无
 * 说明   : 无
 *************************************************************************************************/ 
void W25Q64_PowerDown(void)   
{ 
	W25Q64_CS_L;                            //使能器件   
	SPI1_ReadWriteByte(W25X_PowerDown);        //发送掉电命令  
	W25Q64_CS_H;                            //取消片选     	      
	delay_us(3);                               //等待TPD  
}   
/************************************************************************************************
 * 描述   : 唤醒
 * 参数   : 无
 * 返回   : 无
 * 说明   : 无
 *************************************************************************************************/ 
void W25Q64_WAKEUP(void)   
{  
  W25Q64_CS_L;                            //使能器件   
  SPI1_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	W25Q64_CS_H;                            //取消片选     	      
  delay_us(3);                               //等待TRES1
}   
