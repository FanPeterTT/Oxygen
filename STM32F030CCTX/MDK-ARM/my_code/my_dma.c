#include "my_dma.h"

#if 0
u8 arry1[100];
u8 arry2[200] = {8,8,8,7,8,3,3,3};
HAL_UART_Transmit_DMA(&huart2,&arry1[0],sizeof(arry1));
HAL_Delay(50);
Uart_DMA_bug(&huart2);

HAL_UART_Transmit_DMA(&huart2,&arry2[0],sizeof(arry2));
Uart_DMA_bug(&huart2);
HAL_UART_Transmit_DMA(&huart2,&arry[0],sizeof(arry));


HAL_Delay(10);
Uart_DMA_bug(&huart2);



HAL_UART_Receive_DMA(&huart1,&arry1[0],sizeof(arry1));
Uart_DMA_bug(&huart1);
HAL_Delay(100);
HAL_UART_Transmit_DMA(&huart2,&arry1[0],sizeof(arry1));
Uart_DMA_bug(&huart2);
HAL_Delay(3000);

#endif
