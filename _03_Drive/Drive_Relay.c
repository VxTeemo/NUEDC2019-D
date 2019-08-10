#include "Drive_Relay.h"


void Relay_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_3| GPIO_Pin_5| GPIO_Pin_7| GPIO_Pin_9;
	
	Relay_Control(Relay_ALL,Relay_OFF);
	Relay_Control(Relay_7K,Relay_ON);
	
	GPIO_Init(GPIOG,&GPIO_InitStructure);
}

void Relay_Control(Relay_Select Relay_Num, Relay_State relay_state)
{
	switch(Relay_Num)
	{
		
		case Relay_631HSLS:PGout(1) = relay_state;  break;
		case Relay_MESS_0: PGout(3) = relay_state;  break;
		case Relay_9851:   PGout(5) = relay_state;  break;
		case Relay_LOAD:   PGout(7) = relay_state;  break;
		case Relay_7K:     PGout(9) = relay_state;  break;
		case Relay_ALL:  
			PGout(1) = relay_state; 
			PGout(3) = relay_state;
			PGout(5) = relay_state;
			PGout(7) = relay_state;
			PGout(9) = relay_state;
		break;
		
		default: break;
		
	}
}


