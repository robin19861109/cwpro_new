/**
 ****************************************************************************************************
 * @file        emwin_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-04
 * @brief       emWin�������ʵ��
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

#include "enterImgPwd.h"
#include "my_lcd.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include <stddef.h>
#include "string.h"
#include "stdlib.h"
/*****************************************************************************************************/
/*EMWIN*/
#include "GUI.h"
#include "WM.h"
#include "DIALOG.h"
#include "common.h"

/*****************************************************************************************************/



WM_HWIN hEdit_Pwd = 0;
WM_HWIN hText = 0;
WM_HWIN hButton_OK;
WM_HWIN hButton_Cancel;
int status_pwd = 0;
KEYPADStructTypeDef keypad_dev;
static int status=0;
static int _DrawSkinFlex_BUTTON(const WIDGET_ITEM_DRAW_INFO *pDrawItemInfo);
static int _DrawChineseSkin_BUTTON(const WIDGET_ITEM_DRAW_INFO *pDrawItemInfo);

typedef struct
{
    int          xPos;              /* ��ťXλ�� */
    int          yPos;              /* ��ťYλ�� */
    int          xSize;             /* ��ť��� */
    int          ySize;             /* ��ť�߶� */
    const char *acLabel;            /* ��ť��������ַ� */
    void (* pfDraw)(WM_HWIN hWin);  /* ��ť�Ļ��ƺ��� */
    const char *ButString;          /* ��ť����ʾ���ַ� */
} BUTTON_DATA;


/* �ڰ�ť�ϻ���λͼ������س�����ɾ������ */
static void _DrawCentered(WM_HWIN hWin, const GUI_BITMAP *pBM)
{
    int xSizeWin;
    int ySizeWin;
    int xSizeBMP;
    int ySizeBMP;
    int xPos;
    int yPos;

    xSizeWin = WM_GetWindowSizeX(hWin);
    ySizeWin = WM_GetWindowSizeY(hWin);
    xSizeBMP = pBM->XSize;
    ySizeBMP = pBM->YSize;
    xPos = (xSizeWin - xSizeBMP) >> 1;
    yPos = (ySizeWin - ySizeBMP) >> 1;
    GUI_DrawBitmap(pBM, xPos, yPos);
}

static void _DrawBkSpc (WM_HWIN hWin)
{
    _DrawCentered(hWin, &bmBackSpace); /* �����˸��(ɾ����) */
}
static void _DrawEnter (WM_HWIN hWin)
{
    _DrawCentered(hWin, &bmEnter);     /* ���ƻس��� */
}
static void _DrawShift (WM_HWIN hWin)
{
    _DrawCentered(hWin, &bmShift);    /* ����shift�� */
}
static void _DrawRetern(WM_HWIN hWin)
{
    _DrawCentered(hWin, &bmReturn);   /* ���Ʒ��ؼ� */
}


/* Ӣ�ļ���λ�� */
static  BUTTON_DATA _aEngButtonData[] =
{
    { (ENG_BUTTON_XPAC * 1 + ENG_BUTTON_WIDTH * 0 + ENG_BUTTONLINE1_XOFFSET),  ENG_BUTTON_LINE1YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "Q", 0, "Q" },
    { (ENG_BUTTON_XPAC * 2 + ENG_BUTTON_WIDTH * 1 + ENG_BUTTONLINE1_XOFFSET),  ENG_BUTTON_LINE1YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "W", 0, "W" },
    { (ENG_BUTTON_XPAC * 3 + ENG_BUTTON_WIDTH * 2 + ENG_BUTTONLINE1_XOFFSET),  ENG_BUTTON_LINE1YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "E", 0, "E" },
    { (ENG_BUTTON_XPAC * 4 + ENG_BUTTON_WIDTH * 3 + ENG_BUTTONLINE1_XOFFSET),  ENG_BUTTON_LINE1YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "R", 0, "R" },
    { (ENG_BUTTON_XPAC * 5 + ENG_BUTTON_WIDTH * 4 + ENG_BUTTONLINE1_XOFFSET),  ENG_BUTTON_LINE1YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "T", 0, "T" },
    { (ENG_BUTTON_XPAC * 6 + ENG_BUTTON_WIDTH * 5 + ENG_BUTTONLINE1_XOFFSET),  ENG_BUTTON_LINE1YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "Y", 0, "Y" },
    { (ENG_BUTTON_XPAC * 7 + ENG_BUTTON_WIDTH * 6 + ENG_BUTTONLINE1_XOFFSET),  ENG_BUTTON_LINE1YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "U", 0, "U" },
    { (ENG_BUTTON_XPAC * 8 + ENG_BUTTON_WIDTH * 7 + ENG_BUTTONLINE1_XOFFSET),  ENG_BUTTON_LINE1YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "I", 0, "I" },
    { (ENG_BUTTON_XPAC * 9 + ENG_BUTTON_WIDTH * 8 + ENG_BUTTONLINE1_XOFFSET),  ENG_BUTTON_LINE1YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "O", 0, "O" },
    { (ENG_BUTTON_XPAC * 10 + ENG_BUTTON_WIDTH * 9 + ENG_BUTTONLINE1_XOFFSET), ENG_BUTTON_LINE1YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "P", 0, "P" },

    { (ENG_BUTTON_XPAC * 1 + ENG_BUTTON_WIDTH * 0 + ENG_BUTTONLINE2_XOFFSET),  ENG_BUTTON_LINE2YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "A", 0, "A" },
    { (ENG_BUTTON_XPAC * 2 + ENG_BUTTON_WIDTH * 1 + ENG_BUTTONLINE2_XOFFSET),  ENG_BUTTON_LINE2YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "S", 0, "S" },
    { (ENG_BUTTON_XPAC * 3 + ENG_BUTTON_WIDTH * 2 + ENG_BUTTONLINE2_XOFFSET),  ENG_BUTTON_LINE2YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "D", 0, "D" },
    { (ENG_BUTTON_XPAC * 4 + ENG_BUTTON_WIDTH * 3 + ENG_BUTTONLINE2_XOFFSET),  ENG_BUTTON_LINE2YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "F", 0, "F" },
    { (ENG_BUTTON_XPAC * 5 + ENG_BUTTON_WIDTH * 4 + ENG_BUTTONLINE2_XOFFSET),  ENG_BUTTON_LINE2YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "G", 0, "G" },
    { (ENG_BUTTON_XPAC * 6 + ENG_BUTTON_WIDTH * 5 + ENG_BUTTONLINE2_XOFFSET),  ENG_BUTTON_LINE2YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "H", 0, "H" },
    { (ENG_BUTTON_XPAC * 7 + ENG_BUTTON_WIDTH * 6 + ENG_BUTTONLINE2_XOFFSET),  ENG_BUTTON_LINE2YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "J", 0, "J" },
    { (ENG_BUTTON_XPAC * 8 + ENG_BUTTON_WIDTH * 7 + ENG_BUTTONLINE2_XOFFSET),  ENG_BUTTON_LINE2YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "K", 0, "K" },
    { (ENG_BUTTON_XPAC * 9 + ENG_BUTTON_WIDTH * 8 + ENG_BUTTONLINE2_XOFFSET),  ENG_BUTTON_LINE2YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "L", 0, "L" },

    { (ENG_BUTTON_XPAC * 1 + ENG_BUTTON_WIDTH * 0 + ENG_BUTTONLINE3_XOFFSET),  ENG_BUTTON_LINE3YPOS, ENG_BUTTON_WIDTH + ENG_BUTTONSHIFT_XINC, ENG_BUTTON_HEIGHT, "\x19", _DrawShift }, /* shift�ļ�ֵΪ0X19   */
    { (ENG_BUTTON_XPAC * 2 + ENG_BUTTON_WIDTH * 1 + ENG_BUTTONLINE3_XOFFSET + ENG_BUTTONSHIFT_XINC),  ENG_BUTTON_LINE3YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "Z", 0, "Z" },
    { (ENG_BUTTON_XPAC * 3 + ENG_BUTTON_WIDTH * 2 + ENG_BUTTONLINE3_XOFFSET + ENG_BUTTONSHIFT_XINC),  ENG_BUTTON_LINE3YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "X", 0, "X" },
    { (ENG_BUTTON_XPAC * 4 + ENG_BUTTON_WIDTH * 3 + ENG_BUTTONLINE3_XOFFSET + ENG_BUTTONSHIFT_XINC),  ENG_BUTTON_LINE3YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "C", 0, "C" },
    { (ENG_BUTTON_XPAC * 5 + ENG_BUTTON_WIDTH * 4 + ENG_BUTTONLINE3_XOFFSET + ENG_BUTTONSHIFT_XINC),  ENG_BUTTON_LINE3YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "V", 0, "V" },
    { (ENG_BUTTON_XPAC * 6 + ENG_BUTTON_WIDTH * 5 + ENG_BUTTONLINE3_XOFFSET + ENG_BUTTONSHIFT_XINC),  ENG_BUTTON_LINE3YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "B", 0, "B" },
    { (ENG_BUTTON_XPAC * 7 + ENG_BUTTON_WIDTH * 6 + ENG_BUTTONLINE3_XOFFSET + ENG_BUTTONSHIFT_XINC),  ENG_BUTTON_LINE3YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "N", 0, "N" },
    { (ENG_BUTTON_XPAC * 8 + ENG_BUTTON_WIDTH * 7 + ENG_BUTTONLINE3_XOFFSET + ENG_BUTTONSHIFT_XINC),  ENG_BUTTON_LINE3YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, "M", 0, "M" },
    { (ENG_BUTTON_XPAC * 9 + ENG_BUTTON_WIDTH * 8 + ENG_BUTTONLINE3_XOFFSET + ENG_BUTTONSHIFT_XINC),  ENG_BUTTON_LINE3YPOS, ENG_BUTTON_WIDTH + ENG_BUTTONENTER_XINC, ENG_BUTTON_HEIGHT, "\x08", _DrawBkSpc},

    { (ENG_BUTTON_XPAC * 1 + ENG_BUTTON_WIDTH * 0 + ENG_BUTTONLINE4_XOFFSET),  ENG_BUTTON_LINE4YPOS, ENG_BUTTON_WIDTH + ENG_BUTTONFIGURE_XINC, ENG_BUTTON_HEIGHT, "\x02", 0, "123"},                          /* �л�������� */
    { (ENG_BUTTON_XPAC * 2 + ENG_BUTTON_WIDTH * 1 + ENG_BUTTONLINE4_XOFFSET + ENG_BUTTONFIGURE_XINC),  ENG_BUTTON_LINE4YPOS, ENG_BUTTON_WIDTH + ENG_BUTTONSYMBOL_XINC, ENG_BUTTON_HEIGHT,  "   ", 0, " ~ "}, /* �л����ż��� */
    { (ENG_BUTTON_XPAC * 3 + ENG_BUTTON_WIDTH * 2 + ENG_BUTTONLINE4_XOFFSET + ENG_BUTTONFIGURE_XINC + ENG_BUTTONSYMBOL_XINC),  ENG_BUTTON_LINE4YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT,  ",", 0, "," },
    { (ENG_BUTTON_XPAC * 4 + ENG_BUTTON_WIDTH * 3 + ENG_BUTTONLINE4_XOFFSET + ENG_BUTTONFIGURE_XINC + ENG_BUTTONSYMBOL_XINC),  ENG_BUTTON_LINE4YPOS, ENG_BUTTON_WIDTH + ENG_BUTTONSPACE_XINC, ENG_BUTTON_HEIGHT,  " ", 0, "SPACE"},
    { (ENG_BUTTON_XPAC * 5 + ENG_BUTTON_WIDTH * 4 + ENG_BUTTONLINE4_XOFFSET + ENG_BUTTONFIGURE_XINC + ENG_BUTTONSYMBOL_XINC + ENG_BUTTONSPACE_XINC),  ENG_BUTTON_LINE4YPOS, ENG_BUTTON_WIDTH, ENG_BUTTON_HEIGHT, ".", 0, "."},
   // { (ENG_BUTTON_XPAC * 6 + ENG_BUTTON_WIDTH * 5 + ENG_BUTTONLINE4_XOFFSET + ENG_BUTTONFIGURE_XINC + ENG_BUTTONSYMBOL_XINC + ENG_BUTTONSPACE_XINC),  ENG_BUTTON_LINE4YPOS, ENG_BUTTON_WIDTH + ENG_BUTTONCE_XINC, ENG_BUTTON_HEIGHT, "\x0B", 0, "��"}, /* �л������ļ���  */
    { (ENG_BUTTON_XPAC * 6 + ENG_BUTTON_WIDTH * 5 + ENG_BUTTONLINE4_XOFFSET + ENG_BUTTONFIGURE_XINC + ENG_BUTTONSYMBOL_XINC + ENG_BUTTONSPACE_XINC ),  ENG_BUTTON_LINE4YPOS, ENG_BUTTON_WIDTH + ENG_BUTTONBACK_XINC, ENG_BUTTON_HEIGHT, "\x0d", _DrawEnter},
};
//    int          xPos;              /* ��ťXλ�� */
//    int          yPos;              /* ��ťYλ�� */
//   int          xSize;             /* ��ť��� */
//    int          ySize;             /* ��ť�߶� */
//    const char *acLabel;            /* ��ť��������ַ� */
//    void (* pfDraw)(WM_HWIN hWin);  /* ��ť�Ļ��ƺ��� */
 //   const char *ButString;          /* ��ť����ʾ���ַ� */
/* ���ּ���λ�� */
static  BUTTON_DATA _aNumButtonData[] = //NUM_BUTTONBACKSPACE_XINC
{
    { (NUM_BUTTON_XPAC * 1 + NUM_BUTTON_WIDTH * 0 + NUM_BUTTON_XOFFSET),  NUM_BUTTON_LINE1YPOS, (NUM_BUTTON_WIDTH - NUM_BUTTONSING_XINC), NUM_BUTTON_HEIGHT, "+", 0, "+" },
    { (NUM_BUTTON_XPAC * 2 + NUM_BUTTON_WIDTH * 1 + NUM_BUTTON_XOFFSET - NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE1YPOS, NUM_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "1", 0, "1"},
    { (NUM_BUTTON_XPAC * 3 + NUM_BUTTON_WIDTH * 2 + NUM_BUTTON_XOFFSET - NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE1YPOS, NUM_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "2", 0, "2"},
    { (NUM_BUTTON_XPAC * 4 + NUM_BUTTON_WIDTH * 3 + NUM_BUTTON_XOFFSET - NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE1YPOS, NUM_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "3", 0, "3"},
    { (NUM_BUTTON_XPAC * 5 + NUM_BUTTON_WIDTH * 4 + NUM_BUTTON_XOFFSET - NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE1YPOS, (NUM_BUTTON_WIDTH + NUM_BUTTONBACKSPACE_XINC), NUM_BUTTON_HEIGHT, "\x08", _DrawBkSpc}, /* ɾ���� */

    { (NUM_BUTTON_XPAC * 1 + NUM_BUTTON_WIDTH * 0 + NUM_BUTTON_XOFFSET),  NUM_BUTTON_LINE2YPOS, (NUM_BUTTON_WIDTH - NUM_BUTTONSING_XINC), NUM_BUTTON_HEIGHT, "-", 0, "-" },
    { (NUM_BUTTON_XPAC * 2 + NUM_BUTTON_WIDTH * 1 + NUM_BUTTON_XOFFSET - NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE2YPOS, NUM_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "4", 0, "4"},
    { (NUM_BUTTON_XPAC * 3 + NUM_BUTTON_WIDTH * 2 + NUM_BUTTON_XOFFSET - NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE2YPOS, NUM_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "5", 0, "5"},
    { (NUM_BUTTON_XPAC * 4 + NUM_BUTTON_WIDTH * 3 + NUM_BUTTON_XOFFSET - NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE2YPOS, NUM_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "6", 0, "6"},
    { (NUM_BUTTON_XPAC * 5 + NUM_BUTTON_WIDTH * 4 + NUM_BUTTON_XOFFSET - NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE2YPOS, (NUM_BUTTON_WIDTH - NUM_BUTTONSPACE_XINC),  NUM_BUTTON_HEIGHT,"  ", 0, "SP"}, /* �ո��  */

    { (NUM_BUTTON_XPAC * 1 + NUM_BUTTON_WIDTH * 0 + NUM_BUTTON_XOFFSET),  NUM_BUTTON_LINE3YPOS, (NUM_BUTTON_WIDTH - NUM_BUTTONSING_XINC), NUM_BUTTON_HEIGHT, "/", 0, "/" },
    { (NUM_BUTTON_XPAC * 2 + NUM_BUTTON_WIDTH * 1 + NUM_BUTTON_XOFFSET - NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE3YPOS, NUM_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "7", 0, "7"},
    { (NUM_BUTTON_XPAC * 3 + NUM_BUTTON_WIDTH * 2 + NUM_BUTTON_XOFFSET - NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE3YPOS, NUM_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "8", 0, "8"},
    { (NUM_BUTTON_XPAC * 4 + NUM_BUTTON_WIDTH * 3 + NUM_BUTTON_XOFFSET - NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE3YPOS, NUM_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "9", 0, "9"},
		
    { (NUM_BUTTON_XPAC * 5 + NUM_BUTTON_WIDTH * 4 + NUM_BUTTON_XOFFSET - NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE3YPOS, (NUM_BUTTON_WIDTH + NUM_BUTTONENTER_XINC), NUM_BUTTON_HEIGHT, "\x0d", _DrawEnter}, /* �س��� */


    { (NUM_BUTTON_XPAC * 1 + NUM_BUTTON_WIDTH * 0 + NUM_BUTTON_XOFFSET),  NUM_BUTTON_LINE4YPOS, (NUM_BUTTON_WIDTH - NUM_BUTTONSING_XINC), NUM_BUTTON_HEIGHT, "@", 0, "@" },
    { (NUM_BUTTON_XPAC * 2 + NUM_BUTTON_WIDTH * 1 + NUM_BUTTON_XOFFSET - NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE4YPOS, NUM_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, ",", 0, ","},
    { (NUM_BUTTON_XPAC * 3 + NUM_BUTTON_WIDTH * 2 + NUM_BUTTON_XOFFSET - NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE4YPOS, NUM_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "0", 0, "0"},
    { (NUM_BUTTON_XPAC * 4 + NUM_BUTTON_WIDTH * 3 + NUM_BUTTON_XOFFSET - NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE4YPOS, NUM_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, ".", 0, "."},
    { (NUM_BUTTON_XPAC * 5 + NUM_BUTTON_WIDTH * 4 + NUM_BUTTON_XOFFSET - NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE4YPOS, (NUM_BUTTON_WIDTH+NUM_BUTTONRETURN_XINC), NUM_BUTTON_HEIGHT, "\x04", _DrawRetern }, /* ���ؼ� */
		
 //   { (NUM_BUTTON_XPAC * 5 + NUM_BUTTON_WIDTH * 4 + NUM_BUTTON_XOFFSET - NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE4YPOS, (NUM_BUTTON_WIDTH - NUM_BUTTONRETURN_XINC), NUM_BUTTON_HEIGHT,  "3", 0, "3"},//"\x04", _DrawRetern }, /* ���ؼ� */
};

/* ���ż��̲���*/
static  BUTTON_DATA _aSinButtonData[][20] =
{
    {
        { (SIGN_BUTTON_XPAC * 1 + SING_BUTTON_WIDTH * 0 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE1YPOS, SING_BUTTON_WIDTH, SIGN_BUTTON_HEIGHT, "~", 0, "~"},
        { (SIGN_BUTTON_XPAC * 2 + SING_BUTTON_WIDTH * 1 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE1YPOS, SING_BUTTON_WIDTH, SIGN_BUTTON_HEIGHT, "-", 0, "-"},
        { (SIGN_BUTTON_XPAC * 3 + SING_BUTTON_WIDTH * 2 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE1YPOS, SING_BUTTON_WIDTH, SIGN_BUTTON_HEIGHT, ":", 0, ":"},
        { (SIGN_BUTTON_XPAC * 4 + SING_BUTTON_WIDTH * 3 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE1YPOS, SING_BUTTON_WIDTH, SIGN_BUTTON_HEIGHT, "_", 0, "_"},
        { (SIGN_BUTTON_XPAC * 5 + SING_BUTTON_WIDTH * 4 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE1YPOS, (SING_BUTTON_WIDTH - SIGN_BUTTONCONTR_XINC), SIGN_BUTTON_HEIGHT, "\x08", _DrawBkSpc}, /* ɾ����  */

        { (SIGN_BUTTON_XPAC * 1 + SING_BUTTON_WIDTH * 0 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE2YPOS, SING_BUTTON_WIDTH, SIGN_BUTTON_HEIGHT, "\"", 0, "\"" },
        { (SIGN_BUTTON_XPAC * 2 + SING_BUTTON_WIDTH * 1 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE2YPOS, SING_BUTTON_WIDTH, SIGN_BUTTON_HEIGHT, ",", 0, ","},
        { (SIGN_BUTTON_XPAC * 3 + SING_BUTTON_WIDTH * 2 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE2YPOS, SING_BUTTON_WIDTH, SIGN_BUTTON_HEIGHT, "<", 0, "<"},
        { (SIGN_BUTTON_XPAC * 4 + SING_BUTTON_WIDTH * 3 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE2YPOS, SING_BUTTON_WIDTH, SIGN_BUTTON_HEIGHT, ">", 0, ">"},
        { (SIGN_BUTTON_XPAC * 5 + SING_BUTTON_WIDTH * 4 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE2YPOS, (SING_BUTTON_WIDTH - SIGN_BUTTONCONTR_XINC), SIGN_BUTTON_HEIGHT,  "\x05", 0, "��"}, /* ��һҳ */

        { (SIGN_BUTTON_XPAC * 1 + SING_BUTTON_WIDTH * 0 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE3YPOS, SING_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "*", 0, "*" },
        { (SIGN_BUTTON_XPAC * 2 + SING_BUTTON_WIDTH * 1 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE3YPOS, SING_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "+", 0, "+" },
        { (SIGN_BUTTON_XPAC * 3 + SING_BUTTON_WIDTH * 2 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE3YPOS, SING_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, ".", 0, "." },
        { (SIGN_BUTTON_XPAC * 4 + SING_BUTTON_WIDTH * 3 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE3YPOS, SING_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "#", 0, "#" },
        { (SIGN_BUTTON_XPAC * 5 + SING_BUTTON_WIDTH * 4 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE3YPOS, (SING_BUTTON_WIDTH - SIGN_BUTTONCONTR_XINC), SIGN_BUTTON_HEIGHT, "\x06", 0, "��"}, /* ��һҳ */
        
        { (SIGN_BUTTON_XPAC * 1 + SING_BUTTON_WIDTH * 0 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE4YPOS, SING_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "%", 0, "%"},
        { (SIGN_BUTTON_XPAC * 2 + SING_BUTTON_WIDTH * 1 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE4YPOS, SING_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "@", 0, "@" },
        { (SIGN_BUTTON_XPAC * 3 + SING_BUTTON_WIDTH * 2 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE4YPOS, SING_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "!", 0, "!" },
        { (SIGN_BUTTON_XPAC * 4 + SING_BUTTON_WIDTH * 3 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE4YPOS, SING_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "/", 0, "/" },
        { (SIGN_BUTTON_XPAC * 5 + SING_BUTTON_WIDTH * 4 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE4YPOS, (SING_BUTTON_WIDTH - SIGN_BUTTONCONTR_XINC), SIGN_BUTTON_HEIGHT, "\x04", _DrawRetern }, /* ���ؼ� */
    },

    {
        { (SIGN_BUTTON_XPAC * 1 + SING_BUTTON_WIDTH * 0 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE1YPOS, SING_BUTTON_WIDTH, SIGN_BUTTON_HEIGHT, "?", 0, "?" },
        { (SIGN_BUTTON_XPAC * 2 + SING_BUTTON_WIDTH * 1 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE1YPOS, SING_BUTTON_WIDTH, SIGN_BUTTON_HEIGHT, ";", 0, ";" },
        { (SIGN_BUTTON_XPAC * 3 + SING_BUTTON_WIDTH * 2 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE1YPOS, SING_BUTTON_WIDTH, SIGN_BUTTON_HEIGHT, "`", 0, "`" },
        { (SIGN_BUTTON_XPAC * 4 + SING_BUTTON_WIDTH * 3 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE1YPOS, SING_BUTTON_WIDTH, SIGN_BUTTON_HEIGHT, "$", 0, "$" },
        { (SIGN_BUTTON_XPAC * 5 + SING_BUTTON_WIDTH * 4 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE1YPOS, (SING_BUTTON_WIDTH - SIGN_BUTTONCONTR_XINC), SIGN_BUTTON_HEIGHT, "\x08", _DrawBkSpc},	/* ɾ����  */

        { (SIGN_BUTTON_XPAC * 1 + SING_BUTTON_WIDTH * 0 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE2YPOS, SING_BUTTON_WIDTH, SIGN_BUTTON_HEIGHT, "^", 0, "^"},
        { (SIGN_BUTTON_XPAC * 2 + SING_BUTTON_WIDTH * 1 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE2YPOS, SING_BUTTON_WIDTH, SIGN_BUTTON_HEIGHT, "\\", 0, "\\" },
        { (SIGN_BUTTON_XPAC * 3 + SING_BUTTON_WIDTH * 2 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE2YPOS, SING_BUTTON_WIDTH, SIGN_BUTTON_HEIGHT, "(", 0, "("},
        { (SIGN_BUTTON_XPAC * 4 + SING_BUTTON_WIDTH * 3 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE2YPOS, SING_BUTTON_WIDTH, SIGN_BUTTON_HEIGHT, ")", 0, ")"},
        { (SIGN_BUTTON_XPAC * 5 + SING_BUTTON_WIDTH * 4 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE2YPOS, (SING_BUTTON_WIDTH - SIGN_BUTTONCONTR_XINC), SIGN_BUTTON_HEIGHT,  "\x05", 0, "��" },	/* ��һҳ  */

        { (SIGN_BUTTON_XPAC * 1 + SING_BUTTON_WIDTH * 0 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE3YPOS, SING_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "&", 0, "&" },
        { (SIGN_BUTTON_XPAC * 2 + SING_BUTTON_WIDTH * 1 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE3YPOS, SING_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "=", 0, "=" },
        { (SIGN_BUTTON_XPAC * 3 + SING_BUTTON_WIDTH * 2 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE3YPOS, SING_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "'", 0, "'" },
        { (SIGN_BUTTON_XPAC * 4 + SING_BUTTON_WIDTH * 3 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE3YPOS, SING_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "|", 0, "|" },
        { (SIGN_BUTTON_XPAC * 5 + SING_BUTTON_WIDTH * 4 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE3YPOS, (SING_BUTTON_WIDTH - SIGN_BUTTONCONTR_XINC), SIGN_BUTTON_HEIGHT, "\x06", 0, "��"},	/* ��һҳ */

        { (SIGN_BUTTON_XPAC * 1 + SING_BUTTON_WIDTH * 0 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE4YPOS, SING_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "[", 0, "[" },
        { (SIGN_BUTTON_XPAC * 2 + SING_BUTTON_WIDTH * 1 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE4YPOS, SING_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "]", 0, "]" },
        { (SIGN_BUTTON_XPAC * 3 + SING_BUTTON_WIDTH * 2 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE4YPOS, SING_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "{", 0, "{" },
        { (SIGN_BUTTON_XPAC * 4 + SING_BUTTON_WIDTH * 3 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE4YPOS, SING_BUTTON_WIDTH, NUM_BUTTON_HEIGHT, "}", 0, "}" },
        { (SIGN_BUTTON_XPAC * 5 + SING_BUTTON_WIDTH * 4 + SIGN_BUTTON_XOFFSET),  SIGN_BUTTON_LINE4YPOS, (SING_BUTTON_WIDTH - SIGN_BUTTONCONTR_XINC), SIGN_BUTTON_HEIGHT, "\x04", _DrawRetern }, /* ���ؼ�  */
    },
};

/* ���ļ���1λ�� */
static  BUTTON_DATA _aChineseButtonData1[] =
{
    /* ���ļ��̵�һ���� */
    { (CHIN1_BUTTON_XPAC * 1 + CHIN1_BUTTON_WIDTH * 0 + CHIN1_BUTTON_XOFFSET),  CHIN1_BUTTON_LINE1YPOS, CHIN1_BUTTON_WIDTH, CHIN1_BUTTON_HEIGHT, ";", 0, ";" },
    { (CHIN1_BUTTON_XPAC * 2 + CHIN1_BUTTON_WIDTH * 1 + CHIN1_BUTTON_XOFFSET),  CHIN1_BUTTON_LINE1YPOS, CHIN1_BUTTON_WIDTH, CHIN1_BUTTON_HEIGHT, "2", 0, "ABC"},
    { (CHIN1_BUTTON_XPAC * 3 + CHIN1_BUTTON_WIDTH * 2 + CHIN1_BUTTON_XOFFSET),  CHIN1_BUTTON_LINE1YPOS, CHIN1_BUTTON_WIDTH, CHIN1_BUTTON_HEIGHT, "3", 0, "DEF"},
    { (CHIN1_BUTTON_XPAC * 4 + CHIN1_BUTTON_WIDTH * 3 + CHIN1_BUTTON_XOFFSET),  CHIN1_BUTTON_LINE1YPOS, (CHIN1_BUTTON_WIDTH - CHIN1_BUTTONCONTR_XINC), CHIN1_BUTTON_HEIGHT, "\x08", _DrawBkSpc}, /* ɾ���� */

    { (CHIN1_BUTTON_XPAC * 1 + CHIN1_BUTTON_WIDTH * 0 + CHIN1_BUTTON_XOFFSET),  CHIN1_BUTTON_LINE2YPOS, CHIN1_BUTTON_WIDTH, CHIN1_BUTTON_HEIGHT, "4", 0, "GHI"},
    { (CHIN1_BUTTON_XPAC * 2 + CHIN1_BUTTON_WIDTH * 1 + CHIN1_BUTTON_XOFFSET),  CHIN1_BUTTON_LINE2YPOS, CHIN1_BUTTON_WIDTH, CHIN1_BUTTON_HEIGHT, "5", 0, "JKL"},
    { (CHIN1_BUTTON_XPAC * 3 + CHIN1_BUTTON_WIDTH * 2 + CHIN1_BUTTON_XOFFSET),  CHIN1_BUTTON_LINE2YPOS, CHIN1_BUTTON_WIDTH, CHIN1_BUTTON_HEIGHT, "6", 0, "MNO"},
    { (CHIN1_BUTTON_XPAC * 4 + CHIN1_BUTTON_WIDTH * 3 + CHIN1_BUTTON_XOFFSET),  CHIN1_BUTTON_LINE2YPOS, (CHIN1_BUTTON_WIDTH - CHIN1_BUTTONCONTR_XINC), CHIN1_BUTTON_HEIGHT,  " ", 0, "����" }, /* �ո��  */

    { (CHIN1_BUTTON_XPAC * 1 + CHIN1_BUTTON_WIDTH * 0 + CHIN1_BUTTON_XOFFSET),  CHIN1_BUTTON_LINE3YPOS, CHIN1_BUTTON_WIDTH, CHIN1_BUTTON_HEIGHT, "7", 0, "PQRS" },
    { (CHIN1_BUTTON_XPAC * 2 + CHIN1_BUTTON_WIDTH * 1 + CHIN1_BUTTON_XOFFSET),  CHIN1_BUTTON_LINE3YPOS, CHIN1_BUTTON_WIDTH, CHIN1_BUTTON_HEIGHT, "8", 0, "TUV" },
    { (CHIN1_BUTTON_XPAC * 3 + CHIN1_BUTTON_WIDTH * 2 + CHIN1_BUTTON_XOFFSET),  CHIN1_BUTTON_LINE3YPOS, CHIN1_BUTTON_WIDTH, CHIN1_BUTTON_HEIGHT, "9", 0, "WXYZ"},
    { (CHIN1_BUTTON_XPAC * 4 + CHIN1_BUTTON_WIDTH * 3 + CHIN1_BUTTON_XOFFSET),  CHIN1_BUTTON_LINE3YPOS, (CHIN1_BUTTON_WIDTH - CHIN1_BUTTONCONTR_XINC), CHIN1_BUTTON_HEIGHT, "\x04", _DrawRetern }, /* ���ؼ� */
};
/* ���ļ���2λ�� */
static BUTTON_DATA _aChineseButtonData2[] =
{
    { (CHIN2_BUTTON_XPAC * 1 + CHIN2_BUTTON_WIDTH * 0 + CHIN2_BUTTON_XOFFSET),  CHIN2_BUTTON_LINE1YPOS, CHIN2_BUTTON_WIDTH, CHIN2_BUTTON_HEIGHT, "", 0, "��"}, /* ����  */
    { (CHIN2_BUTTON_XPAC * 2 + CHIN2_BUTTON_WIDTH * 1 + CHIN2_BUTTON_XOFFSET),  CHIN2_BUTTON_LINE1YPOS, CHIN2_BUTTON_WIDTH, CHIN2_BUTTON_HEIGHT, "" },
    { (CHIN2_BUTTON_XPAC * 3 + CHIN2_BUTTON_WIDTH * 2 + CHIN2_BUTTON_XOFFSET),  CHIN2_BUTTON_LINE1YPOS, CHIN2_BUTTON_WIDTH, CHIN2_BUTTON_HEIGHT, "" },
    { (CHIN2_BUTTON_XPAC * 4 + CHIN2_BUTTON_WIDTH * 3 + CHIN2_BUTTON_XOFFSET),  CHIN2_BUTTON_LINE1YPOS, CHIN2_BUTTON_WIDTH, CHIN2_BUTTON_HEIGHT, "" },
    { (CHIN2_BUTTON_XPAC * 5 + CHIN2_BUTTON_WIDTH * 4 + CHIN2_BUTTON_XOFFSET),  CHIN2_BUTTON_LINE1YPOS, CHIN2_BUTTON_WIDTH, CHIN2_BUTTON_HEIGHT, "" },
    { (CHIN2_BUTTON_XPAC * 6 + CHIN2_BUTTON_WIDTH * 5 + CHIN2_BUTTON_XOFFSET),  CHIN2_BUTTON_LINE1YPOS, CHIN2_BUTTON_WIDTH, CHIN2_BUTTON_HEIGHT, "" },
    { (CHIN2_BUTTON_XPAC * 7 + CHIN2_BUTTON_WIDTH * 6 + CHIN2_BUTTON_XOFFSET),  CHIN2_BUTTON_LINE1YPOS, CHIN2_BUTTON_WIDTH, CHIN2_BUTTON_HEIGHT, "" },
    { (CHIN2_BUTTON_XPAC * 8 + CHIN2_BUTTON_WIDTH * 7 + CHIN2_BUTTON_XOFFSET),  CHIN2_BUTTON_LINE1YPOS, CHIN2_BUTTON_WIDTH, CHIN2_BUTTON_HEIGHT, "", 0, "��"},  /* ���� */

    { (CHIN2_BUTTON_XPAC * 1 + CHIN2_BUTTON_WIDTH * 0 + CHIN2_BUTTON_XOFFSET),  CHIN2_BUTTON_LINE2YPOS, (CHIN2_BUTTON_WIDTH + CHIN2_BUTTONPY_XINC), (CHIN2_BUTTON_HEIGHT + CHIN2_BUTTONPY_YINC), "\x05", 0, "��"}, /* ��һҳ  */
    { (CHIN2_BUTTON_XPAC * 1 + CHIN2_BUTTON_WIDTH * 0 + CHIN2_BUTTON_XOFFSET),  CHIN2_BUTTON_LINE3YPOS, (CHIN2_BUTTON_WIDTH + CHIN2_BUTTONPY_XINC), (CHIN2_BUTTON_HEIGHT + CHIN2_BUTTONPY_YINC), " " },
    { (CHIN2_BUTTON_XPAC * 1 + CHIN2_BUTTON_WIDTH * 0 + CHIN2_BUTTON_XOFFSET),  CHIN2_BUTTON_LINE4YPOS, (CHIN2_BUTTON_WIDTH + CHIN2_BUTTONPY_XINC), (CHIN2_BUTTON_HEIGHT + CHIN2_BUTTONPY_YINC), " " },
    { (CHIN2_BUTTON_XPAC * 1 + CHIN2_BUTTON_WIDTH * 0 + CHIN2_BUTTON_XOFFSET),  CHIN2_BUTTON_LINE5YPOS, (CHIN2_BUTTON_WIDTH + CHIN2_BUTTONPY_XINC), (CHIN2_BUTTON_HEIGHT + CHIN2_BUTTONPY_YINC), " " },
    { (CHIN2_BUTTON_XPAC * 1 + CHIN2_BUTTON_WIDTH * 0 + CHIN2_BUTTON_XOFFSET),  CHIN2_BUTTON_LINE6YPOS, (CHIN2_BUTTON_WIDTH + CHIN2_BUTTONPY_XINC), (CHIN2_BUTTON_HEIGHT + CHIN2_BUTTONPY_YINC), "\x06", 0, "��" }, /* ��һҳ  */

};

/**
 * @brief       ��Сд�л�����
 * @param       change_flag���޸ı�־
 * @param       buttondata����������
 * @return      ��
 */
void emwin_changecase(uint8_t change_flag, BUTTON_DATA *buttondata)
{
    uint16_t i;
    char NumToStr;
    char buffer[5];

    if (keypad_dev.LowerCase) /* Сд,��ť�޸�ΪСд */
    {
        /* ���İ�ť���ַ�*/
        for (i = 0; i < 27; i++)
        {
            if (i == 19) continue;

            NumToStr = (char)((int) * (buttondata[i].acLabel) + 0x20);
            sprintf(buffer, "%c", NumToStr);            /* ת��Ϊ�ַ� */
            BUTTON_SetFont(keypad_dev.EngButton_Handle[i], GUI_FONT_8X16_1);
            BUTTON_SetText(keypad_dev.EngButton_Handle[i], buffer);
            BUTTON_SetFocussable(keypad_dev.EngButton_Handle[i], 0);
        }
    }
    else /* ��д����ť�ϵ��ַ���Ϊ��д */
    {
        /* ���İ�ť���ַ� */
        for (i = 0; i < 27; i++)
        {
            if (i == 19) continue;

            BUTTON_SetFont(keypad_dev.EngButton_Handle[i], GUI_FONT_8X16_1);
            BUTTON_SetText(keypad_dev.EngButton_Handle[i], buttondata[i].acLabel);
            BUTTON_SetFocussable(keypad_dev.EngButton_Handle[i], 0);
        }
    }
}

/**
 * @brief       ����Ӣ�ļ���
 * @param       hWin�����
 * @return      ��
 */
void emwin_drawenglish_pad(WM_HWIN hWin)
{
    uint16_t i;

    /* �������̰�ť */
    BUTTON_SetDefaultSkin(_DrawSkinFlex_BUTTON); /* ����BUTTON��Ƥ�� */

    for (i = 0; i < GUI_COUNTOF(_aEngButtonData); i++)
    {
        /* ������ť */
        keypad_dev.EngButton_Handle[i] = BUTTON_CreateEx(_aEngButtonData[i].xPos, _aEngButtonData[i].yPos, _aEngButtonData[i].xSize, _aEngButtonData[i].ySize,
                                         hWin, WM_CF_SHOW | WM_CF_HASTRANS, 0, ID_BUTTON + i);
        BUTTON_SetFont(keypad_dev.EngButton_Handle[i], GUI_FONT_8X16_ASCII);
        BUTTON_SetText(keypad_dev.EngButton_Handle[i], _aEngButtonData[i].ButString);
        BUTTON_SetFocussable(keypad_dev.EngButton_Handle[i], 0);
    }

    emwin_changecase(keypad_dev.LowerCase, _aEngButtonData);
}

/**
 * @brief       �������ּ���
 * @param       hWin�����
 * @return      ��
 */
void emwin_drawnumber_pad(WM_HWIN hWin)
{
    uint16_t i;
	  
	
	  printf("emwin_drawnumber_pad num=%d \r\n",GUI_COUNTOF(_aNumButtonData));

    for (i = 0; i < GUI_COUNTOF(_aNumButtonData); i++) /* �������ּ��� */
    {
        /* ������ť */
        BUTTON_SetDefaultSkin(_DrawSkinFlex_BUTTON); /* ����BUTTON��Ƥ�� */
        keypad_dev.NumButton_Handle[i] = BUTTON_CreateEx(_aNumButtonData[i].xPos, _aNumButtonData[i].yPos, _aNumButtonData[i].xSize, _aNumButtonData[i].ySize,
                                         hWin, WM_CF_SHOW | WM_CF_HASTRANS, 0, ID_BUTTON + i);
        BUTTON_SetFont(keypad_dev.NumButton_Handle[i], GUI_FONT_8X16_ASCII);
        BUTTON_SetText(keypad_dev.NumButton_Handle[i], _aNumButtonData[i].ButString);
        BUTTON_SetFocussable(keypad_dev.NumButton_Handle[i], 0);
    }
}

/**
 * @brief       ���Ʒ��ż���
 * @param       hWin�����
 * @return      ��
 */
void emwin_drawsign_pad(WM_HWIN hWin)
{
    uint16_t i;

    for (i = 0; i < GUI_COUNTOF(_aSinButtonData[keypad_dev.signpad_flag]); i++) /* �������ż��� */
    {
        /* ������ť */
        BUTTON_SetDefaultSkin(_DrawSkinFlex_BUTTON); /* ����BUTTON��Ƥ�� */
        keypad_dev.SignButton_Handle[i] = BUTTON_CreateEx(_aSinButtonData[keypad_dev.signpad_flag][i].xPos, _aSinButtonData[keypad_dev.signpad_flag][i].yPos,
                                          _aSinButtonData[keypad_dev.signpad_flag][i].xSize, _aSinButtonData[keypad_dev.signpad_flag][i].ySize,
                                          hWin, WM_CF_SHOW | WM_CF_HASTRANS, 0, ID_BUTTON + i);
        BUTTON_SetFont(keypad_dev.SignButton_Handle[i], GUI_FONT_8X16_ASCII);
        BUTTON_SetText(keypad_dev.SignButton_Handle[i], _aSinButtonData[keypad_dev.signpad_flag][i].ButString);
        BUTTON_SetFocussable(keypad_dev.SignButton_Handle[i], 0);
    }
}

/**
 * @brief       �������ļ���1
 * @param       hWin�����
 * @return      ��
 */
void emwin_drawchinese1_pad(WM_HWIN hWin)
{
    uint16_t i;

    for (i = 0; i < GUI_COUNTOF(_aChineseButtonData1); i++) /* �������ļ���1 */
    {
        /* ������ť */
        BUTTON_SetDefaultSkin(_DrawSkinFlex_BUTTON); /* ����BUTTON��Ƥ�� */
        keypad_dev.ChineseButton_Handle[i] = BUTTON_CreateEx(_aChineseButtonData1[i].xPos, _aChineseButtonData1[i].yPos, _aChineseButtonData1[i].xSize, _aChineseButtonData1[i].ySize,
                                             hWin, WM_CF_SHOW | WM_CF_HASTRANS, 0, ID_BUTTON + i);
        BUTTON_SetFont(keypad_dev.ChineseButton_Handle[i], GUI_FONT_8X16_ASCII);
        BUTTON_SetText(keypad_dev.ChineseButton_Handle[i], _aChineseButtonData1[i].ButString);
        BUTTON_SetFocussable(keypad_dev.ChineseButton_Handle[i], 0);
    }
}

/**
 * @brief       �������ļ���2
 * @param       hWin�����
 * @return      ��
 */
void emwin_drawchinese2_pad(WM_HWIN hWin)
{
    uint16_t i;

    for (i = 0; i < GUI_COUNTOF(_aChineseButtonData2); i++) /* �������ļ���2 */
    {
        /* ������ť */
        BUTTON_SetDefaultSkin(_DrawChineseSkin_BUTTON); /* ����BUTTON��Ƥ�� */
        keypad_dev.ChineseButton_Handle[i + 12] = BUTTON_CreateEx(_aChineseButtonData2[i].xPos, _aChineseButtonData2[i].yPos, _aChineseButtonData2[i].xSize, _aChineseButtonData2[i].ySize,
                hWin, WM_CF_SHOW | WM_CF_HASTRANS, 0, ID_BUTTON + i + 12);
        BUTTON_SetFont(keypad_dev.ChineseButton_Handle[i + 12], GUI_FONT_8X16_ASCII);
        BUTTON_SetText(keypad_dev.ChineseButton_Handle[i + 12], _aChineseButtonData2[i].ButString);
        BUTTON_SetFocussable(keypad_dev.ChineseButton_Handle[i + 12], 0);
    }
}

/**
 * @brief       Ӣ�ļ��̴�����
 * @param       buttondata�����ż��̴�����
 * @param       Id����ťID
 * @param       *pMsg��ָ����Ϣ
 * @return      ��
 */
void emwin_engkeypad_process(BUTTON_DATA *buttondata, int Id, WM_MESSAGE *pMsg)
{
    char  c;
    WM_MESSAGE Msg;
    uint8_t    KeyID;

    KeyID = Id - ID_BUTTON;

    if (buttondata[Id - ID_BUTTON].acLabel)
    {
        c = buttondata[Id - ID_BUTTON].acLabel[0];

        if (KeyID == 19)                  /* ��Сд�л� */
        {
            keypad_dev.LowerCase ^= 1;    /* ��LowerCase���������� */
            emwin_changecase(keypad_dev.LowerCase, buttondata);
        }
        else  if (KeyID == 28)                  /* �л������ּ���  */
        {
            keypad_dev.padtype = NUMBER_KEYPAD; /* ���ּ��� */
            Msg.MsgId = CHANGE_ENGLISHPADTO_NUMPAD;
            Msg.hWinSrc = pMsg->hWin;
            WM_SendMessage(keypad_dev.hKeypad, &Msg);
        }
        else if (KeyID == 29)                  /* �л������ż���*/
        {
            keypad_dev.padtype = SIGN_KEYPAD;
            Msg.MsgId = CHANGE_ENGLISHPADTO_SIGNPAD1;
            Msg.hWinSrc = pMsg->hWin;
            WM_SendMessage(keypad_dev.hKeypad, &Msg);
        }
     /*   else if (KeyID == 33)                  �л������ļ���
        {
            keypad_dev.padtype = CHINESE_KEYPAD;
            Msg.MsgId = CHANGE_ENGLISHPADTO_CHINESEPAD;
            Msg.hWinSrc = pMsg->hWin;
            WM_SendMessage(keypad_dev.hKeypad, &Msg);
        } */
        else
        {
            if (keypad_dev.LowerCase && (c >= 0X41) && (c <= 0X5A))  /* Сд */
            {
                c += 0x20;
            }

            GUI_StoreKeyMsg(c, 1); /* ����Ϣ������̻�����������״̬ */
            GUI_StoreKeyMsg(c, 0); /* ����Ϣ������̻��������ɿ�״̬*/
        }
    }
}

/**
 * @brief       ���ּ��̴�����
 * @param       buttondata�����ż��̴�����
 * @param       Id����ťID
 * @param       *pMsg��ָ����Ϣ
 * @return      ��
 */
void emwin_numkeypad_process(BUTTON_DATA *buttondata, int Id, WM_MESSAGE *pMsg)
{
    char  c;
    WM_MESSAGE Msg;
    uint8_t    KeyID;

    KeyID = Id - ID_BUTTON;

    if (buttondata[Id - ID_BUTTON].acLabel)
    {
        c = buttondata[Id - ID_BUTTON].acLabel[0];

        if (KeyID == 19)                            /* ����Ӣ�ļ���*/
        {
            keypad_dev.padtype = ENGLISH_KEYPAD;   /* Ӣ�ļ��� */
            Msg.MsgId = CHANGE_NUMPADTO_ENGLISHPAD;
            Msg.hWinSrc = pMsg->hWin;
            WM_SendMessage(keypad_dev.hKeypad, &Msg);
        }
        else
        {
            GUI_StoreKeyMsg(c, 1);  /* ����Ϣ������̻�����������״̬ */
            GUI_StoreKeyMsg(c, 0);  /* ����Ϣ������̻��������ɿ�״̬ */
        }
    }
}

/**
 * @brief       ���Ĵ�����
 * @param       buttondata�����ż��̴�����
 * @param       Id����ťID
 * @param       *pMsg��ָ����Ϣ
 * @return      ��
 */
void emwin_signkeypad_process(BUTTON_DATA *buttondata, int Id, WM_MESSAGE *pMsg)
{
    char   c;
    WM_MESSAGE Msg;
    uint8_t    KeyID;

    KeyID = Id - ID_BUTTON;

    if (buttondata[Id - ID_BUTTON].acLabel)
    {
        c = buttondata[Id - ID_BUTTON].acLabel[0];

        if (KeyID == 19)                            /* ���ص�Ӣ�ļ���  */
        {
            keypad_dev.padtype = ENGLISH_KEYPAD;    /* Ӣ�ļ��� */
            Msg.MsgId = CHANGE_SIGNPADTO_ENGLISHPAD;
            Msg.hWinSrc = pMsg->hWin;
            WM_SendMessage(keypad_dev.hKeypad, &Msg);
        }
        else if (KeyID == 14)                       /* ��һ�����ż��� */
        {
            if (keypad_dev.signpad_flag < (keypad_dev.signpad_num - 1))
            {
                keypad_dev.signpad_flag++;          /* ���ż���������һ*/
                Msg.MsgId = CHANGE_SIGNPAD;
                Msg.hWinSrc = pMsg->hWin;
                WM_SendMessage(keypad_dev.hKeypad, &Msg);
            }
        }
        else if (KeyID == 9)                        /* ��һ�����ż���  */
        {
            if (keypad_dev.signpad_flag > 0)
            {
                keypad_dev.signpad_flag--;          /* ���ż���������1 */
                Msg.MsgId = CHANGE_SIGNPAD;
                Msg.hWinSrc = pMsg->hWin;
                WM_SendMessage(keypad_dev.hKeypad, &Msg);
            }
        }
        else
        {
            GUI_StoreKeyMsg(c, 1);  /* ����Ϣ������̻�����������״̬  */
            GUI_StoreKeyMsg(c, 0);  /* ����Ϣ������̻��������ɿ�״̬  */
        }
    }
}

/**
 * @brief       ���Ĵ�����
 * @param       buttondata�����ּ��̰�ť����
 * @param       Id����ťID
 * @param       *pMsg��ָ����Ϣ
 * @return      ��
 */
void emwin_chinesekeypad_process(BUTTON_DATA *buttondata, int Id, WM_MESSAGE *pMsg)
{

}


/**
 * @brief       ����BUTTONѡ����̵İ���
 * @param       pDrawItemInfo����������Ϣ
 * @return      ��
 */
static int _DrawSkinFlex_BUTTON(const WIDGET_ITEM_DRAW_INFO *pDrawItemInfo)
{
    static GUI_MEMDEV_Handle ahMemEdges[4];
    GUI_MEMDEV_Handle        hMem;
    WM_HWIN                  hWin;
    unsigned                 i;
    int                      xPos;
    int                      yPos;
    int                      xSize;
    int                      ySize;
    int                      x;
    int                      y;
    int                      Id;
    int                      IsPressed;
    int                      butheigh;
    void (* pfDraw)(WM_HWIN hWin);

    x = 0;
    y = 0;

    switch (pDrawItemInfo->Cmd)
    {
        case WIDGET_ITEM_CREATE:
            if (ahMemEdges[0] == 0)
            {
                /* ����һ��С�İ������ڣ����ǲ��ɼ� */
                hWin = WM_CreateWindowAsChild(0, 0, 16, 16, WM_HBKWIN, WM_CF_SHOW | WM_CF_STAYONTOP, NULL, 0);
                WM_SelectWindow(hWin);

                /* Ϊbutton��ÿ����Ե�����洢�豸 */
                for (i = 0; i < GUI_COUNTOF(ahMemEdges); i++)
                {
                    switch (i)
                    {
                        case 0:
                            x = 0;
                            y = 0;
                            break;

                        case 1:
                            x = -16;
                            y = 0;
                            break;

                        case 2:
                            x = -16;
                            y = -16;
                            break;

                        case 3:
                            x = 0;
                            y = -16;
                            break;
                    }

                    /* �����Ŵ��豸 */
                    hMem = GUI_MEMDEV_CreateFixed(0, 0, 16, 16, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV_8888);
                    GUI_MEMDEV_Select(hMem);
                    GUI_SetBkColor(GUI_TRANSPARENT);
                    GUI_Clear();
                    GUI_SetColor(COLOR_BORDER);
                    GUI_DrawRoundedFrame(x, y, x + 31, y + 31, 16, 4);	/* ���ƻ�����İ�ť�Ľǵ�Բ�� */

                    /* ������ָ���Ĵ�С */
                    ahMemEdges[i] = GUI_MEMDEV_CreateFixed(0, 0, 4, 4, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV_8888);
                    GUI_MEMDEV_Select(ahMemEdges[i]);
                    GUI_SetBkColor(GUI_TRANSPARENT);
                    GUI_Clear();
                    GUI_MEMDEV_RotateHQ(hMem, ahMemEdges[i], -6, -6, 0, 250);
                    GUI_MEMDEV_Delete(hMem);
                    GUI_MEMDEV_Select(0);
                }

                WM_DeleteWindow(hWin);
            }

            break;

        case WIDGET_ITEM_DRAW_TEXT:
            /* �ڰ�ť����ʾ�Զ������Ĭ�ϵ���ɫ�����ı� */
            hWin = pDrawItemInfo->hWin;
            Id   = WM_GetId(hWin);
            i    = Id - ID_BUTTON;

            if (keypad_dev.padtype == ENGLISH_KEYPAD) pfDraw = _aEngButtonData[i].pfDraw;
            else if (keypad_dev.padtype == NUMBER_KEYPAD) pfDraw = _aNumButtonData[i].pfDraw;
            else if (keypad_dev.padtype == SIGN_KEYPAD) pfDraw = _aSinButtonData[keypad_dev.signpad_flag][i].pfDraw;
            else if (keypad_dev.padtype == CHINESE_KEYPAD) pfDraw = _aChineseButtonData1[i].pfDraw;

            if (pfDraw) pfDraw(hWin);
            else BUTTON_DrawSkinFlex(pDrawItemInfo);

            break;

        case WIDGET_ITEM_DRAW_BACKGROUND:
            /* ���ư�ť�ı���  */
            IsPressed = BUTTON_IsPressed(pDrawItemInfo->hWin);  /* �ж��Ƿ�ť�Ƿ񱻰��� */
            xPos      = WM_GetWindowOrgX(pDrawItemInfo->hWin);  /* ��ȡ��ťС���ߵ�X����ԭ��λ��  */
            yPos      = WM_GetWindowOrgY(pDrawItemInfo->hWin);  /* ��ȡ��ťС���ߵ�Y����ԭ��λ�� */
            xSize     = WM_GetWindowSizeX(pDrawItemInfo->hWin); /* ��ȡ��ťС���ߵ�X��С */
            ySize     = WM_GetWindowSizeY(pDrawItemInfo->hWin); /* ��ȡ��ťС���ߵ�Y��С */

            if (IsPressed)  /* �����ť������ */
            {
                GUI_DrawGradientRoundedV(0, 0, xSize - 1, ySize - 1, 4, BUTTON_COLOR2, BUTTON_COLOR3);/* ���ƴ�ֱ��ɫ�ݶ�����Բ�Ǿ��� */
            }
            else
            {
                GUI_DrawGradientRoundedV(0, 0, xSize - 1, ySize - 1, 4, BUTTON_COLOR0, BUTTON_COLOR1);/* ���ƴ�ֱ��ɫ�ݶ�����Բ�Ǿ��� */
            }

            GUI_SetColor(COLOR_BORDER);
            /* ���ư�ť�������߽� */
            GUI_DrawHLine(        0, 4, xSize - 5);
            GUI_DrawHLine(ySize - 1, 4, xSize - 5);
            GUI_DrawVLine(        0, 4, ySize - 5);
            GUI_DrawVLine(xSize - 1, 4, ySize - 5);

            /* �������洢�豸������д�뵱ǰѡ���洢�豸���ƶ�λ��  */
            if (keypad_dev.padtype == CHINESE_KEYPAD) butheigh = 77;
            else butheigh = 68;

            GUI_MEMDEV_WriteAt(ahMemEdges[0], xPos +  0, yPos +  0);         /* ��ť���Ͻ�Բ��  */
            GUI_MEMDEV_WriteAt(ahMemEdges[1], xPos + xSize - 4, yPos +  0);  /* ��ť���Ͻ�Բ��*/
            GUI_MEMDEV_WriteAt(ahMemEdges[2], xPos + xSize - 4, yPos + butheigh); /* ��ť���½�Բ�� */
            GUI_MEMDEV_WriteAt(ahMemEdges[3], xPos +  0, yPos + butheigh);        /* ��ť���½�Բ��  */
            break;

        default:
            return BUTTON_DrawSkinFlex(pDrawItemInfo); /* ���������ʹ��Ĭ�ϵ�Ƥ�� */
    }

    return 0;
}

/**
 * @brief       �������ļ��̰���
 * @param       pDrawItemInfo����������Ϣ
 * @return      ��
 */
static int _DrawChineseSkin_BUTTON(const WIDGET_ITEM_DRAW_INFO *pDrawItemInfo)
{
    WM_HWIN                  hWin;
    unsigned                 i;
    int                      xSize;
    int                      ySize;
    int                      Id;
    int                      IsPressed;
    void (* pfDraw)(WM_HWIN hWin);

    switch (pDrawItemInfo->Cmd)
    {
        case WIDGET_ITEM_DRAW_TEXT:
            /* �ڰ�ť����ʾ�Զ������Ĭ�ϵ���ɫ�����ı� */
            hWin = pDrawItemInfo->hWin;
            Id   = WM_GetId(hWin);
            i    = Id - ID_BUTTON;
            pfDraw = _aChineseButtonData1[i].pfDraw;

            if (pfDraw) pfDraw(hWin);
            else BUTTON_DrawSkinFlex(pDrawItemInfo);

            break;

        case WIDGET_ITEM_DRAW_BACKGROUND:
            /* ���ư�ť�ı��� */
            IsPressed = BUTTON_IsPressed(pDrawItemInfo->hWin);  /* �ж��Ƿ�ť�Ƿ񱻰��� */
            xSize     = WM_GetWindowSizeX(pDrawItemInfo->hWin); /* ��ȡ��ťС���ߵ�X��С */
            ySize     = WM_GetWindowSizeY(pDrawItemInfo->hWin); /* ��ȡ��ťС���ߵ�Y��С*/

            if (IsPressed)  /* �����ť������*/
            {
                GUI_DrawGradientV(0, 0, xSize - 1, ySize - 1, BUTTON_COLOR2, BUTTON_COLOR3);
            }
            else
            {
                GUI_DrawGradientV(0, 0, xSize - 1, ySize - 1, GUI_WHITE, GUI_WHITE);
            }

            GUI_SetColor(COLOR_BORDER);
            /* ���ư�ť�������߽� */
            GUI_DrawHLine(        0, 0, xSize - 1);
            GUI_DrawHLine(ySize - 1, 0, xSize - 1);
            GUI_DrawVLine(        0, 0, ySize - 1);
            GUI_DrawVLine(xSize - 1, 0, ySize - 1);
            break;

        default:
            return BUTTON_DrawSkinFlex(pDrawItemInfo);/* ���������ʹ��Ĭ�ϵ�Ƥ�� */
    }

    return 0;
}

/**
 * @brief       ���º����6����ť
 * @param       str�������ַ���
 * @return      ��
 */
void emwin_keypad_hbtn_updc(uint8_t *str)
{
    uint8_t i = 0;

    if ((keypad_dev.dispagecur >= keypad_dev.dispagenum) && keypad_dev.dispagenum)return; /* ��ǰҳ���ڻ��ߴ�����ҳ�� */

    for (i = 0; i < 6; i++) keypad_dev.butname[i] = NULL;

    i = 0;

    for (i = 0; i < 6; i++)
    {
        if ((i + 6 * (keypad_dev.dispagecur)) > (keypad_dev.hznum - 1)) /* ������Ӧƴ���ĺ������� */
        {
            keypad_dev.disbuf[i][0] = ' ';
            keypad_dev.disbuf[i][1] = ' ';
            keypad_dev.disbuf[i][2] = '\0';
            keypad_dev.butname[i] = keypad_dev.disbuf[i];
        }
        else
        {
            keypad_dev.disbuf[i][0] = *str++;
            keypad_dev.disbuf[i][1] = *str++;
            keypad_dev.disbuf[i][2] = '\0';
            keypad_dev.butname[i] = keypad_dev.disbuf[i];
        }
    }
}

/**
 * @brief       ���ƺ����6����ť
 * @param       handle�����ھ��
 * @return      ��
 */
void emwin_keypad_draw_hbtn(WM_HWIN *handle)
{
    uint8_t i;

    for (i = 0; i < 6; i++)
    {
        BUTTON_SetFont(handle[13 + i], GUI_FONT_8X16_1);
        BUTTON_SetText(handle[13 + i], (char *)keypad_dev.butname[i]);
    }

}

/**
 * @brief       keypad���ڵ��õĻص�����
 * @param       pMsg����Ϣ�ṹ��
 * @return      ��
 */
static void _cbKeyPad(WM_MESSAGE *pMsg)
{
    WM_HWIN    hWin;
    int        Id;
    int        NCode;
    int        xSize;
    int        ySize;
    uint16_t        slen;
    unsigned   i;

    hWin = pMsg->hWin;
    switch (pMsg->MsgId)
    {
			
        case WM_CREATE:            /* ����Ϣ�ڴ��ڴ������������ͣ��Ǵ��ڿ��Գ�ʼ���������κ��Ӵ��� */
					printf("9999999999999999  pMsg->MsgId=%d \r\n",pMsg->MsgId);
			//	emwin_drawnumber_pad(hWin);
           emwin_drawenglish_pad(hWin); /* ����Ӣ�ļ��� */
            break;

        case WM_PAINT:
            /* ���Ʊ��� */
            xSize = WM_GetWindowSizeX(hWin);
            ySize = WM_GetWindowSizeY(hWin);
            GUI_SetColor(COLOR_BORDER);
            GUI_DrawRect(0, 0, xSize - 1, ySize - 1);
            GUI_DrawGradientV(1, 1, xSize - 2, ySize - 2, COLOR_KEYPAD0, COLOR_KEYPAD1);
            break;

        case WM_NOTIFY_PARENT: /* ���߸����ڣ��Ӵ��ڷ����˸ı� */
            /* �뵱ǰ���㴰�ڷ��Ͱ�����Ϣ */
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;

            switch (NCode)
            {
                case WM_NOTIFICATION_RELEASED:                        /* ��ť���ͷ� */
                    if (keypad_dev.padtype == ENGLISH_KEYPAD)         /* Ӣ�ļ��� */
                    {
                        emwin_engkeypad_process(_aEngButtonData, Id, pMsg); /* ����Ӣ�ļ��� */
                    }
                    else if (keypad_dev.padtype == NUMBER_KEYPAD)     /* ���ּ��� */
                    {
                        emwin_numkeypad_process(_aNumButtonData, Id, pMsg); /* �������ּ��� */
                    }
                    else if (keypad_dev.padtype == SIGN_KEYPAD)
                    {
                        emwin_signkeypad_process(_aSinButtonData[keypad_dev.signpad_flag], Id, pMsg);
                    }
                    else if (keypad_dev.padtype == CHINESE_KEYPAD)
                    {
                        emwin_chinesekeypad_process(_aChineseButtonData1, Id, pMsg);
                    }

                    break;
            }

            break;

        case CHANGE_ENGLISHPADTO_NUMPAD:  /* ��Ӣ�ļ����л������ּ��� */
            for (i = 0; i < GUI_COUNTOF(_aEngButtonData); i++) WM_DeleteWindow(keypad_dev.EngButton_Handle[i]);

            for (i = 0; i < GUI_COUNTOF(_aEngButtonData); i++) keypad_dev.EngButton_Handle[i] = 0;

            emwin_drawnumber_pad(hWin);
            break;

        case CHANGE_NUMPADTO_ENGLISHPAD: /* �����ּ����л���Ӣ�ļ��� */
            for (i = 0; i < GUI_COUNTOF(_aNumButtonData); i++) WM_DeleteWindow(keypad_dev.NumButton_Handle[i]);

            for (i = 0; i < GUI_COUNTOF(_aEngButtonData); i++) keypad_dev.NumButton_Handle[i] = 0;

            emwin_drawenglish_pad(hWin);
            break;

        case CHANGE_ENGLISHPADTO_SIGNPAD1: /* ��Ӣ�ļ����л������ż��� */
            for (i = 0; i < GUI_COUNTOF(_aEngButtonData); i++) WM_DeleteWindow(keypad_dev.EngButton_Handle[i]);

            for (i = 0; i < GUI_COUNTOF(_aEngButtonData); i++) keypad_dev.SignButton_Handle[i] = 0;

            emwin_drawsign_pad(hWin);
            break;

        case CHANGE_SIGNPADTO_ENGLISHPAD: /* �ӷ��ż����л���Ӣ�ļ��� */
            for (i = 0; i < GUI_COUNTOF(_aSinButtonData[keypad_dev.signpad_flag]); i++) WM_DeleteWindow(keypad_dev.SignButton_Handle[i]);

            for (i = 0; i < GUI_COUNTOF(_aSinButtonData[keypad_dev.signpad_flag]); i++) keypad_dev.SignButton_Handle[i] = 0;

            emwin_drawenglish_pad(hWin);
            break;

        case CHANGE_SIGNPAD:             /* �л����ż��� */
            for (i = 0; i < GUI_COUNTOF(_aSinButtonData[keypad_dev.signpad_flag]); i++) WM_DeleteWindow(keypad_dev.SignButton_Handle[i]);

            for (i = 0; i < GUI_COUNTOF(_aSinButtonData[keypad_dev.signpad_flag]); i++) keypad_dev.SignButton_Handle[i] = 0;

            emwin_drawsign_pad(hWin);
            break;

        case CHANGE_ENGLISHPADTO_CHINESEPAD:        /* ��Ӣ�ļ����л������ļ��� */
       /*     for (i = 0; i < GUI_COUNTOF(_aEngButtonData); i++) WM_DeleteWindow(keypad_dev.EngButton_Handle[i]);

            for (i = 0; i < GUI_COUNTOF(_aEngButtonData); i++) keypad_dev.EngButton_Handle[i] = 0;

            emwin_drawchinese1_pad(hWin);
            emwin_drawchinese2_pad(hWin);*/
            break;

        case CHANGE_CHINESEPADTO_ENGLISHPAD:        /* �����ļ����л���Ӣ�ļ��� */


            break;
    }
}
void DeleteButton(void)
{
	int i;
  int id = keypad_dev.padtype;
	switch(id)
	{
	
	        case ENGLISH_KEYPAD:  // Ӣ�ļ��� 
            for (i = 0; i < GUI_COUNTOF(_aEngButtonData); i++) WM_DeleteWindow(keypad_dev.EngButton_Handle[i]);

            for (i = 0; i < GUI_COUNTOF(_aEngButtonData); i++) keypad_dev.EngButton_Handle[i] = 0;


            break;

        case NUMBER_KEYPAD: // ���ּ��� 
            for (i = 0; i < GUI_COUNTOF(_aNumButtonData); i++) WM_DeleteWindow(keypad_dev.NumButton_Handle[i]);

            for (i = 0; i < GUI_COUNTOF(_aEngButtonData); i++) keypad_dev.NumButton_Handle[i] = 0;


            break;


        case SIGN_KEYPAD: // ���ż��� 
            for (i = 0; i < GUI_COUNTOF(_aSinButtonData[keypad_dev.signpad_flag]); i++) WM_DeleteWindow(keypad_dev.SignButton_Handle[i]);

            for (i = 0; i < GUI_COUNTOF(_aSinButtonData[keypad_dev.signpad_flag]); i++) keypad_dev.SignButton_Handle[i] = 0;

            break;

        default:


            break;
			}
	
	WM_DeleteWindow(hButton_Cancel);
	hButton_Cancel=0;		
		WM_DeleteWindow(hButton_OK);
	hButton_OK=0;	
	WM_DeleteWindow(hText);
	hText=0;	
	WM_DeleteWindow(hEdit_Pwd);
	hEdit_Pwd=0;		
				

}
/**
 * @brief       �������ڻص�����
 * @param       pMsg����Ϣ�ṹ��
 * @return      ��
 */
static void _cbBk(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            GUI_DrawGradientV(0, 0, lcd_width, lcd_height, COLOR_BACK0, COLOR_BACK1);
            break;
    }
}
static void _setPwd_cb(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId)
    {
        case WM_PAINT:
          //  GUI_DrawGradientV(0, 0, lcd_width, lcd_height, COLOR_BACK0, COLOR_BACK1);
            break;
			  case WM_NOTIFY_PARENT:
					
		        if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) /* ��ť���ͷ�ʱ */
            {	
                if (pMsg->hWinSrc == hButton_OK)              /* hButton_OK���ͷ� */
                {
									if(status_pwd==0)
									{
										TEXT_SetText(hText, "Re-enter the PWD for confirm");
										BUTTON_SetText(hButton_OK, "OK");
										status_pwd = 1;
									}
									else if(status_pwd==1)
									{
										//�Ƚ��������룬��������
									/*	{
												TEXT_SetText(hText, "PWD don't match,re-enter");
										    BUTTON_SetText(hButton_OK, "Next");
									    	status_pwd = 0;
										}*/
										
									status=1;
									uint32_t msg;
								  DeleteButton();
									
									WM_DeleteWindow(keypad_dev.hKeypad);						
									WM_DeleteWindow(pMsg->hWin);
	                GUI_Delay(500);

	//											    GUI_SetBkColor(GUI_BLUE);
   // GUI_SetFont(&GUI_FontHZ16);
    //             GUI_SetColor(GUI_RED);
   //              GUI_Clear();							
  			          msg	= MSG_CW_RESULT;
									(void)osMessagePut(myQueue01Handle, msg, 0U);	
								


									
                }	  
							}
								else if(pMsg->hWinSrc == hButton_Cancel)
								{
									status=1;
									uint32_t msg;
								  DeleteButton();
									
									WM_DeleteWindow(keypad_dev.hKeypad);						
									WM_DeleteWindow(pMsg->hWin);
	                GUI_Delay(500);

	//											    GUI_SetBkColor(GUI_BLUE);
   // GUI_SetFont(&GUI_FontHZ16);
    //             GUI_SetColor(GUI_RED);
   //              GUI_Clear();							
  			          msg	= MSG_CREATE_WALLET;
									(void)osMessagePut(myQueue01Handle, msg, 0U);	
             //     invokeSeleImgWin();
									
								}									
								
							
						}
						break;			
				
				
    }
}

/**
 * @brief       emwin_main(emwin���)
 * @param       ��
 * @return      ��
 */
void enter_ImgPwdPad(void) 
{
	
	    WM_HWIN hFrame;

    WM_SetCallback(WM_HBKWIN, _cbBk);      /* ��ָ�������ڻص����� */

    keypad_dev.xpos = 0;
    keypad_dev.ypos = (lcd_height * 5) / 10;
    keypad_dev.width = lcd_width;
    keypad_dev.height = lcd_height - ((lcd_height * 5) / 10);
	
		  printf("lcd_width=%d  lcd_height=%d   keypad_dev.width=%d  keypad_dev.height=%d\r\n",lcd_width, lcd_height ,  keypad_dev.width,  keypad_dev.height);
    keypad_dev.padtype = ENGLISH_KEYPAD;    /* Ĭ��ΪӢ�ļ��� */
    keypad_dev.signpad_flag = 0;
    keypad_dev.signpad_num = 2;
    keypad_dev.inputlen = 0;
    keypad_dev.pynowpage = 0;
    keypad_dev.cur_index = 0;
    keypad_dev.sta = 0;
	
	

    /* ����keypad��ʹ�õİ�ť��Ƥ�� */
    BUTTON_SetDefaultSkin(_DrawSkinFlex_BUTTON); /* ����BUTTON��Ƥ�� */
    /* ����keypad */
    keypad_dev.hKeypad = WM_CreateWindowAsChild(keypad_dev.xpos, keypad_dev.ypos, keypad_dev.width, keypad_dev.height, WM_HBKWIN, WM_CF_SHOW | WM_CF_STAYONTOP, _cbKeyPad, 0);

    /* ����notepad���� */
    BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
    FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
    FRAMEWIN_SetDefaultTextAlign(GUI_TA_HCENTER);

    /* ����FRAME���� */
    hFrame = FRAMEWIN_CreateEx(0, 0, 240, 160, WM_HBKWIN, WM_CF_SHOW, 0, 0, "Login", _setPwd_cb);
    FRAMEWIN_SetTextColor(hFrame, GUI_RED);
    FRAMEWIN_SetFont(hFrame, &GUI_Font20_ASCII);
    FRAMEWIN_SetClientColor(hFrame, GUI_WHITE);
	

    /* ����edit(�ı�С����)С���� */
		 hEdit_Pwd = EDIT_CreateEx(28, 47, 166, 22, WM_GetClientWindow(hFrame), WM_CF_SHOW, 0,ID_BUTTON-1, 100);
   //  EDIT_EnableBlink(hEdit_Pwd, 500, 1); /* �������,����500ms */
		EDIT_SetInsertMode(hEdit_Pwd, 1);     /* ��������ģʽ */
		 EDIT_SetFont(hEdit_Pwd, GUI_FONT_8X16_1);
		 WM_SetFocus(hEdit_Pwd);
		 
		 
		 hText = TEXT_CreateEx(5, 13, 189, 21, WM_GetClientWindow(hFrame),WM_CF_SHOW,0,ID_BUTTON-2,0);
  //   TEXT_SetText(hEdit, "Enter your password to login.");
	
	//Please check your password and try again
	
	TEXT_SetText(hText, "Check your password and try again.");
     TEXT_SetFont(hText, GUI_FONT_13HB_ASCII);
	
     hButton_OK = BUTTON_CreateEx(120, 85, 70, 30, WM_GetClientWindow(hFrame),WM_CF_SHOW,0,ID_BUTTON-3);
     BUTTON_SetText(hButton_OK, "Next");
     BUTTON_SetFont(hButton_OK, GUI_FONT_16B_1);
		 BUTTON_SetFocussable(hButton_OK,0);
	   hButton_Cancel = BUTTON_CreateEx(28, 85, 70, 30, WM_GetClientWindow(hFrame),WM_CF_SHOW,0,ID_BUTTON-4);
     BUTTON_SetText(hButton_Cancel, "Cancel");
     BUTTON_SetFont(hButton_Cancel, GUI_FONT_13B_1);	 
		 BUTTON_SetFocussable(hButton_Cancel,0);
		 
		 
  //  hMulti = MULTIEDIT_CreateEx(0, 0, 0, 0, WM_GetClientWindow(hFrame), WM_CF_SHOW, 0, GUI_ID_MULTIEDIT0, 100, NULL);
 //   MULTIEDIT_EnableBlink(hMulti, 500, 1); /* �������,����500ms */
  //  MULTIEDIT_SetInsertMode(hMulti, 1);     /* ��������ģʽ */
  //  MULTIEDIT_SetFont(hMulti, GUI_FONT_8X16_1);
 //   WM_SetFocus(hMulti);

    while (1)
    {
        GUI_Delay(100);
			  if(status==1)
				{
					status=0;
					status_pwd = 0;
					
					break;
				}
    }
}
