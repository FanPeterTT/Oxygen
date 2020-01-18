#include "my_switch.h"

/*======================================
函数名称:void SwitchABWork(void)
函数功能:
         此参数从SystemPar中读取，可在SystemPar中修改
         AC:A压缩阀 开机打开10.2S后开始循环,周期:12.8S  6.0S关  6.8S开
         BC:B压缩阀 开机打开10.2S后开始循环,周期:12.8S  6.0S开  6.8S关
         AD:A排氮阀 开机打开10.2S后开始循环,周期:13.6S  5.2S开  8.4S关
         BD:B排氮阀 开机打开10.2S后开始循环,周期:13.6S  8.4S关  5.2S开
参    数：State  制氧机状态
返 回 值：无
创建时间：2019.11.2
======================================*/
void W_SwitchWork(SwitchParameter_TypeDef *SwitchParameter)
{
      
      if(Timer6_Cnt >= (SystemPar.ValveOpenDelay))  //开机延时阀不动作
      {
          //=========================================================
          if(SwitchParameter->SwitchState_WA == 0)  //阀周期未开始
          {
              SwitchParameter->SwitchState_WA = 1;  
              WA_SWITCH_CLOSE;     //WA上升沿
              SwitchParameter ->StartTimeScale_WA = Timer6_Cnt;  
          }
          
          //如果当前时间-时间刻度大于2倍的偏移周期   且 小于  当前时间-时间刻度大于（后半周期6.8S-2倍偏移）
          else if(((Timer6_Cnt - SwitchParameter->StartTimeScale_WA) >= (SystemPar.FrontHalfPeriodTime + SystemPar.OffsetTime * 2))
                  && (Timer6_Cnt - SwitchParameter ->StartTimeScale_WA < ((SystemPar.FrontHalfPeriodTime + SystemPar.OffsetTime * 2) + (SystemPar.AfterHalfPeriodTime - (SystemPar.OffsetTime * 2))))
                  && (SwitchParameter->SwitchState_WA == 1)
                  )
          {
              WA_SWITCH_OPEN;      //WA下降沿
              SwitchParameter->SwitchState_WA = 2; 
          }
          //如果当前时间-时间刻度大于（后半周期6.8S-2倍偏移）
          else if((Timer6_Cnt - SwitchParameter ->StartTimeScale_WA >= ((SystemPar.FrontHalfPeriodTime + SystemPar.OffsetTime * 2) + (SystemPar.AfterHalfPeriodTime - (SystemPar.OffsetTime * 2))))
                    && (SwitchParameter->SwitchState_WA == 2)
                   )
          {
              SwitchParameter->SwitchState_WA = 0;

          }
      }
      
      
      if(Timer6_Cnt >= (SystemPar.ValveOpenDelay + SystemPar.OffsetTime * 2 -10))  //开机延时阀不动作
      {
          //=========================================================
          if(SwitchParameter->SwitchState_WB == 0)  //阀周期未开始
          {
              SwitchParameter->SwitchState_WB = 1;  
              WB_SWITCH_OPEN;     //WB下降沿
              SwitchParameter ->StartTimeScale_WB = Timer6_Cnt;  
          }
          
          //如果当前时间-时间刻度大于2倍的偏移周期   且 小于  当前时间-时间刻度大于（后半周期6.8S-2倍偏移）
          else if(((Timer6_Cnt - SwitchParameter->StartTimeScale_WB) >= (SystemPar.AfterHalfPeriodTime - (SystemPar.OffsetTime * 2)))
                  && (Timer6_Cnt - SwitchParameter ->StartTimeScale_WB < ((SystemPar.FrontHalfPeriodTime + SystemPar.OffsetTime * 2) + (SystemPar.AfterHalfPeriodTime - (SystemPar.OffsetTime * 2))))
                  && (SwitchParameter->SwitchState_WB == 1)
                  )
          {
              WB_SWITCH_CLOSE;      //WB上升沿
              SwitchParameter->SwitchState_WB = 2; 
          }
          //如果当前时间-时间刻度大于（后半周期6.8S-2倍偏移）
          else if(((Timer6_Cnt - SwitchParameter ->StartTimeScale_WB) >= ((SystemPar.FrontHalfPeriodTime + SystemPar.OffsetTime * 2) + (SystemPar.AfterHalfPeriodTime - (SystemPar.OffsetTime * 2))))
                    && (SwitchParameter->SwitchState_WB == 2)
                   )
          {
              SwitchParameter->SwitchState_WB = 0;

          }
      }

}

void F_SwitchWork(SwitchParameter_TypeDef *SwitchParameter)
{
     if(Timer6_Cnt >= (SystemPar.ValveOpenDelay + SystemPar.OffsetTime))  //开机延时阀不动作
     {
        if(SwitchParameter->SwitchState_F == END)  //阀是否在周期内
        {
            SwitchParameter->SwitchState_F = FrontHalfPeriod;  
            FA_SWITCH_OPEN;  FB_SWITCH_CLOSE;
            SwitchParameter ->StartTimeScale_F = Timer6_Cnt;  
        }
        //如果当前时间-时间刻度大于前半周期，并且标志为前半周期时
        else if(((Timer6_Cnt - SwitchParameter->StartTimeScale_F) >= SystemPar.FrontHalfPeriodTime) && (SwitchParameter->SwitchState_F == FrontHalfPeriod))
        {
            FA_SWITCH_CLOSE;  FB_SWITCH_OPEN;
            SwitchParameter->SwitchState_F = AfterHalfPeriod; 
        }
        //如果当前时间-时间刻度大于前后半周期之和，并且标志为后半周期时
        else if((Timer6_Cnt - SwitchParameter ->StartTimeScale_F >= (SystemPar.FrontHalfPeriodTime + SystemPar.AfterHalfPeriodTime)) && (SwitchParameter->SwitchState_F == AfterHalfPeriod))
        {
            SwitchParameter->SwitchState_F = END;
        }
     }
     else
     {
					Switch_Init(1);       
     }
}


void EQ_SwitchWork(SwitchParameter_TypeDef *SwitchParameter)
{
     if(Timer6_Cnt >= (SystemPar.ValveOpenDelay + SystemPar.OffsetTime*2 - 10))  //开机延时阀不动作
     {
        if(SwitchParameter->SwitchState_EQ == 0)  //阀是否在周期内
        {
            SwitchParameter->SwitchState_EQ = 1;  
            EQ_SWITCH_CLOSE; 
            SwitchParameter ->StartTimeScale_EQ = Timer6_Cnt;  
        }
        //如果当前时间-时间刻度大于前半周期，并且标志为前半周期时
        else if(((Timer6_Cnt - SwitchParameter->StartTimeScale_EQ) >= (SystemPar.AfterHalfPeriodTime - (SystemPar.OffsetTime * 2)))
                &&(SystemPar.AfterHalfPeriodTime - (SystemPar.OffsetTime * 2) + (SystemPar.OffsetTime * 2 - 10))
                && (SwitchParameter->SwitchState_EQ == 1)
               )
        {
            EQ_SWITCH_OPEN;
            SwitchParameter->SwitchState_EQ = 2; 
        }
        //如果当前时间-时间刻度大于前后半周期之和，并且标志为后半周期时
        else if(((Timer6_Cnt - SwitchParameter ->StartTimeScale_EQ) >= ((SystemPar.AfterHalfPeriodTime - (SystemPar.OffsetTime * 2) + (SystemPar.OffsetTime * 2 - 10))))
                && (SwitchParameter->SwitchState_EQ == 2)
                )
        {
            SwitchParameter->SwitchState_EQ = 0;
        }
     }

}

#if 0

void SwitchWork(SystemParameter_TypeDef *State, unsigned int timeTick)
{


	static u8  start = 0;           //开机10秒标志
	static u8  start_run = 0;       //阀门交替开始标志	

	static u32 get_time = 0;        //记录循环开始时间
	static u32 get_time_value;      //获取当前时间
	
    get_time_value = timeTick;      //或者直接使用timeTick
	//get_time_value = GetTimeCnt();
	
	if(State->System_Mode == POWER_Working)
	{
			if((get_time_value < SystemPar.ValveOpenDelay) && (start == 0))  //开机前10秒，阀均打开
			{
					Switch_Init(1);
					start = 1;
			}
			else if((get_time_value >= SystemPar.ValveOpenDelay)&& (start_run == 0)) //10S后设置开始运行标志
			{
				get_time = get_time_value;	
				start_run = 1;
			}
			
			if(start_run == 1)
			{
          //第一步：0.4S内
				  if((get_time_value - get_time) < SystemPar.FrontHalfPeriodTime)
					{
						FA_SWITCH_OPEN; FB_SWITCH_CLOSE; WA_SWITCH_CLOSE; WB_SWITCH_CLOSE;
					}
					
					//第二步：0.4~2*0.4S内
					else if(((get_time_value - get_time) >= SystemPar.FrontHalfPeriodTime) && ((get_time_value - get_time) < (2 * SystemPar.FrontHalfPeriodTime)))
					{
						FA_SWITCH_CLOSE; FB_SWITCH_CLOSE; WA_SWITCH_OPEN; WB_SWITCH_CLOSE;			
					}
					
					//第三步：2*0.4S~(2*0.4S+6S)内
					else if(((get_time_value - get_time) >= (2 * SystemPar.FrontHalfPeriodTime)) && ((get_time_value - get_time) < (2 * SystemPar.FrontHalfPeriodTime + SystemPar.AC_Valve_Close_time)))
					{
						FA_SWITCH_CLOSE; FB_SWITCH_OPEN; WA_SWITCH_OPEN; WB_SWITCH_CLOSE;		
					}					

					//第四步：(2*0.4S+6S)~(3*0.4S+6S)内
					else if(((get_time_value - get_time) >= (2 * SystemPar.FrontHalfPeriodTime + SystemPar.AC_Valve_Close_time)) && ((get_time_value - get_time) < (3 * SystemPar.FrontHalfPeriodTime + SystemPar.AC_Valve_Close_time)))
					{
						FA_SWITCH_CLOSE; FB_SWITCH_CLOSE; WA_SWITCH_OPEN; WB_SWITCH_CLOSE;		
					}								
					
					//第五步：(3*0.4S+6S)~(4*0.4S+6S)内
					else if(((get_time_value - get_time) >= (3 * SystemPar.FrontHalfPeriodTime + SystemPar.AC_Valve_Close_time)) && ((get_time_value - get_time) < (4 * SystemPar.FrontHalfPeriodTime + SystemPar.AC_Valve_Close_time)))
					{
						FA_SWITCH_OPEN; FB_SWITCH_CLOSE; WA_SWITCH_CLOSE; WB_SWITCH_CLOSE;		
					}										

					//第六步：(4*0.4S+6S)~((4*0.4S+6S)+(6S-0.4S))内
					else if(((get_time_value - get_time) >= (4 * SystemPar.FrontHalfPeriodTime + SystemPar.AC_Valve_Close_time)) && ((get_time_value - get_time) < (3 * SystemPar.FrontHalfPeriodTime + 2 * SystemPar.AC_Valve_Close_time)))
					{
						FA_SWITCH_OPEN; FB_SWITCH_CLOSE; WA_SWITCH_CLOSE; WB_SWITCH_OPEN;		
					}	

					//第七步：大于等于((4*0.4S+6S)+(6S-0.4S))
					else if((get_time_value - get_time) >= (3 * SystemPar.FrontHalfPeriodTime + 2 * SystemPar.AC_Valve_Close_time))
					{
						FA_SWITCH_OPEN; FB_SWITCH_CLOSE; WA_SWITCH_CLOSE; WB_SWITCH_CLOSE;		
						start_run = 0;
					}							
			}
  }
}

#endif
/*=========================================================
函数名称：void SwitchPinInit(void)
函数功能：制氧电磁阀初始化
参数：无
返回值：无
创建时间：2019.11.2
备注：
=========================================================**/
void Switch_Init(u8 Valve)
{
	 if(Valve == 1)
	 {
			FA_SWITCH_OPEN;    //FA
			FB_SWITCH_OPEN;    //FB
			WA_SWITCH_OPEN;    //WA
			WB_SWITCH_OPEN;    //WB
      EQ_SWITCH_CLOSE;
	 }
	 else if(Valve == 0)
	 {
			FA_SWITCH_CLOSE;
			FB_SWITCH_CLOSE;
			WA_SWITCH_CLOSE;
			WB_SWITCH_CLOSE;
      EQ_SWITCH_CLOSE;
	 }
}
