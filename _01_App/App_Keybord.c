#include "App_Keybord.h"


//先开电显示出 "USB Connecting..." 再接鼠标或键盘
//键值的显示、操作在 usbh_usr.c 中

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core_dev;
extern HID_Machine_TypeDef HID_Machine;	


//USB信息显示
//msgx:0,USB无连接
//     1,USB键盘
//     2,USB鼠标
//     3,不支持的USB设备
void USBH_Msg_Show(u8 msgx)
{
	
	LCD_SetTextColor(Red);
	
	switch(msgx)
	{
		
		case 0:	//USB无连接
			OS_String_Show(30,130,16,1,"USB Connecting...");	
			LCD_Appoint_Clear(0,150,800,480,White);
			break;
		
		case 1:	//USB键盘
			OS_String_Show(30,130,16,1,"USB Connected    ");	
			OS_String_Show(30,150,16,1,"USB KeyBoard");	 
			OS_String_Show(30,180,16,1,"KEYVAL:");	
			OS_String_Show(30,200,16,1,"INPUT STRING:");	
			break;
		
		case 2:	//USB鼠标
			OS_String_Show(30,130,16,1,"USB Connected    ");	
			OS_String_Show(30,150,16,1,"USB Mouse");	 
			OS_String_Show(30,180,16,1,"BUTTON:");	
			OS_String_Show(30,200,16,1,"X POS:");	
			OS_String_Show(30,220,16,1,"Y POS:");	
			OS_String_Show(30,240,16,1,"Z POS:");	
			break; 		
		
		case 3:	//不支持的USB设备
			OS_String_Show(30,130,16,1,"USB Connected    ");	
			OS_String_Show(30,150,16,1,"Unknow Device");	 
			break; 	 
		
	} 
}   


//HID重新连接
void USBH_HID_Reconnect(void)
{
	
	//关闭之前的连接
	USBH_DeInit(&USB_OTG_Core_dev,&USB_Host);	//复位USB HOST
	USB_OTG_StopHost(&USB_OTG_Core_dev);		  //停止USBhost
	
	if(USB_Host.usr_cb->DeviceDisconnected)		//存在,才禁止
	{
		
			USB_Host.usr_cb->DeviceDisconnected(); 	//关闭USB连接
			USBH_DeInit(&USB_OTG_Core_dev, &USB_Host);
			USB_Host.usr_cb->DeInit();
			USB_Host.class_cb->DeInit(&USB_OTG_Core_dev,&USB_Host.device_prop);
	}
	
	USB_OTG_DisableGlobalInt(&USB_OTG_Core_dev);//关闭所有中断
	
	//重新复位USB
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,ENABLE);//USB OTG FS 复位
	delay_ms(5);
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,DISABLE);	//复位结束  

	memset(&USB_OTG_Core_dev,0,sizeof(USB_OTG_CORE_HANDLE));
	memset(&USB_Host,0,sizeof(USB_Host));
	
	//重新连接USB HID设备
	USBH_Init(&USB_OTG_Core_dev,USB_OTG_FS_CORE_ID,&USB_Host,&HID_cb,&USR_Callbacks); 
	
}


void Keybord_main(void)
{
	
	OS_LCD_Clear(White);
	LCD_SetTextColor(Blue);
	
	uart_init(115200);		//初始化串口波特率为115200，用于查看连接状态
	USBH_Init(&USB_OTG_Core_dev,USB_OTG_FS_CORE_ID,&USB_Host,&HID_cb,&USR_Callbacks); //初始化USB主机
	OS_String_Show(30,130,16,1,"USB Connecting...");	
	
	while(1)
	{
	
		OS_SchedLock();    /*  不要改  */
		
		USBH_Process(&USB_OTG_Core_dev, &USB_Host);
		
		if(bDeviceState==1)//连接建立了
		{ 
			if(USBH_Check_HIDCommDead(&USB_OTG_Core_dev,&HID_Machine))//检测USB HID通信,是否还正常? 
			{ 	    
				USBH_HID_Reconnect();//重连
			}				
			
		}
		else	//连接未建立的时候,检测
		{
			if(USBH_Check_EnumeDead(&USB_Host))	//检测USB HOST 枚举是否死机了?死机了,则重新初始化 
			{ 	    
				USBH_HID_Reconnect();//重连
			}			
		}
		
		OS_SchedUnlock();  /*  不要改  */

	}
	
}



