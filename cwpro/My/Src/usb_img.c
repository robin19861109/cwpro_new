#include "usb_img.h"
#include "ff.h"
#include "exfuns.h"
#include "usb_host.h"
#include "fatfs.h"
#include "my_lcd.h"
#include "malloc.h"
#include "piclib.h"
#define FF_USE_LFN		3
#define FF_MAX_LFN		255
//cwpro\cwpro.axf: Error: L6218E: Undefined symbol exfuns_file_type (referred from usb_img.o).
//cwpro\cwpro.axf: Error: L6218E: Undefined symbol g_flag_mount (referred from usb_img.o).
//cwpro\cwpro.axf: Error: L6218E: Undefined symbol piclib_ai_load_picfile (referred from usb_img.o).
//cwpro\cwpro.axf: Error: L6218E: Undefined symbol piclib_init (referred from usb_img.o).
	void lcdbenin(void);
uint16_t pic_get_tnum(char *path)
{
    uint8_t res;
    uint16_t rval = 0;
    DIR tdir;                                                 /* 临时目录 */
	 FILINFO dtfileinfo;
	
    FILINFO *tfileinfo=(FILINFO *)&dtfileinfo;                                       /* 临时文件信息 */

  //  tfileinfo = (FILINFO *)Mem_malloc(INSRAM, sizeof(FILINFO)); /* 申请内存 */
	//  goto  _end_;

    res = f_opendir(&tdir, (const TCHAR *)path);              /* 打开目录 */

    if (res == FR_OK && tfileinfo)
    {
        while (1)                               /* 查询总的有效文件数 */
        {
            res = f_readdir(&tdir, tfileinfo);  /* 读取目录下的一个文件 */

            if (res != FR_OK || tfileinfo->fname[0] == 0)
                break;                          /* 错误了/到末尾了,退出 */

            res = exfuns_file_type(tfileinfo->fname);

            if ((res & 0XF0) == 0X50)           /* 取高四位,看看是不是图片文件 */
            {
                rval++;                         /* 有效文件数增加1 */
            }
        }
    }

//		_end_:		    Mem_free(INSRAM, tfileinfo);                  /* 释放内存 */
        return rval;
}

int USBDiskTest(void)
{
	static ApplicationTypeDef lastAppliState = APPLICATION_IDLE;
		int flag_app = 0;
	if (GetUsbAppliState() != lastAppliState)
		{
						if (GetUsbAppliState() == APPLICATION_READY)
			{
				flag_app=1;
			}
			
		}			
		
		return flag_app;
}
int USBDiskTest1(void)
{
static ApplicationTypeDef lastAppliState = APPLICATION_IDLE;
	UINT br, bw;
	int flag_app = 0;
	
	DIR picdir;             /* 图片目录 */
	    uint16_t totpicnum;     /* 图片文件总数 */
	uint8_t *pname;
	FILINFO *picfileinfo;   /* 文件信息 */
    uint16_t curindex;      /* 图片当前索引 */	
    uint32_t *picoffsettbl; /* 图片文件offset索引表 */

    uint16_t temp;	
	if (GetUsbAppliState() != lastAppliState)
		{
			if (GetUsbAppliState() == APPLICATION_READY)
			{
				flag_app=1;
				
				int res = f_mount(&USBHFatFS,USBHPath,1);//挂载U盘
												 //USBHFatFS、USBHPath这两个参数系统在fatfs.c里面已经定义好了，直接用
				if(res == FR_OK)
				{
					printf("f_mount SUCCESSFUL: %s\r\n",USBHPath);
//					g_flag_mount=1;
				}
				else
				{
					printf("f_mount ERROR: %d\r\n",res);
				}
				
				res = f_opendir(&picdir, "0:/");
				if(res==FR_OK)
				{
					printf("f_opendir SUCCESSFUL 0:/ddd\r\n");
				}
				 totpicnum = pic_get_tnum("0:/");     /* 得到总有效文件数 */
				
				printf("000totpicnum = %d\r\n",totpicnum);
				
	      picoffsettbl =(uint32_t *) Mem_malloc(EXSRAM,4 * totpicnum);             /* 申请4*totpicnum个字节的内存,用于存放图片索引 */	
				if(!picoffsettbl)
				   				printf("malloc picoffsettbl err err !!!\r\n");
				else
					printf("111malloc picoffsettbl succ !!!\r\n");
			

				
				
				pname =(uint8_t *) malloc(FF_MAX_LFN * 2 + 1);            /* 为带路径的文件名分配内存 */
				
				if(!pname)
					printf("malloc pname err err !!!\r\n");
				else
					printf("malloc pname succ !!!\r\n");
				
				
				
				picfileinfo = (FILINFO *)malloc(sizeof(FILINFO)); /* 申请内存 */
				
				if(!picfileinfo)
				   				printf("malloc picfileinfo err err !!!\r\n");
				
				
    /* 记录索引 */
    res = f_opendir(&picdir, "0:/PICTURE");         /* 打开目录 */

    if (res == FR_OK)
    {
        curindex = 0;   /* 当前索引为0 */

        while (1)       /* 全部查询一遍 */
        {
            temp = picdir.dptr;                     /* 记录当前dptr偏移 */
            res = f_readdir(&picdir, picfileinfo);  /* 读取目录下的一个文件 */

            if (res != FR_OK || picfileinfo->fname[0] == 0)
                break;  /* 错误了/到末尾了,退出 */

            res = 0;//exfuns_file_type(picfileinfo->fname);

            if ((res & 0XF0) == 0X50)               /* 取高四位,看看是不是图片文件 */
            {
                picoffsettbl[curindex] = temp;      /* 记录索引 */
                curindex++;
            }
        
					}
				}
		
	      LCD_DisplayString(20,190,16,RED,(uint8_t *)"show a picture");	
				
	      osDelay(1500);
//        piclib_init();                                          /* 初始化画图 */
        curindex = 0;                                           /* 从0开始显示 */
        res = f_opendir(&picdir, (const TCHAR *)"0:/PICTURE");  /* 打开目录 */

    if (res == FR_OK)    /* 打开成功 */
    {
        dir_sdi(&picdir, picoffsettbl[curindex]);           /* 改变当前目录索引 */		
	        res = f_readdir(&picdir, picfileinfo);              /* 读取目录下的一个文件 */

        if (res != FR_OK || picfileinfo->fname[0] == 0)
					printf("xxxxxxxxxxxxxx\r\n");
//            break;          /* 错误了/到末尾了,退出 */		
	        strcpy((char *)pname, "0:/PICTURE/");                    /* 复制路径(目录) */
          strcat((char *)pname, (const char *)picfileinfo->fname); /* 将文件名接在后面 */		
		 //     LCD_Clear(BLACK);	
//		      res = piclib_ai_load_picfile((char*)pname, 0, 0, lcd_width, lcd_height, 1); /* 显示图片 */
          printf("piclib_ai_load_picfile  res=0x%x\r\n",res);
				
				
				
		}			
				free(pname);
				free(picfileinfo);
				free(picoffsettbl);
				
/*				res = f_open(&USBHFile,"0:/PICTURE/config.bmp",FA_READ);//打开文件
		    printf("xxxxxxxxxxxxxxxxxxx\r\n");
				if(res == FR_OK)
				{
					printf("f_open SUCCESSFUL 0:/PICTURE/config.bmp\r\n");
					char buffer[10] = { 0 };
					res = f_read(&USBHFile,buffer,10,&br);
					if (br == 10)
						printf("f_read buffer = %s \r\n",buffer);
					else
							printf("f_read buffer %s len =%d \r\n",buffer,br);
					f_close(&USBHFile);	
				}
				else
				{
					printf("!!!!!!!!f_open ERROR: %d\r\n",res);
				}*/
				
				res = f_open(&USBHFile, "0:/config.txt", FA_WRITE | FA_OPEN_APPEND);
				if (res == FR_OK)
				{
					printf("f_open SUCCESSFUL 0:/config.txt\r\n");
					UINT len = strlen("Hello\n");
					f_write(&USBHFile, "Hello\n", len, &bw);
					if (len == bw)
						printf("f_write success \r\n");
					f_close(&USBHFile);
				}
				else
				{
					printf("f_open ERROR: %d\r\n",res);
				}
			}
			lastAppliState = GetUsbAppliState();
		}
	  return flag_app;
}
