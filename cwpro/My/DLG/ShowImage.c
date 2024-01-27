/**
 ****************************************************************************************************
 * @file        emwin_demo.c
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

#include "SelectImgWin.h"
#include "ShowImage.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

/*****************************************************************************************************/
/*EMWIN*/
#include "GUI.h"
#include "WM.h"
#include "DIALOG.h"
#include "WM.h"
#include "malloc.h"
#include "ff.h"
#include "my_lcd.h"
/*****************************************************************************************************/



static FIL PNGFile;

/*******************************************************************
*
*       Static functions
*
********************************************************************
*/
/*********************************************************************
*
*       PngGetData
*
* Function description
*   This routine is called by GUI_PNG_DrawEx(). The routine is responsible
*   for setting the data pointer to a valid data location with at least
*   one valid byte.
*
* Parameters:
*   p           - Pointer to application defined data.
*   NumBytesReq - Number of bytes requested.
*   ppData      - Pointer to data pointer. This pointer should be set to
*                 a valid location.
*   StartOfFile - If this flag is 1, the data pointer should be set to the
*                 beginning of the data stream.
*
* Return value:
*   Number of data bytes available.
*/
static int emwin_png_get_data(void *p, const U8 **ppData, unsigned NumBytesReq, U32 Off)
{
    CPU_SR_ALLOC();
    static int readaddress = 0;
    FIL *phFile;
    U8 *pData;
    UINT NumBytesRead;
	  int ret;

    pData = (U8 *)*ppData;
    phFile = (FIL *)p;

    /* �ƶ�ָ�뵽Ӧ�ö�ȡ��λ�� */
    if (Off == 1) readaddress = 0;
    else readaddress = Off;

#if SYS_SUPPORT_OS
	//GUI_X_Lock();
 //   taskENTER_CRITICAL(); /* �ٽ��� */
#endif
    f_lseek(phFile, readaddress);

    /* ��ȡ���ݵ��������� */
    ret = f_read(phFile, pData, NumBytesReq, &NumBytesRead);
		
#if SYS_SUPPORT_OS
  //GUI_X_Unlock();
//  taskEXIT_CRITICAL(); /* �˳��ٽ��� */
#endif
  if(NumBytesReq!=NumBytesRead)
		printf("err  emwin_png_get_data NumBytesReq=%d NumBytesRead=%d ret=%d\r\n",NumBytesReq,NumBytesRead,ret);
	else
		printf("succ  emwin_png_get_data NumBytesReq=%d NumBytesRead=%d ret=%d\r\n",NumBytesReq,NumBytesRead,ret);
  
  return NumBytesRead; /* ���ض�ȡ�����ֽ��� */
	
}

/**
 * @brief       �õ�һ���ļ���
 * @param       pname:�õ����ļ���
 * @retval      ��
 */
void emwin_emwinjpg_new_pathname(uint8_t *pname)
{
    static FIL ScrSortFile; /* ��Ļ��ͼ�ļ� */
    uint8_t res;
    uint16_t index = 0;

    while (index < 0XFFFF)
    {
        sprintf((char *)pname, "0:/SCREENSHORT/��Ļ��ͼ%05d.bmp", index);
        res = f_open(&ScrSortFile, (const TCHAR *)pname, FA_READ); /* ���Դ�����ļ� */

        if (res == FR_NO_FILE)
        {
            break;
        }
        else if (res == FR_OK)
        {
            f_close(&ScrSortFile);  /* ����򿪳ɹ����͹رյ� */
        }

        index++;
    }
}

/**
 * @brief       �˺�����GUI_BMP_Serialize()���ã��������ļ�д���ֽ�
 * @param       ��
 * @retval      ��
 */
static void emwin_write_byte_1_2file(U8 Data, void *p)
{
    UINT nWritten;
    f_write((FIL *)p, &Data, 1, &nWritten);
}
/**
 * @brief       ����һ��BMPͼƬ�����浽SD����,ʵ����Ļ��ͼ���ܣ�
 * @param       x0:����BMP�ļ���X��ʼλ��
 * @param       y0:����BMP�ļ���Y��ʼλ��
 * @param       Xsize:X�Ĵ�С
 * @param       Ysize:Y�Ĵ�С
 * @retval      ��
 */
void emwin_create_pngpicture(uint8_t *filename, int x0, int y0, int Xsize, int Ysize)
{
    static FIL hFile;
    GUI_COLOR forecolor, bkcolor;
    forecolor = GUI_GetColor();   /* ��ȡ��ǰǰ��ɫ */
    bkcolor = GUI_GetBkColor();   /* ��ȡ��ǰ����ɫ */
    /* ����һ���ļ���·��Ϊfilename,����ļ�ԭ���ʹ��ڵĻ��ᱻ�½����ļ����ǵ���*/
    f_open(&hFile,(const TCHAR*)filename,FA_READ | FA_WRITE | FA_CREATE_ALWAYS);
    GUI_BMP_SerializeEx(emwin_write_byte_1_2file,x0,y0,Xsize,Ysize,&hFile);
    f_close(&hFile);  /* �ر��ļ� */
    GUI_SetColor(forecolor);   /* �ָ�ǰ��ɫ */
    GUI_SetBkColor(bkcolor);   /* �ָ�����ɫ */
}

/**
 * @brief       ��ָ��λ����ʾ���ص�RAM�е�GIFͼƬ
                GIFFileName:ͼƬ��SD�����������洢�豸�е�·��(���ļ�ϵͳ֧�֣�)
 * @param       mode:��ʾģʽ
      @arg      0 ��ָ��λ����ʾ���в���x,yȷ����ʾλ��
      @arg      1 ��LCD�м���ʾͼƬ����ѡ���ģʽ��ʱ�����x,y��Ч
 * @param       x:ͼƬ���Ͻ���LCD�е�x��λ��(������modeΪ1ʱ���˲�����Ч)
 * @param       y:ͼƬ���Ͻ���LCD�е�y��λ��(������modeΪ1ʱ���˲�����Ч)
 * @param       member:  ���ű����ķ�����
 * @param       denom:���ű����ķ�ĸ��
 * @retval      0 ��ʾ����,���� ʧ��
 */
int emwin_displaypng(char *PNGFileName, uint8_t mode, uint32_t x, uint32_t y)
{
    CPU_SR_ALLOC();
    uint16_t bread;
    char *pngbuffer;
    char result;
    int XSize, YSize;
    int flag;
      GUI_DispStringHCenterAt("99999999999999", 4, 0);
    result = f_open(&PNGFile, (const TCHAR *)PNGFileName, FA_READ); /* ���ļ� */

    /* �ļ��򿪴�������ļ�����JPEGMEMORYSIZE */
    if ((result != FR_OK) || (PNGFile.obj.objsize > PNGMEMORYSIZE))
    {
        printf("�ļ��򿪴���\r\n");
        return 1;
    }

    pngbuffer = Mem_malloc(EXSRAM, PNGFile.obj.objsize);

    if (pngbuffer == NULL)
    {
        printf("�����ڴ�ʧ��\r\n");
        return 2;
    }


#if SYS_SUPPORT_OS
//		taskENTER_CRITICAL();
		
   GUI_X_Lock(); /* �ٽ��� */
#endif

    result = f_read(&PNGFile, pngbuffer, PNGFile.obj.objsize, (UINT *)&bread); /* ��ȡ���� */

    if (result != FR_OK)
    {
        printf("���ļ�ʧ��\r\n");
        return 3;
    }

#if SYS_SUPPORT_OS
//		taskEXIT_CRITICAL();
    GUI_X_Unlock(); /* �˳��ٽ��� */
#endif

    XSize = GUI_PNG_GetXSize(pngbuffer, PNGFile.obj.objsize); /* ��ȡPNGͼƬ��X���С */
    YSize = GUI_PNG_GetYSize(pngbuffer, PNGFile.obj.objsize); /* ��ȡPNGͼƬ��Y���С */

    switch (mode)
    {
        case 0: /* ��ָ��λ����ʾͼƬ */
            flag = GUI_PNG_Draw(pngbuffer, PNGFile.obj.objsize, x, y); /* ��ʾPNGͼƬ */
            if (flag != 0)
            {
                printf("��ʾPNGͼƬʧ��\r\n");
            }
            break;

        case 1: /* ��LCD�м���ʾͼƬ */
            flag = GUI_PNG_Draw(pngbuffer, PNGFile.obj.objsize, (lcd_width - XSize) / 2 - 1, (lcd_width - YSize) / 2 - 1);
            if (flag != 0)
            {
                printf("��ʾPNGͼƬʧ��\r\n");
            }
            break;
    }

    f_close(&PNGFile); /* �ر�PNGFile�ļ� */
    Mem_free(EXSRAM, pngbuffer);
    return 0;
}

/**
 * @brief       ָ��λ����ʾ������ص�RAM�е�GIFͼƬ(���ļ�ϵͳ֧�֣�����СRAM���Ƽ�ʹ�ô˷�����)
                GIFFileName:ͼƬ��SD�����������洢�豸�е�·��
 * @param       mode:��ʾģʽ
      @arg      0 ��ָ��λ����ʾ���в���x,yȷ����ʾλ��
      @arg      1 ��LCD�м���ʾͼƬ����ѡ���ģʽ��ʱ�����x,y��Ч
 * @param       x:ͼƬ���Ͻ���LCD�е�x��λ��(������modeΪ1ʱ���˲�����Ч)
 * @param       y:ͼƬ���Ͻ���LCD�е�y��λ��(������modeΪ1ʱ���˲�����Ч)
 * @param       member:  ���ű����ķ�����
 * @param       denom:���ű����ķ�ĸ��
 * @retval      0 ��ʾ����,���� ʧ��
 */
int emwin_displaypngex(char *PNGFileName, uint8_t mode, uint32_t x, uint32_t y)
{
    char result;
    int XSize, YSize;

    result = f_open(&PNGFile, (const TCHAR *)PNGFileName, FA_READ); /* ���ļ� */

    /* �ļ��򿪴��� */
    if (result != FR_OK) return 1;
	  printf("2aaaaaaaaaaaaaaaaaaaa\r\n");

    XSize = GUI_PNG_GetXSizeEx(emwin_png_get_data, &PNGFile); /* PNGͼƬX��С */
		  printf("3aaaaaaaaaaaaaaaaaaaa\r\n");
    YSize = GUI_PNG_GetYSizeEx(emwin_png_get_data, &PNGFile); /* PNGͼƬY��С */
		  printf("4aaaaaaaaaaaaaaaaaaaa\r\n");

    switch (mode)
    {
        case 0: /* ��ָ��λ����ʾͼƬ */
            GUI_PNG_DrawEx(emwin_png_get_data, &PNGFile, x, y);
            break;

        case 1: /* ��LCD�м���ʾͼƬ */
            GUI_PNG_DrawEx(emwin_png_get_data, &PNGFile, (lcd_width - XSize) / 2 - 1, (lcd_height - YSize) / 2 - 1);
            break;
    }

    f_close(&PNGFile); /* �ر�PNGFile�ļ� */
    return 0;
}

/**
 * @brief       atk_emwin_main(emwin���)
 * @param       ��
 * @return      ��
 */
void ShowImage(void) 
{
    GUI_SetBkColor(GUI_BLUE);
   // GUI_SetFont(&GUI_FontHZ16);
    GUI_SetColor(GUI_RED);
    GUI_Clear();

    while (1)
    {
        GUI_DispStringHCenterAt("99999999999999", 4, 0);
			  printf("1aaaaaaaaaa\r\n");
        emwin_displaypng("0:/PICTURE/PNG/�ʹ�.png", 1, 200, 100);
        GUI_Delay(1000);
			  GUI_Clear();

        emwin_displaypng("0:/PICTURE/PNG/�㽶.png", 1, 0, 0);
        GUI_Delay(1000);
        GUI_Clear();

        GUI_DispStringHCenterAt("888888888888", 4, 0);

        emwin_displaypng("0:/PICTURE/PNG/�ļ���.png", 1, 0, 0);
        GUI_Delay(1000);
        GUI_Clear();
	

    }
}
