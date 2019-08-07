/*
*********************************************************************************************************
*                                              _04_OS
* File			 : App_FreqAna.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2019/7/26
* function 	 : 频率分析
*********************************************************************************************************
*/
#include "App_FreqAna.h"

#include "log_table.inc"

#define MEASURE_LENGTH	200 			 //单片机显示测量点数
#define Get_Length      4650     	 //总测量地点数 ((35M-500M)/0.1MHZ) 

GRAPH_Struct 	GridData;							//网格结构体定义

float SignalData[Get_Length]= {0};   //采集的数据

float VMax_Fre;

void DDSDataInit(void);
u8 flag_relay = 0;
void FreqAna_main()
{
    GridData_Init();
	DDSDataInit();
    Draw_Grid(GridData);
    Draw_Graph(&GridData,LEFTY);

    while(1)
    {
        Draw_Grid(GridData);

        Show_Label(GridData,LEFTY);

        if(Key_Now_Get(KEY3,KEY_MODE_SHORT))
        {
            OS_LCD_Clear(WHITE);
            Draw_Grid(GridData);
            Draw_Graph(&GridData,LEFTY);
        }
		
		
		
		//AD9851_Sweep();
		
        OSTimeDly(111);
    }

}
/**
* @brief  网格结构体数据初始化
* @param
* @retval none
*/
void GridData_Init(void)
{

    /*	外轮廓大小		*/
    GridData.x1=40;
    GridData.y1=40;
    GridData.x2=760;
    GridData.y2=360;

    /*	显示网格		*/
    GridData.grid=1;

    /*	设置对应线条颜色		*/
    GridData.griacolor=Black;			//网格为白色
    GridData.Backcolor=White;			//背景为黑色
    GridData.left_ycolor=Red;			//左侧纵轴对应的曲线为黄色

    /*	横线、竖线数量		*/
    GridData.xnumber=11;
    GridData.ynumber=11;

    /*	设置数据长度和保存两个Y轴数据的指针		*/
    GridData.Buff_Lenth=MEASURE_LENGTH;
    GridData.left_buff=SignalData;

    /*	横轴的数字范围		*/
    GridData.xmax=1000000;
    GridData.xmin=100;

    /*	左边纵轴的数字范围		*/
    GridData.left_ymax=44;
    GridData.left_ymin=0;
}



/**
* @brief  AD9851扫频测量一次
* @param
* @retval none
*/
int log_table_length = 101;// sizeof(log_table);
void AD9851_Sweep(void)
{
	static int count = 0;

	count++;
	if(count == log_table_length)
		count = 0;
	
	dds.fre = log_table[count];
	sendData(dds);
}


/*
 * Return:      void
 * Parameters:  void
 * Description: DDS数据初始化
 */
__inline void DDSDataInit(void)
{
    /*	输出幅度 2v	*/
    dds.range=0.005;

    /*	输出频率	100000Hz	*/
    dds.fre=100;

    /*	扫频步进	1000Hz	*/
    dds.step=1000;

    /*	扫频时间	1000us	*/
    dds.step_time=1000;

    /*	扫频起始频率1000Hz		*/
    dds.fre_start=1000;

    /*	扫频终止200000	Hz	*/
    dds.fre_stop=200000;

    /*	默认为普通输出模式		*/
    dds.mode=NORMAL;

    /*	默认不打开输出		*/
    dds.output=1;

}

void task_1_3(void)
{
	dds.fre=1000;
	sendData(dds);
	
	
	
	
}


