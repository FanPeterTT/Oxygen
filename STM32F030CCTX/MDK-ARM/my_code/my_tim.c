
#include "my_tim.h"


//��ʱ���жϷ�����
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
    HAL_TIM_Base_Start_IT(&htim6);  //ʹ�ܶ�ʱ��6(10ms)
    //HAL_TIM_Base_Start_IT(&htim3);  //������ʱ��3(motor pwm)    
}

