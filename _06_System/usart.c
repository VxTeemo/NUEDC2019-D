#include "sys.h"
#include "usart.h"
#include "Drive_Communication.h"

#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{

    while((UART4->SR&0X40)==0);//循环发送,直到发送完毕
    UART4->DR = (u8) ch;
    return ch;
}
#endif


#if EN_USART1_RX   //如果使能了接收
//串口4中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记

//初始化IO 串口4
//bound:波特率
void uart_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//使能UART4时钟

    //串口4对应引脚复用映射
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //GPIOC10复用为UART4
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); //GPIOC11复用为UART4

    //UART4端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOC10与GPIOC11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PC10，PC11

    //UART4 初始化设置
    USART_InitStructure.USART_BaudRate = bound;//波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(UART4, &USART_InitStructure); //初始化串口1

    USART_Cmd(UART4, ENABLE);  //使能串口4

    //USART_ClearFlag(UART4, USART_FLAG_TC);

#if EN_USART1_RX
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启相关中断

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//串口4中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器

#endif

}


void UART4_IRQHandler(void)                	//串口4中断服务程序
{
    static u16 i=0;
    static u8 rx_flag =0;
    u8 Res;

    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        Res =USART_ReceiveData(UART4);//(UART4->DR);	//读取接收到的数据

        if(i == 0 && Res == 0xAA)
            rx_flag =1;
        if(rx_flag == 1)
            communicationData.rxBuffer[i++] = Res;
        if(i == RX_LENGTH)
        {
            i=0;
            rx_flag =0;
            communicationData.flag = True;
        }

    }
    //USART_ClearITPendingBit(UART4, USART_IT_RXNE);

}
#endif





