#include "my_key.h"
#include "my_motor.h"
#include "my_uart.h"

/*========================================================
�������ƣ�KeyValue_TypeDef KeyScanf(void)
�������ܣ�����ɨ�躯��
��    ������
�� �� ֵ������ֵ
����ʱ�䣺2019.11.3
��    ע��
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
�������ƣ�void KeyRemoveTremble(void)
�������ܣ�ȥ�������ж���������
��    ������
�� �� ֵ����
����ʱ�䣺2019.11.3
��    ע��
===========================================================*/
void KeyRemoveTremble(void)
{
     if(KeyScanf())                          //��ɨ�赽�м�����
     {
        if(KeyCnt == 0)                      //�����ɨ���־=0��
        {
            KeyFlagStructure.KeyValueOne = KeyScanf();  //��ֵΪ��һ�ΰ���
            KeyCnt++;
        }
        else
        {
            KeyFlagStructure.KeyValueTwo = KeyScanf();  //��ֵΪ�ڶ��ΰ���
            KeyCnt++;
        }
        if(KeyCnt > 1)    //����Ѿ��õ��ڶ��ΰ���ֵ
        {
            if(KeyFlagStructure.KeyValueOne != KeyFlagStructure.KeyValueTwo)   //�����һ�ΰ���ֵ�����ڵڶ��ΰ���ֵ
            {
                KeyCnt = 0;                   //�����ɨ���־
            }
        }
        
        if(KeyCnt == SHORT_KEY_VALID_TIME_LONG)      //�����ɨ���־����SHORT_KEY_VALID_TIME_LONG�����ж�Ϊһ����Ч����
        {
            KeyFlagStructure.KeyValue = KeyFlagStructure.KeyValueTwo;       //���泤����ֵ
            KeyFlagStructure.ShortKeyPressFlag = 1;
        }
        if((KeyFlagStructure.ShortKeyPressFlag == 1) && (KeyCnt >= LONG_KEY_VALID_TIME_LONG))
        {
            if((KeyCnt % SHORT_KEY_VALID_TIME_LONG) == 0)         //�˴�������������Ч���� ���ڳ�����״̬�¶೤ʱ��ִ��һ�ΰ�����Ӧ����
            {
                KeyFlagStructure.KeyValue = KeyFlagStructure.KeyValueTwo;
                KeyFlagStructure.LongKeyPressFlag = 1;
            }
        }
    }
    else                     //ɨ�赽û�м�����ʱ�������а�����־
    {
        KeyCnt = 0;
        KeyFlagStructure.KeyValue = 0;
        KeyFlagStructure.KeyValueOne = 0;
        KeyFlagStructure.KeyValueTwo = 0;
        KeyFlagStructure.LongKeyPressFlag = 0;
        KeyFlagStructure.ShortKeyPressFlag = 0;
    }
    Beep_Key(&KeyFlagStructure);	    //������������
}

//����������
void Beep_Key(KeyFlag_TypeDef *keyflag)
{
    //�̰���ʹ��
    static u32 Beep_Ttime_Short = 0;
    static u8 GetTimeFlag_Short = 0;	

    //�̰�������  δ����ʱ��������Ч
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
    //��������ʱ��ر�
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
�������ƣ�void Key_Action(void)
�������ܣ����ݼ�ֵ������ع��ܺ���
��    ������
�� �� ֵ����
����ʱ�䣺2019.11.4
��    ע��
===========================================================*/ 
void Key_Action(void)
{   
    u8 KeyDown;

    //���水��ֵ�����ڶ�ʱ��������
    KeyDown = KeyFlagStructure.KeyValue;

    if(KeyDown)        
    {
        debug_str("key down\r\n");
        debug_data((char *)&KeyDown, 1);
        KeyFlagStructure.KeyValue = NonKey;     //�����ֵ
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