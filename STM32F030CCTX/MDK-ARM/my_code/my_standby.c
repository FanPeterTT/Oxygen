#include "my_standby.h"

void StandbyMode_Measure(void)
{  

      /* Enable Power Clock*/
    __HAL_RCC_PWR_CLK_ENABLE();

    /* Allow access to Backup */
    HAL_PWR_EnableBkUpAccess();

      /* Reset RTC Domain */
    __HAL_RCC_BACKUPRESET_FORCE();
    __HAL_RCC_BACKUPRESET_RELEASE();


    /* Disable all used wakeup sources: Pin1(PA.0) */
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);

    /* Clear all related wakeup flags */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

    __HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc,RTC_FLAG_WUTF);
    
    /* Re-enable all used wakeup sources: Pin1(PA.0) */
    //HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2);

    /*## Enter Standby Mode ####################################################*/
    /* Request to enter STANDBY mode  */
    HAL_PWR_EnterSTANDBYMode(); 
}

void Sys_Enter_Standby(void)
{
    __HAL_RCC_APB2_FORCE_RESET();//
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOD_CLK_DISABLE();
    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();

    StandbyMode_Measure();
}

uint8_t PWR_Check_Standby(void)
{
    uint8_t press_cnt =0;
    uint8_t release_cnt =0;

    while(1)
    {
        HAL_Delay(20);  
        if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) == GPIO_PIN_SET) 
        {
            press_cnt++;
            release_cnt=0;
            if(press_cnt>=100)
            {
                return 1;
            }
        }
        else
        {
            release_cnt++;
            if(release_cnt>5)
            {
                return 0;
            }
        }
    }
}


void WKUP_Init(void)
{

//    Led_Init();
    
    /* Get the pending status of the AlarmA Interrupt */
//    if (__HAL_RTC_ALARM_GET_FLAG(&hrtc, RTC_FLAG_ALRAF) != RESET)
//    {
//      /* Clear the AlarmA interrupt pending bit */
////      __HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);
//        return;
//    }

    if(PWR_Check_Standby()==0)
    {
        Sys_Enter_Standby();
    }
}

void  HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    
 	//if (GPIO_Pin == WKUP_Pin)
	{
		if(PWR_Check_Standby() == 1)
		{
            Sys_Enter_Standby();
		}
	}   
    
    
}

#if 0
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//if (GPIO_Pin == WKUP_Pin)
	{
		if(PWR_Check_Standby() == 1)
		{
			  Sys_Enter_Standby();
		}
	}
}
#endif
