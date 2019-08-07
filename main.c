/*
*********************************************************************************************************
*                                              _04_OS
* File			 : main.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* Note       ：因为时间有限，只实现了简单的多线程，可能存在部分BUG，如果发现BUG麻烦联系QQ 1145474846
*********************************************************************************************************
*/
#include "User_header.h"

/*
***********************************************************************************************************
*
* 											任务堆栈定区（每个任务都需要开辟一个特定的堆栈）
*
*				Note:	堆栈大小根据任务局部变量多少定，单片机rom有限，尽量合理分配任务栈堆大小
*
***********************************************************************************************************
*/
unsigned int TASK_0_STK[64];
unsigned int TASK_1_STK[128];
unsigned int TASK_2_STK[128];
unsigned int TASK_3_STK[256];
unsigned int TASK_4_STK[4096];

unsigned int TASK_5_STK[256];
unsigned int TASK_6_STK[16000];

unsigned int EXTR_STK[84];
/************************************************************************************************************
*
*																函数定义区
*
************************************************************************************************************
*/


void OS_Init()
{

    System_init();

    LED_Init();

    GPIO_Key_Init();


    /*     LCD屏幕初始化    */
    OS_LCD_Init();	//LCD

    Touch_Init(); 	//触屏


    /*    各模块初始化      */

    ADF4351Init();			//PLL初始化

    ADC1_Init();			  //内置AD初始化

    Dac2_Init(); 				//内置DA初始化

//	  ADS1256_Init();     //24位AD初始化

    GridData_Init();   // 画图表格初始化

    uart_init(460800); //串口初始化



}



int main()
{

    Task_Create(LED_main,&TASK_0_STK[64-1],0);

    Task_Create(Touch_main,&TASK_1_STK[128-1],1);

    Task_Create(Key_main,&TASK_2_STK[128-1],2);

    Task_Create(Switch_main,&TASK_5_STK[256-1],3);

    Task_Create(ADF4351_Sweep_main,&TASK_3_STK[256-1],4);

    Task_Create(ADF4351_main,&TASK_4_STK[4096-1],5);

    Task_Create(FreqAna_main,&TASK_6_STK[16000-1],6);

    OSTaskSuspend(6);    //挂起任务

    OS_Init();

    OS_Start();

}


/************************************************* (C) COPYLEFT 2018 Muhe  *****************************************************/
