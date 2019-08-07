/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Os_cpu.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
*********************************************************************************************************
*/

#define OS_SET

#include "Os_cpu.h"
#include "Os_malloc.h"

unsigned char Sched_flag = 1;//调度器标志位

void OSGetHighRdy(void);
/*
*************************************************************************
*
*	功能  ：	设置优先级
*	参数  ：	1：任务级数
*
**************************************************************************
*/
__inline void OSSetPrioRdy(unsigned char prio)
{
	OSRdyTbl|=0x01<<prio;
}

/*
*************************************************************************
*
*	功能	：	删除优先级
*	参数	：	1：任务级数
*
**************************************************************************
*/
__inline void OSDelPrioRdy(unsigned char prio)
{
	OSRdyTbl&=~(0x01<<prio);
}

/*
*************************************************************************
*
*	功能	：	获取优先级
*	参数	：
*
**************************************************************************
*/
__inline void OSGetHighRdy(void)				
{									
	unsigned char	OS_NEXT_Prio;	
	for(OS_NEXT_Prio=0;(OS_NEXT_Prio<OS_MAX_Task)&&(!(OSRdyTbl&(0x01<<OS_NEXT_Prio)));OS_NEXT_Prio++);
	OS_PrioHighRdy=OS_NEXT_Prio;	
}

/*
*************************************************************************
*
*	功能	：	任务调度（查找优先级最高任务并调度）
*	参数	：
*
**************************************************************************
*/
void OS_Sched(void)
{
	unsigned int cpu_sr;
	OS_ENTER_CRITICAL();                                  //进入临界区 除能所有中断 NMI除外
	
	if(Sched_flag)
	{
			OSGetHighRdy();    							//找出任务就绪表中优先级最高的任务
			if(OS_PrioHighRdy!=OS_PrioCur)              //如果不是当前运行任务，进行任务调度
			{
				p_TCBHightRdy=&TCB_Task[OS_PrioHighRdy];
				//p_TCB_Cur=&TCB_Task[OS_PrioCur];
				OS_PrioCur= OS_PrioHighRdy;//更新OS_PrioCur
				OSCtxSw();//调度任务,在汇编中引用
			}
	}
	
	OS_EXIT_CRITICAL();                                 //退出临界区
}
/*
*************************************************************************
*
*	功能	：	调度器上锁
*	参数	：
*
**************************************************************************
*/
void OS_SchedLock(void)
{
	unsigned int cpu_sr;
	OS_ENTER_CRITICAL();                                  //进入临界区

	Sched_flag = 0;
	
	OS_EXIT_CRITICAL();                                 //退出临界区
}

/*
*************************************************************************
*
*	功能	：	调度器解锁
*	参数	：
*
**************************************************************************
*/
void OS_SchedUnlock(void)
{
	unsigned int cpu_sr;
	OS_ENTER_CRITICAL();                                  //进入临界区

	Sched_flag = 1;
	
	OS_EXIT_CRITICAL();                                 //退出临界区
}

/*
*************************************************************************
*
*	功能	：	初始化滴答定时器
*	参数	：
*
**************************************************************************
*/
void System_init(void)
{
	u32 reload;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8=168/8=21M 
	fac_us=SystemCoreClock/8000000;							//为系统时钟的 1/8
	
	reload=SystemCoreClock/8000000;							//每秒钟的计数次数 单位为K	   
	reload*=1000000/System_Ticks;		//根据System_Ticks设定溢出时间
											//reload为24位寄存器,最大值:16777216,在168M下,约合0.7989s左右	
	fac_ms=1000/System_Ticks;
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 		//每1/System_Ticks秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK    

}

/*
*************************************************************************
*
*	功能	：	滴答定时器中断
*	参数	：	每1/System_Ticks 中断一次 （暂定10ms）
*
**************************************************************************
*/
void SysTick_Handler(void)
{
	unsigned int cpu_sr;
	unsigned char i=0;

	
	//OS_Tisks++;
	
	if(OS_Running == 1 &&Sched_flag == 1)//系统运行后再开始
	{
			for(;i<OS_MAX_Task;i++)
		 {
				
				OS_ENTER_CRITICAL();
				if(TCB_Task[i].DLy)
				{
					TCB_Task[i].DLy-=1000/System_Ticks;
					if(TCB_Task[i].DLy==0&&TCB_Task[i].OSTCBStatPend == OS_Resume)			//延时时钟到达且任务状态为正常状态
					{
						OSSetPrioRdy(i);            //任务重新就绪
					}
				}
					
				OS_EXIT_CRITICAL();
		 }
		 
		OS_Sched();//都是由pendsv中断进行调度
	 }
}

/*
*************************************************************************
*
*	功能	：	用于删除任务
*	P S 	：	任务一般不需要删除
*
**************************************************************************
*/
void Task_End()
{
	while(1)
	{
		;
	}
}

/*
*************************************************************************
*
*	功能	：	用于创建任务
*	参数	：	1：	任务函数指针
*						2：	任务堆栈指针
*						3：	优先级
*
**************************************************************************
*/
void Task_Create(void (*task)(void),unsigned int *stk,unsigned char prio)
{
	unsigned int * p_stk;
	p_stk=stk;
	p_stk=(unsigned int *) ((unsigned int)(p_stk)&0xFFFFFFF8u);
	
#if (__FPU_PRESENT==1)&&(__FPU_USED==1)	
	*(--p_stk) = (unsigned int)0x00000000L; //No Name Register  
	*(--p_stk) = (unsigned int)0x00001000L; //FPSCR
	*(--p_stk) = (unsigned int)0x00000015L; //s15
	*(--p_stk) = (unsigned int)0x00000014L; //s14
	*(--p_stk) = (unsigned int)0x00000013L; //s13
	*(--p_stk) = (unsigned int)0x00000012L; //s12
	*(--p_stk) = (unsigned int)0x00000011L; //s11
	*(--p_stk) = (unsigned int)0x00000010L; //s10
	*(--p_stk) = (unsigned int)0x00000009L; //s9
	*(--p_stk) = (unsigned int)0x00000008L; //s8
	*(--p_stk) = (unsigned int)0x00000007L; //s7
	*(--p_stk) = (unsigned int)0x00000006L; //s6
	*(--p_stk) = (unsigned int)0x00000005L; //s5
	*(--p_stk) = (unsigned int)0x00000004L; //s4
	*(--p_stk) = (unsigned int)0x00000003L; //s3
	*(--p_stk) = (unsigned int)0x00000002L; //s2
	*(--p_stk) = (unsigned int)0x00000001L; //s1
	*(--p_stk) = (unsigned int)0x00000000L; //s0
#endif
	//以下寄存器顺序和PendSV退出时寄存器恢复顺序一致
	*(--p_stk)=(unsigned int)0x01000000uL;//xPSR状态寄存器、第24位THUMB模式必须置位一 
	*(--p_stk)=(unsigned int)task;//entry point//函数入口
	*(--p_stk)=(unsigned int)Task_End ;//R14(LR);
	*(--p_stk)=(unsigned int)0x12121212uL;//R12
	*(--p_stk)=(unsigned int)0x03030303uL;//R3
	*(--p_stk)=(unsigned int)0x02020202uL;//R2
	*(--p_stk)=(unsigned int)0x01010101uL;//R1
	*(--p_stk)=(unsigned int)0x00000000uL;//R0

	
#if (__FPU_PRESENT==1)&&(__FPU_USED==1)	
	*(--p_stk) = (unsigned int)0x00000031L; //s31
	*(--p_stk) = (unsigned int)0x00000030L; //s30
	*(--p_stk) = (unsigned int)0x00000029L; //s29
	*(--p_stk) = (unsigned int)0x00000028L; //s28
	*(--p_stk) = (unsigned int)0x00000027L; //s27
	*(--p_stk) = (unsigned int)0x00000026L; //s26	
	*(--p_stk) = (unsigned int)0x00000025L; //s25
	*(--p_stk) = (unsigned int)0x00000024L; //s24
	*(--p_stk) = (unsigned int)0x00000023L; //s23
	*(--p_stk) = (unsigned int)0x00000022L; //s22
	*(--p_stk) = (unsigned int)0x00000021L; //s21
	*(--p_stk) = (unsigned int)0x00000020L; //s20
	*(--p_stk) = (unsigned int)0x00000019L; //s19
	*(--p_stk) = (unsigned int)0x00000018L; //s18
	*(--p_stk) = (unsigned int)0x00000017L; //s17
	*(--p_stk) = (unsigned int)0x00000016L; //s16
#endif
	//PendSV发生时未自动保存的内核寄存器：R4~R11
	*(--p_stk)=(unsigned int)0x11111111uL;//R11
	*(--p_stk)=(unsigned int)0x10101010uL;//R10
	*(--p_stk)=(unsigned int)0x09090909uL;//R9
	*(--p_stk)=(unsigned int)0x08080808uL;//R8
	*(--p_stk)=(unsigned int)0x07070707uL;//R7
	*(--p_stk)=(unsigned int)0x06060606uL;//R6
	*(--p_stk)=(unsigned int)0x05050505uL;//R5
	*(--p_stk)=(unsigned int)0x04040404uL;//R4
	
	TCB_Task[prio].StkPtr =p_stk;//将该任务控制块中应当指向栈顶的指针，指向了该任务的新栈顶
	TCB_Task[prio].DLy =0;
	TCB_Task[prio].OSTCBStatPend = 1;
	OSSetPrioRdy(prio);
}
/*
*************************************************************************
*
*	功能	：	空闲任务
*	P S 	：	防止CPU空载运行造成系统崩溃
*
**************************************************************************
*/
void OS_IDLE_Task()
{
		unsigned int count=0;
	
		while(1)
		{
			count++;
		
		}
	
}

/*
*************************************************************************
*
*	功能	：	系统启动
*	参数	：
*
**************************************************************************
*/
void OS_Start(void)
{
	//System_init();
	if(OS_Running == 0)
	{
		
		CPU_ExceptStkBase=CPU_ExceptStk+OS_EXCEPT_STK_SIZE-1;//Cortex-M3栈向下增长
		Task_Create(OS_IDLE_Task,&IDLE_STK[IDLE_STK_SIZE-1],OS_MAX_Task-1);
		OSGetHighRdy();//获得最高级的就绪任务
		OS_PrioCur= OS_PrioHighRdy;
		p_TCBHightRdy=&TCB_Task[OS_PrioHighRdy];
		OS_Running = 1 ;
		OSStartHighRdy();
	
	}

}

/*
*************************************************************************
*
*	功能	：	系统延时函数
*	参数	：	1：时间  ms
*
**************************************************************************
*/
void OSTimeDly(unsigned int ticks)
{
	if(ticks> 0)
	{
		unsigned int cpu_sr;
		OS_ENTER_CRITICAL();                                  //进入临界区
		OSDelPrioRdy(OS_PrioCur);                             //将任务挂起
		TCB_Task[OS_PrioCur].DLy= ticks;                      //设置TCB中任务延时节拍数
		OS_EXIT_CRITICAL();                                   //退出临界区
		OS_Sched();                                           //任务调度
	}
}
/*
*************************************************************************
*
*	功能	：	挂起任务
*	参数	:		1: 被挂起任务的优先级
*
*************************************************************************
*/
int OSTaskSuspend(unsigned char prio)
{
		unsigned char flag=1,nprio;
		unsigned int  cpu_sr;
	
		OS_ENTER_CRITICAL();                                  //进入临界区
	
		if(prio==OS_PrioCur)//判断是否是挂起当前任务
		{
			flag = 0;
		}
		
		nprio = 0x01 && ( OSRdyTbl >> prio );//判断挂起任务是否存在
		
		if(nprio==0)		//挂起的任务不存在
		{
			OS_EXIT_CRITICAL();  														  //退出临界区
			
			return nprio;
		}
		else
		{
			TCB_Task[prio].OSTCBStatPend = OS_Suspend; //改变任务状态为挂起状态
			OSDelPrioRdy(prio);			//删除任务优先级
		}
		
		OS_EXIT_CRITICAL();                                   //退出临界区
		
		if(flag==0)
		{
				OS_Sched();                                           //任务调度
		}
		
		return 1;
}

/*
*************************************************************************
*
*	功能	：	恢复任务
*	参数	：	1：被恢复任务的优先级
*
**************************************************************************
*/
int OSTaskRecovery(unsigned char prio)
{
		unsigned char nprio;
		unsigned int  cpu_sr;
	
		OS_ENTER_CRITICAL();                                  //进入临界区
		nprio = 0x01 & ( OSRdyTbl >> prio );//判断挂起任务是否存在
		if(nprio == 1)	//恢复的任务优先级已经存在
		{
				OS_EXIT_CRITICAL();                                   //退出临界区
				return 0;
		}
		if(TCB_Task[prio].OSTCBStatPend == OS_Suspend)//任务为挂起状态 
		{
			
				OSSetPrioRdy(prio); //恢复任务优先级
			
				TCB_Task[prio].OSTCBStatPend=1; //修改任务状态为正常状态
			
				if(TCB_Task[prio].DLy == 0)//任务无延时
				{
						OS_Sched();                                           //任务调度
				}
		}
		else 
		{
				OS_EXIT_CRITICAL();                                   //退出临界区
				return 0;
		}
	
	
		OS_EXIT_CRITICAL();                                   //退出临界区
		
		return 1;
	
	
}



