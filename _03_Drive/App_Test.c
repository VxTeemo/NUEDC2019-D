#include "App_Test.h"



void Test_main()
{
	  float Test_Vol[8];
	  u16 i;
	
	dds.fre= 200;
    dds.range = 1.0f;
	dds.output = 1;
	sendData(dds);
	
	  while(1)
		{
			
			  for(i=0;i<2;i++)
						Test_Vol[0]=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN0|ADS1256_MUXN_AINCOM));
			  delay_ms(100);
			  for(i=0;i<2;i++)
						Test_Vol[1]=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN1|ADS1256_MUXN_AINCOM));
			  delay_ms(100);
//			  for(i=0;i<2;i++)
//						Test_Vol[2]=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN2|ADS1256_MUXN_AINCOM));
//			  delay_ms(100);
//			  for(i=0;i<2;i++)
//						Test_Vol[3]=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN3|ADS1256_MUXN_AINCOM));
//			  delay_ms(100);
//			  for(i=0;i<2;i++)
//						Test_Vol[4]=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN4|ADS1256_MUXN_AINCOM));
//			  delay_ms(100);
//			  for(i=0;i<2;i++)
//						Test_Vol[5]=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN5|ADS1256_MUXN_AINCOM));
//			  delay_ms(100);
//			  for(i=0;i<2;i++)
//						Test_Vol[6]=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN6|ADS1256_MUXN_AINCOM));
//			  delay_ms(100);
//			  for(i=0;i<2;i++)
//						Test_Vol[7]=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN7|ADS1256_MUXN_AINCOM));
//			  delay_ms(100);
			
			  for(i=0;i<8;i++)
			  {
					
					OS_Num_Show(100,50+i*50,24,1,Test_Vol[i],"%.3fV  ");
					
				}
				
				OSTimeDly(55);
		}
}

