/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_IIC.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : TMP275测温模块驱动程序
*********************************************************************************************************
*/
#include "Drive_IIC.h"

/* 私有宏定义 ----------------------------------------------------------------*/
//IO方向设置
#define SDA_IN()  {GPIOF->MODER&=~(3<<(2*2));GPIOF->MODER|=0<<2*2;}	//PF2输入模式
#define SDA_OUT() {GPIOF->MODER&=~(3<<(2*2));GPIOF->MODER|=1<<2*2;} //PF2输出模式
//IO操作函数	 
#define IIC_SCL    GPIOF_OUT(1) //SCL	
#define IIC_SDA    GPIOF_OUT(2) //SDA	 
#define READ_SDA   GPIOF_IN(2)	//输入SDA 	

/* 私有（静态）函数声明 ------------------------------------------------------*/

/* 全局变量定义 --------------------------------------------------------------*/
 
/* 全局函数编写 --------------------------------------------------------------*/
/**----------------------------------------------------------------------------
* @FunctionName  : IIC_Init()     
* @Description   : None 
* @Data          : 2016/7/24
* @Explain       : IIC初始化
------------------------------------------------------------------------------*/  
//初始化IIC
void IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOB时钟

  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
  IIC_SCL=1;
  IIC_SDA=1;
}

/**----------------------------------------------------------------------------
* @FunctionName  : IIC_Start()     
* @Description   : None 
* @Data          : 2016/7/24
* @Explain       : IIC信号产生
------------------------------------------------------------------------------*/  
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	Delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	Delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  

/**----------------------------------------------------------------------------
* @FunctionName  : IIC_Stop()     
* @Description   : None 
* @Data          : 2016/7/24
* @Explain       : 产生IIC停止信号
------------------------------------------------------------------------------*/  

void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	Delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	Delay_us(4);							   	
}

/**----------------------------------------------------------------------------
* @FunctionName  : IIC_Wait_Ack()     
* @Description   : None 
* @Data          : 2016/7/24
* @Explain       : 等待应答信号到来
------------------------------------------------------------------------------*/  
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;Delay_us(1);	   
	IIC_SCL=1;Delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 

/**----------------------------------------------------------------------------
* @FunctionName  : IIC_Ack()     
* @Description   : None 
* @Data          : 2016/7/24
* @Explain       : 产生ACK应答
------------------------------------------------------------------------------*/  
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	Delay_us(2);
	IIC_SCL=1;
	Delay_us(2);
	IIC_SCL=0;
}

/**----------------------------------------------------------------------------
* @FunctionName  : IIC_NAck()     
* @Description   : None 
* @Data          : 2016/7/24
* @Explain       : 不产生ACK应答
------------------------------------------------------------------------------*/     
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	Delay_us(2);
	IIC_SCL=1;
	Delay_us(2);
	IIC_SCL=0;
}					 			

/**----------------------------------------------------------------------------
* @FunctionName  : IIC_Send_Byte()     
* @Description   : None 
* @Data          : 2016/7/24
* @Explain       : IIC发送一个字节
------------------------------------------------------------------------------*/  
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	  SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
    IIC_SDA=(txd&0x80)>>7;
    txd<<=1; 	  
		Delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		Delay_us(2); 
		IIC_SCL=0;	
		Delay_us(2);
    }	 
} 	    

/**----------------------------------------------------------------------------
* @FunctionName  : IIC_Read_Byte()     
* @Description   : None 
* @Data          : 2016/7/24
* @Explain       : 读1个字节，ack=1时，发送ACK，ack=0，发送nACK  
------------------------------------------------------------------------------*/  
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        Delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		Delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}
	
	
	
	







