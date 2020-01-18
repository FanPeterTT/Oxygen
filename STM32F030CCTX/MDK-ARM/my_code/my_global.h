#ifndef _MY_GLOBAL_H
#define _MY_GLOBAL_H
#include "main.h"

//���¶���һЩȫ�ֱ������ڵ�һ��ʹ��ʱ
//��Ҫ����MY_EXT��������Ҫ����

#ifdef   MY_GLOBAL
#define  MY_EXT
#else
#define  MY_EXT extern
#endif

#define u8      uint8_t
#define u16     uint16_t
#define u32     uint32_t

#define  OPENCLOSESYSTIMES  10    //  ���ػ���Ҫ����������ʱ��  10*100ms = 1000ms = 1s
#define UART2_RX_LENGTH   6

//ADCͨ���������ֱ�Ϊ:SMALL��1KPa  BIT_500KPa   Temperature  Voltage
#define ADC_CH                     4
#define ADC_TIMES            20    //�¶�ADֵ��200�����ڲɼ�20����ȡƽ��ֵ  ����
#define BP_COMPENSATE_VALUE   15   //��ѹ������
#define TEMPETURE_COMPENSATE_VALUE  10  //�¶Ȳ���ֵ


//ϵͳ����		
typedef struct
{
    u32	  System_ID;            //�豸ID    1~1048575       FFFFFʮ��������ֵ
    u8    System_Mode;          //�豸״̬  0���ģʽ  1������ģʽ   2������ģʽ
    u8    Alarm_Type;           //��������  0����   1���¶�   2��ѹ��  3����ѹ  4�����
    
    u32   Cumulative_Time;      //�ۼ�ʱ��  0~1048575����   FFFFFʮ��������ֵ
    u32   Available_time;       //����ʱ��  0~1048575����   FFFFFʮ��������ֵ
    
    u16    Uncap_Times;          //���Ǵ���  0~255��
    
    u8    Phone_Number[6];         //�绰�����8λ         11λ   0~9����
    
    u8    ThresholdTemp;        //�¶���ֵ  40~80��      70
    u8    ThresholdLowPress;    //��ѹ��ֵ  0~100KPa    20
    
    u16    ValveOpenDelay;       //������ʱ   100-2000   10ms 
    u32   Breath_Open_Press;    //��������  400~1200ms
    u16   Breath_Close_Time;    //��������  400~1200ms  �ط�����
    int Breath_Close_Press;   //��������  600-1200Pa  �ط����� 1.2
    
    u16   EnerjySave_Delay;     //������ʱ  120~300s    180    
    u16   Alarm_Detec_Time;     //��������ʱ��  1~300��     60

    float Longitude;            //����      0
    float Latitude;             //γ��      0
    float Altitude;             //����      0

    u32   AC_Valve_Close_time;  //A??????              600
    u32   Valve_interval_time;  //?????                 40

    u32   FrontHalfPeriodTime;        //����Aʱ�� 400-800ms   600
    u32   AfterHalfPeriodTime;        //����Bʱ�� 10-200      80
    u32   OffsetTime;        //����Cʱ��
    
    u8    Sos_Password[3];         //��������   5λ����ÿλȡֵ1~10����λ�롣
    u8    Xor_CRC;              //���У���   
}SystemParameter_TypeDef;

//W��״̬ö������
typedef enum
{
    END = 0,             //δ����
    FrontHalfPeriod = 1,  //ǰ������
    AfterHalfPeriod = 2  //�������
}Switch_TypeDef;

typedef struct
{
    u32   StartTimeScale_F;             //F����ʼ����ʱ��̶�
    Switch_TypeDef   SwitchState_F;        //F������״̬��־
    u32   StartTimeScale_WA;             //WA����ʼ����ʱ��̶�
    u32   StartTimeScale_WB;             //WB����ʼ����ʱ��̶�
    u32   StartTimeScale_EQ;             //WB����ʼ����ʱ��̶�
    u8   SwitchState_WA;             //WA������״̬��־
    u8   SwitchState_WB;             //WB������״̬��־
    u8   SwitchState_EQ;             //EQ������״̬��־
}SwitchParameter_TypeDef;
////????		
//typedef struct
//{
//    u32	  System_ID;            //??ID    1~1048575       FFFFF??????
//    u32   System_Mode;          //????  0:????  1:????   2:????
//    u32   Alarm_Type;           //????  0:?   1:??   2:??  3:??  4:??
//    u32   Cumulative_Time;      //????  0~1048575??   FFFFF??????
//    u32   Available_time;       //????  0~1048575??   FFFFF??????
//    u32   Uncap_Times;          //????  0~255?
//    u32   Phone_Number;         //????  11?   0~9??
//    u32   Temperature;          //????  40~80?    
//    u32   WashLow_Press;        //????  0~100KPa
//    u32   ValveOpenDelay;      //????   10~2000 *10ms   
//    u32   Breath_Open_Press;    //????  -200pa~+200Pa
//    u32   Breath_Close_Time;    //????  400~1200ms  ????
//    u32   Breath_Close_Press;   //????  0.6~1.2KPa  ????
//    u32   EnerjySave_Delay;     //????  120~300s
//    u32   Alarm_Detec_Time;     //????  1~300?
//    u32   AC_Valve_Close_time;  //A??????              600
//    u32   Valve_interval_time;  //?????                 40
//    u32   Longitude;            //??
//    u32   Latitude;             //??
//    u32   Altitude;             //??
//    u32   Sos_Password;         //????   6??,????1~10,????
//    u32   Xor_CRC;              //?????   
//}SystemParameter_TypeDef;
////�����ֵö������
typedef enum
{
    NonKey = 0,          //��Ч��ֵ
    ModKey = 1,          //ģʽ��ֵ
    IncKey = 2,
    DecKey = 3,
    WakeUpKey = 4,
}KeyValue_TypeDef;


//����������־�����ṹ��
typedef struct
{
    u8 KeyValueOne;         //��һ�μ�ֵ  
    u8 KeyValueTwo;         //�ڶ��μ�ֵ
    u8 LongKeyPressFlag;    //��������־
    u8 ShortKeyPressFlag;   //�̰�����־
    u8 KeyValue;            //��Ч��ֵ
    u8 IncDecSleepKeyFlag;  //�����+-���ܼ�    ������ʾ�����¶�
    u8 IncDecSleepKeyFlag1; //�����+-���ܼ�	  ��������
    u8 KeySaveFlag;         //�����+-���ܼ�	  ��������
    u32 IncDecSleepStartTime;//�����+-���ܼ���ʼ��ʱ
}KeyFlag_TypeDef;


//�¶ȱ���
typedef struct
{
    u8 Negative;         //�����������
    u8 value;  //�¶�ֵ
}Tempeture_TypeDef;

//��ǰ�Ĺ���ģʽ
typedef enum 
{
    POWER_Working   = 0,       //����ģʽ
    POWER_WeakUping   = 1,     //����ģʽ
    POWER_Standby  = 2         //�ػ�ģʽ
}Oxygenerator_State_Type;

//���ȴ�����ö������
typedef enum
{
    SOUND_NONE = 0, //��������ռ��
    SHORT_KEY = 1,  //�̰�������
    LONG_KEY = 2,   //����������
    SOUND_ALARM = 3 //����������
}Sound_TypeDef;

//����ADC_DMA�����е�ADC����
typedef enum
{
   S_PRESSURE = 0x00,       //ADC data type :small pressure
   B_PRESSURE = 0x01,       //ADC data type :big pressure
   TEMPERATURE = 0x02,      //ADC data type :temperature
   EXT_VOLTAGEE = 0x03      //ADC data type :external power
}ADCdata_TypeDef;

MY_EXT Tempeture_TypeDef temperture;   //�¶�ֵ
MY_EXT u16 Ext_Voltage;                //��ѹֵ
MY_EXT u16 B_Pressure_value;           //��ѹ��
MY_EXT u16 S_Pressure_value;           //Сѹ��

MY_EXT  u8                        KeyDownTimes;	                       //������ʱ��
MY_EXT  u32                       Timer6_Cnt;                          //��ʱ��6��10ms������
MY_EXT  u32                       ADC_DMA_BUF[ADC_CH];                 //ADCת�����
MY_EXT  u8                        temp1;                               //����ʹ����ʱ����
MY_EXT  u8                        KeyCnt;                              //����������
MY_EXT  KeyFlag_TypeDef           KeyFlagStructure;                    //�������ṹ��
MY_EXT  SystemParameter_TypeDef   SystemPar;                           //ϵͳ����	
MY_EXT  Sound_TypeDef             Sound_Type;                          //�����źŷ����������ö�ٱ���
MY_EXT  SwitchParameter_TypeDef   SwitchPar;                         //����������ʱ��ṹ�����

MY_EXT  u8                        Uart1_Rx_Buf[UART2_RX_LENGTH];       //����1���ջ���
MY_EXT  u8                        Uart1_Tx_Buf[UART2_RX_LENGTH];       //����1���ͻ���
MY_EXT  u8                        Uart1_Rx_Flag;                       //����1���ձ�־
MY_EXT  u8                        Uart2_Rx_Flag;                       //����2���ձ�־
MY_EXT  u8                        Uart2_Rx_Buf[UART2_RX_LENGTH];       //����2���ջ���
MY_EXT  u8                        Uart2_Tx_Buf[UART2_RX_LENGTH];       //����2���ͻ���

MY_EXT  u16 i;


MY_EXT  RTC_TimeTypeDef  Current_RTC_Time;   //��ǰRTCʱ��
MY_EXT  RTC_DateTypeDef  Current_RTC_Date;   //��ǰRTCʱ��
MY_EXT  u32  rtc_i;   //��ǰRTCʱ��

/*
MY_EXT u32 Minute_cnt_falg;                 //���Ӽ�ʱ��ʼ��־
MY_EXT u32 Minute_cnt;                      //���Ӽ�ʱ��ʼ
MY_EXT u32 Second_cnt;                      //���ڵ����ȿ���ʱ�ļ�ʱ��ʼ
MY_EXT u32 Timer6_Cnt;                      //��ʱ��16����ֵ
MY_EXT u8 AD_Finish_Flag;                   //20��AD�ɼ���ɲ��Ѿ�ʹ�õı�־
MY_EXT u32 Out_Temperatrue_buf[ADC_TIMES];  //��ˮ�¶Ȳ�������
MY_EXT u32 Back_Temperatrue_buf[ADC_TIMES]; //��ˮ�¶Ȳ�������
MY_EXT u32 PUMP1_I_buf[ADC_TIMES];          //��1������������
MY_EXT u32 PUMP2_I_buf[ADC_TIMES];          //��2������������  

MY_EXT u32 Timer16_Cnt;                     //��ʱ��16����ֵ
MY_EXT u32 Timer17_Cnt;                     //��ʱ��16����ֵ
MY_EXT u8  txbuf[TXBATY_NUMBER];            //USART ���ͻ���
MY_EXT u8  Display_Set_Buf;                 //�����¶Ȼ���
MY_EXT u8  Display_Back_Buf;                //��ˮ�¶Ȼ���
MY_EXT u8  Display_Out_Buf;                 //��ˮ�¶Ȼ���
MY_EXT u16 Pump1_Current_Buf;               //��1��������
MY_EXT u8  Pump1_State_Flag;                //��1״̬��ʶ
MY_EXT u8  Pump1_Check_Flag;                //��1״̬��ʶ

MY_EXT u32 ADC_DMA_Buf[ADC_BIT];            //ADת��ֵ
MY_EXT KeyFlag_TypeDef  KeyFlagStructure;   //�������ṹ��
MY_EXT u8 KeyCnt;                           //����������
MY_EXT u8 KeyDownTimes;	                    //���ػ�������ʱ��

*/
#endif
