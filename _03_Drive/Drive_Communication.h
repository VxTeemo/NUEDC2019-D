#ifndef DRIVE_COMMUNICATION_H
#define DRIVE_COMMUNICATION_H

#include "stdint.h"
#include "stm32f4xx.h"
#include "Drive_ADC.h"
#include "Drive_Timer.h"
//#include "App_NIT.h"

#ifndef True
#define True 1
#endif

#ifndef False
#define False  0
#endif

/* 通信协议指令 */
#define    NORMAL         0x0001 //普通模式
#define    SWEEP          0x0002 //扫频模式

#define    RX_LENGTH              48
#define    TX_LENGTH              48

typedef struct communicationDataStruct
{
    u8 rxBuffer[RX_LENGTH];
    u8 txBuffer[TX_LENGTH];
    u8 flag;					  //接收置位
} communicationDataStruct;

typedef struct DDSDataStruct
{
    u8  mode;      //输出模式
    u8  output;    //输出开关

    float range;   //幅度
    u32 fre;       //普通模式的频率

    u32 step;      //步进
    u32 step_time; //步进时间(us)
    u32 fre_start; //扫频模式的起始频率
    u32 fre_stop;	 //扫频模式的截止频率

} DDSDataStruct;


void sendData(DDSDataStruct data);
void Refresh(DDSDataStruct data,u8 choose);

extern DDSDataStruct dds;
extern communicationDataStruct communicationData;

#endif



/******************************************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/











