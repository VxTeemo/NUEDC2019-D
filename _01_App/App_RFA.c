#include "App_RFA.h"


void RFA_main(void)
{
	  float result=0;
	
	  TouchKey_Draw();
	
		while(1)
		{
      result=TouchKey_Scan();
			
			if(result!=0)
				OS_Num_Show(100,100,32,1,result,"set_v:%.3f");
			
			OSTimeDly(133);
		}
}
