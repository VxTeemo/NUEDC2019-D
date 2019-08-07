#ifndef __PG2310_H
#define __PG2310_H

#include "stm32f4xx.h"
#include "sys.h"
#include "User_header.h"

void pg2310_Init(void);
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 


/*-----PGA2310硬件连接-----------------------------------------------------------------------------*/

#define PGA2310_CS    PFout(7)
#define PGA2310_SDI    PFout(5)
#define PGA2310_CLK     PFout(3)   
#define PGA2310_MUTE   PFout(1)


/*出口函数模型---------------------------------------------------------------------------------- */
void PGA2310_Init(void);
void PGA2310_Mute( FunctionalState NewState);
void DB_AMP(float L_InuptV,float L_OutV,float R_InuptV,float R_OutV);



#endif
