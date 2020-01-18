#include "my_key.h"
#include "my_motor.h"
#include "my_uart.h"

/*========================================================
函数名称：KeyValue_TypeDef KeyScanf(void)
函数功能：按键扫描函数
参    数：无
返 回 值：按键值
创建时间：2019.11.3
备    注：
======================================================*/

KeyValue_TypeDef KeyScanf(void)
{
    if((GetIncKeyValue == 0)&&(GetDecKeyValue == 1)&&(GetModeKeyValue ==1))
    {
        return IncKey;
    }
    else if((GetIncKeyValue == 1)&&(GetDecKeyValue == 0)&&(GetModeKeyValue ==1))
    {
        return DecKey;
    }
    else if((GetIncKeyValue == 1)&&(GetDecKeyValue == 1)&&(GetModeKeyValue ==0))
    {
        return ModKey;
    }	
//    else if((GetIncKeyValue == 1)&&(GetDecKeyValue == 1)&&(GetModeKeyValue ==1))
//    {
//        return OnOffKey;
//    }			
    else 
    {
        return NonKey;
    }
}

/*==========================================================
函数名称：void KeyRemoveTremble(void)
函数功能：去抖动及判断连续按键
参    数：无
返 回 值：无
创建时间：2019.11.3
备    注：
===========================================================*/
void KeyRemoveTremble(void)
{
     if(KeyScanf())                          //键扫描到有键按下
     {
        if(KeyCnt == 0)                      //如果键扫描标志=0，
        {
            KeyFlagStructure.KeyValueOne = KeyScanf();  //键值为第一次按键
            KeyCnt++;
        }
        else
        {
            KeyFlagStructure.KeyValueTwo = KeyScanf();  //键值为第二次按键
            KeyCnt++;
        }
        if(KeyCnt > 1)    //如果已经得到第二次按键值
        {
            if(KeyFlagStructure.KeyValueOne != KeyFlagStructure.KeyValueTwo)   //如果第一次按键值不等于第二次按键值
            {
                KeyCnt = 0;                   //清零键扫描标志
            }
        }
        
        if(KeyCnt == SHORT_KEY_VALID_TIME_LONG)      //如果键扫描标志大于SHORT_KEY_VALID_TIME_LONG次则判定为一次有效按键
        {
            KeyFlagStructure.KeyValue = KeyFlagStructure.KeyValueTwo;       //保存长按键值
            KeyFlagStructure.ShortKeyPressFlag = 1;
        }
        if((KeyFlagStructure.ShortKeyPressFlag == 1) && (KeyCnt >= LONG_KEY_VALID_TIME_LONG))
        {
            if((KeyCnt % SHORT_KEY_VALID_TIME_LONG) == 0)         //此处调整长按键有效速率 即在长按键状态下多长时间执行一次按键相应动作
            {
                KeyFlagStructure.KeyValue = KeyFlagStructure.KeyValueTwo;
                KeyFlagStructure.LongKeyPressFlag = 1;
            }
        }
    }
    else                     //扫描到没有键按下时清零所有按键标志
    {
        KeyCnt = 0;
        KeyFlagStructure.KeyValue = 0;
        KeyFlagStructure.KeyValueOne = 0;
        KeyFlagStructure.KeyValueTwo = 0;
        KeyFlagStructure.LongKeyPressFlag = 0;
        KeyFlagStructure.ShortKeyPressFlag = 0;
    }
    Beep_Key(&KeyFlagStructure);	    //按键蜂鸣器响
}

//按键音处理
void Beep_Key(KeyFlag_TypeDef *keyflag)
{
    //短按键使用
    static u32 Beep_Ttime_Short = 0;
    static u8 GetTimeFlag_Short = 0;	

    //短按键音打开  未开机时按键音无效
    if(((SystemPar.System_Mode == POWER_WeakUping) || (SystemPar.System_Mode == POWER_Working)) && (keyflag ->KeyValue != NonKey))
    {
        if(keyflag ->ShortKeyPressFlag == 1) 
        {
            if(GetTimeFlag_Short == 0)
            {
                Beep_Ttime_Short = GetTimeCnt();
                GetTimeFlag_Short = 1;
                if(Sound_Type == SOUND_NONE)
                {
                    BUZZER_ON;
                    Sound_Type = SHORT_KEY;					
                }									
            }							
        }
    }
    //按键音到时间关闭
    if(Sound_Type != SOUND_NONE)
    {
        if((GetTimeFlag_Short == 1) && ((GetTimeCnt() - Beep_Ttime_Short) == (SHORT_KEY_VALID_TIME_LONG - 3)))
        {
            BUZZER_OFF;
            Sound_Type = SOUND_NONE;
            GetTimeFlag_Short = 0;
            Beep_Ttime_Short = 0;						 
        }
    }
}
/*==========================================================
函数名称：void Key_Action(void)
函数功能：根据键值调用相关功能函数
参    数：无
返 回 值：无
创建时间：2019.11.4
备    注：
===========================================================*/ 
void Key_Action(void)
{   
    u8 KeyDown;

    //保存按键值，已在定时器中消抖
    KeyDown = KeyFlagStructure.KeyValue;

    if(KeyDown)        
    {
        debug_str("key down\r\n");
        debug_data((char *)&KeyDown, 1);
        KeyFlagStructure.KeyValue = NonKey;     //清除键值
        switch(KeyDown) {
            case NonKey:
                break;
            case ModKey:
                //Sys_Enter_Standby();
                motor_speedCtl(0x02);
                break;		
            case IncKey:
                break;
            case DecKey:
                break;
            default: 
                break;
        }
    }
}

u8 getKeyValue(void)
{
    return KeyFlagStructure.KeyValue;
}

void clearKeyValue(void)
{
    KeyFlagStructure.KeyValue = NonKey;
}