#ifndef __24C02_H
#define __24C02_H
#include "common.h"  
#include "main.h"

////////////////////////////////////////////////////////////////////////////////// 	

//IO���庯��	 
#define IIC_SCL(x)      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,x) //PBout(8) //SCL
#define IIC_SDAOUT(x)   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,x)  //PBout(9) //���SDA	 
#define IIC_SDAIN    HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9)   

//IIC��غ���
void IIC_Init(void);          //��ʼ��IIC��IO��				 
void IIC_Start(void);				  //����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
uint8_t MCU_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MCU_Send_Ack(void);			//IIC����ACK�ź�
void MCU_NOAck(void);				  //IIC������ACK�ź�
void IIC_write_OneByte(uint8_t Senddata);
uint8_t IIC_Read_OneByte(uint8_t ack);
	
//EEPROM24c02��غ���
uint8_t AT24C02_ReadByte(uint8_t ReadAddr);							     //ָ����ַ��ȡһ���ֽ�
void AT24C02_WriteByte(uint8_t WriteAddr,uint8_t DataToWrite);		//ָ����ַд��һ���ֽ�

void AT24C02_Write(uint8_t WriteAddr,uint8_t *pBuffer,uint8_t WriteNum);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24C02_Read(uint8_t ReadAddr,uint8_t *pBuffer,uint8_t ReadNum);   	//��ָ����ַ��ʼ����ָ�����ȵ�����

uint8_t AT24C02_Test(void);  //�������
void AT24C02_Init(void); //��ʼ��IIC

#endif
















