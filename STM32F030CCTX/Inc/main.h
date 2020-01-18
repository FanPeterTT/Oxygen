/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OLED_RST_Pin GPIO_PIN_13
#define OLED_RST_GPIO_Port GPIOC
#define OLED_PWOER_EN_Pin GPIO_PIN_14
#define OLED_PWOER_EN_GPIO_Port GPIOC
#define BAT_LOW_Pin GPIO_PIN_15
#define BAT_LOW_GPIO_Port GPIOC
#define P_POWER_Pin GPIO_PIN_0
#define P_POWER_GPIO_Port GPIOF
#define UNCAP_Pin GPIO_PIN_1
#define UNCAP_GPIO_Port GPIOF
#define V12_ADC_Pin GPIO_PIN_1
#define V12_ADC_GPIO_Port GPIOA
#define T_ADC_Pin GPIO_PIN_4
#define T_ADC_GPIO_Port GPIOA
#define P1_ADC_Pin GPIO_PIN_5
#define P1_ADC_GPIO_Port GPIOA
#define P2_ADC_Pin GPIO_PIN_6
#define P2_ADC_GPIO_Port GPIOA
#define KEY1_Pin GPIO_PIN_7
#define KEY1_GPIO_Port GPIOA
#define KEY2_Pin GPIO_PIN_0
#define KEY2_GPIO_Port GPIOB
#define MOTOR_PWM_Pin GPIO_PIN_1
#define MOTOR_PWM_GPIO_Port GPIOB
#define KEY3_Pin GPIO_PIN_2
#define KEY3_GPIO_Port GPIOB
#define Motor_En_Pin GPIO_PIN_10
#define Motor_En_GPIO_Port GPIOB
#define Motor_BAK1_Pin GPIO_PIN_11
#define Motor_BAK1_GPIO_Port GPIOB
#define Motor_BAK2_Pin GPIO_PIN_12
#define Motor_BAK2_GPIO_Port GPIOB
#define BC20_RST_Pin GPIO_PIN_13
#define BC20_RST_GPIO_Port GPIOB
#define BC20_ONOFF_Pin GPIO_PIN_14
#define BC20_ONOFF_GPIO_Port GPIOB
#define BC20_WKUP_Pin GPIO_PIN_15
#define BC20_WKUP_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_8
#define BUZZER_GPIO_Port GPIOA
#define FAN_IO_Pin GPIO_PIN_11
#define FAN_IO_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_12
#define LED1_GPIO_Port GPIOA
#define BREATHE_IO_Pin GPIO_PIN_15
#define BREATHE_IO_GPIO_Port GPIOA
#define WASH_IO_Pin GPIO_PIN_3
#define WASH_IO_GPIO_Port GPIOB
#define B_DENITROGEN_IO_Pin GPIO_PIN_4
#define B_DENITROGEN_IO_GPIO_Port GPIOB
#define B_COMPRESS_IO_Pin GPIO_PIN_5
#define B_COMPRESS_IO_GPIO_Port GPIOB
#define A_DENITROGEN_IO_Pin GPIO_PIN_6
#define A_DENITROGEN_IO_GPIO_Port GPIOB
#define A_COMPRESS_IO_Pin GPIO_PIN_7
#define A_COMPRESS_IO_GPIO_Port GPIOB
#define OLED_I2C_SCL_Pin GPIO_PIN_8
#define OLED_I2C_SCL_GPIO_Port GPIOB
#define OLED_I2C_SDA_Pin GPIO_PIN_9
#define OLED_I2C_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
