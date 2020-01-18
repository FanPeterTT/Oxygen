#include "my_gpio.h"


/*======================================
函数名称:void SwitchABWork(void)
函数功能:开机键按下时让LED熄灭
参    数:无
返 回 值:无
创建时间:2019.11.2
在Standby模式中调用
======================================*/
void Led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
	
	GPIO_InitStruct.Pin = LED1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);	
}




