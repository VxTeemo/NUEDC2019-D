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

char Fault_Type_str[][20]=
{
    "正常    ",
    "未知异常",
    "R1开路  ",
    "R1短路  ",
    "R2开路  ",
    "R2短路  ",
    "R3开路  ",
    "R3短路  ",
    "R4开路  ",
    "R4短路  ",
    "C1开路  ",
    "C2开路  ",
    "C3开路  ",
    "C1翻倍  ",
    "C2翻倍  ",
    "C3翻倍  "
};

#define ADS1256_MUX_AIN0 (ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM)
#define ADS1256_MUX_AIN1 (ADS1256_MUXP_AIN1 | ADS1256_MUXN_AINCOM)

#define ShowX1 10
#define ShowX2 180
#define ShowX3 360

//#define MEASURE_LENGTH	200 	//单片机显示测量点数
//#define Get_Length      201    //总测量地点数 ((10^2-10^6)对数步进)
#define R_Real    6800.0f       //固定电阻大小
#define ADS9851_V_IN   0.555f       //9851输出幅度 测输入电阻用的大电压
#define ADS9851_V_IN2   0.11f       //9851输出幅度 现在没用到
#define ADS9851_V_IN3   0.020f       //9851输出幅度 扫频时用的幅度

GRAPH_Struct 	GridData;		//网格结构体定义
const int log_table_length = sizeof(log_table)/sizeof(float);//101
float SignalData[log_table_length]= {0};   //采集的原始数据
float AvData[log_table_length]= {0};			//转换成对数

float VMax_Fre, Rin, Rout, All_Gain, Vol_IN_Std;
int last_fault = Fault_Type_Normal;
u8 Fault_Change_Flag = 1;//上电检测一次
u8 UpdateGragh = 0;

void DDSDataInit2(void);
void task_1_3(void);
Fault_Type Fault_Detect(void);
float ADS1256_Measure(float fre, float range, u32 delay);

void FreqAna_main()
{
    GridData_Init();

    DDSDataInit2();
    //sendData(dds);

    Draw_Grid(GridData);
    Show_Label(GridData,LEFTY);
    //Draw_Graph(&GridData,LEFTY);

//    Relay_Control(Relay_ALL, Relay_OFF);
//    Relay_Control(Relay_OUT, Relay_ON);
//    dds.fre = 15;
//    dds.range = 1;
//    sendData(dds);


    while(1)
    {

        if(Fault_Change_Flag)
        {
            task_1_3();
            OS_Num_Show(ShowX1,390     ,16,1,Rin , "输入电阻:%0.1f   ");
            OS_Num_Show(ShowX1,390+16  ,16,1,Rout, "输出电阻:%0.1f   ");
            OS_Num_Show(ShowX1,390+16*2,16,1,All_Gain,"增益:%0.1f    ");
            OS_Num_Show(ShowX1,390+16*3,16,1,1,"上限频率:%0.0fMhz  ");

            Fault_Change_Flag = 0;
        }

//			LED1 = 0;
//			Fault_Detect();
//			LED1 = 1;

        if(Key_Now_Get(KEY2,KEY_MODE_SHORT))
        {
            AD9851_Sweep();
        }

//        if(Key_Now_Get(KEY3,KEY_MODE_SHORT))
//        {
//            OS_LCD_Clear(WHITE);
//
//            Draw_Grid(GridData);
//
//            Draw_Graph(&GridData,LEFTY);
//
//        }



//        AD9851_Sweep();

        if(UpdateGragh)
        {
            Draw_Graph(&GridData,LEFTY);
            UpdateGragh = 0;
        }
        //Cursor_Data_Show(&GridData,LEFTY);

        OSTimeDly(111);//考虑删掉，任务执行时有延时

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
    GridData.griacolor=White;			//网格为白色
    GridData.Backcolor=Black;			//背景为黑色
    GridData.left_ycolor=Red;			//左侧纵轴对应的曲线为黄色

    /*	横线、竖线数量		*/
    GridData.xnumber=11;
    GridData.ynumber=11;

    /*	设置数据长度和保存两个Y轴数据的指针		*/
    GridData.Buff_Lenth = log_table_length;
    GridData.left_buff = AvData;

    /*	横轴的数字范围		*/
    GridData.xmax=1000000;
    GridData.xmin=100;

    /*	左边纵轴的数字范围		*/
    GridData.left_ymax=50;
    GridData.left_ymin=-5;
}



/**
* @brief  AD9851扫频测量一次
* @param
* @retval none
*/
void AD9851_Sweep(void)
{
    u32 i;
    int j;
//    Fault_Type fault_Type;
    LED1 = 0;
    //测试延时1ms，101点，一轮循环耗时600ms
    for(i=0; i<101; i++)
    {
        //Relay_AllOFF;
        Relay_Control(Relay_ALL,Relay_OFF);
        Relay_Control(Relay_OUT,Relay_ON);	//连接输出检测端
        dds.fre= log_table[i];
        dds.range = ADS9851_V_IN3;

        sendData(dds);
        if(i<20)
            delay_ms(100);
        else
            delay_ms(40);

        SignalData[i] = Get_Val(ADS1256ReadData(ADS1256_MUX_AIN0));
        SignalData[i] = Get_Val(ADS1256ReadData(ADS1256_MUX_AIN0));

        AvData[i] = 20 * log10f(SignalData[i] / ((ADS9851_V_IN3-0.01f)/2.828f)) - 6.0f;//匹配衰减6db

//	All_Gain = Vol_Out / ( Rin/(R_Real+Rin) * 0.01f / 2 / 1.414);   //增益

        OS_Num_Show(ShowX3,390     ,16,1,SignalData[i],"***%0.3f   ");
        OS_Num_Show(ShowX3,390+16  ,16,1,AvData[i],"###%0.3f   ");

//        if(i % 33)  //一个循环3次
//        {
//            fault_Type = Fault_Detect();
//            if(last_fault != fault_Type)//和上次状态不一样，更新参数，更新故障类型显示
//            {
//                Fault_Change_Flag = 1;
//                last_fault = fault_Type;
//                OS_String_Show(10,390+16*3,16,1,Fault_Type_str[fault_Type]);
//                return ;//剩下的频率暂时不扫描，优先测量显示参数
//            }
//            last_fault = fault_Type;

//        }

        if(i==100)
        {
            for(j=10; j>=0; j--)
            {
                AvData[j] = (AvData[j] + AvData[j+1] + AvData[j+2] + AvData[j+3] + AvData[j+4] + AvData[j+5] + AvData[j+6]) / 7;
            }
            UpdateGragh = 1;
        }

    }

    LED1 = 1;
}

#define RANGEIN(Vol,val,range)   (((Vol)>=((val) - (range))) && ((Vol)<=((val) + (range))))


//测量输入网络的交流电压
__inline float ADS1256_Measure(float fre, float range, u32 delay)
{
    dds.fre = fre;
    dds.range = range;
    sendData(dds);
    delay_ms(delay);
    Get_Val(ADS1256ReadData(ADS1256_MUX_AIN0));
    return Get_Val(ADS1256ReadData(ADS1256_MUX_AIN0));
}

float AD_ACNormal    = 0.162f;	//正常交流
float AD_AC50k_C1C2D = 0.131f;	//50k 100mv C1C2翻倍的情况
float AD_AC50k_C3O   = 0.146f;	//50k 100mv C3开路的情况
float AD_AC50k_C3D   = 0.0f;    	//50k 100mv C3翻倍的情况
float AD_AC15_C1D    = 1.304f;			//15hz 1V C1翻倍的情况
float AD_AC15_C2D    = 1.286f;			//15hz 1V C2翻倍的情况


float AD_DC_C1C2O    = 7.6f/4.0f;  			//是否检测C1 C2开路情况

float AD_DC_C1O      = 0.0f;    				//C1 开路情况
float AD_IAC_C1O      = 0.03536f; 		   //检测C1 / C2 条件 输入交流
float AD_OAC_C1O      = 0.03536f; 		   //检测C1 / C2 条件 输出交流
float AD_IAC_C2O      = 0.053f;				// C2 开路的情况 输入交流
float AD_OAC_C2O      = 0.053f;				// C2 开路的情况 输出交流

float AD_DC_R3S      = 0.030f;    		//R3短
float AD_AC_R1R4O    =  0.0f;         //R1 R4 断开条件

//float AD_AC_R        = 0.0f;          //检测

float AD_DC_R_FULL   = 11.98f/4.0f;    	//电阻故障中直流最大的情况 包括R1开 R2短 R3短 R4开
float AD_RS_R1O      = 0.084f;    		//R1开
float AD_RS_R4O      = 0.074f;    		//R4开
float AD_RS_R2S      = 0.000f;    		//R2短

float AD_DC_R1S   = 11.23f/4.0f;    	//R1短 RS应为0
float AD_DC_R2O   = 4.19f/4.0f;    		//R2开 RS应为0
float AD_DC_R3O   = 0.221f/4.0f;    	//R3开 RS应为5mv
float AD_DC_R4S   = 0.135f/4.0f;    	//R4短 RS应为0

Fault_Type Fault_Detect(void)
{
    //当前一轮测量时间21ms 最大应该在80ms
    float Vol,VolDC,VolAC,VolIN;

//    dds.fre = 1000;
//    dds.range = 0.1;
//    sendData(dds);
//    delay_ms(50);
//    VolIN = Get_Val(ADS1256ReadData(ADS1256_MUX_AIN0));

    //测量AC
//    Relay_Control(Relay_IN,Relay_OFF);  //断开输入检测端
//    Relay_Control(Relay_OUT,Relay_ON);	//连接输出检测端
//    delay_ms(50);
//    VolAC =  ADS1256_Measure(1000, 0.1, 1);
//    VolAC =  ADS1256_Measure(1000, 0.1, 1);




    //Relay_AllOFF;
    Relay_Control(Relay_ALL,Relay_OFF);
    Relay_Control(Relay_IN,Relay_ON); //连接输入检测端

    /***输出1k 100MV信号 **/
    dds.fre = 1000;
    dds.range = 0.01;
    sendData(dds);
    delay_ms(1000);
//		dds.fre = 50000;
//		dds.range = 0.01;
//		sendData(dds);
//		delay_ms(1000);
//		Relay_Control(Relay_OUT,Relay_OFF);	//输出关闭
//		Relay_Control(Relay_IN,Relay_ON);	//输入打开
//		dds.fre = 15;
//		dds.range = 1;
//		sendData(dds);
    //delay_ms(1000);


    //测量输出交流电压
    VolAC =  Get_Val(ADS1256ReadData(ADS1256_MUX_AIN0));//ADS1256_Measure(1000, 0.1, 1);
    VolAC =  Get_Val(ADS1256ReadData(ADS1256_MUX_AIN0));//ADS1256_Measure(1000, 0.1, 1);


    //测量输入交流电压
    Relay_Control(Relay_OUT,Relay_OFF);	//输出关闭
    Relay_Control(Relay_IN,Relay_ON);	//输入打开
    delay_ms(1000);
    VolIN =  Get_Val(ADS1256ReadData(ADS1256_MUX_AIN0));//ADS1256_Measure(1000, 0.1, 1);
    VolIN =  Get_Val(ADS1256ReadData(ADS1256_MUX_AIN0));//ADS1256_Measure(1000, 0.1, 1);

    //delay_ms(1000);


    if(RANGEIN(VolAC,AD_ACNormal,0.01f)) //交流正常
    {
        Relay_Control(Relay_IN,Relay_OFF);	//输入关闭
        Relay_Control(Relay_OUT,Relay_ON);	//输出打开
        delay_ms(1000);
        Vol =  ADS1256_Measure(50000, 0.01, 1000);

        if(RANGEIN(Vol,AD_AC50k_C1C2D,0.01f)) //继续测量C1翻倍 C2翻倍
        {

            Relay_Control(Relay_OUT,Relay_OFF);	//输出关闭
            Relay_Control(Relay_IN,Relay_ON);	//输入打开
            delay_ms(1000);
            Vol =  ADS1256_Measure(15, 1, 1000);

            if(RANGEIN(Vol,AD_AC15_C1D,0.005f))//C1翻倍
            {
                return Fault_Type_C1Double;
            }
            else if(RANGEIN(Vol,AD_AC15_C2D,0.005f))//C2翻倍
            {
                return Fault_Type_C2Double;
            }
            else //正常
            {
                return Fault_Type_Normal;
            }

        }
        else
        {
			Relay_Control(Relay_IN,Relay_OFF);	//输入关闭
			Relay_Control(Relay_OUT,Relay_ON);	//输出打开
            delay_ms(1000);
            Vol =  ADS1256_Measure(50000, 0.01, 1000);

            if(RANGEIN(Vol,AD_AC50k_C3O,0.01f))//C3开路
            {
                return Fault_Type_C3Open;
            }
            else
            {
                return Fault_Type_C3Double;
            }
        }

    }
    else    //交流不正常
    {
        /*关闭交流输出*/
        dds.output=0;
        sendData(dds);
		delay_ms(1000);
        VolDC =  Get_Val(ADS1256ReadData(ADS1256_MUX_AIN1));//检测直流
        VolDC =  Get_Val(ADS1256ReadData(ADS1256_MUX_AIN1));

        if(RANGEIN(VolDC,AD_DC_C1C2O,0.5f))//检测 C1 C2 开路 进行交流检测
        {
            dds.output=1;
			sendData(dds);
			delay_ms(1000);
            Vol = ADS1256_Measure(1000, 0.01,1000);

            if(RANGEIN(Vol,AD_IAC_C1O,0.005f)) //是否检测C1开路
            {

                //这个分支只通向C1开路情况，正常来说此时输出交流为0，测量的代码已经注释了，如有需要取消注释
//				dds.output=1;
//				dds.range = 0.01;//输出1k小幅度信号
//				dds.fre = 1000;
//				sendData(dds);
//
//				Relay_Control(Relay_IN,Relay_OFF);	//输入关闭
//				Relay_Control(Relay_OUT,Relay_ON);	//输出打开
//				delay_ms(1000);
				
//				Vol =  Get_Val(ADS1256ReadData(ADS1256_MUX_AIN0));//检测输出交流
//				Vol =  Get_Val(ADS1256ReadData(ADS1256_MUX_AIN0));
//
//				if(RANGEIN(Vol,AD_OAC_C1O,0.005f))
//				{
				
                return Fault_Type_C1Open;
				
//				}
//				else
//				{
//					return Fault_Type_Error;
//				}

            }
            else if(RANGEIN(Vol,AD_IAC_C2O,0.005f))  //是否检测C2开路
            {

                //这个分支只通向C2开路情况，正常来说此时输出交流为53mv，测量的代码已经注释了，如有需要取消注释
//				dds.output=1;
//				dds.range = 0.01;//输出1k小幅度信号
//				dds.fre = 1000;
//				sendData(dds);
//	
//				Relay_Control(Relay_IN,Relay_OFF);	//输入关闭
//				Relay_Control(Relay_OUT,Relay_ON);	//输出打开
//				delay_ms(1000);
				
//				Vol =  Get_Val(ADS1256ReadData(ADS1256_MUX_AIN0));//检测输出交流
//				Vol =  Get_Val(ADS1256ReadData(ADS1256_MUX_AIN0));
//
//				if(RANGEIN(Vol,AD_OAC_C2O,0.005f))
//				{
				
                return Fault_Type_C2Open;
				
//				}
//				else
//				{
//					return Fault_Type_Error;
//				}
            }

        }
        else   //进行电阻判断
        {
//			Vol =  Get_Val(ADS1256ReadData(ADS1256_MUX_AIN0));//检测交流
//			Vol =  Get_Val(ADS1256ReadData(ADS1256_MUX_AIN0));

            if(RANGEIN(VolIN,Vol_IN_Std,0.005f))
            {

                /*关闭交流输出*/
                dds.output=0;
                sendData(dds);
				delay_ms(1000);
                Vol =  Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN1));//检测直流
                Vol =  Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN1));

                if(RANGEIN(Vol,AD_DC_R3S,0.005f))
                {
                    return Fault_Type_R3Short;
                }

            }
            else if(VolIN > (Vol_IN_Std + 0.005f) )
            {
                dds.output=1;
                dds.fre = 1000;
                dds.range = 1.0;
                sendData(dds);

                Relay_Control(Relay_ALL,Relay_OFF);	//关闭全部
                Relay_Control(Relay_IN,Relay_ON);	//打开输入
                delay_ms(1000);

                Vol =  Get_Val(ADS1256ReadData(ADS1256_MUX_AIN0));//检测交流
                Vol =  Get_Val(ADS1256ReadData(ADS1256_MUX_AIN0));

                if(RANGEIN(Vol,AD_AC_R1R4O,0.005f))
                {
                    return Fault_Type_R1Open;
                }
                else
                {
                    return Fault_Type_R4Open;
                }

            }
            else//输出1k 小信号
            {
                return Fault_Type_Error;

            }


        }//检测直流

    }//最外层 交流不正常判断结束


    return Fault_Type_Error;

}


void AD9851_Sweep_once(int control, u32 index)
{
    static u32 i;

    if(control)
        i = index;

    dds.fre= log_table[i];
    sendData(dds);
    delay_ms(1);
    SignalData[i] = ADS1256ReadData(ADS1256_MUX_AIN0);

}


/*
 * Return:      void
 * Parameters:  void
 * Description: DDS数据初始化
 */
__inline void DDSDataInit2(void)
{
    /*	输出幅度 15mv	*/
    dds.range=0.015;

    /*	输出频率	100Hz	*/
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

    /*	默认打开输出		*/
    dds.output=1;

}

/*
 * Return:      void
 * Parameters:  void
 * Description: 校准三极管参数
 */
void Calib_Audion()
{

}
/*
 * Return:      void
 * Parameters:  void
 * Description: 测试输入、输出电阻，增益
 */
#define KEY_TEST 0 //按键测试
#define CircuitGain 4.0f//4.35f//4.51f
void task_1_3(void)
{
    float Vol_in,Vol_Out,Vol_Out_Load;

    LED1 = 0;

#if KEY_TEST == 1
    OS_String_Show(ShowX2-16,390+16*2,16,1,"  ");
    OS_String_Show(ShowX2-16,390     ,16,1,"->");
    while(1)
    {
#endif

        dds.fre=1000;
        dds.range=ADS9851_V_IN;		//大电压测输入
        sendData(dds);

        Relay_Control(Relay_IN,Relay_ON);	//连接输入检测端
        delay_ms(500);
        Vol_in=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN0|ADS1256_MUXN_AINCOM));  //输入端电压
        Vol_in=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN0|ADS1256_MUXN_AINCOM));  //输入端电压
        Vol_in += 0.003f;
        OS_Num_Show(180,390     ,16,1,Vol_in,"Vol_in:%0.3f   ");

#if KEY_TEST == 1
        if(Key_Now_Get(KEY3,KEY_MODE_SHORT))
            break;
    }
#endif


    dds.range=0.0115;
    sendData(dds);
#if KEY_TEST == 1
    OS_String_Show(ShowX2-16,390     ,16,1,"  ");
    OS_String_Show(ShowX2-16,390+16  ,16,1,"->");
    while(1)
    {
#endif

        Relay_Control(Relay_IN,Relay_OFF);	//断开输入检测端
        Relay_Control(Relay_OUT,Relay_ON);	//连接输出检测端
        delay_ms(500);
        Vol_Out=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN0|ADS1256_MUXN_AINCOM));  //测量放大电路输出端电压
        Vol_Out=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN0|ADS1256_MUXN_AINCOM));  //测量放大电路输出端电压
        Vol_Out += 0.003f;
        OS_Num_Show(180,390+16  ,16,1,Vol_Out,"Vol_Out:%0.3f   ");

#if KEY_TEST == 1
        if(Key_Now_Get(KEY3,KEY_MODE_SHORT))
            break;
    }
#endif

#if KEY_TEST == 1
    OS_String_Show(ShowX2-16,390+16  ,16,1,"  ");
    OS_String_Show(ShowX2-16,390+16*2,16,1,"->");
    while(1)
    {
#endif

        Relay_Control(Relay_LOAD,Relay_ON);	//连接负载
        delay_ms(500);
        Vol_Out_Load=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN0|ADS1256_MUXN_AINCOM));  //测量放大电路输出端电压 带4k负载
        Vol_Out_Load=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN0|ADS1256_MUXN_AINCOM));  //测量放大电路输出端电压 带4k负载
        Vol_Out_Load += 0.003f;
        OS_Num_Show(180,390+16*2,16,1,Vol_Out_Load,"Vol_Out_Load:%0.3f   ");

#if KEY_TEST == 1
        if(Key_Now_Get(KEY3,KEY_MODE_SHORT))
            break;
    }
#endif



    Rin=(R_Real * Vol_in/CircuitGain ) / (ADS9851_V_IN/2.828f/2.0f - Vol_in/CircuitGain );  //输入电阻

    Rout=(Vol_Out / Vol_Out_Load - 1.0f ) * 4000.0f;   //输出电阻

    //Rin = 3500;
    All_Gain = Vol_Out / ( Rin/(R_Real+Rin) * 0.01f / 2.0f / 1.414f);   //增益

    //Relay_AllOFF;
    Relay_Control(Relay_ALL,Relay_OFF);
    delay_ms(10);
    LED1 = 1;

}

void SelfCalibration(void)
{

}


