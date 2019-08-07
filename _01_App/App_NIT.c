#include "App_NIT.h"

u8 SetChoose=2;//设置目标选择
void DDSDataInit(void);
void NIT_main(void)
{
    u8 key;
    float result;

    DDSDataInit();
    while(1)
    {
        /*	按键扫描		*/
        key=KEY_Scan(0);
        Key_Handle(key);

        /*	扫描小键盘		*/
        result=TouchKey_Scan();

        /*	触摸屏处理		*/
        Touch_Handle(result);

        OSTimeDly(30);
    }
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
    dds.fre=1000;

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
    dds.output=0;

    Refresh(dds,SetChoose);
}


/*
 * Return:      void
 * Parameters:  data:小键盘输入值
 * Description: 触摸屏处理
 */
void Touch_Handle(float data)
{
    if(data !=0 )//小键盘有输入，改变对应参数值
    {
        switch(SetChoose)
        {
        case 1:
            if(data <= 3.3f) dds.range=data;
            break;

        case 2:
            dds.fre=data;
            break;

        case 3:
            if(data < dds.fre_stop - dds.fre_start) dds.step=data;
            break;

        case 4:
            if(dds.step_time > 0) dds.step_time=data;
            break;

        case 5:
            if(data < dds.fre_stop)  dds.fre_start=data;
            break;

        case 6:
            if(data > dds.fre_start) dds.fre_stop=data;
            break;

        default :
            break;
        }

        sendData(dds);
        Refresh(dds,SetChoose);
    }

    if(Touch_Judge(130,25,250,70) == TOUCH_VALID_FULL)
    {
        dds.mode=NORMAL;
        sendData(dds);
    }
    else if(Touch_Judge(260,25,370,70) == TOUCH_VALID_FULL)
    {
        dds.mode=SWEEP;
        sendData(dds);
    }
    else if(Touch_Judge(575,375,720,440) == TOUCH_VALID_FULL)
    {
        dds.output^=1;
        sendData(dds);
    }
    else if(Touch_Judge(130,85,290,130) == TOUCH_VALID_FULL)
        SetChoose=1;
    else if(Touch_Judge(130,145,290,190) == TOUCH_VALID_FULL)
        SetChoose=2;
    else if(Touch_Judge(130,205,290,250) == TOUCH_VALID_FULL)
        SetChoose=3;
    else if(Touch_Judge(190,265,350,310) == TOUCH_VALID_FULL)
        SetChoose=4;
    else if(Touch_Judge(190,325,350,370) == TOUCH_VALID_FULL)
        SetChoose=5;
    else if(Touch_Judge(190,385,350,430) == TOUCH_VALID_FULL)
        SetChoose=6;
    else
        return ;

    Refresh(dds,SetChoose);
}

/*
 * Return:      void
 * Parameters:  key:按键返回值
 * Description: 按键处理
 */
void Key_Handle(u8 key)
{

    if(key && (dds.mode == NORMAL))
    {
        switch(key)
        {
        case 2:
            if(dds.fre -dds.step >0)   dds.fre -= dds.step;
            break;

        case 4:
            dds.fre += dds.step;
            break;

        case 1:
            break;
        case 3:
            break;
        default:
            break;
        }

        sendData(dds);
        Refresh(dds,SetChoose);
    }

}




