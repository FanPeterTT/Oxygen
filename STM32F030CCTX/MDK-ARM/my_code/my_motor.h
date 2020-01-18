#ifndef __MY_MOTOR_H__
#define __MY_MOTOR_H__

typedef enum {
    MOTOR_STA_STOP,
    MOTOR_STA_HIGH,
    MOTOR_STA_LOW,
}MOTOR_STA;

extern char motorSpeed;
void motor_enable(char isRun);
void motor_speedCtl(char speed);

#endif

