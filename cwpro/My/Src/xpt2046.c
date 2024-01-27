#include "xpt2046.h" 
#include "my_lcd.h"
#include "stdlib.h"
 


/*********************************************************************************
*********************�������� STM32F407Ӧ�ÿ�����(�����)*************************
**********************************************************************************
* �ļ�����: xpt2046.c                                                            *
* �ļ�������������������������                                                   *
* �������ڣ�2018.08.30                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    ����                                                                     * 
**********************************************************************************
*********************************************************************************/	 
	  // DRV_TOUCH_XPT2046:

uint16_t Xdown=0; 		 
uint16_t Ydown=0;	     //�����������¾ͷ��صĵ�����ֵ
uint16_t Xup=0;
uint16_t Yup=0; 			   //������������֮��̧�𷵻صĵ�����ֵ 

/**************Ĭ��Ϊ��������***************/
uint8_t  CMD_RDX=0XD0;
uint8_t  CMD_RDY=0X90;

float xFactor=0.06671114;	 //����У׼���� 
float yFactor=0.09117551;
short xOffset=-11;       //xOffset��yOffset ���Ը���ʵ����ͷ�ĵ������ʵ���������׼
short yOffset=-18;
/*******************************************/

/**************����Ϊ��ͨIOģ��SPIͨ��XPT2046***************/
/****************************************************************************
* ��    ��: void SPI_Write_Byte(u8 num) 
* ��    �ܣ�SPIд����,������ICд��1byte���� 
* ��ڲ�����num:Ҫд�������
* ���ز�������
* ˵    ����       
****************************************************************************/
void SPI_Write_Byte(uint8_t num)    
{  
	uint8_t count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN(GPIO_PIN_SET);  
		else TDIN(GPIO_PIN_RESET);   
		num<<=1;    
		TCLK(GPIO_PIN_RESET); 
		delay_us(1);
		TCLK(GPIO_PIN_SET);		 //��������Ч	        
	}		 			    
} 	

/****************************************************************************
* ��    ��: u16 SPI_Read_AD(u8 CMD)
* ��    �ܣ�SPI������ ,�Ӵ�����IC��ȡadcֵ
* ��ڲ�����CMD:ָ��
* ���ز���: ����������	   
* ˵    ����       
****************************************************************************/  
uint16_t SPI_Read_AD(uint8_t CMD)	  
{ 	 
	uint8_t count=0; 	  
	uint16_t Num=0; 
	TCLK(GPIO_PIN_RESET);		//������ʱ�� 	 
	TDIN(GPIO_PIN_RESET); 	//����������
	TCS(GPIO_PIN_RESET); 		//ѡ�д�����IC 2046
	SPI_Write_Byte(CMD);//����������
	delay_us(6); 
	TCLK(GPIO_PIN_RESET); 	     	    
	delay_us(1);    	   
	TCLK(GPIO_PIN_SET);		 
	delay_us(1);    
	TCLK(GPIO_PIN_RESET); 	     	    
	for(count=0;count<16;count++)//����16λ����,ֻ�и�12λ��Ч 
	{ 				  
		Num<<=1; 	 
		TCLK(GPIO_PIN_RESET);	 //�½�����Ч  	    	   
		delay_us(1);    
 		TCLK(GPIO_PIN_SET);
 		if(DOUT)Num++; 		 
	}  	
	Num>>=4;   	//��12λ����Ч
	TCS(GPIO_PIN_SET);		  //�ͷ�Ƭѡ	 
	return(Num);   
}
/**************����Ϊ��ͨIOģ��SPIͨ��XPT2046***************/


/****************************************************************************
* ��    ��: u16 RTouch_Read_XorY(u8 xy)
* ��    �ܣ���ȡһ������ֵ(x����y)
* ��ڲ�����xoy:ָ�CMD_RDX/CMD_RDY
* ���ز���: ����������	   
* ˵    ����������ȡ5������,����Щ������������, Ȼ��ȥ����ͺ���1����,ȡƽ��ֵ       
****************************************************************************/ 
uint16_t RTouch_Read_XorY(uint8_t xoy)
{
	uint16_t i, j;
	uint16_t buf[5];
	uint16_t sum=0;
	uint16_t temp;
	
	for(i=0;i<5;i++)buf[i] = SPI_Read_AD(xoy);		 		    
	for(i=0;i<5-1; i++)  //����
	{
		for(j=i+1;j<5;j++)
		{
			if(buf[i]>buf[j])//���� 
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=1;i<5-1;i++)sum+=buf[i];
	temp=sum/(5-2*1);
	return temp;   
} 

/****************************************************************************
* ��    ��: u8 RTouch_Read_XY(u16 *x,u16 *y)
* ��    �ܣ���ȡx,y����
* ��ڲ�����x,y:��ȡ��������ֵ
* ���ز���: ����ֵ:0,ʧ��;1,�ɹ�   
* ˵    ������Сֵ��������50.       
****************************************************************************/ 
uint8_t RTouch_Read_XY(uint16_t *x,uint16_t *y)
{
	uint16_t xtemp,ytemp;			 	 		  
	xtemp=RTouch_Read_XorY(CMD_RDX);
	ytemp=RTouch_Read_XorY(CMD_RDY);	  												   
	if(xtemp<50||ytemp<50)return 0;//����ʧ��
	*x=xtemp;
	*y=ytemp;
	return 1;//�����ɹ�
}

/****************************************************************************
* ��    ��: u8 RTouch_Read_XY2(u16 *x,u16 *y)
* ��    �ܣ�����2�ζ�ȡ������IC
* ��ڲ�����x,y:��ȡ��������ֵ
* ���ز���: ����ֵ:0,ʧ��;1,�ɹ�   
* ˵    ����        
****************************************************************************/
uint8_t RTouch_Read_XY2(uint16_t *x,uint16_t *y) 
{
	uint16_t x1,y1;
 	uint16_t x2,y2;
 	uint8_t flag;    
    flag=RTouch_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=RTouch_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+50)||(x1<=x2&&x2<x1+50))  //ǰ�����β�����+-50��
    &&((y2<=y1&&y1<y2+50)||(y1<=y2&&y2<y1+50)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
}  


uint16_t x;
uint16_t y;
uint8_t time;
/****************************************************************************
* ��    ��: void XPT2046_Scan(u8 tp)
* ��    �ܣ���������ɨ��	
* ��ڲ�����type:0,��Ļ����;1,�������� 
* ���ز���: ��  
* ˵    ����        
****************************************************************************/
void XPT2046_Scan(uint8_t type)
{			   
	//	Xup=0xffff;
 //   Yup=0xffff;	 
	if(PEN==GPIO_PIN_RESET)//�а�������
	{
		if(type)RTouch_Read_XY2(&x,&y);//��ȡ��������
		else if(RTouch_Read_XY2(&x,&y))//��ȡ��Ļ����
		{
	 		x=xFactor*x+xOffset;     //�����ת��Ϊ��Ļ����
			y=yFactor*y+yOffset;  
	 	} 	
		Xdown=x;
		Ydown=y;
		
		time++;		   
	}else  //��̧��
	{    
		
		
//		 if(time>2)
		 if( Xdown!=0xffff || Ydown!=0xffff)
		 {
			 		Xup=Xdown;
		      Yup=Ydown;	 
			 		time=0;
		 }

		 Xdown=0xffff;
     Ydown=0xffff;	 
	}
}	

//xpt2046��ʼ��  		    
void XPT2046_Init(void)
{

   
		if(dir_flag)  //���������ת X Y
		{
			CMD_RDX=0X90;
			CMD_RDY=0XD0;	
      xFactor=-0.09195402;	//����У׼���� 
      yFactor=0.06736275;
      xOffset=348;
      yOffset=-19;			
		}
		else				    
		{
			CMD_RDX=0XD0;
			CMD_RDY=0X90;
      xFactor=0.06671114;	  //����У׼���� 
      yFactor=0.09117551;
      xOffset=-11;  
      yOffset=-18;				
		}										 
}

