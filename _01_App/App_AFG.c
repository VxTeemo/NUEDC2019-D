/*
*********************************************************************************************************
*                                              _04_OS
* File			 : App_AFG.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : 简单的模拟函数信号发生器
*********************************************************************************************************
*/
#include "App_AFG.h"


#define MAXX 800
#define MAXY 480

#define MAXDUTYCYCLE 90
#define MINDUTYCYCLE 10

#define MAXMODULATHZ 5000

#define MAXMODULATVPP 3
#define MAXHZ      500000
#define MINHZ      50

#define MAXVPP     3
#define MINVPP     1
void ShowWaveInfo(void);


struct inputstruct
{
	uint64_t num;
	int update;
}Input={0,0};


/* 全局变量定义 --------------------------------------------------------------*/
int freshwave = 1;
 
/* 全局变量声明 --------------------------------------------------------------*/
uint32_t hzhz = 10;
u8 AFG_Flag=0;

void AFG_main()
{
	
	while(1)
	{
		if(AFG_Flag==0)
		{
			
				AFG_Flag=1;
			OS_LCD_Clear(White);


			OS_String_Show(380,0,32,1,"AFG");
			OS_String_Show(400,100+28,24,1," 1      2      3      Hz");
			OS_String_Show(400,180+28,24,1," 4      5      6      kHz");
			OS_String_Show(400,260+28,24,1," 7      8      9      10mV");
			OS_String_Show(400,340+28,24,1," 0      C      B      duty");
			OS_String_Show(400,420,24,1," ChangeWave             ");
			
			ShowWaveInfo();
			setDDS(ddsStructData.vpp,ddsStructData.hz,ddsStructData.dutycycle,ddsStructData.wave);
			
		
		}
		if(freshwave)
		{
			Input.num = 0;
			
			if(ddsStructData.vpp>3.5){ddsStructData.vpp=3.5;}
			if(ddsStructData.vpp<1.0){ddsStructData.vpp=1.0;}
			if(ddsStructData.hz>200000){ddsStructData.hz=200000;}
			if(ddsStructData.hz<5){ddsStructData.hz=5;}
			if(ddsStructData.dutycycle>90){ddsStructData.dutycycle=90;}
			if(ddsStructData.dutycycle<10){ddsStructData.dutycycle=10;}
			
			ShowWaveInfo();
			setDDS(ddsStructData.vpp,ddsStructData.hz,ddsStructData.dutycycle,ddsStructData.wave);
			freshwave=0;
		}
//		if(Key_Now_Get(KEY1,KEY_MODE_SHORT))
//		{
//			OSTask_Alter(&App_AFG,NULL,NULL,OS_STATE_CLOSE,NULL);
//		}


		if(Touch_Judge(400,100,480,180) == 2){Input.num=Input.num*10+1;Input.update = 1;}
		if(Touch_Judge(480,100,560,180) == 2){Input.num=Input.num*10+2;Input.update = 1;}
		if(Touch_Judge(560,100,640,180) == 2){Input.num=Input.num*10+3;Input.update = 1;}
		
		if(Touch_Judge(400,180,480,260) == 2){Input.num=Input.num*10+4;Input.update = 1;}
		if(Touch_Judge(480,180,560,260) == 2){Input.num=Input.num*10+5;Input.update = 1;}
		if(Touch_Judge(560,180,640,260) == 2){Input.num=Input.num*10+6;Input.update = 1;}
		
		if(Touch_Judge(400,260,480,340) == TOUCH_VALID_FULL){Input.num=Input.num*10+7;Input.update = 1;}
		if(Touch_Judge(480,260,560,340) == TOUCH_VALID_FULL){Input.num=Input.num*10+8;Input.update = 1;}
		if(Touch_Judge(560,260,640,340) == TOUCH_VALID_FULL){Input.num=Input.num*10+9;Input.update = 1;}
		
		if(Touch_Judge(400,340,480,420) == TOUCH_VALID_FULL){Input.num=Input.num*10;Input.update = 1;}
		if(Touch_Judge(480,340,560,420) == TOUCH_VALID_FULL){Input.num = 0;Input.update = 1;}
		if(Touch_Judge(560,340,640,420) == TOUCH_VALID_FULL){Input.num /=10;Input.update = 1;}
		
		
		if(Touch_Judge(640,100,740,180) == TOUCH_VALID_FULL&&Input.num!=0){ddsStructData.hz = Input.num;freshwave = 1;}
		if(Touch_Judge(640,180,740,260) == TOUCH_VALID_FULL&&Input.num!=0){ddsStructData.hz = Input.num*1000;freshwave = 1;}
		if(Touch_Judge(640,260,740,340) == TOUCH_VALID_FULL&&Input.num!=0){ddsStructData.vpp = Input.num/100.0;freshwave = 1;}
		if(Touch_Judge(640,340,740,420) == TOUCH_VALID_FULL&&Input.num!=0){ddsStructData.dutycycle = Input.num;freshwave = 1;}

		if(Touch_Judge(400,420,560,460) == TOUCH_VALID_FULL){ddsStructData.wave++;ddsStructData.wave%=4;freshwave = 1;}
		
		if(Input.update)
		{
			OS_Num_Show(100,340,32,1,Input.num,"%0.0f              ");
			Input.update = 0;
		}
		OSTimeDly(50);
	}
	
}

void ShowWaveInfo(void)
{
	OS_Num_Show(80,100,24,1,ddsStructData.hz/2,"Freq:%0.0f       ");
	OS_Num_Show(80,140,24,1,ddsStructData.vpp,"Vpp:%0.2f       ");
	OS_Num_Show(80,180,24,1,ddsStructData.dutycycle,"Duty:%0.2f%%    ");
	switch(ddsStructData.wave)
	{
		case SINWAVE:OS_String_Show(80,220,24,1,"SINWAVE       ");break;
		case TRIANGLEWAE:OS_String_Show(80,220,24,1,"TRIANGLE   ");break;
		case SAWTOOTHWAVE:OS_String_Show(80,220,24,1,"SAWTOOTH    ");break;
		case SQUAREWAVE:OS_String_Show(80,220,24,1,"SQUARE     ");break;
		default :OS_String_Show(80,220,24,1,"ERROR!!     ");break;
	}
}	
						
	
