/*
*********************************************************************************************************
*                                              _03_OS
* File			 : App_CAN.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : can通信测试程序
*********************************************************************************************************
*/
#include "App_CAN.h"


void CAN_main(void)
{
	
	  u8 i=0,result,CANBUF[8]={1,2,3,4,5,6,7,8};
	
    CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_Normal);//CAN初始化普通模式,波特率500Kbps    
		
		
		OS_String_Show(100,70,24,1,"SendData:");
		OS_String_Show(100,130,24,1,"ResiveData:");

	  while(1)
		{
			
		   if(Key_Now_Get(KEY3,KEY_MODE_SHORT))
			 {		 
			    result=CAN2_Send_Msg(CANBUF,8);
				 
				  for(i=0;i<8;i++)
			      OS_Num_Show(100+50*i,100,24,1,CANBUF[i],"%0.0f ");
				 
				  if(result)
						OS_String_Show(220,70,24,1,"Failed");
					else
						OS_String_Show(220,70,24,1,"OK    ");
									
			 }		 
			 
			 OSTimeDly(100);
		}	
	  
}



