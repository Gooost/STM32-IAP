/*
 * download.c
 *
 *  Created on: 2017年9月21日
 *      Author: Administrator
 *    Function:等待用户选择传送文件操作,或者放弃操作以及一些提示信息，
 *             但真正实现传送的是ymodem．c源文件。
 */
#include "include.h"

extern uint8_t file_name[FILE_NAME_LENGTH];
uint8_t tab_1024[1024] = {0};

/**
  * @brief   通过串口接收一个文件
  * @param  
  * @retval None.
  */
void SerialDownload(void)
{
    uint8_t Number[10] = "          ";
    int32_t Size = 0;

    SerialPutString("Waiting for the file to be sent ... (press 'a' to abort)\n\r");
    Size = Ymodem_Receive(&tab_1024[0]);
    if (Size > 0)
    {
        SerialPutString("\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
        SerialPutString(file_name);
        Int2Str(Number, Size);
        SerialPutString("\n\r Size: ");
        SerialPutString(Number);
        SerialPutString(" Bytes\r\n");
        SerialPutString("-------------------\r\n");
        FLASH_ErasePage(FLASH_LAST_PAGE);               //擦除IAP升级标志位存放页
    }
    else if (Size == -1)
    {
        SerialPutString("\n\n\rThe image size is higher than the allowed space memory!\n\r");
    }
    else if (Size == -2)
    {
        SerialPutString("\n\n\rVerification failed!\n\r");
    }
    else if (Size == -3)
    {
        SerialPutString("\r\n\nAborted by user.\n\r");
    }
    else
    {
        SerialPutString("\n\rFailed to receive the file!\n\r");
    }
}
