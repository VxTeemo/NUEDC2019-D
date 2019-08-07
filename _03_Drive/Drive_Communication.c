#include "Drive_Communication.h"


/*通信数据结构体*/
communicationDataStruct communicationData = {{0},{0}};


/**
* @brief  获取CRC校验码
* @param  none
* @retval CRC校验码
*/
static u16 crc_16(u8 *Datas, u16 Length) 
{
	u8 j;
	u16 temp = 0xFFFF, i;

	for(i=0; i<Length; ++i)
	{
		temp ^= Datas[i];

		for(j=0; j<8; ++j)
		{
			if(temp & 1)
			{
				temp >>= 1;
				temp ^= 0xA001;
			}
			else
			{
				temp >>= 1;
			}
		}
	}

	return temp;
}

/**
 * @brief  USART1发送数据
 * @param  *sendData : 待发送的数据
           length    : 数据长度
 * @retval 无
 */
static void usartSendData(u8 *sendData,u16 length)
{
	u16 i;
	
	for(i=0 ; i<length ; i++)
	{
		USART_SendData(USART1,sendData[i]);
		
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
}




/**
* @brief  发送波形数据
* @param  data :DDS结构体变量
* @retval none
*/
void sendData(u8 buff[],u16 length,u8 num)     
{
    u16 crcValue;
    u16 i;
	
    communicationData.txBuffer[0] = 0xAA;
    communicationData.txBuffer[1] = num ;
	
	  for(i=0;i<length;i++)
	  {
			communicationData.txBuffer[i+2]=buff[i];
		}

//    /*生成CRC校验码*/
    crcValue = crc_16(&communicationData.txBuffer[0],TX_LENGTH - 3);

    communicationData.txBuffer[TX_LENGTH-3] = crcValue>>8;
    communicationData.txBuffer[TX_LENGTH-2] = crcValue & 0xff;
	
		communicationData.txBuffer[TX_LENGTH-1] = 0x55;			
		
		printf("%s\r\n","AT+CIPSEND=2048");//TX_LENGTH修改是自行更改
	  delay_us(1000);
		
    usartSendData(communicationData.txBuffer,TX_LENGTH);			//发送指定长度的数据
}



///**
//* @brief  接收数据处理
//* @param  none
//* @retval none
//*/
//void rxDataprocess(void)
//{
//    
//}


/**----------------------------------------------------------------------------
* @FunctionName  : ESP8266_Client_Connect_Server    
* @Description   : 客户端连接服务器 
* @Data          : 2017/9/28
* @Explain       : None
------------------------------------------------------------------------------*/
void ESP8266_Client_Connect_Server(void)
{
		//OS_String_Show(30,400,32,1,"ESP8266 Client Connecting to Server...");
	
	  printf("%s\r\n","AT");
		delay_ms(10);
	
		printf("%s\r\n","ATE0");
		delay_ms(10);
	
		printf("%s\r\n","AT+CIPMUX=0");
		delay_ms(10);
	
		while(atk_8266_send_cmd((u8 *)"AT+CIPSTART=\"TCP\",\"192.168.4.1\",8080",(u8 *)"CONNECT",2000)) {}
	
	  OS_String_Show(350,5,24,1," ESP8266 OK ");
}


/**----------------------------------------------------------------------------
* @FunctionName  : ESP8266_Server_Init    
* @Description   : 服务器初始化 
* @Data          : 2017/9/28
* @Explain       : None
------------------------------------------------------------------------------*/
void ESP8266_Server_Init(void)
{
		OS_String_Show(30,200,32,1,"ESP8266 Server Initializing...");
	
//	  printf("%s\r\n","AT+RST");
//		delay_ms(1000);
	
		printf("%s\r\n","ATE0");
		delay_ms(10);
	
		printf("%s\r\n","AT+CIPMUX=1");
		delay_ms(10);
	
	  printf("%s\r\n","AT+CIPSERVER=1,8080");
		delay_ms(10);
	
	  OS_String_Show(30,200,32,1,"ESP8266 Server Initializ Successful");
}

