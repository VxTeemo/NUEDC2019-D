/*
*********************************************************************************************************
*                                              _04_OS
* File			 : OS_UI.c
* By  			 : Wind
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
*********************************************************************************************************
*/
/* 防止重调用 ----------------------------------------------------------------*/
#ifndef __OS_UI_H
#define __OS_UI_H 

/* 头文件包含 ----------------------------------------------------------------*/  
#include "User_header.h"

/* 全局宏定义 ----------------------------------------------------------------*/
//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0

/*----------------兼容颜色---------------------------------------*/
/* LCD color */
#define White          0xFFFF //白色
#define Black          0x0000 //黑色
#define Grey           0xF7DE //灰色
#define Blue           0x001F //蓝色
#define Blue2          0x051F //深蓝色
#define Red            0xF800 //红色
#define Magenta        0xF81F //品红
#define Green          0x07E0 //绿色
#define Cyan           0x7FFF //蓝绿色
#define Yellow         0xFFE0 //黄色
#define BROWN 		   0XBC40 //棕色
#define GRAY  		   0X8430 //灰色

/* 尺寸 --------------------------------------------------------*/
#define OS_LCD_WHITH  	800	 //宽度	
#define OS_LCD_HEIGHT  	480	 //高度	
#define OS_ICO_COLOUR_BACK  GREEN //系统桌面 图标背景颜色
#define OS_ICO_COLOUR_TEXT  WHITE  //系统桌面 图标文字颜色
		
#define FONT_1   12				
#define FONT_2   16			
#define FONT_3   24 
#define FONT1   FONT_1				
#define FONT2   FONT_2			
#define FONT3   FONT_3 

#define IOC_SIZE 64			



#ifndef OS_DEFAULT
#define OS_DEFAULT 	     	     NULL	  //缺省值
#endif

#ifndef LEFTY
#define LEFTY 	     	     0x05	  
#endif

#ifndef RIGHTY
#define RIGHTY 	     	     0x50	  
#endif

#ifndef LEFTY_AND_RIGHTY
#define LEFTY_AND_RIGHTY 	 0x55	  
#endif



/* 兼容以前的代码 -----------------------------------------------*/ 
 	
/* 程序需要 ----------------------------------------------------*/
#define OS_CLEAR_PURE				(0x01 << 0)	
#define OS_CLEAR_IMAGE				 0x00	
#define OS_CLEAR_WEAKNESS			(0x01 << 1)		
#define OS_CLEAR_CONSTRAINT			 0x00 

#define OSC_FORMAT_U8				0x01
#define OSC_FORMAT_U16				0x02
#define OSC_FORMAT_FLOAT			0x03
#define OSC_FORMAT_DOUBLE			0x04

#define OSC_MODE_WINDOWS_SHOW		0x01 			 
#define OSC_MODE_LINE_SHOW			0x02	 	

#define OSC_MODE_ERROR				0x00	
#define OSC_MODE_NORMAL				0x01 			 
#define OSC_MODE_FFT				0x02	 	

/* 结构体声明 ----------------------------------------------------------------*/ 
typedef struct __TYPE_OSC
{ 
	u16 x,y;//OSC窗口的位置 
	float ZoomWindows;//显示窗口缩放量  1.0是标准的
	int ShiftX; //横轴偏移量	0是标准的
	int ShiftY; //纵轴偏移量	0是标准的
	float ZoomX;  //横轴缩放量	1.0是标准的
	float ZoomY;  //纵轴缩放量   1.0是标准的
	u16 LineColor;//线条颜色
	u16 BackColor;//背景颜色
	u16 WindowsColor;//背景颜色
	u8 LineSize;//线条粗细的尺寸
	u8 Mode;//0:不显示面积	1：显示面积（FFT模式）
	
	u16 BuffLenth;//点数 
	u8 BuffFormat;//Buff的格式
	u16 *Buff_u16;
	u8 *Buff_u8;
	float *Buff_float;
	double *Buff_double;
	
	//以下内容不需要用户去处理
	u16 LastBuff[2][OS_LCD_WHITH];	
	u16 LastZeroLine;
	u8 LastBuff_Cnt;
	u8 LastMode;
	u8 LastLineSize; 
	
}Type_OSC;
  
typedef struct GRAPH_Struct
{
	  u16 x1;							//起始横坐标
	  u16 y1;							//起始纵坐标
	  u16 x2;							//终止横坐标
	  u16 y2;							//终止纵坐标坐标

	  u16 griacolor;			//网格颜色
		u16 Backcolor;			//背景颜色
	  u16 left_ycolor;		//网格颜色
		u16 right_ycolor;		//背景颜色
	  u16 cursor_color;		//光标颜色
	
	  u16 grid;						//有无网格	
	
	  u16 xnumber;				//横线数量(包括外轮廓)
	  u16 ynumber;				//竖线数量(包括外轮廓)
	
		float xmax;					//横坐标最大值
	  float xmin;					//横坐标最小值
	  float left_ymax;		//左侧纵坐标最大值
	  float left_ymin;		//左侧纵坐标最小值
	  float right_ymax;		//右侧纵坐标最大值
	  float right_ymin;		//右侧纵坐标最小值
	
	  u16 Buff_Lenth;			//数据长度
	  float *left_buff;   //保存左侧纵坐标数据的数组
	  float *right_buff;  //保存右侧纵坐标数据的数组
		
		u16 cursor_x;     	//光标位置的横坐标
		float cursor_yl;    //光标位置左侧纵坐标
		float cursor_yr;    //光标位置右侧纵坐标
		u16  lastcursor_x; 	//上次光标位置的横坐标
		u8   cursor_refrssh;//光标更新标志
	
	  //以下内容不需要用户去处理
	  u16 lastbuff[2][OS_LCD_WHITH];			//上一次的两个纵坐标值

}GRAPH_Struct;

/* 全局变量声明 --------------------------------------------------------------*/  
extern Type_OSC Struct_OSC;
extern GRAPH_Struct GridData;

/* 全局函数声明 --------------------------------------------------------------*/  
int OS_LCD_Init(void);  
int OS_LCD_Clear(u16 color);
unsigned int OS_Point_Read(u16 x,u16 y);
unsigned int OS_TextColor_Set(u16 _color);
unsigned int OS_BackColor_Set(u16 _color);
int OS_Point_Draw(u16 x,u16 y,u16 color);
int OS_Line_Draw(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);  
int OS_Circle_Draw(u16 x, u16 y, u8 r, u8 size, u16 color);
int OS_Rect_Draw(u16 x1, u16 y1, u16 x2, u16 y2, u8 size, u16 color); 
int OS_Picture_Draw(u16 x1,u16 y1,u16 x2,u16 y2,u8 mode,const u8 *pic);  
 
int OS_String_Show(u16 x, u16 y, u8 size, u8 mode, char *str);
int OS_Num_Show(u16 x,u16 y,u8 size,u8 mode,double num,char* format);
int OS_Wave_Draw(Type_OSC *Struct_OSC,u8 mode);
int OS_Wave_Unite(Type_OSC *Struct_OSC_Source, Type_OSC *Struct_New);

void Draw_Grid(GRAPH_Struct griddata); 
void Show_Label(GRAPH_Struct griddata,u8 yselect);   
void Draw_Graph(GRAPH_Struct *griddata,u8 yselect);
void Cursor_Data_Show(GRAPH_Struct *griddata,u8 yselect); 

#endif
/*******************************(C) COPYRIGHT 2016 Wind（谢玉伸）*********************************/













