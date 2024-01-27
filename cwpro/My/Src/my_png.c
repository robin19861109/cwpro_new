#include "my_png.h"
#include "ff.h"
#include "exfuns.h"
#include "malloc.h"

//Check if the PNG file exists.
#define FF_USE_LFN		3
#define FF_MAX_LFN		255


uint16_t pic_get_tnum(char *path)
{
    uint8_t res;
    uint16_t rval = 0;
    DIR tdir;                                                 /* ��ʱĿ¼ */
	  FILINFO dtfileinfo;
	
    FILINFO *tfileinfo=(FILINFO *)&dtfileinfo;                                       /* ��ʱ�ļ���Ϣ */

  //  tfileinfo = (FILINFO *)Mem_malloc(INSRAM, sizeof(FILINFO)); /* �����ڴ� */
	//  goto  _end_;

    res = f_opendir(&tdir, (const TCHAR *)path);              /* ��Ŀ¼ */

    if (res == FR_OK && tfileinfo)
    {
        while (1)                               /* ��ѯ�ܵ���Ч�ļ��� */
        {
            res = f_readdir(&tdir, tfileinfo);  /* ��ȡĿ¼�µ�һ���ļ� */

            if (res != FR_OK || tfileinfo->fname[0] == 0)
                break;                          /* ������/��ĩβ��,�˳� */

            res = exfuns_file_type(tfileinfo->fname);

            if ((res & 0XF0) == 0X50)           /* ȡ����λ,�����ǲ���ͼƬ�ļ� */
            {
                rval++;                         /* ��Ч�ļ�������1 */
            }
        }
    }

//		_end_:		    Mem_free(INSRAM, tfileinfo);                  /* �ͷ��ڴ� */
        return rval;
}



uint16_t pic_get_pngnum(char *path)
{
    uint8_t res;
    uint16_t rval = 0;
    DIR tdir;                                                 /* ��ʱĿ¼ */
	  FILINFO dtfileinfo;
	
     FILINFO *tfileinfo=(FILINFO *)&dtfileinfo;                                       /* ��ʱ�ļ���Ϣ */

    tfileinfo = (FILINFO *)Mem_malloc(EXSRAM, sizeof(FILINFO)); /* �����ڴ� */
	  if(tfileinfo==NULL)
			goto  _end_;

    res = f_opendir(&tdir, (const TCHAR *)path);              /* ��Ŀ¼ */

    if (res == FR_OK && tfileinfo)
    {
        while (1)                               /* ��ѯ�ܵ���Ч�ļ��� */
        {
            res = f_readdir(&tdir, tfileinfo);  /* ��ȡĿ¼�µ�һ���ļ� */

            if (res != FR_OK || tfileinfo->fname[0] == 0)
                break;                          /* ������/��ĩβ��,�˳� */

            res = exfuns_file_type1(tfileinfo->fname,"PNG");

            if(res==1)           /* ȡ����λ,�����ǲ���ͼƬ�ļ� */
            {
                rval++;                         /* ��Ч�ļ�������1 */
            }
        }
    }

		_end_:		    Mem_free(EXSRAM, tfileinfo);                  /* �ͷ��ڴ� */
        return rval;
}

int chkPngFile(void)
{
		DIR picdir;             /* ͼƬĿ¼ */
	  uint16_t totpicnum=0;     /* ͼƬ�ļ����� */
	  uint32_t *picoffsettbl; /* ͼƬ�ļ�offset������ */
		FILINFO *picfileinfo;   /* �ļ���Ϣ */
	  uint8_t *pname;	
	
	  totpicnum = pic_get_pngnum("0:/PICTURE/PNG");     /* �õ�����Ч�ļ��� */
				
	  printf("000totpicnum = %d\r\n",totpicnum);	
		picoffsettbl =(uint32_t *) Mem_malloc(EXSRAM,4 * totpicnum);             /* ����4*totpicnum���ֽڵ��ڴ�,���ڴ��ͼƬ���� */	
	  if(!picoffsettbl)
				   				printf("malloc picoffsettbl err err !!!\r\n");
		else
		printf("111malloc picoffsettbl succ !!!\r\n");
	/*	pname =(uint8_t *) malloc(FF_MAX_LFN * 2 + 1);            // Ϊ��·�����ļ��������ڴ� 
		if(!pname)
					printf("malloc pname err err !!!\r\n");
		else
		{

					printf("malloc pname succ !!!\r\n");	
		}*/

	 return totpicnum;
}
