#ifndef __LCD_H
#define __LCD_H
#include "main.h"

#include "stdlib.h"

//////////////////////////////////////////////////////////////////////////////////	 
 
//LCD��������
extern  uint16_t  lcd_id;         //LCD ID
extern  uint8_t   dir_flag;       //���������������ƣ�0��������1��������	
extern  uint16_t  lcd_width;      //LCD ���
extern  uint16_t  lcd_height;     //LCD �߶�
extern  uint16_t	 write_gramcmd;	 //дgramָ��
extern	uint16_t  setxcmd;		     //����x����ָ��
extern	uint16_t  setycmd;		     //����y����ָ��	 


//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern uint16_t  BRUSH_COLOR;//Ĭ�Ϻ�ɫ    
extern uint16_t  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

#define	LCD_BACK(x) HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,x) 
//A12��Ϊ��������������  ����ʱSTM32�ڲ�������һλ����	    
#define  CMD_BASE     ((uint32_t)(0x6C000000 | 0x00001FFE))
#define  DATA_BASE    ((uint32_t)(0x6C000000 | 0x00002000))

#define LCD_CMD       ( * (uint16_t *) CMD_BASE )
#define LCD_DATA      ( * (uint16_t *) DATA_BASE)

//ɨ�跽����
#define  L2R_U2D  0 //������,���ϵ���
#define  L2R_D2U  1 //������,���µ���
#define  R2L_U2D  2 //���ҵ���,���ϵ���
#define  R2L_D2U  3 //���ҵ���,���µ���

#define  U2D_L2R  4 //���ϵ���,������
#define  U2D_R2L  5 //���ϵ���,���ҵ���
#define  D2U_L2R  6 //���µ���,������
#define  D2U_R2L  7 //���µ���,���ҵ���	 

//��ɫֵ����
#define  WHITE        0xFFFF
#define  BLACK        0x0000	
#define  RED          0xF800
#define  GREEN        0x07E0
#define  BLUE         0x001F 
#define  BRED         0XF81F
#define  GRED 			  0XFFE0
#define  GBLUE			  0X07FF
#define  BROWN 			  0XBC40  
#define  BRRED 			  0XFC07  
#define  GRAY  			  0X8430  
#define  MAGENTA      0xF81F
#define  CYAN         0x7FFF
#define  YELLOW       0xFFE0


void LCD_Init(void);
//void LCD_DisplayString(uint16_t x,uint16_t y,uint8_t size,uint8_t *p);
void LCD_DisplayString(uint16_t x,uint16_t y,uint8_t size,int color,uint8_t *p);
void LCD_Clear(uint16_t color);
void LCD_DrawPoint(uint16_t x,uint16_t y);
void LCD_Fill_onecolor(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);
uint16_t LCD_GetPoint(uint16_t x,uint16_t y);
void LCD_DrawPoint(uint16_t x,uint16_t y);
void LCD_Color_DrawPoint(uint16_t x,uint16_t y,uint16_t color);
//LCD������ʾ
void LCD_DisplayOn(void);

//LCD�ر���ʾ
void LCD_DisplayOff(void);
void LCD_SetCursor(uint16_t Xaddr, uint16_t Yaddr);

#endif

