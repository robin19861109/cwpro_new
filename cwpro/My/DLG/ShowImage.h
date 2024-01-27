/**
 ****************************************************************************************************
 * @file        emwin_demo.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin��PNGͼƬ��ʾʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#ifndef __SHOW_IMAGE_H
#define __SHOW_IMAGE_H
#include "main.h"
#include "GUI.h"
#include "WM.h"
#include "cmsis_os.h"
#define  CPU_SR_ALLOC()

/* ʹ��GUI_PNG_Draw()��������BMPͼƬ�Ļ�
   ͼƬ�Ǽ��ص�RAM�еģ���˲��ܴ���PNGMEMORYSIZE
   ע�⣺��ʾPNGͼƬʱ�ڴ�����ʹ�õ�EMWIN���ڴ����뺯�������
   PNGMEMORYSIZE���ܴ������Ǹ�EMWIN������ڴ�ش�С */
#define PNGMEMORYSIZE 500*1024 /*ͼƬ��С������500kb  */

/* ����������ص�RAM�е�PNGͼƬʱ��ͼƬÿ�е��ֽ��� */
#define PNGPERLINESIZE 5*1024 

void emwin_create_pngpicture(uint8_t *filename, int x0, int y0, int Xsize, int Ysize);
void ShowImage(void) ; /* emwin������� */

#endif
