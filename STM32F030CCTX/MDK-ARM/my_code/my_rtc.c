#include "my_rtc.h"
#include "my_uart.h"
#include "my_key.h"

#include <stdio.h>

//  LSI/(RTC_AsynchPrediv+1)/(RTC_SynchPrediv +1) =1

void rtc_init(void)
{
    /** Initialize RTC Only 
    */
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 99;
    hrtc.Init.SynchPrediv = 399;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    if (HAL_RTC_Init(&hrtc) != HAL_OK)
    {
    Error_Handler();
    }

    /* USER CODE BEGIN Check_RTC_BKUP */
    if (GetIncKeyValue == 0x00) {
        rtc_setTime(16, 30, 50, RTC_HOURFORMAT12_PM);
        rtc_setDate(19, 12, 22, 7);
    }

    /* USER CODE END Check_RTC_BKUP */

    /** Enable the Alarm A 
    */

    /** Enable the WakeUp 
    */
//    if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK)
//    {
//    Error_Handler();
//    }
}

HAL_StatusTypeDef rtc_setTime(uint8_t h, uint8_t m, uint8_t s, uint8_t ampm)
{
    RTC_TimeTypeDef rtcTime;
    rtcTime.Hours = h;
    rtcTime.Minutes = m;
    rtcTime.Seconds = s;
    rtcTime.TimeFormat = ampm;
    rtcTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    rtcTime.StoreOperation = RTC_STOREOPERATION_RESET;
    
    return HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
}

HAL_StatusTypeDef rtc_setDate(uint8_t year, uint8_t month, uint8_t date, uint8_t week)
{
	RTC_DateTypeDef rtcDate;
    
	rtcDate.Date = date;
	rtcDate.Month = month;
	rtcDate.WeekDay = week;
	rtcDate.Year = year;
	return HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);
}

void rtc_setAlarmA(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec)
{ 
    RTC_AlarmTypeDef RTC_AlarmSturuct;
    
    RTC_AlarmSturuct.AlarmTime.Hours=hour;  //小时
    RTC_AlarmSturuct.AlarmTime.Minutes=min; //分钟
    RTC_AlarmSturuct.AlarmTime.Seconds=sec; //秒
    RTC_AlarmSturuct.AlarmTime.SubSeconds=0;
    RTC_AlarmSturuct.AlarmTime.TimeFormat=RTC_HOURFORMAT12_AM;
    
    RTC_AlarmSturuct.AlarmMask=RTC_ALARMMASK_NONE;//精确匹配星期，时分秒
    RTC_AlarmSturuct.AlarmSubSecondMask=RTC_ALARMSUBSECONDMASK_NONE;
    RTC_AlarmSturuct.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;//按星期
    RTC_AlarmSturuct.AlarmDateWeekDay=week; //星期
    RTC_AlarmSturuct.Alarm=RTC_ALARM_A;     //闹钟A
    HAL_RTC_SetAlarm_IT(&hrtc,&RTC_AlarmSturuct,RTC_FORMAT_BIN);
    
    HAL_NVIC_SetPriority(RTC_IRQn,0x00,0x00); //抢占优先级1,子优先级2
    HAL_NVIC_EnableIRQ(RTC_IRQn);
}


void rtc_setWakeUp(uint32_t wksel, uint16_t cnt)
{
     __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);//清除RTC WAKE UP的标志
	
	HAL_RTCEx_SetWakeUpTimer_IT(&hrtc,cnt,wksel);            //设置重装载值和时钟 
	
    HAL_NVIC_SetPriority(RTC_IRQn,0x0,0x0); //抢占优先级1,子优先级2
    HAL_NVIC_EnableIRQ(RTC_IRQn);
}

////RTC闹钟中断服务函数
//void RTC_Alarm_IRQHandler(void)
//{
//    HAL_RTC_AlarmIRQHandler(&hrtc);
//}

uint8_t isSetAlarm = 0x01;
    
//RTC闹钟A中断处理回调函数
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    isSetAlarm = 0x01;
    debug_str("ALARM A!\r\n");
}

////RTC WAKE UP中断服务函数
//void RTC_WKUP_IRQHandler(void)
//{
//    HAL_RTCEx_WakeUpTimerIRQHandler(&hrtc); 
//}

//RTC WAKE UP中断处理
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    debug_str("HAL_RTCEx_WakeUpTimerEventCallback\r\n");
}

void rtc_printfDataTime(void)
{
    uint8_t tbuf[64] = {0};
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    
    HAL_RTC_GetTime(&hrtc,&RTC_TimeStruct,RTC_FORMAT_BIN);
    sprintf((char*)tbuf,"Time:%02d:%02d:%02d",RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds); 
    debug_str((char *)tbuf);
    HAL_RTC_GetDate(&hrtc,&RTC_DateStruct,RTC_FORMAT_BIN);
    sprintf((char*)tbuf,"Date:20%02d-%02d-%02d",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date); 
    debug_str((char *)tbuf);
    sprintf((char*)tbuf,"Week:%d\r\n",RTC_DateStruct.WeekDay); 
    debug_str((char *)tbuf);
}

uint8_t rtc_getMin(void)
{
    RTC_TimeTypeDef RTC_TimeStruct;
    HAL_RTC_GetTime(&hrtc,&RTC_TimeStruct,RTC_FORMAT_BIN);
    return RTC_TimeStruct.Minutes;
}

/* rtc 测试 */
void rtc_alarmWakeUptest(void)
{
    static uint8_t tick = 0;
    uint8_t tbuf[40];
    uint8_t h;
    uint8_t m;
    uint8_t s;
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    tick++;
    
    if (tick >= 10 && isSetAlarm == 0x01) {
          isSetAlarm = 0x00;
        
        HAL_RTC_GetTime(&hrtc, &RTC_TimeStruct, RTC_FORMAT_BIN);
        sprintf((char*)tbuf,"Time:%02d:%02d:%02d\n",RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds); 
        HAL_RTC_GetDate(&hrtc,&RTC_DateStruct,RTC_FORMAT_BIN);
        debug_str((char *)tbuf);
        
        h = RTC_TimeStruct.Hours;
        m = RTC_TimeStruct.Minutes;
        s = RTC_TimeStruct.Seconds;
        s += 5;
        if (s >= 60) {
            m += 1;
            s = 4;
        }
        if (m >= 60) {
            m = 0;
            h += 1;
        }
        #if 1
        HAL_UART_Transmit(&huart1, "hello world\n", 12, 1000);
        sprintf((char *)tbuf, "w = %d, h = %d, m = %d, s = %d\n",RTC_DateStruct.WeekDay, h, m, s);
        debug_str((char *)tbuf);

        rtc_setAlarmA(RTC_DateStruct.WeekDay, h, m , s);          
        
        tick = 0;
        //Sys_Enter_Standby();
        sys_enterStandby();
        #endif
      }
}


void system_power_config(void);
void sys_enterStandby(void)
{
    system_power_config();
    if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
	{
		/* Clear Standby flag */
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB); 
	}
  
    /* Disable Wakeup Counter */
    HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
	
	/*To configure the wake up timer to 4s the WakeUpCounter is set to 0x242B:
	RTC_WAKEUPCLOCK_RTCCLK_DIV = RTCCLK_Div16 = 16 
	Wakeup Time Base = 16 /(~37KHz) = ~0,432 ms
	Wakeup Time = ~5s = 0,432ms  * WakeUpCounter
	==> WakeUpCounter = ~5s/0,432ms = 11562 */
	//i = standbytime*2396;
    /* ????rtc???? */
    //HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 10, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);
    
	/* Clear all related wakeup flags */
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

	 /* Enter the Standby mode */
	HAL_PWR_EnterSTANDBYMode();
}

void system_power_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* Enable GPIOs clock */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
	/* Note: Debug using ST-Link is not possible during the execution of this   */
	/*       example because communication between ST-link and the device       */
	/*       under test is done through UART. All GPIO pins are disabled (set   */
	/*       to analog input mode) including  UART I/O pins.           */
	GPIO_InitStructure.Pin = GPIO_PIN_All;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure); 
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Disable GPIOs clock */
	__HAL_RCC_GPIOA_CLK_DISABLE();
	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOD_CLK_DISABLE();

	/* Configure RTC */
	hrtc.Instance = RTC;
	/* Configure RTC prescaler and RTC data registers as follow:
	- Hour Format = Format 24
	- Asynch Prediv = Value according to source clock
	- Synch Prediv = Value according to source clock
	- OutPut = Output Disable
	- OutPutPolarity = High Polarity
	- OutPutType = Open Drain */
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 99;
	hrtc.Init.SynchPrediv = 399;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if(HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler(); 
	}
}

