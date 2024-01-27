#include "my_lcd.h"
#include "common.h"
#ifdef _MYOS_
#include "cmsis_os.h"

#endif
#include "cfont.h"
//��ʼ��LCD�Ļ�����ɫ�ͱ���ɫ	   
uint16_t BRUSH_COLOR=BLACK;	//������ɫ
uint16_t BACK_COLOR=WHITE;   //����ɫ 

//LCD��������
uint16_t  lcd_id;          //LCD ID
uint8_t   dir_flag;        //�������������ƣ�0��������1������
uint16_t  lcd_width;       //LCD�Ŀ���
uint16_t  lcd_height;      //LCD�ĸ߶�
uint16_t	 write_gramcmd;		//дgramָ��
uint16_t  setxcmd;		      //����x����ָ��
uint16_t  setycmd;		      //����y����ָ��	


/****************************************************************************
* ��    ��: void LCD_WriteReg(u16 LCD_Reg, u16 LCD_Value)
* ��    �ܣ�LCDд�Ĵ���
* ��ڲ�����LCD_Reg: �Ĵ�����ַ
*           LCD_RegValue: Ҫд�������
* ���ز�������
* ˵    ����       
****************************************************************************/				   
void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_Value)
{	
	LCD_CMD = LCD_Reg;		 //д��Ҫд�ļĴ������	 
	LCD_DATA = LCD_Value;  //��Ĵ���д�������	    		 
}

/****************************************************************************
* ��    ��: u16 LCD_ReadReg(u16 LCD_Reg)
* ��    �ܣ�LCD���Ĵ���
* ��ڲ�����LCD_Reg:�Ĵ�����ַ
* ���ز����������üĴ���������ֵ
* ˵    ����       
****************************************************************************/	
uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{										   
	LCD_CMD=LCD_Reg;		//д��Ҫ���ļĴ������
	delay_us(4);		  
	return LCD_DATA;		//���ض�����ֵ
} 

//lcd��ʱ����
void lcdm_delay(uint8_t i)
{
	while(i--);
}

//��ʼдGRAM
void LCD_WriteGRAM(void)
{
 	LCD_CMD=write_gramcmd;	  
}

//LCD������ʾ
void LCD_DisplayOn(void)
{					   
   LCD_CMD=0x29;   //9341��1963����ʾ����һ��
}	

//LCD�ر���ʾ
void LCD_DisplayOff(void)
{	
	 LCD_CMD=0x28;   //9341��1963����ʾ����һ��
} 
  
/****************************************************************************
* ��    ��: void LCD_Open_Window(u16 X0,u16 Y0,u16 width,u16 height)
* ��    �ܣ�������,�����û������굽�������Ͻ�(X0,Y0)
* ��ڲ�����X0,Y0:������ʼ����(���Ͻ�)
            width,height:���ڿ��Ⱥ͸߶�
* ���ز�������
* ˵    ���������С:width*height.�B
****************************************************************************/
void LCD_Open_Window(uint16_t X0,uint16_t Y0,uint16_t width,uint16_t height)
{   
	width=X0+width-1;    //������½�����
	height=Y0+height-1;
  
	if(dir_flag==0&&lcd_id==0X1963)  //1963��������
		{
			X0=lcd_width-width-X0; 
			height=Y0+height-1; 
			LCD_CMD=setxcmd; 
			LCD_DATA=X0>>8; 
			LCD_DATA=X0&0XFF;	 
			LCD_DATA=(X0+width-1)>>8; 
			LCD_DATA=(X0+width-1)&0XFF;  
			LCD_CMD=setycmd; 
			LCD_DATA=Y0>>8; 
			LCD_DATA=Y0&0XFF; 
			LCD_DATA=height>>8; 
			LCD_DATA=height&0XFF; 		
		}
	else
		{
			LCD_CMD=setxcmd; 
			LCD_DATA=X0>>8; 
			LCD_DATA=X0&0XFF;	 
			LCD_DATA=width>>8; 
			LCD_DATA=width&0XFF;  
			LCD_CMD=setycmd; 
			LCD_DATA=Y0>>8; 
			LCD_DATA=Y0&0XFF; 
			LCD_DATA=height>>8; 
			LCD_DATA=height&0XFF; 
		}
} 

/****************************************************************************
* ��    ��: void Set_Scan_Direction(u8 direction)
* ��    �ܣ�����LCD��ɨ�跽��
* ��ڲ�����direction��ɨ�跽��
* ���ز�������
* ˵    ����     
****************************************************************************/  	   
void Set_Scan_Direction(uint8_t direction)
{
	uint16_t skhda=0;
	uint16_t diomf=0;
	    //9341������1963����ʱ��Ҫת����
	if( (dir_flag==1&&lcd_id==0X9341)||(dir_flag==0&&lcd_id==0X1963)) 
	  {			   
			switch(direction)//����ת��
				{
					case 0:direction=6;break;
					case 1:direction=7;break;
					case 2:direction=4;break;
					case 3:direction=5;break;
					case 4:direction=1;break;
					case 5:direction=0;break;
					case 6:direction=3;break;
					case 7:direction=2;break;	     
				}
    }	
		
		switch(direction)
		{
			case L2R_U2D:   //������,���ϵ���
				skhda|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U:   //������,���µ���
				skhda|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D:   //���ҵ���,���ϵ���
				skhda|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U:   //���ҵ���,���µ���
				skhda|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R:   //���ϵ���,������
				skhda|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L:   //���ϵ���,���ҵ���
				skhda|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R:   //���µ���,������
				skhda|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L:   //���µ���,���ҵ���
				skhda|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
	  diomf=0X36;
	  if(lcd_id==0X9341)		
			 skhda|=0X08;		
	  LCD_WriteReg(diomf,skhda);		
	  LCD_Open_Window(0,0,lcd_width,lcd_height);  //������ɨ�跽��󣬿���ʾ����Ϊȫ������
}

/****************************************************************************
* ��    ��: void Set_Display_Mode(u8 mode)
* ��    �ܣ�����LCD��ʾ����
* ��ڲ�����mode: 0,����
                  1,����
* ���ز�������
* ˵    �����B
****************************************************************************/
void Set_Display_Mode(uint8_t mode)
{
	if(mode==0)		  //����
	{
		dir_flag=0;	

    if(lcd_id==0X9341)
		{
			write_gramcmd=0X2C;
			setxcmd=0X2A;
			setycmd=0X2B; 
		  lcd_width=240;
		  lcd_height=320;					
		}	
		else if(lcd_id==0X1963)
		{
			write_gramcmd=0X2C;	 //GRAM��ָ�� 
			setxcmd=0X2B;	       //дX����ָ��
		  setycmd=0X2A;	       //дY����ָ��
			lcd_width=480;		   //���ÿ���480
			lcd_height=800;		   //���ø߶�800  
		}
	}
	else 				  //����
	{	  				
    dir_flag=1;
		
		if(lcd_id==0X9341)
		{
			write_gramcmd=0X2C;
			setxcmd=0X2A;
			setycmd=0X2B; 
      lcd_width=320;
   		lcd_height=240;				
		}
		else if(lcd_id==0X1963)
		{
			write_gramcmd=0X2C;	  //GRAM��ָ�� 
			setxcmd=0X2A;	        //дX����ָ��
			setycmd=0X2B;	        //дY����ָ��
			lcd_width=800;		    //���ÿ���800
			lcd_height=480;		    //���ø߶�480  
		}			
	} 
	Set_Scan_Direction(L2R_U2D);	//����ɨ�跽��   ������,���µ���
}	

/****************************************************************************
* ��    ��: void LCD_SetCursor(u16 Xaddr, u16 Yaddr)
* ��    �ܣ����ù��λ��
* ��ڲ�����x��x����
            y��y����
* ���ز�������
* ˵    ����     
****************************************************************************/
void LCD_SetCursor(uint16_t Xaddr, uint16_t Yaddr)
{	 
	if(lcd_id==0X9341)
		{		    
			LCD_CMD=setxcmd; 
			LCD_DATA=(Xaddr>>8); 
			LCD_DATA=(Xaddr&0XFF);	 
			LCD_CMD=setycmd; 
			LCD_DATA=(Yaddr>>8); 
			LCD_DATA=(Yaddr&0XFF);
		}
	else if(lcd_id==0X1963)
		{  			 		
			if(dir_flag==0) 
			{
				Xaddr=lcd_width-1-Xaddr;
				LCD_CMD=setxcmd; 
				LCD_DATA=0;
				LCD_DATA=0; 		
				LCD_DATA=Xaddr>>8;
				LCD_DATA=Xaddr&0XFF;		 	 
			}else
			{
				LCD_CMD=setxcmd; 
				LCD_DATA=Xaddr>>8;
				LCD_DATA=Xaddr&0XFF; 		
				LCD_DATA=(lcd_width-1)>>8;
				LCD_DATA=(lcd_width-1)&0XFF;		 	 			
			}	
			LCD_CMD=setycmd; 
			LCD_DATA=Yaddr>>8;
			LCD_DATA=Yaddr&0XFF; 		
			LCD_DATA=(lcd_height-1)>>8;
			LCD_DATA=(lcd_height-1)&0XFF; 			 		
			
		}
} 

/****************************************************************************
* ��    ��: u16 LCD_GetPoint(u16 x,u16 y)
* ��    �ܣ���ȡĳ�����ɫֵ	
* ��ڲ�����x��x����
            y��y����
* ���ز������˵����ɫ
* ˵    ����     
****************************************************************************/
uint16_t LCD_GetPoint(uint16_t x,uint16_t y)
{
 	volatile uint16_t r=0,g=0,b=0;
   
	LCD_SetCursor(x,y);	
	
	LCD_CMD=0X2E;   //9341��1963��GRAMָ��һ�� 
	r=LCD_DATA; 	
	
 	if(lcd_id==0X1963)
		 return r;		   //1963ֱ�Ӷ���������16λ��ɫֵ
	
	else	 //������������9341
 	{
		 lcdm_delay(2);	  
		 b=LCD_DATA;    //9341Ҫ��2��
		 g=r&0XFF;		  //9341��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
		 g<<=8;
		 return (((r>>11)<<11)|((g>>10)<<5)|(b>>11)); //9341�蹫ʽת��
	} 		  	
}	

/****************************************************************************
* ��    ��: void LCD_DrawPoint(u16 x,u16 y)
* ��    �ܣ����㣨�ڸõ�д�뻭�ʵ���ɫ��
* ��ڲ�����x��x����
            y��y����
* ���ز�������
* ˵    ���BRUSH_COLOR:�˵����ɫֵ
****************************************************************************/
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_SetCursor(x,y);		    //���ù��λ�� 
	LCD_WriteGRAM();	        //��ʼд��GRAM
	LCD_DATA=BRUSH_COLOR; 
}

/****************************************************************************
* ��    ��: void LCD_Color_DrawPoint(u16 x,u16 y,u16 color)
* ��    �ܣ������õ����괦����Ӧ��ɫ���ڸõ�д���Զ�����ɫ��
* ��ڲ�����x��x����
            y��y����
            color �˵����ɫֵ
* ���ز�������
* ˵    ����color:д��˵����ɫֵ   GUI���øú���
****************************************************************************/
void LCD_Color_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{	       
	 LCD_DrawPoint(x,y);
	 LCD_CMD=write_gramcmd; 
	 LCD_DATA=color;  
}	

/****************************************************************************
* ��    ��: void Ssd1963_Set_BackLight(u8 BL_value)
* ��    �ܣ�SSD1963 ���ñ��� 
* ��ڲ�����BL_value���������ȴ�С  ȡֵ:0-255  ����255����
* ���ز�������
* ˵    ����     
****************************************************************************/
void Ssd1963_Set_BackLight(uint8_t BL_value)
{	
	LCD_CMD=0xBE;	 
	LCD_DATA=0x05;	 
	LCD_DATA=BL_value; 
	LCD_DATA=0x01;	 
	LCD_DATA=0xFF;	 
	LCD_DATA=0x00;	 
	LCD_DATA=0x00;	 
}

/****************************************************************************
* ��    ��: void LCD_Clear(u16 color)
* ��    �ܣ���������
* ��ڲ�����color: Ҫ���������ɫ
* ���ز�������
* ˵    �����B
****************************************************************************/
void LCD_Clear(uint16_t color)
{
	uint32_t i=0;      
	uint32_t pointnum=0;

	pointnum=lcd_width*lcd_height; 	 //�õ�LCD�ܵ���
	LCD_SetCursor(0x00,0x00);	       //���ù��λ�� 
	LCD_WriteGRAM();     		 //��ʼд��GRAM	 	  
	for(i=0;i<pointnum;i++)
	{
		LCD_DATA=color;	   
	}
}
uint16_t ILI9341_Read_id(void)
{
 	 uint16_t id;
	
	 LCD_CMD=0xD3;	//9341��ID����   
	HAL_Delay(1);
	 id=LCD_DATA;	 
	 id=LCD_DATA; 	//0x00
	 id=LCD_DATA;   //0x93								   
	 id<<=8;
	 id|=LCD_DATA;  //0x41 
	
	 return id;
}
//��ʼ��lcd
void LCD_Init(void)
{ 	
   lcd_id=ILI9341_Read_id(); //�ȶ�����������Ļ�ǲ���9341����	
	 printf("read id %x\n",lcd_id);
	
		 if(lcd_id!=0x9341)		//�������9341���������ǲ���1963����	 
		{
			LCD_CMD=(0xA1);   //1963��ID����  
			lcd_id=LCD_DATA;
			lcd_id=LCD_DATA;	//0x57
			lcd_id<<=8;	 
			lcd_id|=LCD_DATA;	//0x61	
			if(lcd_id==0x5761)
				 lcd_id=0x1963;  //SSD1963ʵ�ʶ�����ID��0x5761,Ϊ��ֱ�ۣ��������Ϊ1963
		}

  if(lcd_id==0X9341) //������,����дʱ��Ϊ���
	{	 							    
		FSMC_Bank1E->BWTR[6]&=~(0XF<<0); //��ַ����ʱ������ 	 
		FSMC_Bank1E->BWTR[6]&=~(0XF<<8); //���ݱ���ʱ������
		FSMC_Bank1E->BWTR[6]|=3<<0;		   //��ַ����ʱ��Ϊ3��HCLK =18ns  	 
		FSMC_Bank1E->BWTR[6]|=2<<8;    	 //���ݱ���ʱ��Ϊ6ns*3��HCLK=18ns
	}
	
		if(lcd_id==0X9341)	//9341��ʼ��
	{	 
		LCD_CMD=0xCF;  
		LCD_DATA=0x00;
		LCD_DATA=0xC1; 
		LCD_DATA=0X30; 
		LCD_CMD=0xED;  
		LCD_DATA=0x64; 
		LCD_DATA=0x03; 
		LCD_DATA=0X12; 
		LCD_DATA=0X81; 
		LCD_CMD=0xE8;  
		LCD_DATA=0x85; 
		LCD_DATA=0x10; 
		LCD_DATA=0x7A; 
		LCD_CMD=0xCB;  
		LCD_DATA=0x39; 
		LCD_DATA=0x2C; 
		LCD_DATA=0x00; 
		LCD_DATA=0x34; 
		LCD_DATA=0x02; 
		LCD_CMD=0xF7;  
		LCD_DATA=0x20; 
		LCD_CMD=0xEA;  
		LCD_DATA=0x00; 
		LCD_DATA=0x00; 
		LCD_CMD=0xC0;    
		LCD_DATA=0x1B;    
		LCD_CMD=0xC1;    
		LCD_DATA=0x01;    
		LCD_CMD=0xC5;     
		LCD_DATA=0x30; 	  
		LCD_DATA=0x30; 	  
		LCD_CMD=0xC7;      
		LCD_DATA=0XB7; 
		LCD_CMD=0x36;      
		LCD_DATA=0x48; 
		LCD_CMD=0x3A;   
		LCD_DATA=0x55; 
		LCD_CMD=0xB1;   
		LCD_DATA=0x00;   
		LCD_DATA=0x1A; 
		LCD_CMD=0xB6;     
		LCD_DATA=0x0A; 
		LCD_DATA=0xA2; 
		LCD_CMD=0xF2;      
		LCD_DATA=0x00; 
		LCD_CMD=0x26;      
		LCD_DATA=0x01; 
		LCD_CMD=0xE0;      
		LCD_DATA=0x0F; 
		LCD_DATA=0x2A; 
		LCD_DATA=0x28; 
		LCD_DATA=0x08; 
		LCD_DATA=0x0E; 
		LCD_DATA=0x08; 
		LCD_DATA=0x54; 
		LCD_DATA=0XA9; 
		LCD_DATA=0x43; 
		LCD_DATA=0x0A; 
		LCD_DATA=0x0F; 
		LCD_DATA=0x00; 
		LCD_DATA=0x00; 
		LCD_DATA=0x00; 
		LCD_DATA=0x00; 		 
		LCD_CMD=0XE1;     
		LCD_DATA=0x00; 
		LCD_DATA=0x15; 
		LCD_DATA=0x17; 
		LCD_DATA=0x07; 
		LCD_DATA=0x11; 
		LCD_DATA=0x06; 
		LCD_DATA=0x2B; 
		LCD_DATA=0x56; 
		LCD_DATA=0x3C; 
		LCD_DATA=0x05; 
		LCD_DATA=0x10; 
		LCD_DATA=0x0F; 
		LCD_DATA=0x3F; 
		LCD_DATA=0x3F; 
		LCD_DATA=0x0F; 
		LCD_CMD=0x2B; 
		LCD_DATA=0x00;
		LCD_DATA=0x00;
		LCD_DATA=0x01;
		LCD_DATA=0x3f;
		LCD_CMD=0x2A; 
		LCD_DATA=0x00;
		LCD_DATA=0x00;
		LCD_DATA=0x00;
		LCD_DATA=0xef;	 
		LCD_CMD=0x11;  
		osDelay(1);
		LCD_CMD=0x29; 
		
		LCD_BACK(GPIO_PIN_SET);	  //��������
	}
	else if(lcd_id==0X1963)
	{
		LCD_CMD=0xE2;		
		LCD_DATA=0x1D;		
		LCD_DATA=0x02;		
		LCD_DATA=0x04;		
		delay_us(100);
		
		LCD_CMD=0xE0;		
		LCD_DATA=0x01;		
		osDelay(10);
		
		LCD_CMD=0xE0;		
		LCD_DATA=0x03;		
		osDelay(12); 
		
		LCD_CMD=0x01;		//����λ
		osDelay(10);

		LCD_CMD=0xE6;		
		LCD_DATA=0x2F;
		LCD_DATA=0xFF;
		LCD_DATA=0xFF;

		LCD_CMD=0xB0;		  
		LCD_DATA=0x20;		 
		LCD_DATA=0x00;		
	
		LCD_DATA=(800-1)>>8; //LCDˮƽ��������
		LCD_DATA=800-1;		 
		LCD_DATA=(480-1)>>8; //LCD��ֱ��������
		LCD_DATA=480-1;		 
		LCD_DATA=0x00;		   //RGB���� 
		
		LCD_CMD=0xB4;	
		LCD_DATA=(800+46+210-1)>>8;
		LCD_DATA=800+46+210-1;
		LCD_DATA=46>>8;
		LCD_DATA=46;
		LCD_DATA=0;
		LCD_DATA=0x00;
		LCD_DATA=0x00;
		LCD_DATA=0x00;
		
		LCD_CMD=0xB6;		 
		LCD_DATA=(480+23+22-1)>>8;
		LCD_DATA=480+23+22-1;
		LCD_DATA=23>>8;
		LCD_DATA=23;
		LCD_DATA=21;
		LCD_DATA=0x00;
		LCD_DATA=0x00;
		
		LCD_CMD=0xF0;	 
		LCD_DATA=0x03;	//SSD1963��MCU�Ľӿ�Ϊ16bit  

		LCD_CMD=0x29;	//������ʾ
		LCD_CMD=0xD0;	//�����Զ���ƽ��DBC
		LCD_DATA=0x00;	
	
		LCD_CMD=0xBE;	  //PWM���
		LCD_DATA=0x05;	//PWMƵ��
		LCD_DATA=0xFE;	//PWMռ�ձ�
		LCD_DATA=0x01;	
		LCD_DATA=0x00;	
		LCD_DATA=0x00;	
		LCD_DATA=0x00;	
		
		LCD_CMD=0xB8;	 
		LCD_DATA=0x03;	 
		LCD_DATA=0x01;	 
		LCD_CMD=0xBA;
		LCD_DATA=0X01;	//����LCD����

		Ssd1963_Set_BackLight(255);//��������
	}
	Set_Display_Mode(0);		 //��ʼ��Ϊ����		
	LCD_Clear(BACK_COLOR);        //������ɫ
	
	
}
/****************************************************************************
* ��    ��: void LCD_DisplayChar(u16 x,u16 y,u8 num,u8 size)
* ��    �ܣ���ָ��λ����ʾһ���ַ�
* ��ڲ�����x,y:��ʼ����
            word:Ҫ��ʾ���ַ�:abcdefg1234567890...
            size:�����С 12/16/24
* ���ز�������
* ˵    ��������ģȡģ����Ϊ�ȴ����ң��ٴ��ϵ���  ��λ��ǰ  �B
****************************************************************************/
void LCD_DisplayChar(uint16_t x,uint16_t y,uint8_t word,uint8_t size)
{  							  
  uint8_t  bytenum,bytedata, a,b;
	
	uint16_t xmid=x;   //�洢��ʼXֵ(λ��)			     
	 
	if(size==12) bytenum=12;        //���ֿ������п�֪��ÿ�����嵥���ַ���ռ���ֽ���
	else if(size==16) bytenum=16;
	else if(size==24) bytenum=48;
	else return;    //���������˳�
	                
	word=word-' ';  //�ֿ������ǰ�ASCII������
  //cfont.h���ֿ��Ǵӿո�ʼ�� �ո���ǵ�һ��Ԫ�� �����ַ���ASCII���ȥ�ո��͵õ��������е�ƫ��ֵ(λ��) 
	    for(b=0;b<bytenum;b++)
	    {   
					if(size==12)bytedata=char_1206[word][b]; 	 	  //����1206����
					else if(size==16)bytedata=char_1608[word][b];	//����1608����
					else if(size==24)bytedata=char_2412[word][b];	//����2412����

					for(a=0;a<8;a++)   
					{	   
						if(bytedata&0x01) LCD_Color_DrawPoint(x,y,BRUSH_COLOR); //������ģ�ǵ�λ��ǰ �����ȴӵ�λ�ж�  Ϊ1ʱ��ʾ������ɫ      
						else LCD_Color_DrawPoint(x,y,BACK_COLOR);               //0ʱ��ʾ������ɫ		
						bytedata>>=1;    //��λ�ж��� ��������λ�ж�
						x++;	           //��ʾ��һλ ����һλ��ʾ 
						if((x-xmid)==size/2)  //x���򳬳������С �磺16���� ʵ���� 08*16�ĵ���  ����� size/2
						{
							x=xmid;  //�ӳ�ʼXλ����д��һ��
							y++;     //��һ��д�� ����һ����д
							break;   //����for(a=0;a<8;a++)ѭ��
						}
		      }   	 
	    }       	 	  
}   
/****************************************************************************
* ��    ��: void LCD_DisplayString(u16 x,u16 y,u8 size,u8 *p)
* ��    �ܣ���ʾ�ַ���
* ��ڲ�����x,y:�������
*           size:�����С
*           *p:�ַ�����ʼ��ַ	
* ���ز�������
* ˵    ����  �B
****************************************************************************/	  
void LCD_DisplayString(uint16_t x,uint16_t y,uint8_t size,int color,uint8_t *p)
{  
    BRUSH_COLOR=color;  
	
    while((*p>=' ')&&(*p<='~')) //ֻ��ʾ�� ������~��֮����ַ�
    {       
        LCD_DisplayChar(x,y,*p,size);
        x+=size/2;
			  if(x>=lcd_width) break;
        p++;
    }  
}

/****************************************************************************
* ��    ��: void LCD_Fill_onecolor(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
* ��    �ܣ���ָ����������䵥����ɫ
* ��ڲ�����(sx,sy),(ex,ey):�����ζԽ�����
            color:Ҫ������ɫ
* ���ز�������
* ˵    ���������СΪ:(ex-sx+1)*(ey-sy+1)  �B
****************************************************************************/
void LCD_Fill_onecolor(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{          
	uint16_t i,j;
	uint16_t nlen=0;

		nlen=ex-sx+1;	 
		for(i=sy;i<=ey;i++)
		{
		 	LCD_SetCursor(sx,i);      				  //���ù��λ�� 
			LCD_WriteGRAM();     			  //��ʼд��GRAM	  
			for(j=0;j<nlen;j++)
			  LCD_DATA=color;	//���ù��λ�� 	    
		}
} 



/****************************************************************************
* ��    ��: void LCD_Draw_Picture(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
* ��    �ܣ���ָ�������ڻ���ͼƬ
* ��ڲ�����(sx,sy),(ex,ey):�����ζԽ�����
            color:Ҫ����ͼƬ������ɫ����
* ���ز�������
* ˵    ���������СΪ:(ex-sx+1)*(ey-sy+1)  �B
****************************************************************************/
void LCD_Draw_Picture(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{  
	uint16_t height,width;
	uint16_t i,j;
	width=ex-sx+1; 			    //�õ�ͼƬ�Ŀ���
	height=ey-sy+1;			    //�õ�ͼƬ�ĸ߶�
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i); //���ù��λ�� 
		LCD_WriteGRAM();        //��ʼд��GRAM
		for(j=0;j<width;j++)
		   LCD_DATA=color[i*height+j];//д����ɫֵ
	}	  
}  


/****************************************************************************
* ��    ��: void LCD_DisplayString(u16 x,u16 y,u8 size,u8 *p)
* ��    �ܣ���ʾ�Զ����ַ���
* ��ڲ�����x,y:�������
*           width,height:�����С  
*           size:�����С
*           *p:�ַ�����ʼ��ַ	
*           brushcolor���Զ��廭����ɫ
*           backcolor�� �Զ��屳����ɫ
* ���ز�������
* ˵    ����  �B
****************************************************************************/	  
void LCD_DisplayString_color(uint16_t x,uint16_t y,uint8_t size,uint8_t *p,uint8_t brushcolor,uint16_t backcolor)
{
   uint16_t bh_color,bk_color;
	
 	bh_color=BRUSH_COLOR;  //�ݴ滭����ɫ
	bk_color=BACK_COLOR;   //�ݴ汳����ɫ
	
	BRUSH_COLOR=brushcolor;
	BACK_COLOR=backcolor;
	
	LCD_DisplayString(x,y,size,brushcolor,p);
	
	BRUSH_COLOR=bh_color;   //���ı�ϵͳ��ɫ
	BACK_COLOR=bk_color;
}
//a^n����������ֵ:a^n�η�
uint32_t Counter_Power(uint8_t a,uint8_t n)
{
	uint32_t mid=1;	 
	while(n--)mid*=a;    
	return mid;
}

/****************************************************************************
* ��    ��: void LCD_DisplayNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
* ��    �ܣ���ָ��λ����ʾһ������
* ��ڲ�����x,y:�������
            value:��ֵ;	 
            len:����(������ʾ��λ��)
            size:�����С
            mode: 0����λΪ0����ʾ
                  1����λΪ0����len���Ȳ���ʾ����0
* ���ز�������
* ˵    ����  �B
****************************************************************************/
void LCD_DisplayNum(uint16_t x,uint16_t y,uint16_t value,uint8_t len,uint8_t size,uint8_t mode)
{  
	uint8_t t,numtemp;
	uint8_t value_num;     //��ֵ�ܹ���λ��
  uint32_t value_mid;     
	
	value_mid=value;   //����λ��ʱ��Ӱ��Ҫ��ʾ����ֵ��С
	for (value_num = 0; value_mid > 0; value_num++)
  {
    value_mid /= 10;  
  } //ִ����for����֪��Ҫ��ʾ����ֵΪ��λ��

	if(value_num>len)  //��ֵλ����������λ��������ʾ���򲻹� ��ʾ����
	{
	 LCD_DisplayString(x,y,size,BRUSH_COLOR,(uint8_t *)"ERROR");
	 return; //�˳�����
	}
	else
	{
	  for(t=0;t<len;t++)
		{
		  if( t<(len-value_num))
			{
			  if(mode)LCD_DisplayChar(x+(size/2)*t,y,'0',size);  
				else LCD_DisplayChar(x+(size/2)*t,y,' ',size);  
			}
			else
			{
			  numtemp=(value/Counter_Power(10,len-t-1))%10;  //ȡ����λ��ֵ
			  LCD_DisplayChar(x+(size/2)*t,y,numtemp+'0',size); 
			}		
		}
	}
} 

/****************************************************************************
* ��    ��: void LCD_DisplayNum_color(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
* ��    �ܣ���ָ��λ����ʾһ���Զ�����ɫ������
* ��ڲ�����x,y:�������
            num:��ֵ;	 
            len:����(��Ҫ��ʾ��λ��)
            size:�����С
            mode: 0����λΪ0����ʾ
                  1����λΪ0��ʾ0
            brushcolor���Զ��廭����ɫ
            backcolor�� �Զ��屳����ɫ
* ���ز�������
* ˵    ����  �B
****************************************************************************/
void LCD_DisplayNum_color(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode,uint16_t brushcolor,uint16_t backcolor)
{
 uint16_t bh_color,bk_color;
	
 	bh_color=BRUSH_COLOR;  //�ݴ滭����ɫ
	bk_color=BACK_COLOR;   //�ݴ汳����ɫ
	
	BRUSH_COLOR=brushcolor;
	BACK_COLOR=backcolor;
	
	LCD_DisplayNum(x,y,num,len,size,mode);
	
	BRUSH_COLOR=bh_color;   //���ı�ϵͳ��ɫ
	BACK_COLOR=bk_color;
}



