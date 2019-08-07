#include "App_LED.h"


void LED_Control(u8 num);

void LED_main(void)
{
	  static u8 _led_cnt = 0;

		while(1)
		{
			 LED_Control(_led_cnt);
			 _led_cnt = (_led_cnt + 1)%4; 
 
			OSTimeDly(200);
		}
}

void LED_Control(u8 num)
{	
	 switch(num)
	 {
		 case 0: LED1 = 0; LED2 = 1;LED3 = 1;LED4 = 1;break;
		 case 1: LED1 = 1; LED2 = 0;LED3 = 1;LED4 = 1;break;
		 case 2: LED1 = 1; LED2 = 1;LED3 = 0;LED4 = 1;break;
		 case 3: LED1 = 1; LED2 = 1;LED3 = 1;LED4 = 0;break;
		 default :  LED1 = 1; LED2 = 1;LED3 = 1;LED4 = 1;break;
	 }
	
}	

