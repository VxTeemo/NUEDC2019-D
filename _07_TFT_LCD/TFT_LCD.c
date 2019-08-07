/***************************************************************************************************
  * 文 件 名：TFT_LCD.c
  * 作    者：陈弟斌
  *	日    期：2015.6.29
  * 版    本：Stm32F407ZG
  * 开 发 板：stm32f407ZG--2015竞赛板
  * 修改时间：2015.6.29（根据龙元维版修改）
  * 编 译 器: keil.5
  * 库 版 本：V1.4
  *-------------------------------------------------------------------------------------------------
  * 简    介：TFT液晶屏幕显示函数功能集 
  * 参    考: MCD Application Team     
  *************************************************************************************************/

/*--------包含头文件------------------------------------------------------------------------------*/
#include "TFT_LCD.h"
#include "fonts.h"
#if _USE_HZK16==1
#elif _USE_HZK16==2
#define HZK_Address ((uint32_t)0X80BE804)
#endif

/* -------私有定义结构体 --------------------------------------------------------------------------------*/
typedef struct
{
  __IO uint16_t LCD_REG;
  __IO uint16_t LCD_RAM;
} LCD_TypeDef;
/* Note: LCD /CS is NE3 - Bank 3 of NOR/SRAM Bank 1~4 */
#define LCD_BASE           ((uint32_t)(0x60000000))
#define LCD                ((LCD_TypeDef *) LCD_BASE)
/*-------- 私有变量 --------------------------------------------------------------------------------------*/
static sFONT *LCD_Currentfonts=&LCD_DEFAULT_FONT;
/* Global variables to set the written text color */
__IO uint16_t TextColor = 0x0000, BackColor = 0xFFFF;
/* --------私有函数模型 ----------------------------------------------------------------------------------*/
__inline void LCD_WriteRAM_Prepare(void);
__inline void LCD_REG_Select(uint16_t REG_Code);
__inline void LCD_WriteRAM(uint16_t RGB_Code);
/* --------出口函数集-------------------------------------------------------------------------------------*/
//*******************应用层函数********************************************************************/
/***************************************************************************************************
 * 描述   : 液晶初始化，针对于LCD_ILI9320系列
 * 参数   : 无
 * 返回   : 无
 * 说明   :
 **************************************************************************************************/
void TFT_LCD_Init(void)
{ 
/*配置LCD连接线-----------------------------------------------------------------------------------*/
/* Configure the LCD Control pins --------------------------------------------*/
  LCD_CtrlLinesConfig();

/* Configure the FSMC Parallel interface -------------------------------------*/
  LCD_FSMCConfig();
  delay_ms(50); /*delay 10 ms*/

#if __TFT_ID==0xB505
/* Start Initial Sequence ------------------------------------------------------------------------*/
  LCD_WriteReg(LCD_REG_229,0x8000); /* Set the internal vcore voltage */
  LCD_WriteReg(LCD_REG_0,  0x0001); /* Start internal OSC. */
  LCD_WriteReg(LCD_REG_1,  0x0000); /* set SS and SM bit */  //0x0100
  LCD_WriteReg(LCD_REG_2,  0x0700); /* set 1 line inversion */
  LCD_WriteReg(LCD_REG_3,  0x1018); /* 设置GRAM写方向 and BGR=1. *///	1030
  LCD_WriteReg(LCD_REG_4,  0x0000); /* 调整记录 */
  LCD_WriteReg(LCD_REG_8,  0x0202); /* set the back porch and front porch */
  LCD_WriteReg(LCD_REG_9,  0x0000); /* set non-display area refresh cycle ISC[3:0] */
  LCD_WriteReg(LCD_REG_10, 0x0000); /* FMARK function */
  LCD_WriteReg(LCD_REG_12, 0x0000); /* RGB interface setting */
  LCD_WriteReg(LCD_REG_13, 0x0000); /* Frame marker Position */
  LCD_WriteReg(LCD_REG_15, 0x0000); /* RGB interface polarity */

/* Power On sequence -----------------------------------------------------------------------------*/
  LCD_WriteReg(LCD_REG_16, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  LCD_WriteReg(LCD_REG_17, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
  LCD_WriteReg(LCD_REG_18, 0x0000); /* VREG1OUT voltage */
  LCD_WriteReg(LCD_REG_19, 0x0000); /* VDV[4:0] for VCOM amplitude */
  Delay_ms(100);                 /* Dis-charge capacitor power voltage (100ms) */
  LCD_WriteReg(LCD_REG_16, 0x17B0); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  LCD_WriteReg(LCD_REG_17, 0x0137); /* DC1[2:0], DC0[2:0], VC[2:0] */
  Delay_ms(10);                  /* Delay 10 ms */
  LCD_WriteReg(LCD_REG_18, 0x0139); /* VREG1OUT voltage */
  Delay_ms(10);                  /* Delay 10 ms */
  LCD_WriteReg(LCD_REG_19, 0x1d00); /* VDV[4:0] for VCOM amplitude */
  LCD_WriteReg(LCD_REG_41, 0x0013); /* VCM[4:0] for VCOMH */
  Delay_ms(10);                  /* Delay 10 ms */
  LCD_WriteReg(LCD_REG_32, 0x0000); /* GRAM horizontal Address */
  LCD_WriteReg(LCD_REG_33, 0x0000); /* GRAM Vertical Address */

/* Adjust the Gamma Curve调整伽玛曲线 ------------------------------------------------------------*/
  LCD_WriteReg(LCD_REG_48, 0x0006);
  LCD_WriteReg(LCD_REG_49, 0x0101);
  LCD_WriteReg(LCD_REG_50, 0x0003);
  LCD_WriteReg(LCD_REG_53, 0x0106);
  LCD_WriteReg(LCD_REG_54, 0x0b02);
  LCD_WriteReg(LCD_REG_55, 0x0302);
  LCD_WriteReg(LCD_REG_56, 0x0707);
  LCD_WriteReg(LCD_REG_57, 0x0007);
  LCD_WriteReg(LCD_REG_60, 0x0600);
  LCD_WriteReg(LCD_REG_61, 0x020b);
  
/* Set GRAM area ---------------------------------------------------------------------------------*/
  LCD_WriteReg(LCD_REG_80, 0x0000); /* 水平 GRAM 开始地址 */
  LCD_WriteReg(LCD_REG_81, 0x00EF); /* 水平 GRAM 结束地址 */
  LCD_WriteReg(LCD_REG_82, 0x0000); /* 垂直 GRAM 开始地址 */
  LCD_WriteReg(LCD_REG_83, 0x013F); /* 垂直 GRAM 结束地址 */

  LCD_WriteReg(LCD_REG_96,  0x2700); /* Gate Scan Line */
  LCD_WriteReg(LCD_REG_97,  0x0001); /* NDL,VLE, REV */
  LCD_WriteReg(LCD_REG_106, 0x0000); /* set scrolling line */

/* Partial Display Control -----------------------------------------------------------------------*/
  LCD_WriteReg(LCD_REG_128, 0x0000);
  LCD_WriteReg(LCD_REG_129, 0x0000);
  LCD_WriteReg(LCD_REG_130, 0x0000);
  LCD_WriteReg(LCD_REG_131, 0x0000);
  LCD_WriteReg(LCD_REG_132, 0x0000);
  LCD_WriteReg(LCD_REG_133, 0x0000);

/* Panel Control ---------------------------------------------------------------------------------*/
  LCD_WriteReg(LCD_REG_144, 0x0010);
  LCD_WriteReg(LCD_REG_146, 0x0000);
  LCD_WriteReg(LCD_REG_147, 0x0003);
  LCD_WriteReg(LCD_REG_149, 0x0110);
  LCD_WriteReg(LCD_REG_151, 0x0000);
  LCD_WriteReg(LCD_REG_152, 0x0000);

  /* Set GRAM write direction and BGR = 1 */
  /* I/D=01 (Horizontal : increment, Vertical : decrement) */
  /* AM=1 (address is updated in vertical writing direction) */
  LCD_WriteReg(LCD_REG_3, 0x1000);	    // 1018

  LCD_WriteReg(LCD_REG_7, 0x0173); /* 262K color and display ON */  

#elif __TFT_ID==0x4531
	//ID为0x4531液晶
	LCD_WriteReg(0X00,0X0001);   
	Delay_ms(10);   
	LCD_WriteReg(0X10,0X1628);   
	LCD_WriteReg(0X12,0X000e);//0x0006    
	LCD_WriteReg(0X13,0X0A39);   
	Delay_ms(10);   
	LCD_WriteReg(0X11,0X0040);   
	LCD_WriteReg(0X15,0X0050);   
	Delay_ms(10);   
	LCD_WriteReg(0X12,0X001e);//16    
	Delay_ms(10);   
	LCD_WriteReg(0X10,0X1620);   
	LCD_WriteReg(0X13,0X2A39);   
	Delay_ms(10);   
	LCD_WriteReg(0X01,0X0100);   
	LCD_WriteReg(0X02,0X0300);   
	LCD_WriteReg(0X03,0X1030);//改变方向的   
	LCD_WriteReg(0X08,0X0202);   
	LCD_WriteReg(0X0A,0X0008);   
	LCD_WriteReg(0X30,0X0000);   
	LCD_WriteReg(0X31,0X0402);   
	LCD_WriteReg(0X32,0X0106);   
	LCD_WriteReg(0X33,0X0503);   
	LCD_WriteReg(0X34,0X0104);   
	LCD_WriteReg(0X35,0X0301);   
	LCD_WriteReg(0X36,0X0707);   
	LCD_WriteReg(0X37,0X0305);   
	LCD_WriteReg(0X38,0X0208);   
	LCD_WriteReg(0X39,0X0F0B);   
	LCD_WriteReg(0X41,0X0002);   
	LCD_WriteReg(0X60,0X2700);   
	LCD_WriteReg(0X61,0X0001);   
	LCD_WriteReg(0X90,0X0210);   
	LCD_WriteReg(0X92,0X010A);   
	LCD_WriteReg(0X93,0X0004);   
	LCD_WriteReg(0XA0,0X0100);   
	LCD_WriteReg(0X07,0X0001);   
	LCD_WriteReg(0X07,0X0021);   
	LCD_WriteReg(0X07,0X0023);   
	LCD_WriteReg(0X07,0X0033);   
	LCD_WriteReg(0X07,0X0133);   
	LCD_WriteReg(0XA0,0X0000); 
#elif __TFT_ID==0x9325
	LCD_WriteReg(0x00e7,0x0010);      
	LCD_WriteReg(0x0000,0x0001);//开启内部时钟
	LCD_WriteReg(0x0001,0x0100);     
	LCD_WriteReg(0x0002,0x0700);//电源开启                    
	//LCD_WriteReg(0x0003,(1<<3)|(1<<4) ); 	//65K  RGB
	//DRIVE TABLE(寄存器 03H)
	//BIT3=AM BIT4:5=ID0:1
	//AM ID0 ID1   FUNCATION
	// 0  0   0	   R->L D->U
	// 1  0   0	   D->U	R->L
	// 0  1   0	   L->R D->U
	// 1  1   0    D->U	L->R
	// 0  0   1	   R->L U->D
	// 1  0   1    U->D	R->L
	// 0  1   1    L->R U->D 正常就用这个.
	// 1  1   1	   U->D	L->R
	LCD_WriteReg(0x0003,(1<<12)|(3<<4)|(0<<3) );//65K    
	LCD_WriteReg(0x0004,0x0000);                                   
	LCD_WriteReg(0x0008,0x0207);	           
	LCD_WriteReg(0x0009,0x0000);         
	LCD_WriteReg(0x000a,0x0000);//display setting         
	LCD_WriteReg(0x000c,0x0001);//display setting          
	LCD_WriteReg(0x000d,0x0000);//0f3c          
	LCD_WriteReg(0x000f,0x0000);
	//电源配置
	LCD_WriteReg(0x0010,0x0000);   
	LCD_WriteReg(0x0011,0x0007);
	LCD_WriteReg(0x0012,0x0000);                                                                 
	LCD_WriteReg(0x0013,0x0000);                 
	Delay_ms(50); 
	LCD_WriteReg(0x0010,0x1590);   
	LCD_WriteReg(0x0011,0x0227);
	Delay_ms(50); 
	LCD_WriteReg(0x0012,0x009c);                  
	Delay_ms(50); 
	LCD_WriteReg(0x0013,0x1900);   
	LCD_WriteReg(0x0029,0x0023);
	LCD_WriteReg(0x002b,0x000e);
	Delay_ms(50); 
	LCD_WriteReg(0x0020,0x0000);                                                            
	LCD_WriteReg(0x0021,0x013f);           
	Delay_ms(50); 
	//伽马校正
	LCD_WriteReg(0x0030,0x0007); 
	LCD_WriteReg(0x0031,0x0707);   
	LCD_WriteReg(0x0032,0x0006);
	LCD_WriteReg(0x0035,0x0704);
	LCD_WriteReg(0x0036,0x1f04); 
	LCD_WriteReg(0x0037,0x0004);
	LCD_WriteReg(0x0038,0x0000);        
	LCD_WriteReg(0x0039,0x0706);     
	LCD_WriteReg(0x003c,0x0701);
	LCD_WriteReg(0x003d,0x000f);
	Delay_ms(50); 
	LCD_WriteReg(0x0050,0x0000); //水平GRAM起始位置 
	LCD_WriteReg(0x0051,0x00ef); //水平GRAM终止位置                    
	LCD_WriteReg(0x0052,0x0000); //垂直GRAM起始位置                    
	LCD_WriteReg(0x0053,0x013f); //垂直GRAM终止位置  

	LCD_WriteReg(0x0060,0xa700);        
	LCD_WriteReg(0x0061,0x0001); 
	LCD_WriteReg(0x006a,0x0000);
	LCD_WriteReg(0x0080,0x0000);
	LCD_WriteReg(0x0081,0x0000);
	LCD_WriteReg(0x0082,0x0000);
	LCD_WriteReg(0x0083,0x0000);
	LCD_WriteReg(0x0084,0x0000);
	LCD_WriteReg(0x0085,0x0000);

	LCD_WriteReg(0x0090,0x0010);     
	LCD_WriteReg(0x0092,0x0000);  
	LCD_WriteReg(0x0093,0x0003);
	LCD_WriteReg(0x0095,0x0110);
	LCD_WriteReg(0x0097,0x0000);        
	LCD_WriteReg(0x0098,0x0000);  
	//开启显示设置    
	LCD_WriteReg(0x0007,0x0133);   
	LCD_WriteReg(0x0020,0x0000);                                                            
	LCD_WriteReg(0x0021,0x013f);
#elif __TFT_ID==0x1936
	LCD_REG_Select(0x00E2);	  // PLL multiplier, set PLL clock to 120M
	LCD_WriteRAM(0x0023);	    // N=0x36 for 6.5M, 0x23 for 10M crystal
	LCD_WriteRAM(0x0002);
	LCD_WriteRAM(0x0004);
	LCD_REG_Select(0x00E0);  // PLL enable
	LCD_WriteRAM(0x0001);
	delay_ms(1);
	LCD_REG_Select(0x00E0);
	LCD_WriteRAM(0x0003);
	delay_ms(5);
	LCD_REG_Select(0x0001);  // software reset
	delay_ms(5);
	LCD_REG_Select(0x00E6);	//  PLL setting for PCLK, depends on resolution
	LCD_WriteRAM(0x0003);
	LCD_WriteRAM(0x00ff);
	LCD_WriteRAM(0x00ff);
	/***液晶模式设置*****************/
	LCD_REG_Select(0x00B0);	//LCD SPECIFICATION
	LCD_WriteRAM(0x0027);
	LCD_WriteRAM(0x0000);
	LCD_WriteRAM((SSD1963_HDP>>8)&0X00FF);  //Set HDP
	LCD_WriteRAM(SSD1963_HDP&0X00FF);
  LCD_WriteRAM((SSD1963_VDP>>8)&0X00FF);  //Set VDP
	LCD_WriteRAM(SSD1963_VDP&0X00FF);
  LCD_WriteRAM(0x0000);
	LCD_REG_Select(0x00B4);	//HSYNC
	LCD_WriteRAM((SSD1963_HT>>8)&0X00FF);  //Set HT
	LCD_WriteRAM(SSD1963_HT&0X00FF);
	LCD_WriteRAM((SSD1963_HPS>>8)&0X00FF);  //Set HPS
	LCD_WriteRAM(SSD1963_HPS&0X00FF);
	LCD_WriteRAM(SSD1963_HPW);			   //Set HPW
	LCD_WriteRAM((SSD1963_LPS>>8)&0X00FF);  //Set HPS
	LCD_WriteRAM(SSD1963_LPS&0X00FF);
	LCD_WriteRAM(0x0000);
	LCD_REG_Select(0x00B6);	//VSYNC
	LCD_WriteRAM((SSD1963_VT>>8)&0X00FF);   //Set VT
	LCD_WriteRAM(SSD1963_VT&0X00FF);
	LCD_WriteRAM((SSD1963_VPS>>8)&0X00FF);  //Set VPS
	LCD_WriteRAM(SSD1963_VPS&0X00FF);
	LCD_WriteRAM(SSD1963_VPW);			   //Set VPW
	LCD_WriteRAM((SSD1963_FPS>>8)&0X00FF);  //Set FPS
	LCD_WriteRAM(SSD1963_FPS&0X00FF);
	LCD_REG_Select(0x00BA);
	LCD_WriteRAM(0x000F);    //GPIO[3:0] out 1
	LCD_REG_Select(0x00B8);
	LCD_WriteRAM(0x0007);    //GPIO3=input, GPIO[2:0]=output
	LCD_WriteRAM(0x0001);    //GPIO0 normal
	LCD_REG_Select(0x0036); //rotation
	LCD_WriteRAM(0x0000);
	LCD_REG_Select(0x00F0); //pixel data interface
	LCD_WriteRAM(0x0003);
	delay_ms(5);
	/******设置光亮度*****************/
	LCD_REG_Select(0xBC);//重要
	LCD_WriteRAM(0x40);//对比度值     
  LCD_WriteRAM(0x80);//亮度值      
  LCD_WriteRAM(0x40);//饱和度值    
  LCD_WriteRAM(0x01);//处理机允许
  delay_ms(10);
	LCD_REG_Select(0x0029); //display on
	/***********设置PWM***********/
	LCD_REG_Select(0xBE); //
  LCD_WriteRAM(0x0E); //PLL(120M)/(256*Value)/256=120Hz
  LCD_WriteRAM(0xFF); //duty=X/256 
  LCD_WriteRAM(0x09); //DBC enable PWM enable 
  LCD_WriteRAM(0xFF); //Set the brightness level
  LCD_WriteRAM(0x00); //Set the minimum brightness level 
  LCD_WriteRAM(0x00); //Set the brightness prescaler
  LCD_REG_Select(0xD4); 
  //TH1 = display width * display height * 3 * 0.1 /16 
  //800*480 * 3 * 0.1 /16 =1C20H
  LCD_WriteRAM(0x00); //  
  LCD_WriteRAM(0x1C); //  
  LCD_WriteRAM(0x20); //  
 //TH2 = display width * display height * 3 * 0.25 /16
 //800*480 * 3 * 0.25 /16 =4650H
  LCD_WriteRAM(0x00); //  
  LCD_WriteRAM(0x46); //  
  LCD_WriteRAM(0x50); // 
 //TH3 = display width * display height * 3 * 0.6 /16 
 //800*480 * 3 * 0.6 /16  =AC80H
  LCD_WriteRAM(0x00); //  
  LCD_WriteRAM(0xA8); //  
  LCD_WriteRAM(0xC0); // 
	LCD_REG_Select(0x00d0);  //背光设置
	LCD_WriteRAM(0x000d);
	LCD_SetDisplayWindow(0,0,799,479);
#endif
}
/************************************************************************************************
 * 描述   : 设置文本色和背景色.
 * 参数   : Color:背景色._TextColor: specifies the Text Color.
 		        _BackColor: specifies the Background Color.
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_SetColors(__IO uint16_t _TextColor, __IO uint16_t _BackColor)
{
  TextColor = _TextColor; 
  BackColor = _BackColor;
}
/************************************************************************************************
 * 描述   : 获取当前文本色和背景色
 * 参数   : 无
 * 返回   : _TextColorColor:pointer to the variable that will contain the Text Color.
 *		      _BackColor: pointer to the variable that will contain the Background Color.
 * 说明   :
 *************************************************************************************************/
void LCD_GetColors(__IO uint16_t *_TextColor, __IO uint16_t *_BackColor)
{
  *_TextColor = TextColor; *_BackColor = BackColor;
}
/************************************************************************************************
 * 描述   : 设置文本色
 * 参数   : Color:背景色.
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_SetTextColor(__IO uint16_t Color)
{
  TextColor = Color;
}
/************************************************************************************************
 * 描述   : 设置背景色
 * 参数   : Color:背景色.
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_SetBackColor(__IO uint16_t Color)
{
  BackColor = Color;
}
/************************************************************************************************
 * 描述   : 全屏清屏
 * 参数   : Color: 清屏后的背景色.
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_Clear(uint16_t Color)
{
  uint32_t index = 0;
  
  LCD_SetCursor(0x00, 0x00); 
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
  for(index = 0; index < LCD_Pixel_Max; index++)
  {
    LCD->LCD_RAM = Color;
  }  
}
/************************************************************************************************
 * 描述   : 指定范围清屏.(原代码仅清除两点之间，已修复)
 * 参数   : Color: 清屏后的背景色.
 *          (Xs,Ys)：开始坐标
 *          (Xe,Ye): 结束坐标
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_Appoint_Clear(uint16_t Xs,uint16_t Ys,uint16_t Xe,uint16_t Ye,uint16_t Color)
{
	uint32_t i,j;
	for(i=0;i<Xe-Xs;i++)
	{  	
		for (j = 0; j <Ye-Ys; j++)
		{
			LCD_SetCursor(Xs+i,Ys+j);
			LCD_WriteRAM_Prepare();
			LCD->LCD_RAM =Color;
		}
	}
}
/************************************************************************************************
 * 描述   : 设置字体.
 * 参数   : fonts: 要设置的新字体
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_SetFont(sFONT *fonts,uint8_t Mode)
{
  LCD_Currentfonts = fonts;
  LCD_Currentfonts->Mode=Mode;
}
/************************************************************************************************
 * 描述   : 获取当前字体.
 * 参数   : 无
 * 返回   : 当前正在使用的字体.
 * 说明   :
 *************************************************************************************************/
sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}

/************************************************************************************************
 * 描述   : 指定位置写一个字符
 * 参数   : X,Y： X坐标,Y坐标.
 *          char： 指向字符ascii代码的指针
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t Char)
{   
	uint16_t temp,dy,dx,K;//位置偏量
	if(Char>31)		   
	Char=Char-32;//得到偏移后的值
	else  //其他字符忽略，设为空格
	Char=0;
	for(dy=0,K=0;dy<LCD_Currentfonts->Height;dy++)
	{
		temp=LCD_Currentfonts->table[Char][K++];//调用字体
		#if   __FONTS==1
			for(dx=0;dx<LCD_Currentfonts->Height/2;dx++)
			{                 
				if(temp&0x01)
				{ 
					LCD_SetCursor(x+dx,y+dy);//设置光标位置 
					LCD_WriteRAM_Prepare(); 
					LCD_WriteRAM(TextColor);
				}
				else
				if(LCD_Currentfonts->Mode)
				{
					LCD_SetCursor(x+dx,y+dy);//设置光标位置 
					LCD_WriteRAM_Prepare(); 
					LCD_WriteRAM(BackColor);
				}     
				temp>>=1; 
			}
		#elif __FONTS==2
			for(dx=0;dx<8;dx++)
			{                 
				if(temp&0x01)
				{ 
					LCD_SetCursor(x+dx,y+dy);//设置光标位置 
					LCD_WriteRAM_Prepare(); 
					LCD_WriteRAM(TextColor);
				}
				else
				if(LCD_Currentfonts->Mode)
				{
					LCD_SetCursor(x+dx,y+dy);//设置光标位置 
					LCD_WriteRAM_Prepare(); 
					LCD_WriteRAM(BackColor);
				}     
				temp>>=1; 
			}
			temp=LCD_Currentfonts->table[Char][K++];//调用字体
			for(;dx<LCD_Currentfonts->Width;dx++)
			{                 
				if(temp&0x01)
				{ 
					LCD_SetCursor(x+dx,y+dy);//设置光标位置 
					LCD_WriteRAM_Prepare(); 
					LCD_WriteRAM(TextColor);
				}
				else
				if(LCD_Currentfonts->Mode)
				{
					LCD_SetCursor(x+dx,y+dy);//设置光标位置 
					LCD_WriteRAM_Prepare(); 
					LCD_WriteRAM(BackColor);
				}     
				temp>>=1; 
			}
		#endif

	}
}
/************************************************************************************************
 * 描述   : 在LCD上显示一个字符(字符点阵是一维数组),这里只用于显示汉字
 * 参数   : _Xpos,_Ypos：X坐标,Y坐标.
 *          width：点阵的宽度，字符是8，汉字是16
 *          ptr:   指向字符点阵的指针
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
#if _USE_HZK16

#if   __FONTS==1
static void TFT_DispChar(uint16_t _Xpos, uint16_t _Ypos, const uint8_t *c, uint8_t _width)
{
	uint32_t height = 0, i = 0;
	uint16_t X_add = _Xpos;
	uint16_t Y_add = _Ypos;

	LCD_SetCursor(_Xpos, _Ypos);
	if (LCD_Currentfonts->Mode)//判断是否叠加
	{
		for (height = 0; height < 16; height++)  //字符高度
		{
			X_add = _Xpos;
			for (i = 0; i < _width; i++)	  //字符宽度
			{
				LCD_SetCursor(X_add, Y_add);
				LCD_WriteRAM_Prepare();

				if ((c[height * (_width / 8) + i / 8] & (0x80 >> (i % 8))) == 0x00)
				{
					LCD_WriteRAM(BackColor);
				}
				else
				{
					LCD_WriteRAM(TextColor);
				}
				X_add++;
			}
			Y_add++;			
		}
	}
	else
	{
		for (height = 0; height < 16; height++)   //字符高度
		{
			X_add = _Xpos;
			for (i = 0; i < _width; i++)	   //字符宽度
			{
				if ((c[height * (_width / 8) + i / 8] & (0x80 >> (i % 8))) != 0x00)
				{
					LCD_SetCursor(X_add, Y_add);
					LCD_WriteRAM_Prepare();
					LCD->LCD_RAM = TextColor;
				}
				X_add++;
			}
			Y_add++;
		}
	}
}
#elif __FONTS==2
//放大一倍后显示
static void TFT_DispChar(uint16_t _Xpos, uint16_t _Ypos, const uint8_t *c, uint8_t _width)
{
	uint32_t height = 0, i = 0;
	uint16_t X_add = _Xpos;
	uint16_t Y_add = _Ypos;

	LCD_SetCursor(_Xpos, _Ypos);
	if (LCD_Currentfonts->Mode)//判断是否叠加
	{
		for (height = 0; height < 16; height++)  //字符高度
		{
			X_add = _Xpos;
			for (i = 0; i < _width; i++)	  //字符宽度
			{
				if ((c[height * (_width / 8) + i / 8] & (0x80 >> (i % 8))) == 0x00)
				{
					LCD_SetCursor(X_add, Y_add);
					LCD_WriteRAM_Prepare();
					LCD->LCD_RAM = BackColor;
					LCD_SetCursor(X_add+1, Y_add);
					LCD_WriteRAM_Prepare();
					LCD->LCD_RAM = BackColor;
					LCD_SetCursor(X_add, Y_add+1);
					LCD_WriteRAM_Prepare();
					LCD->LCD_RAM = BackColor;
					LCD_SetCursor(X_add+1, Y_add+1);
					LCD_WriteRAM_Prepare();
					LCD->LCD_RAM = BackColor;
				}
				else
				{
					LCD_SetCursor(X_add, Y_add);
					LCD_WriteRAM_Prepare();
					LCD->LCD_RAM = TextColor;
					LCD_SetCursor(X_add+1, Y_add);
					LCD_WriteRAM_Prepare();
					LCD->LCD_RAM = TextColor;
					LCD_SetCursor(X_add, Y_add+1);
					LCD_WriteRAM_Prepare();
					LCD->LCD_RAM = TextColor;
					LCD_SetCursor(X_add+1, Y_add+1);
					LCD_WriteRAM_Prepare();
					LCD->LCD_RAM = TextColor;
				}
				X_add+=2;
			}
			Y_add+=2;			
		}
	}
	else
	{
		for (height = 0; height < 16; height++)   //字符高度
		{
			X_add = _Xpos;
			for (i = 0; i < _width; i++)	   //字符宽度
			{
				if ((c[height * (_width / 8) + i / 8] & (0x80 >> (i % 8))) != 0x00)
				{
					LCD_SetCursor(X_add, Y_add);
					LCD_WriteRAM_Prepare();
					LCD->LCD_RAM = TextColor;
					LCD_SetCursor(X_add+1, Y_add);
					LCD_WriteRAM_Prepare();
					LCD->LCD_RAM = TextColor;
					LCD_SetCursor(X_add, Y_add+1);
					LCD_WriteRAM_Prepare();
					LCD->LCD_RAM = TextColor;
					LCD_SetCursor(X_add+1, Y_add+1);
					LCD_WriteRAM_Prepare();
					LCD->LCD_RAM = TextColor;
				}
				X_add+=2;
			}
			Y_add+=2;
		}
	}
}

#endif

#endif
/************************************************************************************************
 * 描述   : 显示一行字符.
 * 参数   : Line,Column：行与列位置.
 *          *ptr: 指向要显示的字符串的指针.
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
uint32_t LCD_DisplayStringLine(uint16_t Line,uint16_t Column, uint8_t *ptr)
{
	#if _USE_HZK16
		uint32_t address;
		uint16_t HZK16[16];
	#endif
	#if _USE_HZK16==2
		uint32_t *HZK=(uint32_t*)HZK16;
	#endif
		uint16_t refcolumn =Column;
		uint16_t  refLine =Line;
	#if _USE_HZK16
		uint8_t code1,code2;
	#endif
	if(refLine>=LCD_Pixel_Height)
	{  
		refLine=0;//换页
		LCD_Currentfonts->Mode=1;//切换为非叠加模式
	}
	while (*ptr!='\0')
	{
	#if _USE_HZK16	
		code1=*ptr;			     
		if(code1<0x80)	//判断是汉字代码，还是ascii代码 
		{
	#endif
			if(refcolumn>=LCD_Pixel_Width)
			{	
				refcolumn=0;
				refLine+=LCD_Currentfonts->Height;//换行
				if(refLine>=LCD_Pixel_Height)
				{  
					refLine=0;//换页
					LCD_Currentfonts->Mode=1;//切换为非叠加模式
				} 
			}
			LCD_ShowChar(refcolumn, refLine, *ptr);
			refcolumn += LCD_Currentfonts->Width;
	#if  _USE_HZK16
		}
		//汉字内码
		else
		{
			code2 = *++ptr;
			if(code2=='\0')
			return code1;
			//计算16点阵汉字点阵地址addreSS=[(code1-0xa1)*94 + (code2-0xa1)]*32	
			address =((code1-0xa1) * 94 + (code2-0xa1))<<5;
		#if _USE_HZK16==1
			SRAM_ReadBuffer(HZK16, address, 16);
		#elif _USE_HZK16==2
			address+=HZK_Address;		
			for(code1=0;code1<8;code1++)
			{
				HZK[code1]=(*(__IO uint32_t*)address);
				address+=4;
			}
		#endif 
			if(refcolumn>=LCD_Pixel_Width-LCD_Currentfonts->Height)
			{	
				refcolumn=0;
				refLine+=LCD_Currentfonts->Height;//换行
				if(refLine>=LCD_Pixel_Height)
				{  
				refLine=0;//换页
				LCD_Currentfonts->Mode=1;//切换为非叠加模式
				} 
			}
			TFT_DispChar(refcolumn, refLine, (const uint8_t *)HZK16, 16);
			refcolumn +=LCD_Currentfonts->Width*2;
		} 
	#endif
		ptr++;
	}
	return 0;
}
/************************************************************************************************
 * 描述   : 防溢出显示字符，被下面的函数所调用.
 * 参数   : *refLine,*refColumn：行与列位置.
 *          Char: 要显示的字符.
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/

#if _USE_0X

static void	Display_Control(uint8_t *refLine,uint16_t *refcolumn,uint8_t Char)
{  
	 if(*refcolumn>=LCD_Pixel_Width-LCD_Currentfonts->Width)
	 {	
	 	*refcolumn=0;
	 	*refLine+=LCD_Currentfonts->Height;//换行
		if(*refLine>=LCD_Pixel_Height)
		 {  
		   *refLine=0;//换页
		   LCD_Currentfonts->Mode=1;//切换为非叠加模式
		 } 
	 }
 	  LCD_ShowChar(*refcolumn,*refLine,Char); 
	  *refcolumn+=LCD_Currentfonts->Width;
}
/************************************************************************************************
 * 描述   : 显示16进制数.
 * 参数   : Line,Column：行与列位置.
 *          X:数组，数组大小.
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_Display0x(uint16_t Line,uint16_t Column, uint8_t *X,uint8_t N)
{       	
  uint16_t refcolumn =Column;
  uint8_t  i=0,temp=0, refLine =Line; 
   for(i=0;i<N;i++)
   {
   	  temp=X[i]/16;
	  if(temp<10)
		  temp+='0';
	  else
		  temp+=55;
	  Display_Control(&refLine,&refcolumn,temp);//高位
	  temp=X[i]%16;
	  if(temp<10)
		  temp+='0';
	  else
		  temp+=55;
	  Display_Control(&refLine,&refcolumn,temp);//低位
	  Display_Control(&refLine,&refcolumn,' ');//空格

	}					   
}
#endif

/************************************************************************************************
 * 描述   : 显示{数字数值(0~4294967295)}.
 * 参数   : Line,Column：行与列位置.
 *          num:数值(0~4294967295).
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/	 
void LCD_DisplayNum(uint16_t Line,uint16_t Column, uint32_t num)
{ 
  uint32_t temp=num;       	
  uint16_t refcolumn =Column;
  uint16_t  refLine =Line,len=0; 
  //计算数值长度
	do
	{
			temp = temp/10;
			len++;
	}
	while(temp); 
	len--;
  /********************************/
	refcolumn +=len*LCD_Currentfonts->Width;//从后往前显示
	//显示数字
	do
	{ 
	  temp=num%10;
	  LCD_ShowChar(refcolumn,refLine,temp+'0'); 
	  refcolumn-=LCD_Currentfonts->Width;
	  num=num/10;
	}
	while(len--);					   
}
/************************************************************************************************
 * 描述   : 显示浮点数.
 * 参数   : Line,Column：行与列位置.
 *          num:数值.
 * 返回   : 无
 * 说明   :整数部分小于2147483647大于-2147483648，
 *         但整数部分大的一定值后小数部分不准确了，小数位保留5位
 *************************************************************************************************/
#if _USE_Float

void LCD_Display_FloatNum(uint16_t Line,uint16_t Column, double num)
{
	 uint64_t Temp;
	 uint32_t Temp1;
	 uint8_t len=0;
	 if(num<0)//负数
	 {
	 	num=-num;
	    LCD_DisplayStringLine(Line,Column,"-");
	 	len++;
	 }
	 Temp=(uint64_t)(num*100000);//化为整数
	 //整数部分显示
	 Temp1=(uint32_t)(Temp/100000);
	 //显示整数部分
	 LCD_DisplayNum(Line,Column+Column(len),Temp1);
	 //计算整数部分长度
	do
	{
		Temp1 = Temp1/10;
		len++;
	}
	while(Temp1);
	//小数部分
	Temp1=(uint32_t)(Temp%100000);
	//去掉末尾多余0
	while(Temp1%10==0)
	{
		Temp1=Temp1/10;
	}
	//显示小数点
	if(Temp1!=0)//判断是否是小数，是则显示
	{
		LCD_DisplayStringLine(Line,Column+Column(len),".");
		len++;
	  LCD_DisplayNum(Line,Column+Column(len),Temp1);
	}
}
#endif
/*****************若_USE_GRAPH为1则使用绘图函数*************************************************/
#if _USE_GRAPH

/************************************************************************************************
 * 描述   : 显示一个像素点
 * 参数   : x: pixel x.
 *		   y: pixel y. 
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void PutPixel(int16_t x, int16_t y)
{ 
   LCD_SetCursor(x,y);//设置光标位置 
   LCD_WriteRAM_Prepare(); 
   LCD_WriteRAM(TextColor);
}
/************************************************************************************************
 * 描述   : 在LCD上显示一个bin格式的图片.
 * 参数   : ptr ： 图片点阵指针.
 *          ptr ： 图片数据大小.
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_WeBMP_SIZE(uint16_t *ptr,uint32_t SIZE)
{
	uint32_t index;
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	for (index = 0; index < SIZE; index++)
	{
		LCD_WriteRAM(*ptr++);
	}
}
/************************************************************************************************
 * 描述   : 绘制水平线/垂直
 * 参数   : x1: specifies the point 1 x position.
 *          y1: specifies the point 1 y position.
 *          x2: specifies the point 2 x position.
 *          y2: specifies the point 2 y position.
 *          Direction 0: horizontal 1: vertical
 * 返回   : 无
 * 说明   : 
 *************************************************************************************************/
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction)
{
  uint32_t i = 0;
  
  LCD_SetCursor(Xpos, Ypos);
  if(Direction == 0)
  {
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    for(i = 0; i < Length; i++)
    {
      LCD_WriteRAM(TextColor);
    }
  }
  else
  {
    for(i = 0; i < Length; i++)
    {
      LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
      LCD_WriteRAM(TextColor);
      Ypos++;
      LCD_SetCursor(Xpos, Ypos);
    }
  }
}
/************************************************************************************************
 * 描述   : 绘制直线
 * 参数   : x1: specifies the point 1 x position.
 *          y1: specifies the point 1 y position.
 *          x2: specifies the point 2 x position.
 *          y2: specifies the point 2 y position.
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/

#define ABS(X)  ((X) > 0 ? (X) : -(X)) 

void LCD_DrawuniLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
  curpixel = 0;
  
  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */
  
  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }
  
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    PutPixel(x, y);             /* Draw the current pixel */
    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }
}
/************************************************************************************************
 * 描述   : 画矩形.
 * 参数   : Xpos: specifies the X position.
 *          Ypos: specifies the Y position.
 *          Height: display rectangle height.
 *          Width: display rectangle width.
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width)
{ 
  LCD_DrawLine(Xpos,      Ypos,       Width,0);
	LCD_DrawLine(Xpos,      Ypos+Height,Width,0); 
	LCD_DrawLine(Xpos,      Ypos,       Height,1);  
  LCD_DrawLine(Xpos+Width,Ypos,       Height,1);
}
/************************************************************************************************
 * 描述   : 画填充矩形.
 * 参数   : Xpos: specifies the X position.
 *          Ypos: specifies the Y position.
 *          Height: display rectangle height.
 *          Width: display rectangle width.
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_DrawRectS(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 temp=TextColor;
	TextColor=color;
	for(;x2>=x1;x2--)
	LCD_DrawuniLine(x2,y1,x2,y2);
	TextColor=temp;
}
/************************************************************************************************
 * 描述   : 在指定位置画一个指定大小的圆.
 * 参数   : (x0,y0):中心点
 *		   Radius:半径 
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  D;/* Decision Variable */ 
  uint32_t  CurX;/* Current X Value */
  uint32_t  CurY;/* Current Y Value */ 
  
  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;
  
  while (CurX <= CurY)
  {
    LCD_SetCursor(Xpos + CurX, Ypos + CurY);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos + CurX, Ypos - CurY);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos - CurX, Ypos + CurY);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos - CurX, Ypos - CurY);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos + CurY, Ypos + CurX);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos + CurY, Ypos - CurX);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos - CurY, Ypos + CurX);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos - CurY, Ypos - CurX);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    if (D < 0)
    { 
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }
}
/************************************************************************************************
 * 描述   : 在指定位置画一个指定大小的实心圆.
 * 参数   : (x0,y0):中心点
 *		   Radius:半径 
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_DrawCircleS(uint16_t x,uint16_t y,uint16_t size,uint16_t color)
{
	uint16_t temp=TextColor;
	TextColor =color;
	for(;size>0;size--)
	LCD_DrawCircle(x,y,size);
	TextColor=temp;
}

#endif
/********************中间层函数*****************************************************************/
#if __TFT_ID==0x1936
/************************************************************************************************
 * 描述   : 4像素点作为一个点显示.
 * 参数   : (X,Y).坐标(原点在右上角)
 *          Col： 颜色
 * 返回   : 无
 * 说明   : 不使用开窗，即显示原点固定为物理原点，以加快显示速度
 *************************************************************************************************/
void LCD_DrawPoint_4(uint16_t X,uint16_t Y,uint16_t Col)
{
	LCD_REG_Select(0x002A);	
	LCD_WriteRAM(X>>8);   
	LCD_WriteRAM(X&0x00ff); 
	LCD_WriteRAM(0x03);    
	LCD_WriteRAM(0x1F);
	
	LCD_REG_Select(0x002b);	
	LCD_WriteRAM(Y>>8); 
	LCD_WriteRAM(Y&0x00ff); 
	LCD_WriteRAM(0x01);
	LCD_WriteRAM(0xdf);
	LCD_REG_Select(0x002c);
	LCD->LCD_RAM = Col;
	LCD->LCD_RAM = Col;
	Y++;
	LCD_REG_Select(0x002A);	
	LCD_WriteRAM(X>>8);   
	LCD_WriteRAM(X&0x00ff); 
	LCD_WriteRAM(0x03);    
	LCD_WriteRAM(0x1F);
	
	LCD_REG_Select(0x002b);	
	LCD_WriteRAM(Y>>8); 
	LCD_WriteRAM(Y&0x00ff); 
	LCD_WriteRAM(0x01);
	LCD_WriteRAM(0xdf);
	LCD_REG_Select(0x002c);
	LCD->LCD_RAM = Col;
	LCD->LCD_RAM = Col;
} 
/************************************************************************************************
 * 描述   : 显示像素点.
 * 参数   : (X,Y).坐标(原点在右上角)
 *          Col： 颜色
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_DrawPoint(uint16_t X,uint16_t Y,uint16_t Col)
{
	LCD_REG_Select(0x002A);	
	
	LCD_WriteRAM(X>>8);   
	LCD_WriteRAM(X&0x00ff); 
	LCD_WriteRAM(0x03);    
	LCD_WriteRAM(0x1F);
	
	LCD_REG_Select(0x002b);	
	LCD_WriteRAM(Y>>8); 
	LCD_WriteRAM(Y&0x00ff); 
	LCD_WriteRAM(0x01);
	LCD_WriteRAM(0xdf);		
	LCD_REG_Select(0x002c);
	LCD->LCD_RAM = Col;
} 
#else
void LCD_DrawPoint(uint16_t X,uint16_t Y,uint16_t Col)
{
	/* Write 16-bit Index, then Write Reg */
	LCD->LCD_REG = LCD_REG_32;
	/* Write 16-bit Reg */
	LCD->LCD_RAM = X;

	/* Write 16-bit Index, then Write Reg */
	LCD->LCD_REG = LCD_REG_33;
	/* Write 16-bit Reg */
	LCD->LCD_RAM = Y;
	 
	LCD->LCD_REG = LCD_REG_34;//开始写入GRAM
	LCD->LCD_RAM = Col; 
} 
#endif

#if __TFT_ID==0x1936
/************************************************************************************************
 * 描述   : 设置显示区域
 * 参数   : (X1,Y1).坐标(原点在右上角)
 *          (X2,Y2).坐标(原点在左下角)
 * 返回   : 无
 * 说明   : LCD_SetDisplayWindow(0,0,799,479);
 *************************************************************************************************/
void LCD_SetDisplayWindow(uint16_t SC,uint16_t SP,uint16_t EC,uint16_t EP)
{
	LCD_REG_Select(0x002A);	
	LCD_WriteRAM(SC>>8);	
	LCD_WriteRAM(SC&0x00ff);
	LCD_WriteRAM((uint8_t)(EC>>8));
	LCD_WriteRAM((uint8_t)(EC&0x00ff));
	
  LCD_REG_Select(0x002b);	
	LCD_WriteRAM(SP>>8);	
	LCD_WriteRAM(SP&0x00ff);
	LCD_WriteRAM((uint8_t)(EP>>8));
	LCD_WriteRAM((uint8_t)(EP&0x00ff));							 
}
#endif

#if __TFT_ID==0x1936
/************************************************************************************************
 * 描述   : Sets the cursor position.
 * 参数   : Xpos: specifies the X position.
 *		   Ypos: specifies the Y position.
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	LCD_REG_Select(0x002A);	
	
	LCD_WriteRAM(Xpos>>8);   
	LCD_WriteRAM(Xpos&0x00ff); 
	LCD_WriteRAM(0x03);    
	LCD_WriteRAM(0x1F);
	
	LCD_REG_Select(0x002b);	
	LCD_WriteRAM(Ypos>>8); 
	LCD_WriteRAM(Ypos&0x00ff); 
	LCD_WriteRAM(0x01);
	LCD_WriteRAM(0xdf);		
}
#else
/************************************************************************************************
 * 描述   : Sets the cursor position.
 * 参数   : Xpos: specifies the X position.
 *		   Ypos: specifies the Y position.
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
 void LCD_SetCursor(uint16_t Xpos, uint8_t Ypos)
{
  /*  px，py 是物理坐标， Xpos，Ypos是虚拟坐标
   *  转换公式: px=240-Ypos py=Xpos		  	
   */
  LCD_WriteReg(LCD_REG_32,240-Ypos);
  LCD_WriteReg(LCD_REG_33,Xpos);
}
#endif

#if _USE_DisplayOn
/************************************************************************************************
 * 描述   : Enables the Display.
 * 参数   : 无
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_DisplayOn(void)
{
	GPIOD_OUT(3)=1;
}
/**************************************************************************************************
 * 描述   : Disables the Display.
 * 参数   : 无
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_DisplayOff(void)
{
	GPIOD_OUT(3)=0;
}
#endif
/***************************************************************************************************
 * 描述   : Writes to the selected LCD register.
 * 参数   : LCD_Reg: address of the selected register.
 *		      LCD_RegValue: value to write to the selected register.
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue)
{
  /* Write 16-bit Index, then Write Reg */
  LCD->LCD_REG = LCD_Reg;
  /* Write 16-bit Reg */
  LCD->LCD_RAM = LCD_RegValue;  
}
/***************************************************************************************************
 * 描述   : Reads the LCD RAM.
 * 参数   : LCD_Reg: address of the selected register.		      
 * 返回   : LCD RAM Value
 * 说明   :
 *************************************************************************************************/
uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{
  /* Write 16-bit Index (then Read Reg) */
  LCD->LCD_REG = LCD_Reg;
  /* Read 16-bit Reg */
  return (LCD->LCD_RAM);
}
/***************************************************************************************************
 * 描述   : Prepare to write to the LCD RAM.
 * 参数   : 无
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
	#if __TFT_ID==0x1936
		LCD_REG_Select(0x002c);	
	#else
		LCD->LCD_REG = LCD_REG_34;
	#endif
}
/***************************************************************************************************
 * 描述   : Writes REG to the LCD 
 * 参数   : RGB_Code: the pixel color in RGB mode (5-6-5).
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_REG_Select(uint16_t REG_Code)
{
  LCD->LCD_REG = REG_Code;
}
/***************************************************************************************************
 * 描述   : Writes to the LCD RAM
 * 参数   : RGB_Code: the pixel color in RGB mode (5-6-5).
 * 返回   : 无
 * 说明   :
 *************************************************************************************************/
void LCD_WriteRAM(uint16_t RGB_Code)
{
  LCD->LCD_RAM = RGB_Code;
}
/*********************底层函数*********************************************************************/
/**
  * @brief  Configures LCD Control lines (FSMC Pins) in alternate function mode.
  * @param  None
  * @retval None
  */
void LCD_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOE |
                         RCC_AHB1Periph_GPIOF, ENABLE);

/*-- GPIO Configuration ------------------------------------------------------*/
  /* SRAM Data lines,  NOE and NWE configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
                                GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15 |
                                GPIO_Pin_4 |GPIO_Pin_5;;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);

  /* SRAM Address lines configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOF, &GPIO_InitStructure);  
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_FSMC);	   

  /* NE3 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 

  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);
	/*PD3(LCD_BL) configuration*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIOD_OUT(3)=1;
}

/**
  * @brief  Configures the Parallel interface (FSMC) for LCD(Parallel mode)
  * @param  None
  * @retval None
  */
void LCD_FSMCConfig(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;
   
  /* Enable FSMC clock */
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
  
/*-- FSMC Configuration ------------------------------------------------------*/
/*----------------------- SRAM Bank 1 ----------------------------------------*/
  /* FSMC_Bank1_NORSRAM4 configuration */
  p.FSMC_AddressSetupTime = 4;
  p.FSMC_AddressHoldTime = 0;
  p.FSMC_DataSetupTime = 8;
  p.FSMC_BusTurnAroundDuration = 0;
  p.FSMC_CLKDivision = 0;
  p.FSMC_DataLatency = 0;
  p.FSMC_AccessMode = FSMC_AccessMode_A;
  /* Color LCD configuration ------------------------------------
     LCD configured as follow:
        - Data/Address MUX = Disable
        - Memory Type = SRAM
        - Data Width = 16bit
        - Write Operation = Enable
        - Extended Mode = Enable
        - Asynchronous Wait = Disable */

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);   

  /* Enable FSMC NOR/SRAM Bank1 */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}
/******************* (C) 版权 2012 longwei *****文件结束*******************************************/
