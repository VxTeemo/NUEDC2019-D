/*
*********************************************************************************************************
*                                              _04_OS
* File			 : OS_UI.c
* By  			 : Wind
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
*********************************************************************************************************
*/

/* 头文件包含 ----------------------------------------------------------------*/
#include "OS_UI.h"

/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有（静态）函数声明 -------------------------------------------------------*/
void OS_Char_Show(u16 x, u16 y, u8 num, u8 size, u8 mode);

/* 全局变量定义 --------------------------------------------------------------*/

/* 全局变量声明 --------------------------------------------------------------*/

/* 全局函数编写 --------------------------------------------------------------*/

/*============================================= 底层程序层 =========================================*/
/**----------------------------------------------------------------------------
* @FunctionName  : OS_LCD_Init()
* @Description   : 初始化函数
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
* None
------------------------------------------------------------------------------*/
int OS_LCD_Init(void)
{
    TFT_LCD_Init();
    LCD_Clear(WHITE);//清屏
    if(font_init())
    {
        OS_String_Show(400, 200, 32, 1, "Font Error!!!"); //检查字库
        return -1;
    }
    TextColor = BLACK;
    BackColor = WHITE;

    return 0;

}

/**----------------------------------------------------------------------------
* @FunctionName  : OSTextColor_Set()
* @Description   : None
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
* None
------------------------------------------------------------------------------*/
unsigned int OS_TextColor_Set(u16 _color)
{
    uint16_t temp = TextColor;
    TextColor = _color;
    return temp;
}


/**----------------------------------------------------------------------------
* @FunctionName  : OS_BackColor_Set()
* @Description   : None
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
* None
------------------------------------------------------------------------------*/
unsigned int OS_BackColor_Set(u16 _color)
{
    uint16_t temp = BackColor;
    BackColor = _color;
    return temp;
}

/**----------------------------------------------------------------------------
* @FunctionName  : OS_LCD_Clear()
* @Description   : None
* @Data          : 2016/7/12
* @Explain
-------------------------------------------------------------------------------
* None
------------------------------------------------------------------------------*/
int OS_LCD_Clear(u16 color)
{
    LCD_Clear(color);
    return 0;
}



/**----------------------------------------------------------------------------
* @FunctionName  : OS_Point_Draw()
* @Description   : 画点
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
* x,y:坐标
* POINT_COLOR:此点的颜色
------------------------------------------------------------------------------*/
int OS_Point_Draw(u16 x, u16 y, u16 color)
{
    LCD_SetCursor(x, y); //设置光标位置
    LCD_WriteRAM_Prepare();
    LCD_WriteRAM(color);
    return 0;
}

/**----------------------------------------------------------------------------
* @FunctionName  : OS_Char_Show()
* @Description   : 打印字符
* @Data          : 2016/7/14
* @Explain
-------------------------------------------------------------------------------
在指定位置显示一个字符
x,y:起始坐标
num:要显示的字符:" "--->"~"
size:字体大小 16/24/32
mode:叠加方式(1)还是非叠加方式(0)
------------------------------------------------------------------------------*/
void OS_Char_Show(u16 x, u16 y, u8 num, u8 size, u8 mode)
{
    u8 temp, t1, t;
    u16 y0 = y;
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);		//得到字体一个字符对应点阵集所占的字节数
    //设置窗口
    num = num - ' '; //得到偏移后的值
    for(t = 0; t < csize; t++)
    {
        if(size == 16)temp = asc2_1608[num][t];	//调用1608字体
        else if(size == 24)temp = asc2_2412[num][t]; //调用2412字体
        else if(size == 32)temp = asc2_3216[num][t]; //调用3216字体
        else return;//没有的字库
        for(t1 = 0; t1 < 8; t1++)
        {
            if(temp & 0x80)OS_Point_Draw(x, y, TextColor);
            else if(mode == 1)OS_Point_Draw(x, y, BackColor);
            temp <<= 1;
            y++;
            if(y >= 480)return;		//超区域了
            if((y - y0) == size)
            {
                y = y0;
                x++;
                if(x >= 800)return;	//超区域了
                break;
            }
        }
    }
}

/**----------------------------------------------------------------------------
* @FunctionName  : OS_String_Show()
* @Description   : 打印句子
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
* x,y 坐标    *code 语句指针   size 字体大小
------------------------------------------------------------------------------*/
void OS_HzMat_Get(char *code, uint8_t *mat, u8 size)
{
    uint8_t qh, ql;
    uint8_t i;
    uint32_t foffset;

    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size); //得到字体一个字符对应点阵集所占的字节数
    qh = *code;
    ql = *(++code);
    if(qh < 0x81 || ql < 0x40 || ql == 0xff || qh == 0xff) //非常用汉字
    {
        for(i = 0; i < csize; i++)*mat++ = 0x00; //填充满格
        return; //结束访问
    }
    if(ql < 0x7f)ql -= 0x40; //注意!
    else ql -= 0x41;
    qh -= 0x81;
    foffset = ((uint32_t)190 * qh + ql) * csize;	//得到字库中的字节偏移量
    switch(size)
    {
    case 32:
        W25Q64_Read(mat, foffset + ftinfo.f12addr, csize);
        break;
    case 16:
        W25Q64_Read(mat, foffset + ftinfo.f16addr, csize);
        break;
    case 24:
        W25Q64_Read(mat, foffset + ftinfo.f24addr, csize);
        break;

    }
}


/**----------------------------------------------------------------------------
* @FunctionName  : OS_Font_Show()
* @Description   : 打印句子
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
* x,y 坐标    *str 语句指针   size 字体大小
------------------------------------------------------------------------------*/
void OS_Font_Show(u16 x, u16 y, char *font, u8 size, u8 mode)
{
    u8 temp, t, t1;
    u16 y0 = y;
    u8 dzk[144];
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size); //得到字体一个字符对应点阵集所占的字节数
    if(size != 32 && size != 16 && size != 24) return;	//不支持的size
    OS_HzMat_Get(font, dzk, size);	//得到相应大小的点阵数据
    for(t = 0; t < csize; t++)
    {
        temp = dzk[t];			//得到点阵数据
        for(t1 = 0; t1 < 8; t1++)
        {
            if(temp & 0x80) OS_Point_Draw(x, y, TextColor);
            else if(mode == 1) OS_Point_Draw(x, y, BackColor);
            temp <<= 1;
            y++;
            if((y - y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}



/*============================================= 应用程序层 =========================================*/
/**----------------------------------------------------------------------------
* @FunctionName  : OS_Line_Draw()
* @Description   : 画线
* @Data          : 2016/7/14
* @Explain
-------------------------------------------------------------------------------
x1,y1	:起点坐标
x2,y2	:终点坐标
size	:线条宽度
------------------------------------------------------------------------------*/
int OS_Line_Draw(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{

    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    if(x1 == x2 && y1 == y2) x1 = x2 + 1;

    delta_x = x2 - x1; //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if(delta_x > 0)incx = 1; //设置单步方向
    else if(delta_x == 0)incx = 0; //垂直线
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if(delta_y > 0)incy = 1;
    else if(delta_y == 0)incy = 0; //水平线
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if( delta_x > delta_y)distance = delta_x; //选取基本增量坐标轴
    else distance = delta_y;
    for(t = 0; t <= distance + 1; t++ ) //画线输出
    {
        OS_Point_Draw(uRow, uCol, color); //画点
        xerr += delta_x ;
        yerr += delta_y ;
        if(xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if(yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }

    return 0;
}




/**----------------------------------------------------------------------------
* @FunctionName  : OS_Circle_Draw()
* @Description   : 画圆
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
(x,y)		:中心点
r			:圆的半径
size		:线条的宽度  	0,表示实心
------------------------------------------------------------------------------*/
int OS_Circle_Draw(u16 x, u16 y, u8 r, u8 size, u16 color)
{
    u16 temp = TextColor;

    TextColor = color;
    if(size == 0)
    {
        LCD_DrawCircleS(x, y, r, color);
    }
    else
    {
        while(size--)
            LCD_DrawCircle(x, y, r - size);
    }
    TextColor = temp;

    return 0;
}


/**----------------------------------------------------------------------------
* @FunctionName  : OS_Rect_Draw()
* @Description   : 画矩形
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
x1,y1,x2,y2 ：矩形两对角坐标
size		：线条宽度   0,表示实心
------------------------------------------------------------------------------*/
int OS_Rect_Draw(u16 x1, u16 y1, u16 x2, u16 y2, u8 size, u16 color)
{
    u16 temp = TextColor;

    TextColor = color;
    if(size == 0)
    {
        LCD_DrawRectS(x1, y1, x2, y2, color);

    }
    else
    {
        while(size --)
        {
            OS_Line_Draw(x1, y1 + size, x2, y1 + size, color);
            OS_Line_Draw(x1, y2 - size, x2, y2 - size, color);
            OS_Line_Draw(x2 - size, y1, x2 - size, y2, color);
            OS_Line_Draw(x1 + size, y1, x1 + size, y2, color);
        }
    }
    TextColor = temp;

    return 0;
}



/**----------------------------------------------------------------------------
* @FunctionName  : OS_Picture_Draw()
* @Description   : 显示图片
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
* RGB565  16位真彩显示
* mode :  0 不可放缩      1 可缩放       2 可缩放，不显示白色
		  3 可缩放，不显示白色,且变灰
* 带头数据
------------------------------------------------------------------------------*/
int OS_Picture_Draw(u16 x1, u16 y1, u16 x2, u16 y2, u8 mode, const u8 *pic)
{
    u16 temp = TextColor;
    static u8 UeseState = 0;

    if(UeseState) return -1;
    UeseState = 1;

    if(mode == 0)
    {
        u16 x, y, p_color;
        u16 len_x = (pic[2] << 8) | pic[3];
        u16 len_y = (pic[4] << 8) | pic[5];

        for(y = 0; y < len_y; y++)
            for(x = 0; x < len_x * 2; x += 2)
            {
                if((u16)(x / 2 + x1) <= x2)
                    if((u16)(y + y1) <= y2)
                    {
                        p_color = pic[x + 6 + y * len_x * 2] << 8 | pic[(x + 7) + y * len_x * 2];

                        LCD_SetCursor(x / 2 + x1, y + y1); //设置光标位置
                        LCD_WriteRAM_Prepare();
                        LCD_WriteRAM(p_color);
                    }
            }
    }
    else if(mode == 1)
    {
        u16 x, y, p_color;
        float density_x, density_y;
        u16 len_x = (pic[2] << 8) | pic[3];
        u16 len_y = (pic[4] << 8) | pic[5];

        density_x = (float)(x2 - x1) / len_x;
        density_y = (float)(y2 - y1) / len_y;

        for(y = 0; y < len_y; y++)
            for(x = 0; x < len_x * 2; x += 2)
            {
                if((u16)((x / 2)*density_x + x1) <= x2)
                    if((u16)(y * density_y + y1) <= y2)
                    {
                        p_color = pic[x + 6 + y * len_x * 2] << 8 | pic[(x + 7) + y * len_x * 2];
                        LCD_SetCursor((x / 2)*density_x + x1, y * density_y + y1); //设置光标位置
                        LCD_WriteRAM_Prepare();
                        LCD_WriteRAM(p_color);
                    }

            }

    }

    else if(mode == 2)
    {
        u16 x, y, p_color;
        float density_x, density_y;
        u16 len_x = (pic[2] << 8) | pic[3];
        u16 len_y = (pic[4] << 8) | pic[5];

        density_x = (float)(x2 - x1) / len_x;
        density_y = (float)(y2 - y1) / len_y;

        for(y = 0; y < len_y; y++)
            for(x = 0; x < len_x * 2; x += 2)
            {
                if((u16)((x / 2)*density_x + x1) <= x2)
                    if((u16)(y * density_y + y1) <= y2)
                    {
                        p_color = pic[x + 6 + y * len_x * 2] << 8 | pic[(x + 7) + y * len_x * 2];
                        if(p_color > 1) //不显示白色背景
                        {
                            LCD_SetCursor((x / 2)*density_x + x1, y * density_y + y1); //设置光标位置
                            LCD_WriteRAM_Prepare();
                            LCD_WriteRAM(p_color);
                        }
                    }

            }
    }

    else if(mode == 3)
    {
        u16 x, y, p_color, BitRed, BitGreen, BitBlue;
        float density_x, density_y;
        u16 len_x = (pic[2] << 8) | pic[3];
        u16 len_y = (pic[4] << 8) | pic[5];

        density_x = (float)(x2 - x1) / len_x;
        density_y = (float)(y2 - y1) / len_y;

        for(y = 0; y < len_y; y++)
            for(x = 0; x < len_x * 2; x += 2)
            {
                if((u16)((x / 2)*density_x + x1) <= x2)
                    if((u16)(y * density_y + y1) <= y2)
                    {
                        p_color = pic[x + 6 + y * len_x * 2] << 8 | pic[(x + 7) + y * len_x * 2];
                        if(p_color > 1) //不显示黑色背景
                        {
                            p_color = pic[x + 6 + y * len_x * 2] << 8 | pic[(x + 7) + y * len_x * 2];
                            BitRed   = (p_color & (0x1f << 11)) >> 11;
                            BitGreen = (p_color & (0x3f << 5 )) >> 5;
                            BitBlue = p_color & 0x1f;
                            p_color = ((BitRed / 3) << 11) | ((BitGreen / 3) << 5) | (BitBlue / 3);

                            LCD_SetCursor((x / 2)*density_x + x1, y * density_y + y1); //设置光标位置
                            LCD_WriteRAM_Prepare();
                            LCD_WriteRAM(p_color);
                        }
                    }

            }

    }
    TextColor = temp;

    UeseState = 0;
    return 0;
}







/**----------------------------------------------------------------------------
* @FunctionName  : OS_String_Show()
* @Description   : 打印句子
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
x,y 		坐标
str 		语句指针
size 		字体大小 16 24 32
mode 		0,无背景色，1带背景色
return 		0,打印完成		-1,打印被占用
------------------------------------------------------------------------------*/
int OS_String_Show(u16 x, u16 y, u8 size, u8 mode, char *str)
{
    uint16_t x0 = x;
    uint8_t bHz = 0;   //字符或者中文


    while(*str != 0)		//数据未结束
    {

        if(!bHz)
        {
            if((signed char)*str < 0)bHz = 1; //中文
            else              //字符
            {
                if(*str == '\n') //换行符号
                {
                    y += size;
                    x = x0;
                    str++;
                }
                OS_Char_Show(x, y, *str, size, mode);
                str++;
                x += size / 2; //字符,为全字的一半
            }
        }
        else //中文
        {
            bHz = 0; //有汉字库
            OS_Font_Show(x, y, str, size, mode);
            str += 2;
            x += size; //下一个汉字偏移
        }
    }

    return 0;
}




/**----------------------------------------------------------------------------
* @FunctionName  : OS_Num_Show()
* @Description   : 多功能打印数值
* @Data          : 2017/7/12
* @Explain
-------------------------------------------------------------------------------
x,y 		坐标
num 		数值
size 		字体大小 12 16 24 32
mode 		0,无背景色，1带背景色
format		--------------------------------
			%d 十进制有符号整数，有BUG，请用%0.0f替代
			%u 十进制无符号整数
			%f 浮点数	%lf 长浮点数
			%s 字符串
			%c 单个字符
			%p 指针的值
			%e 指数形式的浮点数
			%x,%X 无符号以十六进制表示的整数
			%o 无符号以八进制表示的整数
			%g,NULL 自动选择合适的表示法
			%p 输出地址符
			--------------------------------
return 		0,打印完成		-1,打印被占用
注：format还有巧妙的用法，例如："时间：%0.0f ms"
------------------------------------------------------------------------------*/
int OS_Num_Show(u16 x, u16 y, u8 size, u8 mode, double num, char *format)
{
    char temp[100] = {0};

    if(format == NULL) sprintf(temp, "%g", num); //自动选择格式
    else sprintf(temp, format, num);

    OS_String_Show(x, y, size, mode, temp);

    return 0;
}

/**----------------------------------------------------------------------------
* @FunctionName  : OS_Wave_Draw()
* @Description   : 画波形组合函数
* @Data          : 2017/7/13
* @Explain
-------------------------------------------------------------------------------
Struct_OSC：--------------------------------
			u16 x,y;//OSC窗口的位置
			float ZoomWindows;//显示窗口缩放量
			u16 BackColor;//背景颜色

			float ShiftX; //横轴偏移量	1.0是标准的
			float ShiftY; //纵轴偏移量
			float ZoomX;  //横轴缩放量
			float ZoomY;  //纵轴缩放量
			u16 LineColor;//线条颜色
			u8 LineSize;//线条粗细的尺寸
			u8 Mode;//	OSC_MODE_NORMAL:不显示面积 	OSC_MODE_FFT：显示面积（FFT模式）
			u16 BuffLenth;//点数
			u8 BuffFormat;//Buff的格式
			u16 *Buff_u16;
			u8 *Buff_u8;
			float *Buff_float;
			double *Buff_double;
			--------------------------------
mode		--------------------------------
			OSC_MODE_WINDOWS_SHOW	//画窗口
            OSC_MODE_LINE_SHOW		//画线
			--------------------------------
return 		0,打印完成		-1,打印被占用	-2,传参错误
注意：double float 型数据的起点在正中央；而u8 u16 型数据的起点在底端
------------------------------------------------------------------------------*/
int OS_Wave_Windows_Show(Type_OSC *Struct_OSC)
{
    u16 x1 = Struct_OSC->x;
    u16 y1 = Struct_OSC->y;
    u16 x2 = Struct_OSC->x + OS_LCD_WHITH * Struct_OSC->ZoomWindows;
    u16 y2 = Struct_OSC->y + OS_LCD_HEIGHT* Struct_OSC->ZoomWindows;
    u8 size = 10 * Struct_OSC->ZoomWindows;
    u16 i = 0;
    u16 cnt = 0;

    /* 画边框及背景 --------------------------------------*/
    if(size < 1) size = 1;
    OS_Rect_Draw(x1 + size, y1 + size, x2 - size - 1, y2 - size - 1, 0, Struct_OSC->BackColor); //背景
    OS_Rect_Draw(x1, y1, x2 - 1, y2 - 1, size, Struct_OSC->WindowsColor); //方框
    OS_Line_Draw(x1 + 1, y2, x2, y2 - 1, (Struct_OSC->WindowsColor / 2)); //阴影
    OS_Line_Draw(x2, y1 + 1, x2, y2, (Struct_OSC->WindowsColor / 2)); //阴影

    /* 标尺 ---------------------------------------------*/
    for(i = y1 + size - 1; i < y2 - size; i += size) //Y轴
    {
        cnt = (cnt + 1) % 5;
        if(cnt == 0)
        {
            OS_Line_Draw(x1 + size, i, x1 + size * 3, i, Struct_OSC->WindowsColor);
            OS_Line_Draw(x2 - size * 3, i, x2 - size, i, Struct_OSC->WindowsColor);
        }
        else
        {
            OS_Line_Draw(x1 + size, i, x1 + size * 2, i, Struct_OSC->WindowsColor);
            OS_Line_Draw(x2 - size * 2, i, x2 - size, i, Struct_OSC->WindowsColor);
        }
    }

    for(i = x1 + size - 1; i < x2 - size; i += size) //X轴
    {
        cnt = (cnt + 1) % 5;
        if(cnt == 0)
        {
            OS_Line_Draw(i, y1 + size, i, y1 + size * 3, Struct_OSC->WindowsColor);
            OS_Line_Draw(i, y2 - size * 3, i, y2 - size, Struct_OSC->WindowsColor);
        }
        else
        {
            OS_Line_Draw(i, y1 + size, i, y1 + size * 2, Struct_OSC->WindowsColor);
            OS_Line_Draw(i, y2 - size * 2, i, y2 - size, Struct_OSC->WindowsColor);
        }
    }

    return 0;

}





int OS_Wave_Line_Show(Type_OSC *Struct_OSC)
{

//	if(Struct_OSC->Buff_u16[0] >= 0x0FFF||Struct_OSC->Buff_u16[ADC1_DMA2_LENTH/2] >= 0x0FFF\
//		||Struct_OSC->Buff_u16[ADC1_DMA2_LENTH-5] >= 0x0FFF||Struct_OSC->Buff_u16[ADC1_DMA2_LENTH/3] >= 0x0FFF)
//		return -1;


    u8 size = 10 * Struct_OSC->ZoomWindows;
    u16 x1 = Struct_OSC->x + size * 3 + Struct_OSC->LineSize + 3;
    u16 y1 = Struct_OSC->y + size * 3 + Struct_OSC->LineSize + 3;
    u16 x2 = Struct_OSC->x + OS_LCD_WHITH * Struct_OSC->ZoomWindows - size * 3 - Struct_OSC->LineSize - 3;
    u16 y2 = Struct_OSC->y + OS_LCD_HEIGHT* Struct_OSC->ZoomWindows - size * 3 - Struct_OSC->LineSize - 3;
    u16 i = 0, j = 0;
    u16 mid_y = (y1 + y2) / 2;
    u16 WD_lenth = x2 - x1;
    u16 t_x1 = 0, t_y1 = 0, t_x2 = 0, t_y2 = 0;
    u16 ZeroLine = 0;

    for(i = 0; i < WD_lenth - 1; i ++)
    {
        /* 清除上一次的痕迹 ----------------------------------------*/
        if(Struct_OSC ->LastMode != OSC_MODE_ERROR)
        {
            if(Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1] >= y1//防止越界清屏
                    && Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1 + 1] >= y1
                    && Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1] <= y2
                    && Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1 + 1] <= y2)
            {
                if(Struct_OSC ->LastMode == OSC_MODE_NORMAL)
                {
                    for(j = 0; j < Struct_OSC->LastLineSize; j++)
                    {
                        OS_Line_Draw(i + x1 - j, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1], i + x1 + 1 - j, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1 + 1], Struct_OSC->BackColor);
                        OS_Line_Draw(i + x1 + j, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1], i + x1 + 1 + j, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1 + 1], Struct_OSC->BackColor);
                        OS_Line_Draw(i + x1, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1] + j, i + x1 + 1, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1 + 1] + j, Struct_OSC->BackColor);
                        OS_Line_Draw(i + x1, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1] - j, i + x1 + 1, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1 + 1] - j, Struct_OSC->BackColor);
                    }
                }
                else if(Struct_OSC ->LastMode == OSC_MODE_FFT)
                {
                    if(Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1] < ZeroLine)
                        OS_Line_Draw(i + x1, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1], i + x1, Struct_OSC->LastZeroLine + 1, Struct_OSC->BackColor);
                    else if(Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1] > ZeroLine)
                        OS_Line_Draw(i + x1, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1], i + x1, Struct_OSC->LastZeroLine + 3, Struct_OSC->BackColor);

                    OS_Line_Draw(i + x1, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1], i + x1 + 1, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1+1], Struct_OSC->BackColor);
                }

                if(Struct_OSC->LastZeroLine >= y1 && Struct_OSC->LastZeroLine <= y2)//防止越界清屏
                {
                    OS_Point_Draw(i + x1 + 1, Struct_OSC->LastZeroLine + 1, Struct_OSC->BackColor);
                    OS_Point_Draw(i + x1 + 1, Struct_OSC->LastZeroLine + 2, Struct_OSC->BackColor);
                    OS_Point_Draw(i + x1 + 1, Struct_OSC->LastZeroLine + 3, Struct_OSC->BackColor);
                }
            }
        }

        /* 数据转换 ------------------------------------------*/
        t_x1 = x1 + i / Struct_OSC->ZoomX - Struct_OSC->ShiftX;
        t_x2 = x1 + (i + 1) / Struct_OSC->ZoomX - Struct_OSC->ShiftX;

        if((t_x1 < Struct_OSC->BuffLenth) && (t_x2 < Struct_OSC->BuffLenth))
        {
            if(Struct_OSC->BuffFormat == OSC_FORMAT_U8)
            {
                t_y1 = y2 - Struct_OSC->Buff_u8[t_x1] * Struct_OSC->ZoomY - Struct_OSC->ShiftY;
                t_y2 = y2 - Struct_OSC->Buff_u8[t_x2] * Struct_OSC->ZoomY - Struct_OSC->ShiftY;
                ZeroLine = y2 - Struct_OSC->ShiftY;
            }
            else if(Struct_OSC->BuffFormat == OSC_FORMAT_U16)
            {
                t_y1 = y2 - Struct_OSC->Buff_u16[t_x1] * Struct_OSC->ZoomY - Struct_OSC->ShiftY;
                t_y2 = y2 - Struct_OSC->Buff_u16[t_x2] * Struct_OSC->ZoomY - Struct_OSC->ShiftY;
                ZeroLine = y2 - Struct_OSC->ShiftY;
            }
            else if(Struct_OSC->BuffFormat == OSC_FORMAT_FLOAT)
            {
                t_y1 = mid_y - Struct_OSC->Buff_float[t_x1] * Struct_OSC->ZoomY - Struct_OSC->ShiftY;
                t_y2 = mid_y - Struct_OSC->Buff_float[t_x2] * Struct_OSC->ZoomY - Struct_OSC->ShiftY;
                ZeroLine = mid_y - Struct_OSC->ShiftY;
            }
            else if(Struct_OSC->BuffFormat == OSC_FORMAT_DOUBLE)
            {
                t_y1 = mid_y - Struct_OSC->Buff_double[t_x1] * Struct_OSC->ZoomY - Struct_OSC->ShiftY;
                t_y2 = mid_y - Struct_OSC->Buff_double[t_x2] * Struct_OSC->ZoomY - Struct_OSC->ShiftY;
                ZeroLine = mid_y - Struct_OSC->ShiftY;
            }

            /* 防止绘图越界 ------------------------------------------*/
            if(t_y1 < y1) t_y1 = y1;
            else if(t_y1 > y2)  t_y1 = y2;

            if(t_y2 < y1) t_y2 = y1;
            else if(t_y2 > y2)  t_y2 = y2;

            /* 描绘这次的波形 ------------------------------------------*/
            if(t_x1 != 0 && t_x2 != 0)
            {
                if(Struct_OSC->Mode == OSC_MODE_NORMAL)//空心
                {
                    for(j = 0; j < Struct_OSC->LineSize; j++)
                    {
                        OS_Line_Draw(i + x1, t_y1 + j, i + x1 + 1, t_y2 + j, Struct_OSC->LineColor);
                        OS_Line_Draw(i + x1, t_y1 - j, i + x1 + 1, t_y2 - j, Struct_OSC->LineColor);
                        OS_Line_Draw(i + x1 + j, t_y1 , i + x1 + 1 + j, t_y2 , Struct_OSC->LineColor);
                        OS_Line_Draw(i + x1 - j, t_y1 , i + x1 + 1 - j, t_y2 , Struct_OSC->LineColor);
                    }
                }
                if(Struct_OSC->Mode == OSC_MODE_FFT)//带面积
                {
                    if(t_y1 < ZeroLine)
                        OS_Line_Draw(i + x1, t_y1, i + x1, ZeroLine + 1, Struct_OSC->LineColor);
                    else if(t_y1 > ZeroLine)
                        OS_Line_Draw(i + x1, t_y1, i + x1, ZeroLine + 3, Struct_OSC->LineColor);

                    OS_Line_Draw(i + x1, t_y1, i + x1 + 1, t_y2, ~Struct_OSC->LineColor);
                }
            }

            if((ZeroLine >= y1 && ZeroLine <= y2))
            {
                OS_Point_Draw(i + x1 + 1, ZeroLine + 1, ~BLACK);//零点线
                OS_Point_Draw(i + x1 + 1, ZeroLine + 2, BLACK);
                OS_Point_Draw(i + x1 + 1, ZeroLine + 3, ~BLACK);
            }
            /* 记录这次波形的位置 ---------------------------------*/
            if(i == 0)
            {
                Struct_OSC->LastBuff[(Struct_OSC->LastBuff_Cnt + 1) % 2][x1] = t_y1;
            }
            Struct_OSC->LastBuff[(Struct_OSC->LastBuff_Cnt + 1) % 2][i + x1 + 1] = t_y2;
            Struct_OSC->LastZeroLine = ZeroLine;
        }
        else
        {
            Struct_OSC ->LastMode = OSC_MODE_ERROR;
        }

    }
    /* 记录这次设置参数 ---------------------------------*/
    Struct_OSC->LastBuff_Cnt = (Struct_OSC->LastBuff_Cnt + 1) % 2;
    Struct_OSC->LastLineSize = Struct_OSC->LineSize;
    Struct_OSC->LastMode = Struct_OSC->Mode;

    return 0;
}

int OS_Wave_Unite(Type_OSC *Struct_OSC_Source, Type_OSC *Struct_New)
{
    if(Struct_New != NULL && Struct_OSC_Source != NULL)
    {
        Struct_New->ZoomWindows = Struct_OSC_Source->ZoomWindows;
        Struct_New->BackColor = Struct_OSC_Source->BackColor;
        Struct_New->WindowsColor = Struct_OSC_Source->WindowsColor;
        Struct_New->x = Struct_OSC_Source->x;
        Struct_New->y = Struct_OSC_Source->y;
        return 0;
    }

    return -1;
}

int OS_Wave_Draw(Type_OSC *Struct_OSC, u8 mode)
{
    if(mode == OSC_MODE_WINDOWS_SHOW)
        return OS_Wave_Windows_Show(Struct_OSC);
    else if(mode == OSC_MODE_LINE_SHOW)
        return OS_Wave_Line_Show(Struct_OSC);
    else return -2;

}



/**
* @brief  画网格和刻度线 (设置的参数必须让xcase、ycase为整数！！！！)
* @param
* @retval none
*/
void Draw_Grid(GRAPH_Struct griddata)
{
    u16 i,j;
    u16 shift_x,shift_y;
    u16 x1=griddata.x1;
    u16 y1=griddata.y1;
    u16 x2=griddata.x2;
    u16 y2=griddata.y2;
    u16 color=griddata.griacolor;
    u16 xnumber=griddata.xnumber;
    u16 ynumber=griddata.ynumber;

    float xcase=(float)(x2-x1)/((xnumber-1)*5);//横坐标一小格的长度   每大格有5小格
    float ycase=(float)(y2-y1)/((ynumber-1)*5);//纵坐标一小格的长度

    shift_x=xcase;
    shift_y=ycase;

    OS_Rect_Draw(x1-shift_x,y1-shift_y,x2+shift_x,y2+shift_y,1,color);//画外轮廓

    if(griddata.grid == 1)
    {
        for(i=y1; i<=y2; i+=ycase*5) //画横线
        {
            for(j=x1; j<=x2; j+=xcase)
                OS_Point_Draw(j,i,color);
        }

        for(i=x1; i<=x2; i+=xcase*5) //画竖线
        {
            for(j=y1; j<y2; j+=ycase)
                OS_Point_Draw(i,j,color);
        }
    }

    for(i=x1; i<=x2; i+=xcase) //画顶部刻度线
    {
        if((i-x1)%((u16)(xcase*5)) == 0)
            OS_Line_Draw(i,y1-shift_y,i,y1-shift_y+ycase,color);
        else
            OS_Line_Draw(i,y1-shift_y,i,y1-shift_y+ycase/2,color);
    }

    for(i=x1; i<=x2; i+=xcase) //画底部刻度线
    {
        if((i-x1)%((u16)(xcase*5)) == 0)
            OS_Line_Draw(i,y2+shift_y,i,y2+shift_y-ycase,color);
        else
            OS_Line_Draw(i,y2+shift_y,i,y2+shift_y-ycase/2,color);
    }

    for(i=y1; i<=y2; i+=ycase) //画左边刻度线
    {
        if((i-y1)%((u16)(ycase*5)) == 0)
            OS_Line_Draw(x1-shift_x,i,x1-shift_x+xcase,i,color);
        else
            OS_Line_Draw(x1-shift_x,i,x1-shift_x+xcase/2,i,color);
    }

    for(i=y1; i<=y2; i+=ycase) //画右边刻度线
    {
        if((i-y1)%((u16)(ycase*5)) == 0)
            OS_Line_Draw(x2+shift_x,i,x2+shift_x-xcase,i,color);
        else
            OS_Line_Draw(x2+shift_x,i,x2+shift_x-xcase/2,i,color);
    }
}

/**
* @brief  显示标签
* @param  xnumber:横线数量
					ynumber:竖线数量
* @retval none
*/
void Show_Label(GRAPH_Struct griddata,u8 yselect)
{
    u16 i;
    u16 x1=griddata.x1;
    u16 y1=griddata.y1;
    u16 x2=griddata.x2;
    u16 y2=griddata.y2;
    u16 xnumber=griddata.xnumber;
    u16 ynumber=griddata.ynumber;

    float step;
    float xcase=(float)(x2-x1)/(xnumber-1);//横坐标大小格的长度
    float ycase=(float)(y2-y1)/(ynumber-1);//纵坐标大小格的长度


    OS_String_Show(x2-20,y2+25,16,0,"Fre/Hz");//横坐标轴标签

    step=(log10f(griddata.xmax)-log10f(griddata.xmin))/(xnumber-1);
	

    for(i=0; i<xnumber; i++) //横坐标轴单位长度标签
        OS_Num_Show(x1+i*xcase-20,y2+10,16,1, powf(10, ( log10f(griddata.xmin)+i*step )),"%.0f"); //griddata.xmin +

    if(yselect & LEFTY)
    {
        OS_String_Show(x1,y1-25,16,0,"Gain/dB");//左边纵坐标轴标签

        OS_Line_Draw(x2+60,y1,x2+100,y1,griddata.left_ycolor);		//备注不同颜色曲线代表的含义
        OS_String_Show(x2-60,y1-25,16,0,"Gain--Fre");

        step=(griddata.left_ymax-griddata.left_ymin)/(ynumber-1);
        for(i=0; i<ynumber; i++) //左边纵坐标轴单位长度标签
            OS_Num_Show(x1-40,y1+i*ycase-7,16,1,griddata.left_ymax-i*step,"%.1f");
    }

    if(yselect & RIGHTY)
    {
        OS_String_Show(x2-50,y1-25,16,0,"Phase/°");//右边纵坐标轴标签

        OS_Line_Draw(x2+60,y1+30,x2+100,y1+30,griddata.right_ycolor);
        OS_String_Show(x2+110,y1+20,16,0,"Phase--Fre");

        step=(griddata.right_ymax-griddata.right_ymin)/(ynumber-1);
        for(i=0; i<ynumber; i++) //右边纵坐标轴单位长度标签
            OS_Num_Show(x2+15,y1+i*ycase-7,16,1,griddata.right_ymax-i*step,"%.0f");
    }

}

/**
* @brief  画曲线
* @param  yselect:  0---画 x-left_y 1---画 x-right_y
* @retval none
*/
void Draw_Graph(GRAPH_Struct *griddata,u8 yselect)
{
    u16 i,j;
    u16 x1=griddata->x1;
    u16 y1=griddata->y1;
    u16 x2=griddata->x2;
    u16 y2=griddata->y2;
    u16 high=griddata->y2-griddata->y1;
    u16 wide=griddata->x2-griddata->x1;
    u16 length=griddata->Buff_Lenth;
    u16 tempx1,tempx2,tempy1,tempy2;
    float vpp;
    float xstep=(float)wide/(length);//横轴的增量
    float xcase=(float)(x2-x1)/((griddata->xnumber-1)*5);//横坐标一小格的长度   每大格有5小格
    float ycase=(float)(y2-y1)/((griddata->ynumber-1)*5);//纵坐标一小格的长度
    static u8 frist=1;

    //清除上次的波形
    if(yselect & LEFTY)
    {
				tempx1=x1;
				for(i=1;i<length;i++)
				{
						tempx2=x1+i*xstep;
						OS_Line_Draw(tempx1,griddata->lastbuff[0][i-1],tempx2,griddata->lastbuff[0][i],griddata->Backcolor);
				}
        if(!frist)
        {
            for(i=0; i<length; i++)
            {
                tempx1=x1+i*xstep;
                OS_Line_Draw(tempx1,y2,tempx1,griddata->lastbuff[0][i],griddata->Backcolor);
            }
        }

        frist=0;
    }

    if(yselect & RIGHTY)
    {
        tempx1=x1;
        for(i=1; i<length; i++)
        {
            tempx2=x1+i*xstep;
            OS_Line_Draw(tempx1,griddata->lastbuff[1][i-1],tempx2,griddata->lastbuff[1][i],griddata->Backcolor);
        }
    }

    //重新画网格，补充网格
    if(griddata->grid == 1)
    {
        for(i=y1; i<=y2; i+=ycase*5) //画横线
        {
            for(j=x1; j<=x2; j+=xcase)
                OS_Point_Draw(j,i,griddata->griacolor);
        }

        for(i=x1; i<=x2; i+=xcase*5) //画竖线
        {
            for(j=y1; j<y2; j+=ycase)
                OS_Point_Draw(i,j,griddata->griacolor);
        }
    }

    //画波形
    if(yselect & LEFTY)
    {
				vpp=griddata->left_ymax-griddata->left_ymin;

			  tempx1=x1;
			  tempy1=y2-(griddata->left_buff[0]-griddata->left_ymin)/vpp*high;

			  griddata->lastbuff[0][0]=tempy1;//保存本次数据

			  for(i=1;i<length;i++)
			  {
					tempx2=x1+i*xstep;

					tempy2=y2-(griddata->left_buff[i]-griddata->left_ymin)/vpp*high;

					OS_Line_Draw(tempx1,tempy1,tempx2,tempy2,griddata->left_ycolor);

					griddata->lastbuff[0][i]=tempy2;//保存本次数据

					tempx1=tempx2;
					tempy1=tempy2;
				}

//        vpp=griddata->left_ymax-griddata->left_ymin;

//        for(i=0; i<length; i++)
//        {
//            tempx1=x1+i*xstep;

//            tempy1=y2-(griddata->left_buff[i]-griddata->left_ymin)/vpp*high;

//            OS_Line_Draw(tempx1,y2,tempx1,tempy1,griddata->left_ycolor);

//            griddata->lastbuff[0][i]=tempy1;//保存本次数据
//        }
    }

    if(yselect & RIGHTY)
    {
        vpp=griddata->right_ymax-griddata->right_ymin;

        tempx1=x1;
        tempy1=y2-(griddata->right_buff[0]-griddata->right_ymin)/vpp*high;

        griddata->lastbuff[1][0]=tempy1;

        for(i=1; i<length; i++)
        {
            tempx2=x1+i*xstep;

            tempy2=y2-(griddata->right_buff[i]-griddata->right_ymin)/vpp*high;

            OS_Line_Draw(tempx1,tempy1,tempx2,tempy2,griddata->right_ycolor);

            griddata->lastbuff[1][i]=tempy2;

            tempx1=tempx2;
            tempy1=tempy2;
        }
    }

    //更新光标显示
    griddata->cursor_refrssh=1;
}


/**
* @brief  显示光标位置数据
* @param
* @retval none
*/
void Cursor_Data_Show(GRAPH_Struct *griddata,u8 yselect)
{
    u16 i;
    u16 num,temp;
    u16 x1=griddata->x1;
    u16 y1=griddata->y1;
    u16 x2=griddata->x2;
    u16 y2=griddata->y2;
    u16 wide=griddata->x2-griddata->x1;
    u16 length=griddata->Buff_Lenth-1;
    u8  shift;

    float vpp;
    float xstep=(float)wide/length;//横轴的增量
    float xcase=(float)(x2-x1)/((griddata->xnumber-1)*5);//横坐标一小格的长度   每大格有5小格
    float ycase=(float)(y2-y1)/((griddata->ynumber-1)*5);//纵坐标一小格的长度

    if(griddata->cursor_x == 0)//初始位置
    {
        griddata->cursor_x=x1;
    }

    if(x1-10 >= 0 && x2+10 <= 800)	shift=10;
    else	shift=0;//增加范围，便于移动到边界

    if(TouchStruct.NowX >= x1-shift && TouchStruct.NowX <= x2+shift)
    {
        if(TouchStruct.NowX >= x2)
            griddata->cursor_x=x2;
        else if(TouchStruct.NowX <= x1)
            griddata->cursor_x=x1;
        else
        {
            num=(u16)(TouchStruct.NowX/xstep);

            temp=num*xstep;

            if(temp <= x1)
                griddata->cursor_x=x1;
            else
                griddata->cursor_x=temp;
        }
    }

    //改变光标位置，更新显示
    if(griddata->cursor_x != griddata->lastcursor_x || griddata->cursor_refrssh)
    {
        if(griddata->cursor_refrssh)//强行更新光标显示
        {
            if(griddata->lastcursor_x == 0)
                griddata->cursor_x=x1;
            else
                griddata->cursor_x=griddata->lastcursor_x;
        }
        else
        {
            //清除上次的光标线
            OS_Line_Draw(griddata->lastcursor_x,y1,griddata->lastcursor_x,y2,griddata->Backcolor);

            //画上一次光标位置的曲线点
            num=(u16)(griddata->lastcursor_x - x1)/xstep;

            if(num < length)
            {
                if(yselect & LEFTY && griddata->lastcursor_x != 0)
                    OS_Line_Draw(griddata->lastcursor_x,griddata->lastbuff[0][num],griddata->lastcursor_x+xstep,griddata->lastbuff[0][num+1],griddata->left_ycolor);

                if(yselect & RIGHTY && griddata->lastcursor_x != 0)
                    OS_Line_Draw(griddata->lastcursor_x,griddata->lastbuff[1][num],griddata->lastcursor_x+xstep,griddata->lastbuff[1][num+1],griddata->right_ycolor);
            }

            //补全网格
            if((griddata->lastcursor_x % (u16)xcase) == 0 && griddata->lastcursor_x != 0)
            {
                if((griddata->lastcursor_x % (u16)(xcase*5)) == 0)
                {
                    for(i=y1; i<y2; i+=ycase)
                        OS_Point_Draw(griddata->lastcursor_x,i,griddata->griacolor);
                }
                else
                {
                    for(i=y1; i<=y2; i+=ycase*5)
                        OS_Point_Draw(griddata->lastcursor_x,i,griddata->griacolor);
                }
            }
        }

        //画光标
        OS_Line_Draw(griddata->cursor_x,y1,griddata->cursor_x,y2,griddata->cursor_color);

        //显示光标数据
        vpp=griddata->xmax-griddata->xmin;
        OS_Num_Show(x2+60,y1+60,24,1,griddata->xmin+(griddata->cursor_x-x1)/xstep*vpp/length,"Fre:%.2fMHz  ");

        num=(u16)(griddata->cursor_x - x1)/xstep;
        OS_Num_Show(x2+60,y1+100,24,1,griddata->left_buff[num],"Av:%.2fdB  ");
        OS_Num_Show(x2+60,y1+140,24,1,griddata->right_buff[num],"Phase:%.2f°  ");

        griddata->cursor_refrssh=0;
        griddata->lastcursor_x=griddata->cursor_x;
    }
}




/*******************************(C) COPYRIGHT 2016 Wind（谢玉伸）*********************************/



















