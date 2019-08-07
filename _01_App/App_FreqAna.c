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

#define MEASURE_LENGTH	700 			 //单片机显示测量点数

#define Get_Length      4650     	 //总测量地点数 ((35M-500M)/0.1MHZ) 

GRAPH_Struct 	GridData;							//网格结构体定义

float SignalData[Get_Length]= {0};   //采集的数据

float VMax_Fre;

void AF_Multi_Measuring(float signalbuff[],u16 length);


void FreqAna_main()
{

    GridData_Init();

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

            AF_Multi_Measuring(SignalData,1);

            Draw_Graph(&GridData,LEFTY);

        }
        if(Key_Now_Get(WK_UP,KEY_MODE_SHORT))
        {

            ESP8266_Client_Connect_Server(); //服务器连接


        }

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
    GridData.left_ymax=0.3;
    GridData.left_ymin=0;
}


/**
* @brief  频谱信号采集和处理
* @param
* @retval none
*/
void AF_Multi_Measuring(float signalbuff[],u16 length)
{
    u16 i,max_i,j=0,k;
    float max,max2=0;
    float Harm_Vol[2][1024];  //0存幅度  1存频率
    u8 ESP8266_Data[Get_Length]= {0};

    length=(Stop_Freq-Start_Freq)*1000/Step_Freq;  //计算扫频点总长度

    ADF4351_Sweep=0;

    /***    初略扫描       ***/

    for(i=0; i<length; i++)
    {

        ADF4351WriteFreq(Start_Freq+i*Step_Freq/1000.0f);

        delay_ms(10);

        OS_Num_Show(620,5,24,1,i*100/(length-1),"测量中...%.0f%%  ");

        signalbuff[i]=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN0|ADS1256_MUXN_AINCOM));

    }

    /***    寻找幅度最大值       ***/

    max=signalbuff[0];

    for(i=2; i<length; i++)
    {
        if(signalbuff[i] > max)
        {

            max=signalbuff[i];

            max_i=i;

        }
    }

    /***    寻找谐波幅度>最大值*0.02       ***/

    for(i=2; i<length; i++)
    {
        if(signalbuff[i] > max*0.02f && signalbuff[i] > 0.005f)
        {
            for(k=0; k<8; k++)      				//滤除旁瓣信号
            {
                if(max2<signalbuff[i+k])
                {

                    max2=signalbuff[i+k];

                    Harm_Vol[0][j]=max2;

                    Harm_Vol[1][j]=i+k-1;

                }
            }
            max2=0;
            j++;
            i+=8;
        }
    }

    /***     提出多余的10.7M差频        ***/
    for(i=0; i<j; i++)
    {

        ADF4351_Sweep=0;

        ADF4351WriteFreq(Start_Freq+Harm_Vol[1][i]*Step_Freq/1000.0f);

        delay_ms(1000);

        if(Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN0|ADS1256_MUXN_AINCOM))>0.005f)
        {

            ADF4351WriteFreq(Start_Freq+Harm_Vol[1][i]*Step_Freq/1000.0f-21.4f); //减去10.7*2Mhz

            delay_ms(1000);

            if(Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN0|ADS1256_MUXN_AINCOM))>0.005f) //重新进行幅度测量 （扫频时幅度控制有误差）
            {

                Harm_Vol[0][i]=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN0|ADS1256_MUXN_AINCOM));

            }
            else
            {
                for(k=0; k<6; k++)      				 //剔除旁瓣  +/- 6.0*步进频率
                {

                    signalbuff[(u16)Harm_Vol[1][i]+k]=0.004f;

                }
                for(k=0; k<6; k++)
                {

                    signalbuff[(u16)Harm_Vol[1][i]-k]=0.004f;

                }

                Harm_Vol[1][i]=0;
            }
        }
        else
        {
            for(k=0; k<6; k++)      				//剔除旁瓣  +/- 6.0*步进频率
            {

                signalbuff[(u16)Harm_Vol[1][i]+k]=0.004f;

            }
            for(k=0; k<6; k++)
            {

                signalbuff[(u16)Harm_Vol[1][i]-k]=0.004f;

            }

            Harm_Vol[1][i]=0;
        }
    }

    /***    频率、幅度显示       ***/
    k=0;

    for(i=0; i<j; i++)
    {
        if(Harm_Vol[1][i]!=0)
        {

            OS_Num_Show(5+115*k,450,24,1,Start_Freq+Harm_Vol[1][i]*Step_Freq/1000.0f-10.7,"%.2fMhz");

            OS_Num_Show(60+115*k++,80,24,1,Harm_Vol[0][i]/10.0f,"%.3fV");

            OS_Num_Show(5,5,24,1,k,"%.0f个频谱  ");

        }
    }

    /***    处理零输入误差      ***/

    for(i=0; i<length; i++)
    {

        signalbuff[i]=0;

    }
    for(i=0; i<j; i++)
    {
        if(Harm_Vol[1][i]!=0)
        {

            signalbuff[(u16)Harm_Vol[1][i]+1]=Harm_Vol[0][i];

        }
    }

    /***	 调整波形图规格	 ***/

    max2=0;

    for(i=0; i<j; i++)
    {
        if(max2<Harm_Vol[0][i])
        {

            max2=Harm_Vol[0][i];

        }
    }

    GridData.left_ymax=max2*1.50f;

    GridData.xmax=Start_Freq+700*Step_Freq/1000.f-10.7;

    GridData.xmin=Start_Freq-10.7;

    ADF4351_Sweep=0;

    OS_String_Show(620,5,24,1,"开始传输    ");

    /***	 转换数据准备传给上位机	 ***/
    for(i=0; i<length; i++)
    {

        ESP8266_Data[i]=(u8)(signalbuff[i]*500);

    }
    /***	 发送数据	 ***/
    sendData(&ESP8266_Data[0],2000,1);

    delay_ms(1000);

    sendData(&ESP8266_Data[2000],2000,2);

    delay_ms(1000);

    sendData(&ESP8266_Data[4000],650,3);

    delay_ms(1000);

    OS_String_Show(620,5,24,1,"测量完成    ");

}



