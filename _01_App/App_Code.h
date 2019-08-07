#ifndef __APP_CODE_H
#define __APP_CODE_H 

#include "User_header.h"


#define SendOut PGout(2)

#define SCIRXST  0 //  1表示接收  0表示发送

#if SCIRXST

extern u16 Get_Data;
extern u8  Get_Flag;

void Show_Data(void);

#else

extern u16 Send_Flag;
extern u8	Send_Data;
extern u8 dat_buf[14];

void DataSend(void);
void Code_Data(u16 data);
void Show_Send(void);
#endif

void Code_main(void);


#endif
