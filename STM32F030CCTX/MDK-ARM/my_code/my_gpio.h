#ifndef MY_GPIO_H
#define MY_GPIO_H


#include "stm32f0xx_hal.h"
#include "main.h"
#include "my_global.h"
#include "gpio.h"

#define LED1_ON HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)
#define LED1_OFF HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET)

#define PRESS_PWR_ON HAL_GPIO_WritePin(P_POWER_GPIO_Port, P_POWER_Pin, GPIO_PIN_SET)
#define PRESS_PWR_OFF HAL_GPIO_WritePin(P_POWER_GPIO_Port, P_POWER_Pin, GPIO_PIN_RESET)

#define FAN_ON HAL_GPIO_WritePin(FAN_IO_GPIO_Port, FAN_IO_Pin, GPIO_PIN_SET)
#define FAN_OFF HAL_GPIO_WritePin(FAN_IO_GPIO_Port, FAN_IO_Pin, GPIO_PIN_RESET)

#define BUZZER_ON HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET)
#define BUZZER_OFF HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET)
	 

void Led_Init(void);


#endif

