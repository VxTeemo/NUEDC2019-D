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

char Fault_Type_str[][10]=
{
    "正常",
    "未知异常",
    "R1开路",
    "R1短路",
    "R2开路",
    "R2短路",
    "R3开路",
    "R3短路",
    "R4开路",
    "R4短路",
    "C1开路",
    "C2开路",
    "C3开路",
    "C1翻倍",
    "C2翻倍",
    "C3翻倍"
};


#define UNKOWN_VAL0 1.2f
#define UNKOWN_VAL1 1.3f

float AD_ACNormal    = 1.13f;	//正常交流
float AD_AC50k_C1C2D = 1.004f;	//50k 100mv C1C2翻倍的情况
float AD_AC50k_C3O   = 1.096f;	//50k 100mv C3开路的情况
float AD_AC50k_C3D   = 0.0f;    	//50k 100mv C3翻倍的情况
float AD_AC15_C1D    = UNKOWN_VAL0;			//15hz 1V C1翻倍的情况 目前还没有值
float AD_AC15_C2D    = UNKOWN_VAL1;			//15hz 1V C2翻倍的情况 目前还没有值
float AD_AC_C2O      = 0.053f;				// C2开路的情况
float AD_AC_C1O_R    = 0.0f;    			//剩下的情况 C1开路 电阻故障 1k0.1v交流为0
float AD_DC_C1O      = 7.6f/4.0f;  			//C1开路或检测错误的情况
float AD_AC_C1O      = 0.03536f; 		//C1开路的情况
float AD_DC_R_FULL   = 11.98f/4.0f;    	//电阻故障中直流最大的情况 包括R1开 R2短 R3短 R4开
float AD_RS_R1O      = 0.084f;    		//R1开
float AD_RS_R4O      = 0.074f;    		//R4开
float AD_RS_R3S      = 0.030f;    		//R3短
float AD_RS_R2S      = 0.000f;    		//R2短
float AD_DC_R1S      = 11.23f/4.0f;    	//R1短 RS应为0
float AD_DC_R2O      = 4.19f/4.0f;    	//R2开 RS应为0
float AD_DC_R3O      = 0.221f/4.0f;    	//R3开 RS应为5mv
float AD_DC_R4S      = 0.135f/4.0f;    	//R4短 RS应为0


//#define MEASURE_LENGTH	200 	//单片机显示测量点数
//#define Get_Length      201    //总测量地点数 ((10^2-10^6)对数步进)
#define R_Real    5000.0f       //固定电阻大小
#define ADS9851_V   0.01f       //9851输出幅度

GRAPH_Struct 	GridData;		//网格结构体定义
const int log_table_length = sizeof(log_table)/sizeof(float);//101
float SignalData[log_table_length]= {0};   //采集的原始数据
float AvData[log_table_length]= {0};			//转换成对数

float VMax_Fre,Rin,Rout,All_Gain;
int last_fault = Fault_Type_Normal;
u8 Fault_Change_Flag = 1;//上电检测一次
u8 UpdateGragh = 0;

void DDSDataInit(void);
void task_1_3(void);
Fault_Type Fault_Detect(void);
float ADS1256_Measure(float fre, float range, u32 delay);

void FreqAna_main()
{
    GridData_Init();

    DDSDataInit();
    //sendData(dds);

    Draw_Grid(GridData);
    Draw_Graph(&GridData,LEFTY);

    while(1)
    {

//        if(Fault_Change_Flag)
//        {
            task_1_3();
            OS_Num_Show(10,390     ,16,1,Rin,"Rin:%0.3f   ");
            OS_Num_Show(10,390+16  ,16,1,Rout,"Rout:%0.3f   ");
            OS_Num_Show(10,390+16*2,16,1,All_Gain,"Gain:%0.3f   ");
//            Fault_Change_Flag = 0;
//        }

//			LED1 = 0;
//			Fault_Detect();
//			LED1 = 1;


//        Draw_Grid(GridData);
//        Show_Label(GridData,LEFTY);


//        if(UpdateGragh)
//        {
//            Draw_Graph(&GridData,LEFTY);
//            UpdateGragh = 0;
//        }




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
    GridData.griacolor=Black;			//网格为白色
    GridData.Backcolor=White;			//背景为黑色
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
    GridData.left_ymax=44;
    GridData.left_ymin=0;
}



/**
* @brief  AD9851扫频测量一次
* @param
* @retval none
*/
void AD9851_Sweep(void)
{
    u32 i;
    Fault_Type fault_Type;
    LED1 = 0;
    //测试延时1ms，101点，一轮循环耗时600ms
    for(i=0; i<101; i++)
    {
        dds.fre= log_table[i];
        dds.range = ADS9851_V;
        sendData(dds);
        delay_ms(1);
        SignalData[i] = ADS1256ReadData(ADS1256_MUXP_AIN1 | ADS1256_MUXN_AINCOM);
        AvData[i] = 20 * log10(SignalData[i] / ADS9851_V);

        if(i % 33)  //一个循环3次
        {
            fault_Type = Fault_Detect();
            if(last_fault != fault_Type)//和上次状态不一样，更新参数，更新故障类型显示
            {
                Fault_Change_Flag = 1;
                last_fault = fault_Type;
                OS_String_Show(10,390+16*3,16,1,Fault_Type_str[fault_Type]);
                return ;//剩下的频率暂时不扫描，优先测量显示参数
            }
            last_fault = fault_Type;

        }

        if(i==100)
            UpdateGragh = 1;

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
    return Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN1 | ADS1256_MUXN_AINCOM));
}


Fault_Type Fault_Detect(void)
{
    //当前一轮测量时间21ms 最大应该在80ms
    float Vol,VolDC,VolAC,VolRS;

    //RS是被测网络输入前的电阻，VolRS是电阻后的电压

    Relay_Control(0,0);	//J3继电器切换低电平 测量网络输入端
    Relay_Control(1,0);	//J2继电器切换低电平 无负载

    dds.fre = 1000;
    dds.range = 0.1;
    sendData(dds);
    delay_ms(1);
    VolRS = Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN3 | ADS1256_MUXN_AINCOM));

    //测量AC
    Relay_Control(0,1);	//J3继电器切换高电平 测量网络输出 AC
    Relay_Control(1,0);	//J2继电器切换低电平 无负载
    VolAC =  ADS1256_Measure(1000, 0.1, 1);

    if(RANGEIN(VolAC,AD_ACNormal,0.05f)) //交流正常
    {
        Vol =  ADS1256_Measure(50000, 0.1, 1);

        if(RANGEIN(Vol,AD_AC50k_C1C2D,0.05f)) //继续测量C1翻倍 C2翻倍
        {
            Vol =  ADS1256_Measure(15, 1, 70);

            if(RANGEIN(Vol,AD_AC15_C1D,0.05f))//C1翻倍
            {
                return Fault_Type_C1Double;
            }
            else if(RANGEIN(Vol,AD_AC15_C2D,0.05f))//C2翻倍
            {
                return Fault_Type_C2Double;
            }
            else //正常
            {
                return Fault_Type_Normal;
            }

        }
        else if(RANGEIN(Vol,AD_AC50k_C3O,0.05f))//C3开路
        {
            return Fault_Type_C3Open;
        }
        else if(RANGEIN(Vol,AD_AC50k_C3D,0.05f))//C3翻倍
        {
            return Fault_Type_C3Double;
        }
        else
        {
            return Fault_Type_Error;
        }

    }
    else if(RANGEIN(VolAC,AD_AC_C2O,0.02f))//C2开路
    {
        return Fault_Type_C2Open;
    }
    else if(RANGEIN(VolAC,AD_AC_C1O_R,0.02f))//几乎为0
    {

        dds.output = 0;
        sendData(dds);
        dds.output = 1; 			 //9851下次控制DDS时开关才能生效
        delay_ms(10);
        //关闭ADC 测量直流
        VolDC = Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM));

        if(RANGEIN(VolDC,AD_DC_C1O,0.02f)) //直流在7.6V
        {
            if(RANGEIN(VolAC,AD_AC_C1O,0.02f))
            {
                return Fault_Type_C1Open;
            }
            else
            {
                return Fault_Type_Error;
            }

        }
        else //综合分析电阻故障
        {
            //上面已经测量出了1k100mv的直流和交流值
            if(RANGEIN(VolDC,AD_DC_R_FULL,0.02f))//直流在11.9V附近
            {
                //通过判断 VolRS
                if(RANGEIN(VolRS,AD_RS_R1O,0.004f))
                {
                    return Fault_Type_R1Open;
                }
                else if(RANGEIN(VolRS,AD_RS_R4O,0.004f))
                {
                    return Fault_Type_R1Short;
                }
                else if(RANGEIN(VolRS,AD_RS_R3S,0.004f))
                {
                    return Fault_Type_R3Short;
                }
                else if(RANGEIN(VolRS,AD_RS_R2S,0.002f))
                {
                    return Fault_Type_R2Short;
                }
                else //怪怪的值
                {
                    return Fault_Type_Error;
                }

            }
            else if(RANGEIN(VolDC,AD_DC_R1S,0.05f)) //按理说RS之间的电压为0
            {
                return Fault_Type_R1Short;
            }
            else if(RANGEIN(VolDC,AD_DC_R2O,0.05f)) //按理说RS之间的电压为0
            {
                return Fault_Type_R2Open;
            }
            else if(RANGEIN(VolDC,AD_DC_R3O,0.05f)) //按理说RS之间的电压为5mv
            {
                return Fault_Type_R3Open;
            }
            else if(RANGEIN(VolDC,AD_DC_R4S,0.05f)) //按理说RS之间的电压为0
            {
                return Fault_Type_R4Short;
            }
            else
            {
                return Fault_Type_Error;
            }

        } //综合分析电阻故障


    }//最外层并列if

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
    SignalData[i] = ADS1256ReadData(ADS1256_MUXP_AIN1 | ADS1256_MUXN_AINCOM);

}


/*
 * Return:      void
 * Parameters:  void
 * Description: DDS数据初始化
 */
__inline void DDSDataInit(void)
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
void task_1_3(void)
{
    float Vol0,Vol1,Vol2;

    LED1 = 0;
    //当前测量一轮44ms
    dds.fre=1000;

    dds.range=0.01;

    sendData(dds);


    Relay_Control(1,0);	//J3继电器切换高电平
    delay_ms(100);
    Vol0=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN1|ADS1256_MUXN_AINCOM));  //测量标准电阻输出端电压
    delay_ms(100);
    Vol0=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN1|ADS1256_MUXN_AINCOM));  //测量标准电阻输出端电压
    OS_Num_Show(180,370     ,16,1,Vol0,"Vol0:%0.3f   ");

//314mv
//1.68V

    Relay_Control(1,1);	//J3继电器切换低电平
    Relay_Control(4,0);	//J2继电器切换低电平  先测无负载
    delay_ms(100);
    Vol1=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN1|ADS1256_MUXN_AINCOM));  //测量放大电路输出端电压
    delay_ms(100);
    Vol1=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN1|ADS1256_MUXN_AINCOM));  //测量放大电路输出端电压
    OS_Num_Show(180,370+16  ,16,1,Vol1,"Vol1:%0.3f   ");
    //不用关J4
    //Relay_Control(4,1);	//J2继电器切换高电平 输出带4k负载



    Relay_Control(3,0);	//J2继电器切换高电平 输出带4k负载
    delay_ms(100);
    Vol2=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN1|ADS1256_MUXN_AINCOM));  //测量放大电路输出端电压
    delay_ms(100);
    Vol2=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN1|ADS1256_MUXN_AINCOM));  //测量放大电路输出端电压
    OS_Num_Show(180,370+16*2,16,1,Vol2,"Vol2:%0.3f   ");



    All_Gain=Vol1/Vol0;     //增益

    Rin=(R_Real * Vol0 ) / (ADS9851_V - Vol0 );  //输入电阻

    Rout=(Vol1 / Vol2 - 1.0f )* 4000 ;   //输出电阻




    GPIO_SetBits(GPIOG, GPIO_Pin_1 | GPIO_Pin_3| GPIO_Pin_5| GPIO_Pin_7);
    delay_ms(10);
    LED1 = 1;

}

void SelfCalibration(void)
{

}


