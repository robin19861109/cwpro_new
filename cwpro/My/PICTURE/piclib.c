/**
 ****************************************************************************************************
 * @file        piclib.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       图片解码库 代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20200404
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "piclib.h"
#include "my_lcd.h"

_pic_info picinfo;      /* 图片信息 */
_pic_phy pic_phy;       /* 图片显示物理接口 */

uint32_t lcd_read_point(uint16_t x, uint16_t y);
void lcd_draw_point(uint16_t x, uint16_t y, uint32_t color);
/**
 * @brief       填充颜色
 * @param       x, y          : 起始坐标
 * @param       width, height : 宽度和高度
 * @param       color         : 颜色数组
 * @retval      无
 */
static void piclib_fill_color(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *color)
{
   // lcd_color_fill(x, y, x + width - 1, y + height - 1, color);     /* 填充 */
	    uint16_t color_temp = *color;
	
	    LCD_Fill_onecolor(x,y, x + width - 1,height - 1,color_temp);
}
/**
 * @brief       画图初始化
 *   @note      在画图之前,必须先调用此函数, 指定相关函数
 * @param       无
 * @retval      无
 */
void piclib_init(void)
{
    pic_phy.read_point = lcd_read_point;    /* 读点函数实现,仅BMP需要 */
    pic_phy.draw_point = lcd_draw_point;    /* 画点函数实现 */
   // pic_phy.fill = lcd_fill;                /* 填充函数实现,仅GIF需要 */
 //   pic_phy.draw_hline = lcd_draw_hline;    /* 画线函数实现,仅GIF需要 */
    pic_phy.fillcolor = piclib_fill_color;  /* 颜色填充函数实现,仅TJPGD需要 */

    picinfo.lcdwidth = lcd_width;        /* 得到LCD的宽度像素 */
    picinfo.lcdheight = lcd_height;      /* 得到LCD的高度像素 */

    picinfo.ImgWidth = 0;                   /* 初始化宽度为0 */
    picinfo.ImgHeight = 0;                  /* 初始化高度为0 */
    picinfo.Div_Fac = 0;                    /* 初始化缩放系数为0 */
    picinfo.S_Height = 0;                   /* 初始化设定的高度为0 */
    picinfo.S_Width = 0;                    /* 初始化设定的宽度为0 */
    picinfo.S_XOFF = 0;                     /* 初始化x轴的偏移量为0 */
    picinfo.S_YOFF = 0;                     /* 初始化y轴的偏移量为0 */
    picinfo.staticx = 0;                    /* 初始化当前显示到的x坐标为0 */
    picinfo.staticy = 0;                    /* 初始化当前显示到的y坐标为0 */
}
/**
 * @brief       快速ALPHA BLENDING算法
 * @param       src           : 颜色数
 * @param       dst           : 目标颜色
 * @param       alpha         : 透明程度(0~32)
 * @retval      混合后的颜色
 */
uint16_t piclib_alpha_blend(uint16_t src, uint16_t dst, uint8_t alpha)
{
    uint32_t src2;
    uint32_t dst2;
    /* Convert to 32bit |-----GGGGGG-----RRRRR------BBBBB| */
    src2 = ((src << 16) | src) & 0x07E0F81F;
    dst2 = ((dst << 16) | dst) & 0x07E0F81F;
    /* Perform blending R:G:B with alpha in range 0..32
     * Note that the reason that alpha may not exceed 32 is that there are only
     * 5bits of space between each R:G:B value, any higher value will overflow
     * into the next component and deliver ugly result. 
     */
    dst2 = ((((dst2 - src2) * alpha) >> 5) + src2) & 0x07E0F81F;
    return (dst2 >> 16) | dst2;
}
uint32_t lcd_read_point(uint16_t x, uint16_t y)
{
	return (uint32_t) LCD_GetPoint(x,y);
	
}
/**
 * @brief       LCD开启显示
 * @param       无
 * @retval      无
 */
void lcd_display_on(void)
{
//LCD开启显示
    LCD_DisplayOn();

	
	
}

/**
 * @brief       LCD关闭显示
 * @param       无
 * @retval      无
 */
void lcd_display_off(void)
{
    LCD_DisplayOff();
}
void lcd_draw_point(uint16_t x, uint16_t y, uint32_t color)
{
	LCD_Color_DrawPoint(x,y,color);
	
}

/**
 * @brief       判断这个像素是否可以显示
 * @param       x, y          : 像素原始坐标
 * @param       chg           : 功能变量
 * @param       无
 * @retval      操作结果
 *   @arg       0   , 不需要显示
 *   @arg       1   , 需要显示
 */
__inline uint8_t piclib_is_element_ok(uint16_t x, uint16_t y, uint8_t chg)
{
    if (x != picinfo.staticx || y != picinfo.staticy)
    {
        if (chg == 1)
        {
            picinfo.staticx = x;
            picinfo.staticy = y;
        }

        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief       智能画图
 *   @note      图片仅在x,y和width, height限定的区域内显示.
 *
 * @param       filename      : 包含路径的文件名(.bmp/.jpg/.jpeg/.gif等)
 * @param       x, y          : 起始坐标
 * @param       width, height : 显示区域
 * @param       fast          : 使能快速解码
 *   @arg                       0, 不使能
 *   @arg                       1, 使能
 *   @note                      图片尺寸小于等于液晶分辨率,才支持快速解码
 * @retval      无
 */
uint8_t piclib_ai_load_picfile(char *filename, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t fast)
{
    uint8_t	res=0;/* 返回值 */
    uint8_t temp;
	fast = fast;

    if ((x + width) > picinfo.lcdwidth)return PIC_WINDOW_ERR;   /* x坐标超范围了 */

    if ((y + height) > picinfo.lcdheight)return PIC_WINDOW_ERR; /* y坐标超范围了 */

    /* 得到显示方框大小 */
    if (width == 0 || height == 0)return PIC_WINDOW_ERR;        /* 窗口设定错误 */

    picinfo.S_Height = height;
    picinfo.S_Width = width;
    printf("aaaaaaaaaaaaaaaaaaaa  filename=%s\r\n",filename);
    /* 显示区域无效 */
    if (picinfo.S_Height == 0 || picinfo.S_Width == 0)
    {
        picinfo.S_Height = lcd_height;
        picinfo.S_Width = lcd_width;
        return FALSE;
    }

    if (pic_phy.fillcolor == NULL)fast = 0;     /* 颜色填充函数未实现,不能快速显示 */
    printf("bbbbbbbbbbbbbbbbbbbbbbb\r\n");
		
    picinfo.S_YOFF = y;
    picinfo.S_XOFF = x;

    /* 文件名传递 */
    temp = exfuns_file_type(filename);          /* 得到文件的类型 */
         printf("ccccccccccccccccccccc  temp=%x\r\n",temp);
    switch (temp)
    {
        case T_BMP:
            res = stdbmp_decode(filename);      /* 解码bmp */
            break;
				case T_PNG:
					res = png_decode(filename);    /* 解码png */
				
					break;

 //       case T_JPG:
 //       case T_JPEG:
//            res = jpg_decode(filename, fast);   /* 解码JPG/JPEG */
//            break;

//        case T_GIF:
//            res = gif_decode(filename, x, y, width, height);    /* 解码gif */
//            break;

        default:
            res = PIC_FORMAT_ERR;               /* 非图片格式!!! */
            break;
    }

    return res;
}

/**
 * @brief       初始化智能画点
 * @param       无
 * @retval      无
 */
void piclib_ai_draw_init(void)
{
    float temp, temp1;
    temp = (float)picinfo.S_Width / picinfo.ImgWidth;
    temp1 = (float)picinfo.S_Height / picinfo.ImgHeight;

    if (temp < temp1)temp1 = temp;  /* 取较小的那个 */

    if (temp1 > 1)temp1 = 1;

    /* 使图片处于所给区域的中间 */
    picinfo.S_XOFF += (picinfo.S_Width - temp1 * picinfo.ImgWidth) / 2;
    picinfo.S_YOFF += (picinfo.S_Height - temp1 * picinfo.ImgHeight) / 2;
    temp1 *= 8192;  /* 扩大8192倍 */
    picinfo.Div_Fac = temp1;
    picinfo.staticx = 0xffff;
    picinfo.staticy = 0xffff;   /* 放到一个不可能的值上面 */
}
//void LCD_DrawPoint(uint16_t x,uint16_t y)

