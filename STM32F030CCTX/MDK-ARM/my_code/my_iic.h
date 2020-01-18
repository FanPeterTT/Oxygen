#ifndef MY_IIC
#define MY_IIC

#ifdef __cplusplus
 extern "C" {
#endif
     
#include "main.h"
#include "stm32f0xx_hal.h"
     
     
#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9输出模式

void IIC_SCL(char isHigh);
void IIC_SDA(char isHigh);
GPIO_PinState READ_SDA(void);

void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(unsigned char txd);
unsigned char IIC_Read_Byte(unsigned char ack);
unsigned char IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);

#ifdef __cplusplus
}
#endif
#endif
