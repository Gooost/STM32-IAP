/*
 * main.c
 *
 *  Created on: 2017年9月21日
 *      Author: Administrator
 *    Function:USART初始化和RCC设置，然后从common.c中执行主菜单
 */
#include "include.h"

extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;

static void USART_Configuration(void);
static uint8_t FLASH_data_check(uint32_t faddr, uint16_t value);

/**
  * @brief   主函数 
  * @param  
  * @retval None.
  */
int main(void)
{
    uint8_t key = 0;

    USART_Configuration();
    if (FLASH_data_check(FLASH_LAST_PAGE, FLASH_DATA_VAL))
    {
    Upgrade_program:
        FLASH_Unlock();
        SerialPutString("\r\n Upgrade the program with Ymodem \r\n");
        Main_Menu();
    }
    else
    {
        __disable_irq();
        Execute_user_Program();
    }

    while (1)
    {
        SerialPutString("\r\n No user Program, Enter 9 to upgrade\r\n");
        key = GetKey();
        if (key == 0x39)
        {
            goto Upgrade_program;
        }
    }
}
/**
  * @brief   串口配置初始化
  * @param  
  * @retval None.
  */
void USART_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    // 配置 USART1 Tx (PA.09) 作为功能引脚并上拉输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //配置 USART1 Tx (PA.10) 作为功能引脚并是浮空输入模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* USART1 配置 ------------------------------------------------------------
         USART1 配置如下:
          - 波特率      = 115200 baud
          - 字长        = 8 Bits
          - 一个停止位
          - 无校验
          - 无硬件流控制
          - 接受和发送使能
    --------------------------------------------------------------------------*/
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}
/**
  * @brief  检验指定地址的数据是否符合
  * @param  faddr：指定地址
  *         value: 要比较的值
  * @retval 读到的数据
  */
uint8_t FLASH_data_check(uint32_t faddr, uint16_t value)
{
    uint16_t data_first = 0;
    uint8_t check_val = 0;

    data_first = *(vu16 *)faddr;
    if (data_first == value)
    {
        check_val = 1;
    }
    else
    {
        check_val = 0;
    }
    return check_val;
}
