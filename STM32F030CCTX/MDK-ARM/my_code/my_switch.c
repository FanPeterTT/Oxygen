#include "my_switch.h"

/*======================================
��������:void SwitchABWork(void)
��������:
         �˲�����SystemPar�ж�ȡ������SystemPar���޸�
         AC:Aѹ���� ������10.2S��ʼѭ��,����:12.8S  6.0S��  6.8S��
         BC:Bѹ���� ������10.2S��ʼѭ��,����:12.8S  6.0S��  6.8S��
         AD:A�ŵ��� ������10.2S��ʼѭ��,����:13.6S  5.2S��  8.4S��
         BD:B�ŵ��� ������10.2S��ʼѭ��,����:13.6S  8.4S��  5.2S��
��    ����State  ������״̬
�� �� ֵ����
����ʱ�䣺2019.11.2
======================================*/
void W_SwitchWork(SwitchParameter_TypeDef *SwitchParameter)
{
      
      if(Timer6_Cnt >= (SystemPar.ValveOpenDelay))  //������ʱ��������
      {
          //=========================================================
          if(SwitchParameter->SwitchState_WA == 0)  //������δ��ʼ
          {
              SwitchParameter->SwitchState_WA = 1;  
              WA_SWITCH_CLOSE;     //WA������
              SwitchParameter ->StartTimeScale_WA = Timer6_Cnt;  
          }
          
          //�����ǰʱ��-ʱ��̶ȴ���2����ƫ������   �� С��  ��ǰʱ��-ʱ��̶ȴ��ڣ��������6.8S-2��ƫ�ƣ�
          else if(((Timer6_Cnt - SwitchParameter->StartTimeScale_WA) >= (SystemPar.FrontHalfPeriodTime + SystemPar.OffsetTime * 2))
                  && (Timer6_Cnt - SwitchParameter ->StartTimeScale_WA < ((SystemPar.FrontHalfPeriodTime + SystemPar.OffsetTime * 2) + (SystemPar.AfterHalfPeriodTime - (SystemPar.OffsetTime * 2))))
                  && (SwitchParameter->SwitchState_WA == 1)
                  )
          {
              WA_SWITCH_OPEN;      //WA�½���
              SwitchParameter->SwitchState_WA = 2; 
          }
          //�����ǰʱ��-ʱ��̶ȴ��ڣ��������6.8S-2��ƫ�ƣ�
          else if((Timer6_Cnt - SwitchParameter ->StartTimeScale_WA >= ((SystemPar.FrontHalfPeriodTime + SystemPar.OffsetTime * 2) + (SystemPar.AfterHalfPeriodTime - (SystemPar.OffsetTime * 2))))
                    && (SwitchParameter->SwitchState_WA == 2)
                   )
          {
              SwitchParameter->SwitchState_WA = 0;

          }
      }
      
      
      if(Timer6_Cnt >= (SystemPar.ValveOpenDelay + SystemPar.OffsetTime * 2 -10))  //������ʱ��������
      {
          //=========================================================
          if(SwitchParameter->SwitchState_WB == 0)  //������δ��ʼ
          {
              SwitchParameter->SwitchState_WB = 1;  
              WB_SWITCH_OPEN;     //WB�½���
              SwitchParameter ->StartTimeScale_WB = Timer6_Cnt;  
          }
          
          //�����ǰʱ��-ʱ��̶ȴ���2����ƫ������   �� С��  ��ǰʱ��-ʱ��̶ȴ��ڣ��������6.8S-2��ƫ�ƣ�
          else if(((Timer6_Cnt - SwitchParameter->StartTimeScale_WB) >= (SystemPar.AfterHalfPeriodTime - (SystemPar.OffsetTime * 2)))
                  && (Timer6_Cnt - SwitchParameter ->StartTimeScale_WB < ((SystemPar.FrontHalfPeriodTime + SystemPar.OffsetTime * 2) + (SystemPar.AfterHalfPeriodTime - (SystemPar.OffsetTime * 2))))
                  && (SwitchParameter->SwitchState_WB == 1)
                  )
          {
              WB_SWITCH_CLOSE;      //WB������
              SwitchParameter->SwitchState_WB = 2; 
          }
          //�����ǰʱ��-ʱ��̶ȴ��ڣ��������6.8S-2��ƫ�ƣ�
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
     if(Timer6_Cnt >= (SystemPar.ValveOpenDelay + SystemPar.OffsetTime))  //������ʱ��������
     {
        if(SwitchParameter->SwitchState_F == END)  //���Ƿ���������
        {
            SwitchParameter->SwitchState_F = FrontHalfPeriod;  
            FA_SWITCH_OPEN;  FB_SWITCH_CLOSE;
            SwitchParameter ->StartTimeScale_F = Timer6_Cnt;  
        }
        //�����ǰʱ��-ʱ��̶ȴ���ǰ�����ڣ����ұ�־Ϊǰ������ʱ
        else if(((Timer6_Cnt - SwitchParameter->StartTimeScale_F) >= SystemPar.FrontHalfPeriodTime) && (SwitchParameter->SwitchState_F == FrontHalfPeriod))
        {
            FA_SWITCH_CLOSE;  FB_SWITCH_OPEN;
            SwitchParameter->SwitchState_F = AfterHalfPeriod; 
        }
        //�����ǰʱ��-ʱ��̶ȴ���ǰ�������֮�ͣ����ұ�־Ϊ�������ʱ
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
     if(Timer6_Cnt >= (SystemPar.ValveOpenDelay + SystemPar.OffsetTime*2 - 10))  //������ʱ��������
     {
        if(SwitchParameter->SwitchState_EQ == 0)  //���Ƿ���������
        {
            SwitchParameter->SwitchState_EQ = 1;  
            EQ_SWITCH_CLOSE; 
            SwitchParameter ->StartTimeScale_EQ = Timer6_Cnt;  
        }
        //�����ǰʱ��-ʱ��̶ȴ���ǰ�����ڣ����ұ�־Ϊǰ������ʱ
        else if(((Timer6_Cnt - SwitchParameter->StartTimeScale_EQ) >= (SystemPar.AfterHalfPeriodTime - (SystemPar.OffsetTime * 2)))
                &&(SystemPar.AfterHalfPeriodTime - (SystemPar.OffsetTime * 2) + (SystemPar.OffsetTime * 2 - 10))
                && (SwitchParameter->SwitchState_EQ == 1)
               )
        {
            EQ_SWITCH_OPEN;
            SwitchParameter->SwitchState_EQ = 2; 
        }
        //�����ǰʱ��-ʱ��̶ȴ���ǰ�������֮�ͣ����ұ�־Ϊ�������ʱ
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


	static u8  start = 0;           //����10���־
	static u8  start_run = 0;       //���Ž��濪ʼ��־	

	static u32 get_time = 0;        //��¼ѭ����ʼʱ��
	static u32 get_time_value;      //��ȡ��ǰʱ��
	
    get_time_value = timeTick;      //����ֱ��ʹ��timeTick
	//get_time_value = GetTimeCnt();
	
	if(State->System_Mode == POWER_Working)
	{
			if((get_time_value < SystemPar.ValveOpenDelay) && (start == 0))  //����ǰ10�룬������
			{
					Switch_Init(1);
					start = 1;
			}
			else if((get_time_value >= SystemPar.ValveOpenDelay)&& (start_run == 0)) //10S�����ÿ�ʼ���б�־
			{
				get_time = get_time_value;	
				start_run = 1;
			}
			
			if(start_run == 1)
			{
          //��һ����0.4S��
				  if((get_time_value - get_time) < SystemPar.FrontHalfPeriodTime)
					{
						FA_SWITCH_OPEN; FB_SWITCH_CLOSE; WA_SWITCH_CLOSE; WB_SWITCH_CLOSE;
					}
					
					//�ڶ�����0.4~2*0.4S��
					else if(((get_time_value - get_time) >= SystemPar.FrontHalfPeriodTime) && ((get_time_value - get_time) < (2 * SystemPar.FrontHalfPeriodTime)))
					{
						FA_SWITCH_CLOSE; FB_SWITCH_CLOSE; WA_SWITCH_OPEN; WB_SWITCH_CLOSE;			
					}
					
					//��������2*0.4S~(2*0.4S+6S)��
					else if(((get_time_value - get_time) >= (2 * SystemPar.FrontHalfPeriodTime)) && ((get_time_value - get_time) < (2 * SystemPar.FrontHalfPeriodTime + SystemPar.AC_Valve_Close_time)))
					{
						FA_SWITCH_CLOSE; FB_SWITCH_OPEN; WA_SWITCH_OPEN; WB_SWITCH_CLOSE;		
					}					

					//���Ĳ���(2*0.4S+6S)~(3*0.4S+6S)��
					else if(((get_time_value - get_time) >= (2 * SystemPar.FrontHalfPeriodTime + SystemPar.AC_Valve_Close_time)) && ((get_time_value - get_time) < (3 * SystemPar.FrontHalfPeriodTime + SystemPar.AC_Valve_Close_time)))
					{
						FA_SWITCH_CLOSE; FB_SWITCH_CLOSE; WA_SWITCH_OPEN; WB_SWITCH_CLOSE;		
					}								
					
					//���岽��(3*0.4S+6S)~(4*0.4S+6S)��
					else if(((get_time_value - get_time) >= (3 * SystemPar.FrontHalfPeriodTime + SystemPar.AC_Valve_Close_time)) && ((get_time_value - get_time) < (4 * SystemPar.FrontHalfPeriodTime + SystemPar.AC_Valve_Close_time)))
					{
						FA_SWITCH_OPEN; FB_SWITCH_CLOSE; WA_SWITCH_CLOSE; WB_SWITCH_CLOSE;		
					}										

					//��������(4*0.4S+6S)~((4*0.4S+6S)+(6S-0.4S))��
					else if(((get_time_value - get_time) >= (4 * SystemPar.FrontHalfPeriodTime + SystemPar.AC_Valve_Close_time)) && ((get_time_value - get_time) < (3 * SystemPar.FrontHalfPeriodTime + 2 * SystemPar.AC_Valve_Close_time)))
					{
						FA_SWITCH_OPEN; FB_SWITCH_CLOSE; WA_SWITCH_CLOSE; WB_SWITCH_OPEN;		
					}	

					//���߲������ڵ���((4*0.4S+6S)+(6S-0.4S))
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
�������ƣ�void SwitchPinInit(void)
�������ܣ�������ŷ���ʼ��
��������
����ֵ����
����ʱ�䣺2019.11.2
��ע��
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
