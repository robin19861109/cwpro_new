
#include "common.h"

#ifdef  _MYOS_

#endif

#ifdef _MYOS_
//nusΪҪ��ʱ��us��.		    								   
void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;						//LOAD��ֵ	    	 
	ticks=nus*(SystemCoreClock/1000000); 	//��Ҫ�Ľ�����	  		 
	tcnt=0;
	vTaskSuspendAll();									//��ֹOS���ȣ���ֹ���us��ʱ
	told=SysTick->VAL;        					//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;		//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;						//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
	xTaskResumeAll();										//�ָ�OS����									    
}
#else
void delay_us(uint32_t nus)
{		
}
void osDelay(uint32_t ms)
{		
}
#endif






