#include "App_ADF4351.h"

u8 SetChoose=0;			   							//设置选择

u8 ADF4351_Sweep=0;   						 //扫频标志位

u16 Sweep_Time=10;     						//扫频时间

u16 Wave_Vol=50;     					  	//输出幅度

double Data=0;			    				//参数设置

double Out_Freq=50;    				 //输出频率

double Step_Freq=100;   			//步进频率

double Start_Freq=50,Stop_Freq=120; 	//扫频起始、终止频率

double Divisor=30.0f;


void ADF4351_main()
{


    TouchKey_Draw();	//画小键盘

    Start_Meum();    //画界面

    PID_Init();  		//PID初始化 (控制电压稳定)

    while(1)
    {


        /***      数据处理       ***/
        Data=TouchKey_Scan();

        Touch_Handle(Data);

        OSTimeDly(33);

    }
}



/**----------------------------------------------------------------------------
* @FunctionName  : Touch_Handle()
* @Description   : None
* @Data          : 2019/7/26
* @Explain       : 屏幕触摸处理
------------------------------------------------------------------------------*/
void Touch_Handle(double data)
{

    /****          频率设置选择               ****/
    if(Touch_Judge(200,25,450,70) == TOUCH_VALID_FULL)  //频率设置
    {

        SetChoose=0;

        Start_Meum();

    }
    else if(Touch_Judge(200,85,450,130) == TOUCH_VALID_FULL)
    {

        SetChoose=1;

        Start_Meum();

    }
    else if(Touch_Judge(200,145,450,190) == TOUCH_VALID_FULL)
    {

        SetChoose=2;

        Start_Meum();

    }
    else if(Touch_Judge(200,205,450,250) == TOUCH_VALID_FULL)
    {

        SetChoose=3;

        Start_Meum();

    }
    else if(Touch_Judge(200,265,450,310) == TOUCH_VALID_FULL)
    {

        SetChoose=4;

        Start_Meum();

    }
    else if(Touch_Judge(200,325,450,370) == TOUCH_VALID_FULL)
    {

        SetChoose=5;

        Start_Meum();

    }

    /****           按键处理                ****/
    if(Touch_Judge(200,410,320,460) == TOUCH_VALID_FULL)  //扫频开关
    {

        ADF4351_Sweep=1;

        OS_Rect_Draw(200,410,320,460,1,Red);

        OS_Rect_Draw(30,410,150,460,1,BLACK);

    }
    else	if(Touch_Judge(30,410,150,460) == TOUCH_VALID_FULL)
    {

        ADF4351_Sweep=0;

        OS_Rect_Draw(200,410,320,460,1,BLACK);

        OS_Rect_Draw(30,410,150,460,1,RED);

    }
    else if(Touch_Judge(375,410,495,460) == TOUCH_VALID_FULL)  //加减
    {

        Out_Freq=Out_Freq-Step_Freq/1000.0;


        OS_Num_Show(210,30,32,1,Out_Freq,"%.3fMhz   ");

        ADF4351WriteFreq(Out_Freq+(Out_Freq/100)/1000.0f);  //匹配晶振误差

    }
    else if(Touch_Judge(550,410,670,460) == TOUCH_VALID_FULL)
    {

        Out_Freq=Out_Freq+Step_Freq/1000.0;

        OS_Num_Show(210,30,32,1,Out_Freq,"%.3fMhz   ");

        ADF4351WriteFreq(Out_Freq+(Out_Freq/100)/1000.0f); //匹配晶振误差

    }

    if(data !=0 )//参数值改变
    {
        switch(SetChoose)
        {
        case 0:
            Out_Freq=data;
            ADF4351WriteFreq(Out_Freq+(u16)(Out_Freq/100.0f)/1000.0f);
            break;  //匹配晶振误差 Divisor=-0.0000009599f*pow(Out_Freq,3)+0.0006239*pow(Out_Freq,2)-0.118*Out_Freq+34.61;

        case 1:
            Step_Freq=data;
            break;

        case 2:
            Start_Freq=data;
            break;

        case 3:
            Stop_Freq=data;
            break;

        case 4:
            Sweep_Time=data;
            break;

        case 5:
            Wave_Vol=data;
            if(Wave_Vol>70)Wave_Vol+=1;
            break;
        }
        Start_Meum();


    }
    switch(SetChoose)
    {
    case 0:
        OS_Rect_Draw(200,25,450,70,1,Red);
        break;

    case 1:
        OS_Rect_Draw(200,85,450,130,1,Red);
        break;

    case 2:
        OS_Rect_Draw(200,145,450,190,1,Red);
        break;

    case 3:
        OS_Rect_Draw(200,205,450,250,1,Red);
        break;

    case 4:
        OS_Rect_Draw(200,265,450,310,1,Red);
        break;

    case 5:
        OS_Rect_Draw(200,325,450,370,1,Red);
        break;
    }

}

/**----------------------------------------------------------------------------
* @FunctionName  : Start_Meum()
* @Description   : None
* @Data          : 2019/7/26
* @Explain       : 菜单显示
------------------------------------------------------------------------------*/

void Start_Meum(void)
{

    OS_String_Show(30,30,32,0,"设置频率:");
    OS_Rect_Draw(200,25,450,70,1,Black);
    OS_Num_Show(210,30,32,1,Out_Freq,"%.3fMhz   ");

    OS_String_Show(30,90,32,0,"步进频率:");
    OS_Rect_Draw(200,85,450,130,1,Black);
    OS_Num_Show(210,90,32,1,Step_Freq,"%.0fKhz   ");

    OS_String_Show(30,150,32,0,"起始频率:");
    OS_Rect_Draw(200,145,450,190,1,Black);
    OS_Num_Show(210,150,32,1,Start_Freq,"%.3fMhz   ");

    OS_String_Show(30,210,32,0,"终止频率:");
    OS_Rect_Draw(200,205,450,250,1,Black);
    OS_Num_Show(210,210,32,1,Stop_Freq,"%.3fMhz   ");

    OS_String_Show(30,270,32,0,"扫频时间:");
    OS_Rect_Draw(200,265,450,310,1,Black);
    OS_Num_Show(210,270,32,1,Sweep_Time,"%.1fS   ");

    OS_String_Show(30,330,32,0,"输出幅度:");
    OS_Rect_Draw(200,325,450,370,1,Black);
    OS_Num_Show(210,330,32,1,Wave_Vol,"%.1fMV   ");


    OS_String_Show(35,420,32,0," Normal");
    OS_Rect_Draw(30,410,150,460,1,Black);

    OS_String_Show(210,420,32,0," Sweep");
    OS_Rect_Draw(200,410,320,460,1,Black);

    OS_String_Show(385,420,32,0," 减");
    OS_Rect_Draw(375,410,495,460,1,Black);

    OS_String_Show(560,420,32,0," 加");
    OS_Rect_Draw(550,410,670,460,1,Black);

}

