#include "usb.h"

#include "fatfs.h"
/* USBͨѶ���ݴ��� */
	/*void my_USBH_UserProcess(USBH_HandleTypeDef *phost,uint8_t id)//ApplicationTypeDef Appli_state
{

    switch(id)
    {
        case APPLICATION_READY:
            MSC_Application();
          //  id = APPLICATION_DISCONNECT;
			
            break;
        
        case APPLICATION_DISCONNECT:
            f_mount(NULL, (const TCHAR*)"",0);
						 osDelay(10);
            break;
        
        default:
            break;
    }
		
	}

void my_USBH_UserProcess(USBH_HandleTypeDef *phost,ApplicationTypeDef Appli_state)
{
        static int status = 0;  
         if(Appli_state == APPLICATION_READY)//U���Ѿ��������
        {
					printf("99999 ssdlh\n");
		 			  if(status==0)
						{
							status=1;
							retUSBH=f_mount(&USBHFatFS, (TCHAR const*)USBHPath, 0);
							if(retUSBH==FR_OK)
							{
								printf("USB drive mounted successfully!\r\n");
								
								osDelay(1);
							}		
						}
						
						
 	       retUSBH = f_open(&USBHFile,(const char*)"����.txt",FA_CREATE_ALWAYS|FA_WRITE);
            if(retUSBH==FR_OK)
            {
              printf("open file\"test.txt\"successfully!\r\n");
            }		
	          retUSBH = f_close(&USBHFile);			
							{
								printf("close file successfully!\r\n\r\n");
								
							}
          if(status == 0)//U�̼��غ�ֻ����һ��
          {
            status = 1;
            retUSBH=f_mount(&USBHFatFS, (TCHAR const*)USBHPath, 1);
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"����U�̳ɹ�!\r\n",14,100);
            }
            //д���ļ�����
            HAL_UART_Transmit(&huart1,(uint8_t*)"д���ļ�����!\r\n",15,100);
            retUSBH = f_open(&USBHFile,(const char*)"����.txt",FA_CREATE_ALWAYS|FA_WRITE);
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"���ļ�\"����.txt\"�ɹ�!\r\n",25,100);
            }
			
			retUSBH = f_lseek(&USBHFile,f_size(&USBHFile));
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"ָ���ƶ����ļ�ĩβ�ɹ���\r\n",25,100);
            }
			
        //    retUSBH = f_write(&USBHFile,FileBuf,18,&bw);
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"д���ļ��ɹ�!\r\n",15,100);
            }
            retUSBH = f_close(&USBHFile);
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"�ر��ļ��ɹ�!\r\n\r\n",17,100);
            }
            //��ȡ�ļ�����
            HAL_UART_Transmit(&huart1,(uint8_t*)"��ȡ�ļ�����!\r\n",15,100);
            retUSBH = f_open(&USBHFile,(const char*)"����.txt",FA_OPEN_EXISTING|FA_READ);//���ļ�
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"���ļ�\"����.txt\"�ɹ�!\r\n",25,100);
            }
        //    retUSBH = f_read(&USBHFile,FileBuf,18,&bw);
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"��ȡ�ļ��ɹ�!�ļ����ݣ�",23,100);
       //       HAL_UART_Transmit(&huart1,FileBuf,bw,100);
            }
            retUSBH = f_close(&USBHFile);
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"�ر��ļ��ɹ�!\r\n",15,100);
			}
		}
	  else if(Appli_state == APPLICATION_IDLE)//U��δ����
	  {
		   //�ȴ��´�U�̲���������д��
	  }
		else if(Appli_state ==APPLICATION_DISCONNECT)
		{
			
	//	   	retUSBH=f_mount(NULL, (TCHAR const*)USBHPath, 1);
				//						if(retUSBH==FR_OK)
							{
								printf("123 USB drive mounted successfully!\r\n");
							}		
			
			
           retUSBH = f_close(&USBHFile);
            if(retUSBH==FR_OK)
            {
              printf("�ر��ļ��ɹ�!\r\n");
						}
			
		}
	
	
}*/

