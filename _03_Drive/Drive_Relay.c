#include "Drive_Relay.h"


void Relay_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_3| GPIO_Pin_5| GPIO_Pin_7;
	
	Relay_AllOFF;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
}

void Relay_Control(Relay_Select Relay_Num, Relay_State relay_state)
{
	switch(Relay_Num)
	{
		
		case 1:  PGout(1) = relay_state;  break;
		case 2:  PGout(3) = relay_state;  break;
		case 3:  PGout(5) = relay_state;  break;
		case 4:  PGout(7) = relay_state;  break;
		case 5:  
			PGout(1) = relay_state; 
			PGout(3) = relay_state;
			PGout(5) = relay_state;
			PGout(7) = relay_state;
			break;
		default: break;
		
	}
}


