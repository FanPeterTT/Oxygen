#include "my_uart.h"

#include <stdio.h>
#include <string.h>

uint8_t dataUart1 = 0;
uint8_t dataUart2 = 0;

Fifo uart1Fifo;
Fifo uart2Fifo;

void fifoInit(void)
{
    fifoInitial(&uart1Fifo);
    fifoInitial(&uart2Fifo);
}

void uart_enableIt(void)
{
    fifoInit();
    HAL_UART_Receive_IT(&huart1, &dataUart1, 1);
    HAL_UART_Receive_IT(&huart2, &dataUart2, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        fifoWrite(&uart1Fifo, dataUart1);
        HAL_UART_Receive_IT(&huart1, &dataUart1, 1);
    } else if (huart->Instance == USART2) {
        fifoWrite(&uart2Fifo, dataUart2);
        HAL_UART_Receive_IT(&huart2, &dataUart2, 1);
    }
}

void debug_str(char *data)
{
    HAL_UART_Transmit(&huart2, (unsigned char *)data, strlen(data), 1000);
}

void debug_data(char *data, int len)
{
    HAL_UART_Transmit(&huart2, (unsigned char *)data, len, 1000);
}

void uart2_dataTrans(void)
{
    static int dataLen = 0;
    unsigned char rxBuf[128] = {0};
    dataLen = fifoReadDataS(&uart2Fifo, (unsigned char *)rxBuf);
    if (dataLen > 0)
        HAL_UART_Transmit(&huart2, (unsigned char *)rxBuf, dataLen, 1000);
}



#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
    return ch;
}
