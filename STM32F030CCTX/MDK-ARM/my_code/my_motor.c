#include "my_motor.h"
#include "my_global.h"

/* 0: 停止， 1: 高速 2：低速 */
char motorSpeed;

void motor_enable(char isRun)
{
    HAL_GPIO_WritePin(Motor_En_GPIO_Port, Motor_En_Pin, isRun == 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    if (isRun == 0x01)
        HAL_GPIO_WritePin(Motor_BAK1_GPIO_Port, Motor_BAK1_Pin, GPIO_PIN_SET);
    motorSpeed = ((isRun == 0x01) ? (MOTOR_STA_HIGH) : MOTOR_STA_STOP);
}

/* 1: 高速， 2：低速, 0 停止*/
void motor_speedCtl(char speed)
{
    if (motorSpeed == 0)
        return;
    
    if (speed == 0x02) {
        HAL_GPIO_TogglePin(Motor_BAK1_GPIO_Port, Motor_BAK1_Pin);
        motorSpeed = (motorSpeed == MOTOR_STA_LOW) ? MOTOR_STA_HIGH : MOTOR_STA_LOW;
        return;
    }
    HAL_GPIO_WritePin(Motor_BAK1_GPIO_Port, Motor_BAK1_Pin, speed == 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    motorSpeed = (speed == 0x01) ? MOTOR_STA_HIGH : MOTOR_STA_LOW;
}
