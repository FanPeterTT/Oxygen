#include "my_gpio.h"


/*======================================
��������:void SwitchABWork(void)
��������:����������ʱ��LEDϨ��
��    ��:��
�� �� ֵ:��
����ʱ��:2019.11.2
��Standbyģʽ�е���
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




