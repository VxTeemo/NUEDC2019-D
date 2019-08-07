/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_TouchKey.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : 触屏键盘驱动程序
*********************************************************************************************************
*/

#include "Drive_TouchKey.h"

u16 start_x=530; //起始横坐标
u16 start_y=30;  //起始纵坐标
u16 shift_x=60;  //横坐标偏移量
u16 shift_y=60;  //纵坐标偏移量

/*
*************************************************************************
*
*	功能：	画键盘
*	参数：
*
**************************************************************************
*/
void TouchKey_Draw(void)
{
    
	  OS_Rect_Draw(start_x,start_y,start_x+shift_x*4,start_y+shift_y*5,1,Black);//画外轮廓线
	  OS_Line_Draw(start_x,start_y+shift_y*1,start_x+shift_x*4,start_y+shift_y*1,Black);//画横线
	  OS_Line_Draw(start_x,start_y+shift_y*2,start_x+shift_x*4,start_y+shift_y*2,Black);
	  OS_Line_Draw(start_x,start_y+shift_y*3,start_x+shift_x*4,start_y+shift_y*3,Black);
	  OS_Line_Draw(start_x,start_y+shift_y*4,start_x+shift_x*3,start_y+shift_y*4,Black);
		OS_Line_Draw(start_x,start_y+shift_y*5,start_x+shift_x*4,start_y+shift_y*5,Black);
	
	  OS_Line_Draw(start_x+shift_x*1,start_y+shift_y*1,start_x+shift_x*1,start_y+shift_y*5,Black);//画竖线
	  OS_Line_Draw(start_x+shift_x*2,start_y+shift_y*1,start_x+shift_x*2,start_y+shift_y*5,Black);
	  OS_Line_Draw(start_x+shift_x*3,start_y+shift_y*1,start_x+shift_x*3,start_y+shift_y*4,Black);
	
	  OS_String_Show(start_x+20,start_y+shift_y*1+15,32,0,"1   2   3  <--");//显示键
	  OS_String_Show(start_x+20,start_y+shift_y*2+15,32,0,"4   5   6  C");
		OS_String_Show(start_x+20,start_y+shift_y*3+15,32,0,"7   8   9");
		OS_String_Show(start_x+20,start_y+shift_y*4+15,32,0,"0   .   Enter");
}


/*
*************************************************************************
*
*	功能：	清空显示区
*	参数：
*
**************************************************************************
*/
void Clear_Show(void)
{
   OS_Rect_Draw(start_x+1,start_y+1,start_x+shift_x*4-1,start_y+shift_y*1-1,0,White);
}

/*
*************************************************************************
*
*	功能：	扫描按键
*	参数：	
*
**************************************************************************
*/
float TouchKey_Scan(void)   
{
	
	  static u8 End_flag=0,count=0,databuf[40],press_flag=0;


		if(Touch_Judge(start_x+shift_x*0,start_y+shift_y*1,start_x+shift_x*1,start_y+shift_y*2) == TOUCH_VALID_FULL) {databuf[count]='1'; press_flag=1;}
		if(Touch_Judge(start_x+shift_x*1,start_y+shift_y*1,start_x+shift_x*2,start_y+shift_y*2) == TOUCH_VALID_FULL) {databuf[count]='2'; press_flag=1;}
		if(Touch_Judge(start_x+shift_x*2,start_y+shift_y*1,start_x+shift_x*3,start_y+shift_y*2) == TOUCH_VALID_FULL) {databuf[count]='3'; press_flag=1;}	
		if(Touch_Judge(start_x+shift_x*3,start_y+shift_y*1,start_x+shift_x*4,start_y+shift_y*2) == TOUCH_VALID_FULL) {count--;Clear_Show();}
			
		if(Touch_Judge(start_x+shift_x*0,start_y+shift_y*2,start_x+shift_x*1,start_y+shift_y*3) == TOUCH_VALID_FULL) {databuf[count]='4'; press_flag=1;}
		if(Touch_Judge(start_x+shift_x*1,start_y+shift_y*2,start_x+shift_x*2,start_y+shift_y*3) == TOUCH_VALID_FULL) {databuf[count]='5'; press_flag=1;}
		if(Touch_Judge(start_x+shift_x*2,start_y+shift_y*2,start_x+shift_x*3,start_y+shift_y*3) == TOUCH_VALID_FULL) {databuf[count]='6'; press_flag=1;}	
		if(Touch_Judge(start_x+shift_x*3,start_y+shift_y*2,start_x+shift_x*4,start_y+shift_y*3) == TOUCH_VALID_FULL) {count=0;Clear_Show();}
			
		if(Touch_Judge(start_x+shift_x*0,start_y+shift_y*3,start_x+shift_x*1,start_y+shift_y*4) == TOUCH_VALID_FULL) {databuf[count]='7'; press_flag=1;}
		if(Touch_Judge(start_x+shift_x*1,start_y+shift_y*3,start_x+shift_x*2,start_y+shift_y*4) == TOUCH_VALID_FULL) {databuf[count]='8'; press_flag=1;}
		if(Touch_Judge(start_x+shift_x*2,start_y+shift_y*3,start_x+shift_x*3,start_y+shift_y*4) == TOUCH_VALID_FULL) {databuf[count]='9'; press_flag=1;}	
    if(Touch_Judge(start_x+shift_x*3,start_y+shift_y*3,start_x+shift_x*4,start_y+shift_y*4) == TOUCH_VALID_FULL) {End_flag=1;}
			
		if(Touch_Judge(start_x+shift_x*0,start_y+shift_y*4,start_x+shift_x*1,start_y+shift_y*5) == TOUCH_VALID_FULL) {databuf[count]='0'; press_flag=1;}	
		if(Touch_Judge(start_x+shift_x*1,start_y+shift_y*4,start_x+shift_x*2,start_y+shift_y*5) == TOUCH_VALID_FULL) {databuf[count]='.'; press_flag=1;}	
		if(Touch_Judge(start_x+shift_x*2,start_y+shift_y*4,start_x+shift_x*4,start_y+shift_y*5) == TOUCH_VALID_FULL) {End_flag=1;}	 
					
		if(press_flag==1)
		{
			 if(count == 0)
				Clear_Show();
			 
			 count++;
			 press_flag=0;			 
		}
		
		databuf[count]='\0';	
		
		if(count!=0)        
		   OS_String_Show(start_x+10,start_y+15,32,0,(char *)databuf); //显示输入值
		
		if(End_flag==1 && count!=0)
		{		
			count=0; 
			End_flag=0;			
			Clear_Show();
			
		  return atof((char *)databuf);	    
		}	
		else
			return 0;	
}






