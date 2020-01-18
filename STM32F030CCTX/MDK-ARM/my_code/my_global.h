#ifndef _MY_GLOBAL_H
#define _MY_GLOBAL_H
#include "main.h"

//以下定义一些全局变量，在第一次使用时
//需要定义MY_EXT，否则不需要定义

#ifdef   MY_GLOBAL
#define  MY_EXT
#else
#define  MY_EXT extern
#endif

#define u8      uint8_t
#define u16     uint16_t
#define u32     uint32_t

#define  OPENCLOSESYSTIMES  10    //  开关机需要连续按键的时间  10*100ms = 1000ms = 1s
#define UART2_RX_LENGTH   6

//ADC通道数量，分别为:SMALL±1KPa  BIT_500KPa   Temperature  Voltage
#define ADC_CH                     4
#define ADC_TIMES            20    //温度AD值在200毫秒内采集20个数取平均值  ？？
#define BP_COMPENSATE_VALUE   15   //大压力补偿
#define TEMPETURE_COMPENSATE_VALUE  10  //温度补偿值


//系统参数		
typedef struct
{
    u32	  System_ID;            //设备ID    1~1048575       FFFFF十六进制数值
    u8    System_Mode;          //设备状态  0：额定模式  1：节能模式   2：开机模式
    u8    Alarm_Type;           //报警类型  0：无   1：温度   2：压力  3：电压  4：电机
    
    u32   Cumulative_Time;      //累计时间  0~1048575分钟   FFFFF十六进制数值
    u32   Available_time;       //可用时长  0~1048575分钟   FFFFF十六进制数值
    
    u16    Uncap_Times;          //开盖次数  0~255次
    
    u8    Phone_Number[6];         //电话号码低8位         11位   0~9的数
    
    u8    ThresholdTemp;        //温度阈值  40~80℃      70
    u8    ThresholdLowPress;    //低压阈值  0~100KPa    20
    
    u16    ValveOpenDelay;       //开机延时   100-2000   10ms 
    u32   Breath_Open_Press;    //呼吸阀开  400~1200ms
    u16   Breath_Close_Time;    //呼吸阀关  400~1200ms  关阀条件
    int Breath_Close_Press;   //呼吸阀关  600-1200Pa  关阀条件 1.2
    
    u16   EnerjySave_Delay;     //节能延时  120~300s    180    
    u16   Alarm_Detec_Time;     //报警持续时间  1~300秒     60

    float Longitude;            //经度      0
    float Latitude;             //纬度      0
    float Altitude;             //海拔      0

    u32   AC_Valve_Close_time;  //A??????              600
    u32   Valve_interval_time;  //?????                 40

    u32   FrontHalfPeriodTime;        //阀门A时序 400-800ms   600
    u32   AfterHalfPeriodTime;        //阀门B时序 10-200      80
    u32   OffsetTime;        //阀门C时序
    
    u8    Sos_Password[3];         //紧急密码   5位数，每位取值1~10，单位秒。
    u8    Xor_CRC;              //异或校验和   
}SystemParameter_TypeDef;

//W阀状态枚举类型
typedef enum
{
    END = 0,             //未运行
    FrontHalfPeriod = 1,  //前半周期
    AfterHalfPeriod = 2  //后半周期
}Switch_TypeDef;

typedef struct
{
    u32   StartTimeScale_F;             //F阀开始工作时间刻度
    Switch_TypeDef   SwitchState_F;        //F阀工作状态标志
    u32   StartTimeScale_WA;             //WA阀开始工作时间刻度
    u32   StartTimeScale_WB;             //WB阀开始工作时间刻度
    u32   StartTimeScale_EQ;             //WB阀开始工作时间刻度
    u8   SwitchState_WA;             //WA阀工作状态标志
    u8   SwitchState_WB;             //WB阀工作状态标志
    u8   SwitchState_EQ;             //EQ阀工作状态标志
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
////定义键值枚举类型
typedef enum
{
    NonKey = 0,          //无效键值
    ModKey = 1,          //模式键值
    IncKey = 2,
    DecKey = 3,
    WakeUpKey = 4,
}KeyValue_TypeDef;


//声明按键标志变量结构体
typedef struct
{
    u8 KeyValueOne;         //第一次键值  
    u8 KeyValueTwo;         //第二次键值
    u8 LongKeyPressFlag;    //长按键标志
    u8 ShortKeyPressFlag;   //短按键标志
    u8 KeyValue;            //有效键值
    u8 IncDecSleepKeyFlag;  //如果是+-节能键    用于显示设置温度
    u8 IncDecSleepKeyFlag1; //如果是+-节能键	  用于喇叭
    u8 KeySaveFlag;         //如果是+-节能键	  用于喇叭
    u32 IncDecSleepStartTime;//如果是+-节能键开始计时
}KeyFlag_TypeDef;


//温度变量
typedef struct
{
    u8 Negative;         //正数负数标记
    u8 value;  //温度值
}Tempeture_TypeDef;

//当前的工作模式
typedef enum 
{
    POWER_Working   = 0,       //制氧模式
    POWER_WeakUping   = 1,     //开机模式
    POWER_Standby  = 2         //关机模式
}Oxygenerator_State_Type;

//喇叭打开属性枚举类型
typedef enum
{
    SOUND_NONE = 0, //蜂鸣器无占用
    SHORT_KEY = 1,  //短按键声打开
    LONG_KEY = 2,   //长按键声打开
    SOUND_ALARM = 3 //报警声音打开
}Sound_TypeDef;

//定义ADC_DMA缓存中的ADC类型
typedef enum
{
   S_PRESSURE = 0x00,       //ADC data type :small pressure
   B_PRESSURE = 0x01,       //ADC data type :big pressure
   TEMPERATURE = 0x02,      //ADC data type :temperature
   EXT_VOLTAGEE = 0x03      //ADC data type :external power
}ADCdata_TypeDef;

MY_EXT Tempeture_TypeDef temperture;   //温度值
MY_EXT u16 Ext_Voltage;                //电压值
MY_EXT u16 B_Pressure_value;           //大压力
MY_EXT u16 S_Pressure_value;           //小压力

MY_EXT  u8                        KeyDownTimes;	                       //键按下时间
MY_EXT  u32                       Timer6_Cnt;                          //定时器6的10ms计数器
MY_EXT  u32                       ADC_DMA_BUF[ADC_CH];                 //ADC转换结果
MY_EXT  u8                        temp1;                               //调试使用临时变量
MY_EXT  u8                        KeyCnt;                              //按键计数器
MY_EXT  KeyFlag_TypeDef           KeyFlagStructure;                    //键抖动结构体
MY_EXT  SystemParameter_TypeDef   SystemPar;                           //系统参数	
MY_EXT  Sound_TypeDef             Sound_Type;                          //互斥信号蜂鸣器定义的枚举变量
MY_EXT  SwitchParameter_TypeDef   SwitchPar;                         //排气阀工作时间结构体变量

MY_EXT  u8                        Uart1_Rx_Buf[UART2_RX_LENGTH];       //串口1接收缓存
MY_EXT  u8                        Uart1_Tx_Buf[UART2_RX_LENGTH];       //串口1发送缓存
MY_EXT  u8                        Uart1_Rx_Flag;                       //串口1接收标志
MY_EXT  u8                        Uart2_Rx_Flag;                       //串口2接收标志
MY_EXT  u8                        Uart2_Rx_Buf[UART2_RX_LENGTH];       //串口2接收缓存
MY_EXT  u8                        Uart2_Tx_Buf[UART2_RX_LENGTH];       //串口2发送缓存

MY_EXT  u16 i;


MY_EXT  RTC_TimeTypeDef  Current_RTC_Time;   //当前RTC时间
MY_EXT  RTC_DateTypeDef  Current_RTC_Date;   //当前RTC时间
MY_EXT  u32  rtc_i;   //当前RTC时间

/*
MY_EXT u32 Minute_cnt_falg;                 //分钟计时开始标志
MY_EXT u32 Minute_cnt;                      //分钟计时开始
MY_EXT u32 Second_cnt;                      //用于当加热开启时的计时开始
MY_EXT u32 Timer6_Cnt;                      //定时器16计数值
MY_EXT u8 AD_Finish_Flag;                   //20次AD采集完成并已经使用的标志
MY_EXT u32 Out_Temperatrue_buf[ADC_TIMES];  //出水温度采样缓存
MY_EXT u32 Back_Temperatrue_buf[ADC_TIMES]; //回水温度采样缓存
MY_EXT u32 PUMP1_I_buf[ADC_TIMES];          //泵1电流采样缓存
MY_EXT u32 PUMP2_I_buf[ADC_TIMES];          //泵2电流采样缓存  

MY_EXT u32 Timer16_Cnt;                     //定时器16计数值
MY_EXT u32 Timer17_Cnt;                     //定时器16计数值
MY_EXT u8  txbuf[TXBATY_NUMBER];            //USART 发送缓存
MY_EXT u8  Display_Set_Buf;                 //设置温度缓存
MY_EXT u8  Display_Back_Buf;                //回水温度缓存
MY_EXT u8  Display_Out_Buf;                 //出水温度缓存
MY_EXT u16 Pump1_Current_Buf;               //泵1电流缓存
MY_EXT u8  Pump1_State_Flag;                //泵1状态标识
MY_EXT u8  Pump1_Check_Flag;                //泵1状态标识

MY_EXT u32 ADC_DMA_Buf[ADC_BIT];            //AD转换值
MY_EXT KeyFlag_TypeDef  KeyFlagStructure;   //键抖动结构体
MY_EXT u8 KeyCnt;                           //按键计数器
MY_EXT u8 KeyDownTimes;	                    //开关机键按下时间

*/
#endif
