#ifndef MY_RTC_H
#define MY_RTC_H


#include "rtc.h"
#include "my_global.h"

extern uint8_t isSetAlarm;

void rtc_init(void);

HAL_StatusTypeDef rtc_setTime(uint8_t h, uint8_t m, uint8_t s, uint8_t ampm);

HAL_StatusTypeDef rtc_setDate(uint8_t year, uint8_t month, uint8_t date, uint8_t week);

void rtc_setWakeUp(uint32_t wksel, uint16_t cnt);

void rtc_setAlarmA(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec);

void sys_enterStandby(void);

void rtc_printfDataTime(void);

void rtc_alarmWakeUptest(void);

uint8_t rtc_getMin(void);

#endif
