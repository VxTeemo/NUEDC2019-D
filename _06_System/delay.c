#include "delay.h"
#include "sys.h"
#include "User_header.h"

			 

//延时nus
//nus:要延时的us数.	
//nus:0~204522252(最大值即2^32/fac_us@fac_us=21)	    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;	//LOAD的值	 
	if(nus==0)return;			//nus=0,直接退出
	ticks=nus*fac_us; 			//需要的节拍数	  		 
	tcnt=0;
	OS_SchedLock();				//阻止ucos调度，防止打断us延时
	told=SysTick->VAL;        	//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;//时间超过/等于要延迟的时间,则退出.
		}  
	};
	OS_SchedUnlock();			//开启ucos调度 									    
}  
//延时nms
//nms:要延时的ms数
//nms:0~65535
void delay_ms(u16 nms)
{	

	if( OS_Running == 1)//如果os已经在跑了
	{		 
		if(nms>=fac_ms)//延时的时间大于ucos的最少时间周期 
		{ 
   			OSTimeDly(nms/fac_ms);	//ucos延时
		}
		nms%=fac_ms;			//ucos已经无法提供这么小的延时了,采用普通方式延时    
	}
	delay_us((u32)(nms*1000));	//普通方式延时 
}


















