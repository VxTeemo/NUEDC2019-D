#ifndef __DRIVE_IIC_H
#define __DRIVE_IIC_H 

/* 头文件包含 ----------------------------------------------------------------*/  
#include "User_header.h" 							
						 
/* 全局宏定义 ----------------------------------------------------------------*/

/* 结构体声明 ----------------------------------------------------------------*/ 

/* 全局变量声明 --------------------------------------------------------------*/ 

/* 全局函数声明 --------------------------------------------------------------*/  
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);

#endif


