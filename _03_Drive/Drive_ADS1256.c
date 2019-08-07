#include "Drive_ADS1256.h"


//***************************
//		Pin assign	   	
//		STM32						ADS1256

//		GPIOG_Pin_0 		---> SCLK
//		GPIOG_Pin_2			<--- DIN
//		GPIOG_Pin_4  		---> DOUT
//		GPIOG_Pin_6 		<--- DRDY
//		GPIOG_Pin_8 		---> CS
//		GPIOG_Pin_10  	<--- PDWN

//***************************	

/*端口定义*/ 
#define SCLK		 	 GPIO_Pin_0
#define DIN	 			 GPIO_Pin_4
#define DOUT			 GPIO_Pin_2
#define DRDY			 GPIO_Pin_6
#define CS				 GPIO_Pin_8
#define PDWN		   GPIO_Pin_10

#define ADS1256_CS_0()   		GPIO_ResetBits(GPIOG,CS)
#define ADS1256_CS_1()   		GPIO_SetBits(GPIOG,CS)

#define ADS1256_SCLK_0()  	GPIO_ResetBits(GPIOG,SCLK)
#define ADS1256_SCLK_1()  	GPIO_SetBits(GPIOG,SCLK)

#define ADS1256_DIN_0()   	GPIO_ResetBits(GPIOG,DOUT) //注意这里，单片机的DOUT连接ADS1256的DIN
#define ADS1256_DIN_1()   	GPIO_SetBits(GPIOG,DOUT)

#define ADS1256_DOUT			 (GPIOG->IDR & DIN)          //
#define ADS1256_DRDY			 (GPIOG->IDR & DRDY)
#define ADS1256_PDWN		   (GPIOG->IDR & PDWN)



/*
*********************************************************************************************************
*	函 数 名: Init_ADS1256_GPIO
*	功能说明: 初始化ADS1256 GPIO
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Init_ADS1256_GPIO(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

		GPIO_InitStructure.GPIO_Pin = SCLK | DOUT | CS ;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN; 
		GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	  ADS1256_CS_1();
	
	  GPIO_InitStructure.GPIO_Pin = DIN | DRDY | PDWN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
		GPIO_Init(GPIOG,&GPIO_InitStructure);
}


/*
*********************************************************************************************************
*	函 数 名: SPI_WriteByte
*	功能说明: 向SPI总线发送8个bit数据。 不带CS控制。
*	形    参: _data : 数据
*	返 回 值: 无
*********************************************************************************************************
*/
void SPI_WriteByte(u8 TxData)
{
	u8 i;
	
	/* 连续发送多个字节时，需要延迟一下 */
	delay_us(10);
	
	for(i = 0; i < 8; i++)
	{
		ADS1256_SCLK_1();
		
		if (TxData & 0x80)
			ADS1256_DIN_1();  
		else
			ADS1256_DIN_0();
		
		TxData <<= 1;
		
		delay_us(5);
		ADS1256_SCLK_0();	
		delay_us(5);
	}
	
} 

/*
*********************************************************************************************************
*	函 数 名: SPI_ReadByte
*	功能说明: 从SPI总线接收8个bit数据。 不带CS控制。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
u8 SPI_ReadByte(void)
{
	u8 i;
	u8 read = 0;
	
	for (i = 0; i < 8; i++)
	{
		ADS1256_SCLK_1();
		read = read<<1;
		delay_us(5);
		ADS1256_SCLK_0();
		
		if(ADS1256_DOUT)
				read ++;
		
		delay_us(5);
	}
	
	return read;
}


/*
*********************************************************************************************************
*	函 数 名: ADS1256WREG
*	功能说明: ADS1256 写数据
*	形    参: regaddr:寄存器地址
						databyte:待写入数据
*	返 回 值: 无
*********************************************************************************************************
*/
void ADS1256WREG(u8 regaddr,u8 databyte)
{
	
	ADS1256_CS_0();
	while(ADS1256_DRDY);//当ADS1256_DRDY为低时才能写寄存器
	
	//向寄存器写入数据地址
	SPI_WriteByte(ADS1256_CMD_WREG | (regaddr & 0x0F));
	
	//写入数据的个数n-1
	SPI_WriteByte(0x00);
	
	delay_us(10);
	
	//向regaddr地址指向的寄存器写入数据databyte
	SPI_WriteByte(databyte);
	
	ADS1256_CS_1();
}

/*
*********************************************************************************************************
*	函 数 名: ADS1256RREG
*	功能说明: ADS1256 读数据
*	形    参: regaddr:寄存器地址
*	返 回 值: 无
*********************************************************************************************************
*/
u8 ADS1256RREG(u8 regaddr)
{
	u8 databyte;
	
	ADS1256_CS_0();
	while(ADS1256_DRDY);//当ADS1256_DRDY为低时才能写寄存器
	
	//向寄存器写入数据地址
	SPI_WriteByte(ADS1256_CMD_RREG | (regaddr & 0x0F));
	
	//写入数据的个数n-1
	SPI_WriteByte(0x00);
	
	delay_us(10);
	
	//向regaddr地址指向的寄存器写入数据databyte
	databyte=SPI_ReadByte();
	
	ADS1256_CS_1();

  return databyte;
}


/*
*********************************************************************************************************
*	函 数 名: ADS1256_Init
*	功能说明: 初始化ADS1256
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void ADS1256_Init(void)
{
	Init_ADS1256_GPIO(); 																							  //ADS1256_GPIO初始化
	delay_ms(10);
	
	ADS1256_CS_0();
	delay_us(100);
	while(ADS1256_DRDY);
	SPI_WriteByte(ADS1256_CMD_REST);																		//复位
	delay_ms(10);
	
	while(ADS1256_DRDY);
	SPI_WriteByte(ADS1256_CMD_SELFCAL);																	 //自动校准
	
	while(ADS1256_DRDY);                             		 								//当ADS1256_DRDY为低时才能写寄存器
	SPI_WriteByte(ADS1256_CMD_SYNC);                 										//同步
	SPI_WriteByte(ADS1256_CMD_WAKEUP);              										//同步唤醒
	
	while(ADS1256_DRDY);
	ADS1256WREG(ADS1256_STATUS,0x04);               										// 高位在前、不使用缓冲
	ADS1256WREG(ADS1256_MUX,ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);		//设置通道
	ADS1256WREG(ADS1256_ADCON,ADS1256_GAIN_1);      										// 放大倍数1
	ADS1256WREG(ADS1256_DRATE,ADS1256_DRATE_30000SPS); 									// 数据1000sps,不同速率注意采样时间间隔
	ADS1256WREG(ADS1256_IO,0x00);  																			//不使用数据IO口
	
	while(ADS1256_DRDY);
	SPI_WriteByte(ADS1256_CMD_SELFCAL);																	 //自动校准
	ADS1256_CS_1();

}             



/*
*********************************************************************************************************
*	函 数 名: ADS1256ReadData
*	功能说明: 读取24位AD值
*	形    参: channel:通道选择
*	返 回 值: AD值
*********************************************************************************************************
*/
u32 ADS1256ReadData(u8 channel)  
{
	u32 sum=0;

	while(ADS1256_DRDY);								//当ADS1256_DRDY为低时才能写寄存器
	ADS1256WREG(ADS1256_MUX,channel);		//设置通道
	
  ADS1256_CS_0();
	delay_us(10);
	
	SPI_WriteByte(ADS1256_CMD_SYNC);		//同步
	SPI_WriteByte(ADS1256_CMD_WAKEUP);	//同步唤醒               
	SPI_WriteByte(ADS1256_CMD_RDATA);   //读取数据

	sum = (SPI_ReadByte() << 16);
	sum |= (SPI_ReadByte() << 8);
	sum |= SPI_ReadByte();

	ADS1256_CS_1();
	
	while(ADS1256_DRDY);
	
	return sum;
}

/*
*********************************************************************************************************
*	函 数 名: Get_Val
*	功能说明: 将读取的24位AD值转化成电压值
*	形    参: addata:AD值
*	返 回 值: 电压值
*********************************************************************************************************
*/
float Get_Val(u32 addata)
{
	 /*		PGA由ADS1256初始化设置，VREF为电路的基准源，根据实际情况修改*/
	 u8 PGA=1;
	 float VREF=2.4922f;
	
	 float val;
	
	 /*  负电压的转化  */
	 if(addata > 0x800000)
	 {
			addata=0xFFFFFF-addata;
		  val= -2*VREF/(PGA*8388607)*addata;
	 }
   else
			val=2*VREF/(PGA*8388607)*addata;

	 return val;
}



/*
*********************************************************************************************************
*	函 数 名: Middle_Filter
*	功能说明: 滑动平均滤波
*	形    参: channel:通道选择
						times:数据个数
*	返 回 值: 滤波后的AD值
*********************************************************************************************************
*/
u32 Moving_Average_Filter(u8 channel,u16 times) 
{

	 u16 i,counts=0;
	 u32 sum=0,data[100];
	 u32 value;
	 u32 max,min;


	 //读取times个数据
	 while(counts < times)
	 {
			data[counts++]=ADS1256ReadData(channel);
	 }

	 max=min=data[0];
	 
	 for(i=0;i<times;i++)
	 {
		  if(data[i] > max)
				max=data[i];
			
			if(data[i] < min)
				min=data[i];
			
			sum+=data[i];
	 }

	 value=(sum - max - min)/(times - 2);
	 
	 return value;
}






