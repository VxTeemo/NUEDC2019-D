#include "Drive_Relay.h"


void Relay_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7 | GPIO_Pin_8;

    GPIO_Init(GPIOF,&GPIO_InitStructure);
}

void Relay_Control(u8 Relay_Num, u8 Relay_State)
{
    switch(Relay_Num)
    {

    case 0: PFout(7) = Relay_State; break;
    case 1: PFout(8) = Relay_State; break;

    default: break;

    }
}


