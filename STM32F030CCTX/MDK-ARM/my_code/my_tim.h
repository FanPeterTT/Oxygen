#ifndef MY_TIM_H
#define MY_TIM_H


#include "my_gpio.h"
#include "my_global.h"
#include "tim.h"
#include "my_key.h"

u32 GetTimeCnt(void);

void tim_enableIt(void);   //������ʱ��6�жϣ�led

#endif

