#ifndef MY_KEY_H
#define MY_KEY_H


#include "main.h"
#include "my_global.h"
#include "my_gpio.h"
#include "my_tim.h"
#include "my_standby.h"

#define GetIncKeyValue    HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)
#define GetDecKeyValue    HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)
#define GetModeKeyValue   HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin)

#define LONG_KEY_VALID_TIME_LONG   150    	  //长按键有效时间长度*10ms
#define SHORT_KEY_VALID_TIME_LONG  20   		//短按键有效时间长度*10ms 

KeyValue_TypeDef KeyScanf(void);
void KeyRemoveTremble(void);
void Beep_Key(KeyFlag_TypeDef *keyflag);
void Key_Action(void);

u8 getKeyValue(void);
void clearKeyValue(void);
#endif
