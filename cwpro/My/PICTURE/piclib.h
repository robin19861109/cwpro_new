/**
 ****************************************************************************************************
 * @file        piclib.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       ͼƬ����� ����
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200404
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __PICLIB_H
#define __PICLIB_H

#include "lcd.h"
#include "malloc.h"

#include "exfuns.h"

#include "bmp.h"
#include "gif.h"
#include "tjpgd.h"
#include "png.h"
#include "ff.h"


#define PIC_FORMAT_ERR      0x27    /* ��ʽ���� */
#define PIC_SIZE_ERR        0x28    /* ͼƬ�ߴ���� */
#define PIC_WINDOW_ERR      0x29    /* �����趨���� */
#define PIC_MEM_ERR         0x11    /* �ڴ���� */

/* �ж� TRUE �� FALSE �Ƿ��Ѿ�������?, û����Ҫ����! */
#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif

/* ͼƬ��ʾ�����ӿ� */
/* ����ֲ��ʱ��,�������û��Լ�ʵ���⼸������ */
typedef struct
{
    /* uint32_t read_point(uint16_t x,uint16_t y) ���㺯�� */
    uint32_t(*read_point)(uint16_t, uint16_t);
    
    /* void draw_point(uint16_t x,uint16_t y,uint32_t color) ���㺯�� */
    void(*draw_point)(uint16_t, uint16_t, uint32_t);
    
    /* void fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color) ��ɫ��亯�� */
    void(*fill)(uint16_t, uint16_t, uint16_t, uint16_t, uint32_t);
    
    /* void draw_hline(uint16_t x0,uint16_t y0,uint16_t len,uint16_t color) ��ˮƽ�ߺ��� */
    void(*draw_hline)(uint16_t, uint16_t, uint16_t, uint16_t);
    
    /* void piclib_fill_color(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t *color) ��ɫ��� */
    void(*fillcolor)(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t *);
} _pic_phy;

extern _pic_phy pic_phy;


/* ͼ����Ϣ */
typedef struct
{
    uint16_t lcdwidth;      /* LCD�Ŀ�� */
    uint16_t lcdheight;     /* LCD�ĸ߶� */
    uint32_t ImgWidth;      /* ͼ���ʵ�ʿ�Ⱥ͸߶� */
    uint32_t ImgHeight;

    uint32_t Div_Fac;       /* ����ϵ�� (������8192����) */

    uint32_t S_Height;      /* �趨�ĸ߶ȺͿ�� */
    uint32_t S_Width;

    uint32_t S_XOFF;        /* x���y���ƫ���� */
    uint32_t S_YOFF;

    uint32_t staticx;       /* ��ǰ��ʾ���ģ������� */
    uint32_t staticy;
} _pic_info;

extern _pic_info picinfo;   /* ͼ����Ϣ */


/* ͼƬ����� �ӿں��� */


void piclib_init(void);                 /* ��ʼ����ͼ */
void piclib_ai_draw_init(void);         /* ��ʼ�����ܻ�ͼ */
uint16_t piclib_alpha_blend(uint16_t src, uint16_t dst, uint8_t alpha);     /* alphablend���� */
uint8_t piclib_is_element_ok(uint16_t x, uint16_t y, uint8_t chg); /* �ж������Ƿ���Ч */
uint8_t piclib_ai_load_picfile(char *filename, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t fast); /* ���ܻ�ͼ */
DWORD clmt_clust (	/* <2:Error, >=2:Cluster number */
	FIL* fp,		/* Pointer to the file object */
	FSIZE_t ofs		/* File offset to be converted to cluster# */
);
#endif






























