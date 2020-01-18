
#include "my_tim.h"


//定时器中断服务函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim6)
    {
        Timer6_Cnt++;
        KeyRemoveTremble();
        if(Timer6_Cnt % 50 == 0)
        {
            LED1_ON;
        }
        else if(Timer6_Cnt % 50 == 25)
        {
            LED1_OFF;
        }
    }

}

u32 GetTimeCnt(void)
{
	return Timer6_Cnt;
}

void tim_enableIt(void)
{
    HAL_TIM_Base_Start_IT(&htim6);  //使能定时器6(10ms)
    //HAL_TIM_Base_Start_IT(&htim3);  //启动定时器3(motor pwm)    
}

