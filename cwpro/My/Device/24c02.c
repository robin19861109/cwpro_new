#include "24c02.h"  				 

/*********************************************************************************
************************�������� STM32F407���Ŀ�����******************************
**********************************************************************************
* �ļ�����: 24C02.c                                                              *
* �ļ�������24C02��������                                                        *
* �������ڣ�2017.08.30                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    ����IO��ģ��IICЭ����24c02��д����                                       * 
**********************************************************************************
*********************************************************************************/	

//IIC��ӦIO�ڵĳ�ʼ��
void IIC_Init(void)
{			
/*  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //ʹ��GPIOBʱ��
  //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;         //��©���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //����
  GPIO_Init(GPIOB, &GPIO_InitStructure);                 //��ʼ��IO*/
	
  IIC_Stop();   //�ȸ�ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ
}
/*******************************************************************************
*************************����ΪIO��ģ��IICͨ��**********************************
*******************************************************************************/
//IIC��ʼ�ź�  ��SCL�ߵ�ƽʱ��SDA����һ���½��ر�ʾI2C���������ź� 
void IIC_Start(void)
{
	IIC_SDAOUT(1);	  	  
	IIC_SCL(1);
	delay_us(4);
 	IIC_SDAOUT(0);
	delay_us(4);
	IIC_SCL(0);     //׼�����ͻ�������� 
}	  

//IICֹͣ�ź�  ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź�
void IIC_Stop(void)
{
	IIC_SDAOUT(0); 
 	delay_us(4);
	IIC_SCL(1);
  delay_us(4);				
	IIC_SDAOUT(1); //����I2C���߽����ź�				   	
}

/****************************************************************************
* ��    ��: u8 MCU_Wait_Ack(void)
* ��    �ܣ�MCU�ȴ����豸Ӧ���źŵ���
* ��ڲ�������
* ���ز�����1:����Ӧ��ʧ��  0:����Ӧ��ɹ�
* ˵    ����  �B
****************************************************************************/
uint8_t MCU_Wait_Ack(void)
{
	uint8_t ack;
 
	IIC_SDAOUT(1);
	delay_us(1);	   
	IIC_SCL(1);
	delay_us(1);	 
	if (IIC_SDAIN)	/* CPU��ȡSDA����״̬ */
	{
		ack = 1;
	}
	else
	{
		ack = 0;
	}
	IIC_SCL(0); 
	delay_us(1);
	return ack;  
}

/****************************************************************************
* ��    ��: u8 void MCU_Send_Ack(void)
* ��    �ܣ�MCU����ACKӦ��
* ��ڲ�������
* ���ز�����
* ˵    ����  �B
****************************************************************************/
void MCU_Send_Ack(void)
{
	IIC_SDAOUT(0);
	delay_us(2);
	IIC_SCL(1);
	delay_us(2);
	IIC_SCL(0);
	delay_us(2);
	IIC_SDAOUT(1);
}

/****************************************************************************
* ��    ��: u8 void MCU_Send_Ack(void)
* ��    �ܣ�MCU������ACKӦ��	
* ��ڲ�������
* ���ز�����
* ˵    ����  �B
****************************************************************************/  
void MCU_NOAck(void)
{
	IIC_SDAOUT(1);
	delay_us(2);
	IIC_SCL(1);
	delay_us(2);
	IIC_SCL(0);
	delay_us(2);
}	

/****************************************************************************
* ��    ��: void IIC_write_OneByte(u8 Senddata)
* ��    �ܣ�IICдһ���ֽڵ�EEPROM	
* ��ڲ�����Senddata:д���8λ����
* ���ز�����
* ˵    ����  �B
****************************************************************************/	  
void IIC_write_OneByte(uint8_t Senddata)
{                        
    uint8_t t;   
	    
    IIC_SCL(0);    //����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDAOUT((Senddata&0x80)>>7);
        Senddata<<=1; 	  
		delay_us(2);   
		IIC_SCL(1);
		delay_us(2); 
		IIC_SCL(0);	
		delay_us(2);
    }	 
} 

/****************************************************************************
* ��    ��: void IIC_Read_OneByte(u8 Senddata)
* ��    �ܣ�IIC��ȡһ���ֽ�
* ��ڲ�����ack=1������ACK��ack=0������nACK 
* ���ز�����������8λ����
* ˵    ����  �B
****************************************************************************/	  
uint8_t IIC_Read_OneByte(uint8_t ack)
{
	uint8_t i,receivedata=0;

    for(i=0;i<8;i++ )
	  {
        IIC_SCL(0); 
        delay_us(2);
		    IIC_SCL(1);
        receivedata<<=1;
        if(IIC_SDAIN)receivedata++;   
		delay_us(1); 
    }					 
    if (!ack)
        MCU_NOAck();//����nACK
    else
        MCU_Send_Ack(); //����ACK   
    return receivedata;
}
/*******************************IO��ģ��IIC*************************************
*******************************************************************************/


/*******************************************************************************
*************************����ΪEEPROM24C02��д����******************************
*******************************************************************************/
//��ʼ��24c02��IIC�ӿ�
void AT24C02_Init(void)
{
	IIC_Init();  //IIC��ʼ��
}

/****************************************************************************
* ��    ��: u8 AT24C02_ReadByte(u8 ReadAddr)
* ��    �ܣ���AT24C02ָ����ַ����һ������
* ��ڲ�����ReadAddr��Ҫ��ȡ�������ڵĵ�ַ
* ���ز�����������8λ����
* ˵    ����  �B
****************************************************************************/
uint8_t AT24C02_ReadByte(uint8_t ReadAddr)
{				  
	uint8_t receivedata=0;		  	    																 
  
	IIC_Start();  
	IIC_write_OneByte(0XA0);           //����������ַ0XA0
	MCU_Wait_Ack();
  IIC_write_OneByte(ReadAddr);       //���͵͵�ַ
	MCU_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_write_OneByte(0XA1);           //�������ģʽ			   
	MCU_Wait_Ack();	 
  receivedata=IIC_Read_OneByte(0);		   
  IIC_Stop();                    //����һ��ֹͣ����	    
	
	return receivedata;
}

/****************************************************************************
* ��    ��: u8 AT24C02_WriteByte(u8 WriteAddr,u8 WriteData)
* ��    �ܣ���AT24C02ָ����ַд��һ������
* ��ڲ�����WriteAddr��Ҫд���������ڵĵ�ַ
            WriteData: Ҫд�������
* ���ز����� 
* ˵    ����  �B
****************************************************************************/
void AT24C02_WriteByte(uint8_t WriteAddr,uint8_t WriteData)
{				   	  	    																 
  IIC_Start();  
	IIC_write_OneByte(0XA0);       //����0XA0,д���� 	 
	MCU_Wait_Ack();	   
  IIC_write_OneByte(WriteAddr);  //���͵͵�ַ
	MCU_Wait_Ack(); 	 										  		   
	IIC_write_OneByte(WriteData);  //�����ֽ�							   
	MCU_Wait_Ack();  		    	   
  IIC_Stop();                    //����һ��ֹͣ���� 
	osDelay(10);	 
}

/****************************************************************************
* ��    ��: u8 AT24C02_Test(void)
* ��    �ܣ�����AT24C02�Ƿ�����
* ��ڲ�������
* ���ز���������1:���ʧ��
            ����0:���ɹ� 
* ˵    ����  �B
****************************************************************************/
uint8_t AT24C02_Test(void)
{
	uint8_t Testdata;
	Testdata=AT24C02_ReadByte(255); //����������ԣ�����ֵ�����ٴ�д��	   
	if(Testdata==0XAB)return 0;		   
	else                             
	{
		AT24C02_WriteByte(255,0XAB);
	  Testdata=AT24C02_ReadByte(255);	  
		if(Testdata==0XAB)return 0;
	}
	return 1;											  
}

/****************************************************************************
* ��    ��: void AT24C02_Read(u8 ReadAddr,u8 *pBuffer,u8 ReadNum)
* ��    �ܣ���AT24C02�����ָ����ַ��ʼ����ָ������������
* ��ڲ�����ReadAddr :��ʼ�����ĵ�ַ  0~255
            pBuffer  :���������׵�ַ
            ReadNum:Ҫ�������ݵĸ���
* ���ز�����
* ˵    ����  �B
****************************************************************************/
void AT24C02_Read(uint8_t ReadAddr,uint8_t *pBuffer,uint8_t ReadNum)
{
	while(ReadNum--)
	{
		*pBuffer++=AT24C02_ReadByte(ReadAddr++);	
	}
} 

/****************************************************************************
* ��    ��: void AT24C02_Write(u8 WriteAddr,u8 *pBuffer,u8 WriteNum)
* ��    �ܣ���AT24C02�����ָ����ַ��ʼд��ָ������������
* ��ڲ�����WriteAddr :��ʼд��ĵ�ַ  0~255
            pBuffer  :���������׵�ַ
            WriteNum:Ҫд�����ݵĸ���
* ���ز�����
* ˵    ����  �B
****************************************************************************/
void AT24C02_Write(uint8_t WriteAddr,uint8_t *pBuffer,uint8_t WriteNum)
{
	while(WriteNum--)
	{
		AT24C02_WriteByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}








