#ifndef __DRIVE_CAN_H
#define __DRIVE_CAN_H	

#include "User_header.h"   	 							    
					



void CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
u8 CAN2_Send_Msg(u8* msg,u8 len);						                 //发送数据


#endif

















