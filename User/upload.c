/*
 * upload.c
 *
 *  Created on: 2017年9月21日
 *      Author: Administrator
 *    Function:上传文件的相关函数
 */
#include "include.h"

/**
  * @brief   通过串口上传一个文件
  * @param  
  * @retval None.
  */
void SerialUpload(void)
{
    uint32_t status = 0;

    SerialPutString("\n\n\rSelect Receive File ... (press any key to abort)\n\r");

    if (GetKey() == CRC16)
    {
        //通过ymodem协议上传程序
        status = Ymodem_Transmit((uint8_t*)ApplicationAddress, (const uint8_t*)"UploadedFlashImage.bin", FLASH_IMAGE_SIZE);

        if (status != 0)
        {
            SerialPutString("\n\rError Occured while Transmitting File\n\r");
        }
        else
        {
            SerialPutString("\n\rFile Trasmitted Successfully \n\r");
        }
    }
    else
    {
        SerialPutString("\r\n\nAborted by user.\n\r");
    }

}
