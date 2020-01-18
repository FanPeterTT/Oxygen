#include "my_sensor.h"


/*=========================================================
将NTC经ADC转换数据通过查表得出温度数据,范围-20~99 °C
=========================================================*/
const unsigned short int Temperature_Table[120] = {
// -20  -19  -18  -17  -16   -15  -14  -13  -12  -11
   498, 521, 544, 568, 593,  618, 645, 672, 700, 728,
// -10   -9   -8   -7   -6   -5   -4   -3   -2   -1
   757, 787, 818, 849, 881,  914, 947, 981, 1015, 1050,
//  0     1    2    3    4    5   6   7     8    9
   1085,1121,1157,1194,1231,1268,1306,1344,1388,1421,
//  10   11   12   13   14   15   16   17   18   19
   1454,1499,1538,1578,1617,1656,1696,1735,1775,1814,
//  20   21   22   23   24   25   26   27   28   29 
   1854,1896,1932,1971,2010,2048,2086,2124,2162,2200,
//  30   31   32   33   34   35   36   37   38   39 
   2237,2274,2310,2346,2382,2417,2452,2487,2521,2554,
//  40   41   42   43   44   45   46   47   48   49
   2587,2620,2652,2684,2715,2746,2777,2806,2836,2865,
//  50   51   52   53   54   55   56   57   58   59
   2893,2920,2948,2975,3001,3027,3052,3077,3101,3125,
//  60   61   62   63   64   65   66   67   68   69
   3148,3171,3193,3215,3237,3258,3278,3298,3318,3337,
//  70   71   72   73   74   75   76   77   78   79 
   3355,3374,3392,3409,3426,3443,3459,3475,3490,3505,
//  80   81   82   83   84   85   86   87   88   89 
   3520,3534,3548,3562,3575,3586,3601,3613,3625,3637,
//  90   91   92   93   94   95   96   97   98   99              
   3648,3659,3670,3681,3691,3701,3711,3720,3729,3739};

/*==========================================================
函数名称：void Get_Temperature_Numb(void)
函数功能：获取温度值
参    数：无
返 回 值：
创建时间：2019.11.26
备    注：查表获得的温度值,范围-19~99，采集20次数据取平均值
===========================================================*/
void Get_Temperature_Numb(void)
{
  unsigned char i = 0;
  unsigned short int number = 0;
  unsigned short int acc = 0;
  
  static unsigned char t = 0;
  static unsigned short int value[ADC_TIMES];
  
  if(t < ADC_TIMES)
  {
    number = (unsigned short int)ADC_DMA_BUF[TEMPERATURE];     //获取温度值

    //  如果温度值不在-19~99之间3秒则退出
    if((number > Temperature_Table[0]) && (number < Temperature_Table[119]))
    {
        for(i=0;i<120;i++)       //得出温度值，最大为99
        {
            if(number > Temperature_Table[i])
            {  
                if(((Temperature_Table[i]-Temperature_Table[i-1])/2) < (number-Temperature_Table[i-1]))
                {
                    i=i+1;       //四舍五入
                }              
            }
            else
            {
                value[t] = i;    //得出正温度值
                t++;  
                return ;        //exit for loop
            }    
        }           
    }
  }
  else
  {
      for(i = 0;i < ADC_TIMES;i ++)
      {
          acc = acc + value[i];
      }
      temperture.value = acc / ADC_TIMES - 20 - TEMPETURE_COMPENSATE_VALUE;   //这里本来是20的
      
      t = 0;
      return ;
  }    
}

/*==========================================================
函数名称：void Get_External_voltage(void)
函数功能：获取外置电池电压
参    数：无
返 回 值：无
创建时间：2019.11.26
备    注：电压关系：R1+R2=100K+24K串联取得电压U=UIN*(R2/(R1+R2))
          UIN=12V    U=12*(24/124)= 2.3    
          采集到的电压值为U采=AD值*(3.3/4096)
          如果是12V的话AD值是2854
          
===========================================================*/
void Get_External_voltage(void)
{
  unsigned char i = 0;
  unsigned short int number = 0;
  unsigned short int acc = 0;
  
  static unsigned char v = 0;
  static unsigned short int value[ADC_TIMES];
  if(v < ADC_TIMES)
  {
        number = (unsigned short int)ADC_DMA_BUF[EXT_VOLTAGEE];     //获取电压
        value[v] = number;
        v++;
  }
  else
  {
      for(i = 0;i < ADC_TIMES;i ++)
      {
          acc = acc + value[i];
      }
      acc = acc / ADC_TIMES;   //
      Ext_Voltage = acc;
      v = 0;
      return ;
  }   
}

/*==========================================================
函数名称：void Get_BPressure_value(void)
函数功能：获取大压力传感器值
参    数：无
返 回 值：无
创建时间：2019.11.26
备    注：压力关系0~500Kpa  对应0~3.3V
          KPa=(AD)*(3.3/4096)  4096/500=
===========================================================*/
void Get_BPressure_value(void)
{
  unsigned char i = 0;
  unsigned short int number = 0;
  unsigned short int acc = 0;
  
  static unsigned char b = 0;
  static unsigned short int value[ADC_TIMES];
  if(b < ADC_TIMES)
  {
        number = (unsigned short int)ADC_DMA_BUF[B_PRESSURE];     //获取电压
        value[b] = number;
        b++;
  }
  else
  {
      for(i = 0;i < ADC_TIMES;i ++)
      {
          acc = acc + value[i];
      }
      acc = acc / ADC_TIMES;   //
      B_Pressure_value = acc / (4096 / 500) - BP_COMPENSATE_VALUE;
      b = 0;
      return ;
  }   
}
/*==========================================================
函数名称：void Get_SPressure_value(void)
函数功能：获取小压力传感器值
参    数：无
返 回 值：无
创建时间：2019.11.26
备    注：压力关系0~2000pa  对应0~4096
          KPa=(AD)*(3.3/4096)  4096/2000=
===========================================================*/
void Get_SPressure_value(void)
{
  unsigned char i = 0;
  unsigned short int number = 0;
  unsigned short int acc = 0;
  
  static unsigned char s = 0;
  static unsigned short int value[ADC_TIMES];
  if(s < ADC_TIMES)
  {
        number = (unsigned short int)ADC_DMA_BUF[S_PRESSURE];     //获取电压
        value[s] = number;
        s++;
  }
  else
  {
      for(i = 0;i < ADC_TIMES;i ++)
      {
          acc = acc + value[i];
      }
      acc = acc / ADC_TIMES;   //
      S_Pressure_value = acc / (4096 / 2000);
      s = 0;
      return ;
  }   
}
