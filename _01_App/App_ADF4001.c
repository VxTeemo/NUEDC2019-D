#include "Drive_ADF4001.h"

void ADF4001_main()
{
		static float freq=520;
		OS_LCD_Clear(White);
		
		OS_TextColor_Set(White);
		//OS_BackColor_Set(White);
		while(1)
		{
			if(GPIO_KEY1==0)
			{
				freq++;
				ADF4001_SetFreq(120,(u16)freq,R_DIVIDER_OUTPUT);	
				OS_Num_Show(400,200,24,1, freq*0.2f-23.0f ,"1:%.1fMHZ     ");
			}
			else if(GPIO_KEY2==0)
			{
				freq--;
				ADF4001_SetFreq(120,(u16)freq,R_DIVIDER_OUTPUT);	
				OS_Num_Show(400,200,24,1, freq*0.2f -23.0f,"1:%.1fMHZ     ");
			}
			OSTimeDly(100);
		}
}
			
