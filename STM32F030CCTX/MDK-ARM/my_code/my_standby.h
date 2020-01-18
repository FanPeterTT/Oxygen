#ifndef MY_STANDBY_H
#define MY_STANDBY_H

#include "main.h"
#include "stm32f0xx_hal.h"
#include "my_gpio.h"
#include "gpio.h"
#include "my_global.h"
#include "rtc.h"
#include "stm32f0xx_hal_rtc.h"

void StandbyMode_Measure(void);
void Sys_Enter_Standby(void);
uint8_t PWR_Check_Standby(void);
void WKUP_Init(void);


#endif



