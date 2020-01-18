#ifndef MY_UART_H
#define MY_UART_H
#ifdef __cplusplus
 extern "C" {
#endif
     
#include "main.h"
#include "stm32f0xx_hal.h"
#include "my_global.h"
#include "usart.h"
#include  "my_fifo.h"

extern Fifo uart1Fifo;
extern Fifo uart2Fifo;

void uart_enableIt(void);

void debug_str(char *str);

void debug_data(char *data, int len);

#ifdef __cplusplus
}
#endif
#endif
