/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Os_cpu.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
*********************************************************************************************************
*/
#ifndef __OS_CPU_H
#define __OS_CPU_H 	 

#include "sys.h"

#ifdef   OS_SET
#define  OS_EXT
#else
#define  OS_EXT  extern
#endif

/*
*****************************************************************************
*                 	  系统变量宏定义
*****************************************************************************
*/

#define OS_EXCEPT_STK_SIZE 		128u					//堆栈大小
#define OS_MAX_Task 			32u					//最大任务数(优先级数)
#define IDLE_STK_SIZE 			64u					//空闲任务堆栈大小
#define OS_MAX_Event 			32u					//最大事件数
#define System_Ticks  			1000u					//每1000/System_Ticks ms进入一次中断



#define OS_Resume			 1u						//任务为正常状态
#define OS_Suspend     2u						//任务为挂起状态



#define  OS_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}
#define  OS_EXIT_CRITICAL()   {OS_CPU_SR_Restore(cpu_sr);}

/*
*****************************************************************************
*                   	系统变量全局定义
*****************************************************************************
*/

typedef struct os_tcb
{
		unsigned int *StkPtr;//任务栈顶
		unsigned int DLy;//任务延时时钟
		unsigned char OSTCBStatPend;//任务状态
	
}TCB;


OS_EXT unsigned int  CPU_ExceptStk[OS_EXCEPT_STK_SIZE];//主任务堆栈
OS_EXT unsigned int * CPU_ExceptStkBase;//指向的是数组最后一个元素
OS_EXT unsigned int IDLE_STK[IDLE_STK_SIZE];//空闲任务堆栈
OS_EXT TCB *p_TCB_Cur;//指向当前任务的tcb
OS_EXT TCB *p_TCBHightRdy;//指向最高级任务的tcb


OS_EXT TCB TCB_Task[OS_MAX_Task];//任务控制块
OS_EXT volatile unsigned char OS_PrioCur;//记录当前运行的任务优先级
OS_EXT volatile unsigned char OS_PrioHighRdy;
OS_EXT volatile unsigned int OSRdyTbl;//任务就绪表
//OS_EXT unsigned int OS_Tisks;
OS_EXT unsigned int fac_ms;//ms延时倍乘数,在ucos下,代表每个节拍的ms数
OS_EXT unsigned int fac_us;//us延时倍乘数	
OS_EXT unsigned char Sched_flag;//调度器标志位
OS_EXT unsigned char OS_Running;//任务运行标志位

extern void OSCtxSw(void);
extern void OSStartHighRdy(void);
extern unsigned int OS_CPU_SR_Save(void);
extern void OS_CPU_SR_Restore(unsigned int cpu_sr);


/*
*****************************************************************************
*                   	系统函数定义
*****************************************************************************
*/

void System_init(void);
void Task_End(void);//任务结束
void Task_Create(void (*task)(void),unsigned int *stk,unsigned char prio);//创建任务
void OS_Start(void); //创建空闲任务
void OS_Sched(void);//任务调度
void OS_SchedLock(void);//调度器上锁
void OS_SchedUnlock(void);//调度器解锁
void OSSetPrioRdy(unsigned char prio);//设置优先级
void OSDelPrioRdy(unsigned char prio);//删除优先级
void OSTimeDly(unsigned int ticks);//系统延时函数
int OSTaskSuspend(unsigned char prio);//挂起任务
int OSTaskRecovery(unsigned char prio);//任务恢复

#endif

/************************************************* (C) COPYLEFT 2018 Muhe  *****************************************************/
