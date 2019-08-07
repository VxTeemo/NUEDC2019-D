/*
*********************************************************************************************************
*                                              _04_OS
* File			 : App_OSC.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : 简单的示波器
*********************************************************************************************************
*/
#include "App_OSC.h"

#define DEFAULT_OSC_SHIFTX 			(500)				//值越大越往左移
#define DEFAULT_OSC_SHIFTY 			(100)  				//值越大越往上移
#define DEFAULT_OSC_ZOOMX 			(100)			
#define DEFAULT_OSC_ZOOMY			(100)		
#define DEFAULT_OSC_VPP 			0
#define DEFAULT_OSC_T 				0
#define DEFAULT_OSC_MAXVALUE 		0
#define DEFAULT_OSC_MINVALUE		0xffff

Type_OSC Struct_OSC = {DEFAULT_OSC_SHIFTX,DEFAULT_OSC_SHIFTY,DEFAULT_OSC_ZOOMX,DEFAULT_OSC_ZOOMY, \
					   DEFAULT_OSC_VPP,DEFAULT_OSC_T,DEFAULT_OSC_MAXVALUE,DEFAULT_OSC_MINVALUE };
Type_OSC Struct_FFT= {DEFAULT_OSC_SHIFTX,DEFAULT_OSC_SHIFTY,DEFAULT_OSC_ZOOMX,DEFAULT_OSC_ZOOMY, \
					   DEFAULT_OSC_VPP,DEFAULT_OSC_T,DEFAULT_OSC_MAXVALUE,DEFAULT_OSC_MINVALUE };;

/* 私有（静态）函数声明 ------------------------------------------------------*/  
extern int FindMaxMin(u16 *max,u16* min);
void wavejudge(void);
void SelectGear(void);

/* 全局变量定义 --------------------------------------------------------------*/
u8 Speed = 0;
u16  FFT_Buff[2048] = {0};	
float WaveDuty = 0.5;
u8 AutoSet = 1,Stop=0,Gear=0;
int wave = 0;
char wavename[15];
uint32_t ADC_PreDiv=ADC_Prescaler_Div2;

u8 OSC_Flag=0;

/* 全局函数编写 --------------------------------------------------------------*/



void OSC_main()
{
		
	u16 Freq_buf;
	static float harmbuf[16];
	while(1)
	{
	
		if(OSC_Flag==0)
		{
			OSC_Flag=1;
			OS_LCD_Clear(White);
	
			Struct_OSC.ZoomWindows = 0.8f;
			Struct_OSC.BackColor = GRAY;
			Struct_OSC.WindowsColor = WHITE;
			Struct_OSC.x = 0;
			Struct_OSC.y = 0;
			
			Struct_OSC.ShiftX = 0;
			Struct_OSC.ShiftY = 40;
			Struct_OSC.ZoomX = 1.0f;
			Struct_OSC.ZoomY = 0.05f;
			Struct_OSC.LineColor = YELLOW; 
			Struct_OSC.LineSize = 1;
			Struct_OSC.Mode = OSC_MODE_NORMAL; 
			
			Struct_FFT.ShiftX = 30;
			Struct_FFT.ShiftY = 10;
			Struct_FFT.ZoomX = 1.5f;
			Struct_FFT.ZoomY = 0.5f;
			Struct_FFT.LineColor = RED;
			Struct_FFT.LineSize = 1;
			Struct_FFT.Mode = OSC_MODE_FFT; 
			
			OS_Wave_Unite(&Struct_OSC,&Struct_FFT); //与Struct_OSC公用一个窗口
			OS_Wave_Draw(&Struct_OSC,OSC_MODE_WINDOWS_SHOW);//画窗口	
			
			Struct_OSC.BuffFormat = OSC_FORMAT_U16;  	
			Struct_OSC.BuffLenth = 2048;				
			Struct_OSC.Buff_u16 = ADC1_DMA2_Buff;
			
			
			
			
			OS_TextColor_Set(Black);
			OS_BackColor_Set(White);
			OS_String_Show(600,420,24,1,"Run        ");
			OS_String_Show(600,450,24,1,"AutoSet        ");
			
			ADC1_DMA2_Reload(0,ADC_Prescaler_Div2);
			

		}
		if(Key_Now_Get(KEY3,KEY_MODE_SHORT))
		{
			AutoSet = !AutoSet;
			if(AutoSet)
				OS_String_Show(600,450,24,1,"AutoSet        ");
			else
				OS_String_Show(600,450,24,1,"UserSet        ");
		}

		SelectGear();
		
		//测频时频率放大了10倍，方波测量1HZ以下信号
		
		if(	g_Freq/10 >= 0 && g_Freq/10 <= 100 )//1HZ
		{
				TIM3_Init(40,1025);
				Freq_buf=1;
		}
		else if(g_Freq/10 > 100 && g_Freq/10 <=1000)//10HZ
		{
				TIM3_Init(4,1025);
				Freq_buf=10;
		}
		else	if(g_Freq /10>1000 && g_Freq/10<=5000)//50HZ
		{
				TIM3_Init(4,205);
				Freq_buf=50;
		}
		else if(g_Freq/10 >5000 && g_Freq/10<=20000)//100HZ
		{
				TIM3_Init(2,205);
				Freq_buf=100;
		}
		else
		{
				TIM3_Init(2,10);
				Freq_buf=0;
		}

		FFT_Exchange(ADC1_DMA2_Buff,FFT_Buff,2048);  	//ADC1_DMA2_Buff  
		FFT_Harmonic(harmbuf,FFT_Buff,2048);
		Struct_FFT.BuffFormat = OSC_FORMAT_U16;  	
		Struct_FFT.BuffLenth = 2048;				
		Struct_FFT.Buff_u16 = FFT_Buff;	
		
		
		if(DMA_GetFlagStatus(DMA2_Stream0,DMA_FLAG_TCIF0) == SET) 
		{       
			
			if(OSC_Data_Show())
			{ADC1_DMA2_Reload(Speed,ADC_PreDiv); continue;}
			OS_Wave_Draw(&Struct_FFT,OSC_MODE_LINE_SHOW);//画波形	
			OS_Wave_Draw(&Struct_OSC,OSC_MODE_LINE_SHOW);//画波形		
			OS_Num_Show(670,20,24,1, harmbuf[3]*Freq_buf,"1:%0.0fHz     ");
			OS_Num_Show(670,50,24,1, harmbuf[5]*Freq_buf,"2:%0.0fHz     ");
			OS_Num_Show(670,80,24,1, harmbuf[7]*Freq_buf,"3:%0.0fHz     ");
			OS_Num_Show(670,110,24,1, harmbuf[9]*Freq_buf,"4:%0.0fHz    ");
			OS_Num_Show(670,140,24,1, harmbuf[11]*Freq_buf,"5:%0.0fHz     ");
			OS_Num_Show(670,170,24,1, harmbuf[13]*Freq_buf,"6:%0.0fHz     ");
			OS_Num_Show(670,200,24,1, harmbuf[15]*Freq_buf,"7:%0.0fHz     ");
		//	ADC1_DMA2_Reload(Speed,ADC_PreDiv);  
		}
			
		OSTimeDly(99);
	}
}



/**----------------------------------------------------------------------------
* @FunctionName  : OSC_Data_Show()     
* @Description   : None 
* @Data          : 2016/7/13
* @Explain       : None
------------------------------------------------------------------------------*/
int OSC_Data_Show(void)
{ 
	u16 Vmax=0,Vmin=0;
	FindMaxMin(&Vmax,&Vmin);
	
	
	OS_Num_Show(0,390,24,1, g_Freq/10.0f+0.1,"freq:%.1fHz            ");
	OS_Num_Show(0,420,24,1, WaveDuty,"Duty:%.1f%%          ");
	OS_Num_Show(0,450,24,1, Vmax/1240.0,"Vmax:%.3f         ");
	OS_Num_Show(300,390,24,1, Vmin/1240.0,"Vmin:%.3f        ");
	OS_Num_Show(300,420,24,1, (Vmax-Vmin)/1240.0,"Vpp:%.3f        ");
	OS_Num_Show(300,450,24,1, Gear,"Gear:%0.0f        ");
	wavejudge();
	wave?strcpy(wavename,"sine     "):strcpy(wavename,"Square  ");
	OS_String_Show(600,390,24,1,wavename);
	
	return ((Vmax/1240.0)>4.0);
}				



/**----------------------------------------------------------------------------
* @FunctionName  : FindMaxMin()     
* @Description   : None 
* @Data          : 2016/7/13
* @Explain       : None
------------------------------------------------------------------------------*/
int FindMaxMin(u16 *max,u16* min)
{
	int i = 0;
	*max  = 0x000;*min= 0xFFF;
	while(i<ADC1_DMA2_LENTH)
	{
		if(ADC1_DMA2_Buff[i]==0xE00)
		{
			i++;
			continue;
		}
		if(ADC1_DMA2_Buff[i]>*max)
			*max = ADC1_DMA2_Buff[i];
		if(ADC1_DMA2_Buff[i]<*min)
			*min = ADC1_DMA2_Buff[i];
		i++;
	}
	i = 0;
	while(i<ADC1_DMA2_LENTH)
	{
		if(abs(ADC1_DMA2_Buff[i] - ((*max+*min)/2.0)) <0x55)
		{
//			*offset = i;
			break;
		}
		i++;
	}
	
	return 0;
}

uint8_t dutyArray[10] = {0};
void wavejudge(void)
{
    uint16_t i,count =0;
    u16 max = 0;
    u16 min = 0;
	
    u16 ave = (max + min)/2;
    u16 upCell = ave + (max-ave)/2;  //得到一个上下区间
    u16 downCell = ave - (ave-min)/2;
    u16 temp;
	static int firstrun = 1;
    
	
	FindMaxMin(&max,&min);
    if(max < 5)  //噪声 直接返回
    {
        return ;
    }
    
    for(i=0 ; i<400 ; i++)
    {
        
        if((ADC1_DMA2_Buff[i] > downCell) && (ADC1_DMA2_Buff[i] < upCell)) //定义一个区间 去判断波形是否为方波
        {
            count++;
            if(count >=50)
            {
                wave = 1;
                break;
            }
        }
        
    }
    
    if(i == 400) //波形为方波
    {
        wave = 0;
        count = 0;
        
        for(i=0 ; i<2048 ; i++)
        {
            if(ADC1_DMA2_Buff[i]>ave)
            {
                count ++;   //按照概率理论 采样点越多 统计的值越趋近于占空比
            }
        }
//        WaveDuty = count/20.48;
		
		if(firstrun)
		{
			for(i=0 ; i<10 ; i++)
			{
			   dutyArray[i] = count/20.48;
			}	
			firstrun = 0;			
		}
        //滤波 处理
        for(i=0 ; i<9 ; i++)
        {
           dutyArray[i] = dutyArray[i+1]; 
        }
        dutyArray[i] = (uint8_t)(count/20.48);
        
        for(i=0 ;i<10 ; i++)
        {
            temp +=dutyArray[i];
        }
        WaveDuty = (float)((float)temp/10.0f);
        
    }
    else //不是方波 
    {
        WaveDuty = 50.0f;
    }
    
}




/**----------------------------------------------------------------------------
* @FunctionName  : SelectGear()     
* @Description   : None 
* @Data          : 2017/9/28
* @Explain       : None
------------------------------------------------------------------------------*/
void SelectGear(void)
{
	int temp=0;
	if(AutoSet)
	{
		temp = (int)g_Freq;
		if(temp>0&&temp<=100)  Gear = 10;
		else if(temp>100&&temp<=200)  Gear = 9;
		else if(temp>200&&temp<=300)  Gear = 8;
		else if(temp>300&&temp<=400)  Gear = 7;
		else if(temp>400&&temp<=700) Gear = 6;
		else if(temp>700&&temp<=1000) Gear = 5;
		else if(temp>1000&&temp<=2000) Gear = 4;
		else if(temp>2000&&temp<=3000) Gear = 3;
		else if(temp>3000&&temp<=6000) Gear = 2;
		else if(temp>6000&&temp<=10000) Gear = 1;
		else if(temp>10000&&temp<=20000) Gear = 0;
		else Gear = 4;
	}	
	else
	{
		
		if(Key_Now_Get(WK_UP,KEY_MODE_SHORT))
		{
			if(Gear!=10)
				Gear++;
			else
				Gear=0;
		}
	}
	switch(Gear)
	{
		case 0:Speed = 0;break;
		case 1:Speed = 1;break;
		case 2:Speed = 2;break;
		case 3:Speed = 3;break;
		case 4:Speed = 5;break;
		case 5:Speed = 6;break;
		case 6:Speed = 7;break;
		case 7:{Speed = 7;Struct_OSC.ZoomX = 0.5f;}break;
		case 8:{Speed = 7;Struct_OSC.ZoomX = 0.3f;}break;
		case 9:{Speed = 7;Struct_OSC.ZoomX = 0.3f;ADC_PreDiv=ADC_Prescaler_Div4;}break;
		case 10:{Speed = 7;Struct_OSC.ZoomX = 0.3f;ADC_PreDiv=ADC_Prescaler_Div8;}break;
	}
		
	if(Gear<7)
	{
		if(Gear<10)
		{
			if(Gear<9)
				ADC_PreDiv=ADC_Prescaler_Div2;
			else
				ADC_PreDiv=ADC_Prescaler_Div4;
		}
		Struct_OSC.ZoomX = 1.0f;
	}
	
	
	
}






