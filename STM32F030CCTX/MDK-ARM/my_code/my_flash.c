#include "my_flash.h"
#include "my_main.h"
#include <string.h>
/*========================================================
�������ƣ�u8 FlashSaveOnePage(SystemParameter_TypeDef SSPT)
�������ܣ�ϵͳ�����洢�ڵ�1ҳ
������SSPT  StrSystemParameterTemp->ϵͳ����
����ֵ����
����ʱ�䣺2019.11.5
__enable_irq();    //�������ж�ָ��
__disable_irq();   //�ر����ж�ָ��

FLASH��̲��裺
1.���ж�
2.����FLASH
3.����    
4.������֤ 
4.�洢     
5.�洢��֤ 
6.����FLASH
7.���ж�  
========================================================*/

static uint16_t flashBuf[STM32_FLASH_SECTOR_SIZE / 2] = {0};

uint16_t flash_readData(uint32_t addr)
{
    return *(__IO uint16_t*)addr;
}

void stmFlash_read(u32 readAddr, u16 *pbuf, u16 num) 
{
    u16 i = 0;
    for (i = 0; i < num; i++) {
        pbuf[i] = flash_readData(readAddr);
        readAddr += 2;
    }
}

void stmFlash_writeNoCheck(u32 writeAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, writeAddr, pBuffer[i]);
	    writeAddr+=2;//��ַ����2.
	}  
} 

u8 stmFlash_write(uint32_t writeAddr, uint16_t *pBuf, uint16_t num)
{
    uint32_t offsetAddr;
    uint32_t secAddr;
    uint16_t secOffAddr;
    uint16_t secRemain;
    uint16_t i = 0;
    
    uint32_t sectorError = 0;
    FLASH_EraseInitTypeDef FlashEraseInit;
    
    if ((writeAddr < STM32_FLASH_BASE) || (writeAddr > (STM32_FLASH_BASE + STM32_FLASH_SIZE * 1024)))
        return 0;
    
    HAL_FLASH_Unlock();
    offsetAddr = writeAddr - STM32_FLASH_BASE;
    secAddr = offsetAddr / STM32_FLASH_SECTOR_SIZE;
    secOffAddr = (offsetAddr % STM32_FLASH_SECTOR_SIZE) / 2;
    secRemain = STM32_FLASH_SECTOR_SIZE / 2 - secOffAddr;
    if (num < secRemain) secRemain = num;
    
    while(1) {
        stmFlash_read(secAddr * STM32_FLASH_SECTOR_SIZE + STM32_FLASH_BASE, flashBuf, STM32_FLASH_SECTOR_SIZE / 2);
        for (i = 0; i < secRemain; i++) 
            if (flashBuf[secOffAddr + i] != 0xffff)
                break;
            
        if (i < secRemain) {
            FlashEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
            FlashEraseInit.PageAddress = secAddr * STM32_FLASH_SECTOR_SIZE + STM32_FLASH_BASE;
            FlashEraseInit.NbPages = 1;                             
            if(HAL_FLASHEx_Erase(&FlashEraseInit, &sectorError)!=HAL_OK) 
                break;
            for (i = 0; i < secRemain; i++)
                flashBuf[i + secOffAddr] = pBuf[i];
            stmFlash_writeNoCheck(secAddr * STM32_FLASH_SECTOR_SIZE + STM32_FLASH_BASE, flashBuf, STM32_FLASH_SECTOR_SIZE / 2);
        } else 
            stmFlash_writeNoCheck(writeAddr, pBuf, secRemain);
        
        if (num == secRemain) break;
        else {
            secAddr++;
            secOffAddr = 0;
            pBuf += secRemain;
            writeAddr += secRemain;
            num -= secRemain;
            if(num > (STM32_FLASH_SECTOR_SIZE / 2)) secRemain = STM32_FLASH_SECTOR_SIZE / 2;
            else secRemain = num;
        }
    }
    HAL_FLASH_Lock();
    return 0;
}



#if 0
u8 FlashSaveOnePage(SystemParameter_TypeDef SSPT)
{
    u8 erase_times = 2;  //�����ظ�����
    u8 i = 0,j;
    uint32_t Address = 0x00;     //�洢ҳFLASH�׵�ַ
    uint32_t *pSystemParameter;  //����ָ�����
    uint16_t  ReadValue,addr;    //���������ݺͶ�ҳ���׵�ַ
    uint32_t  SysParBuff;        //����д�����ݻ���
    HAL_StatusTypeDef  ST_Flash;         //FLASH״̬-->ö������
    
    //����FLASHʹ�õĽṹ�嶨��
    FLASH_EraseInitTypeDef f;   
    f.TypeErase = FLASH_TYPEERASE_PAGES;   //����ҳ
    f.PageAddress = FLASH_ONE_START_ADDR;  //Ҳ��ַ
    f.NbPages = 1;                         //ҳ��
    //����PageError
    uint32_t PageError = 0;
    
    HAL_FLASH_Unlock();              //����FLASH
    
    /*�����洢��*/
    while(erase_times)
    {
        //���ж�FLASH_SR_BSYλΪ0���ٶ�PGERR����
        while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY)); //���FLASH��FLASH->SRæ��־Ϊ1����һֱ�ȴ�
        
        
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
        //	FLASH_SR_EOP----------��FLASH�Ĳ�����д����˳�����ʱӲ����1,����д1����
        //  FLASH_SR_PGERR--------��FLASH�ı������Ϊ0XFFFFʱӲ����1,����д1����
        //  FLASH_SR_WRPERR-------��FLASH��д����������ʱӲ����1,����д1����
        
        if (HAL_OK != HAL_FLASHEx_Erase(&f, &PageError)) //�����洢��
            return HAL_ERROR;
        
        
        //���������Ĵ洢��������֤�Ƿ�Ϊ0xffff;
        for(addr = 0;addr < 512; addr++)    //���ֶ�
        {
            ReadValue = *(u16*)(FLASH_ONE_START_ADDR+(addr*2));    //������һҳ
            if(ReadValue != 0xffff)
            {
                HAL_FLASH_Lock();    //����FLASH 
                return HAL_ERROR;    //����������з�0xFFFF�����򷵻ر�̴����־
            }
        }
        erase_times = 0;
    }		
    
    /*д��洢������*/
    Address = FLASH_ONE_START_ADDR;                      //д��洢ҳ�׵�ַ
    j = sizeof(SystemParameter_TypeDef) / ADDR_SIZE;     //�������ݸ���
    i = 0;
    pSystemParameter = &SSPT.System_ID;   //��ָ�����pSystemParameterָ��ϵͳ�����ṹ���׵�ַ
    Flash_XorCRC(&SSPT);
    for(i = 0;i<j;i++)
    {
        while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY));	//���FLASH��FLASH->SRæ��־Ϊ1����һֱ�ȴ�	  
        
        ST_Flash = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,Address,*pSystemParameter);    //���ֵ�����д��FLASH			
        if(ST_Flash == HAL_OK)
        {
            Address = Address + 4;
            pSystemParameter++;
            __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
        }				
        else
        { 
            HAL_FLASH_Lock();                     //����FLASH 
            return HAL_ERROR;                 //����������з�0xFFFF�����򷵻�SAVE_ERR
        }
    }
    
    /*���д�����Ƿ�ͽṹ���в���һ��*/
    pSystemParameter = &SSPT.System_ID;   //��ָ�����pSystemParameterָ��ϵͳ�����ṹ���׵�ַ
    for(i = 0;i < j;i ++)
    {
        SysParBuff = *(__IO uint16_t *)(FLASH_ONE_START_ADDR + 2 + i * 4);   //�ȶ�����16λ
        SysParBuff = (SysParBuff << 16) & 0xffff0000;                                     //����16λ�����16λ
        SysParBuff = SysParBuff | ((*(__IO uint16_t *)(FLASH_ONE_START_ADDR + i * 4)) & 0x0000ffff);  //������16λ�͸�16λ�ϲ�
        if(SysParBuff != *pSystemParameter)
        {
            HAL_FLASH_Lock();                 //����FLASH 
            return HAL_ERROR;                 //����������з�0xFFFF�����򷵻�SAVE_ERR				
        }
        else
        {
            pSystemParameter++;
        }
    }
    
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);  //���FLASH״̬��־
    HAL_FLASH_Lock();               //����FLASH
    return HAL_OK;                   //����洢��������ȷ����SAVE_OK	
}
/*========================================================
�������ƣ�u8 FlashSaveTwoPage(SystemParameter_TypeDef SSPT)
�������ܣ�ϵͳ�����洢�ڵ�2ҳ(����)
������SSPT   StrSystemParameterTemp->ϵͳ����
����ֵ����
����ʱ�䣺2017.6.7
========================================================*/
u8 FlashSaveTwoPage(SystemParameter_TypeDef SSPT)
{
    u8 erase_times = 2;  //�����ظ�����
    u8 i = 0,j;
    uint32_t Address = 0x00;     //�洢ҳFLASH�׵�ַ
    uint32_t *pSystemParameter;  //����ָ�����
    uint16_t  ReadValue,addr;    //���������ݺͶ�ҳ���׵�ַ
    uint32_t  SysParBuff;        //����д�����ݻ���
    HAL_StatusTypeDef  ST_Flash;         //FLASH״̬-->ö������

    //����FLASHʹ�õĽṹ�嶨��
    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.PageAddress = FLASH_TWO_START_ADDR;
    f.NbPages = 1;
    //����PageError
    uint32_t PageError = 0;

    HAL_FLASH_Unlock();              //����FLASH

    /*�����洢��*/
    while(erase_times)
    {
    //���ж�FLASH_SR_BSYλΪ0���ٶ�PGERR����
        while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY)); //���FLASH��FLASH->SRæ��־Ϊ1����һֱ�ȴ�


        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
        //	FLASH_SR_EOP----------��FLASH�Ĳ�����д����˳�����ʱӲ����1,����д1����
        //  FLASH_SR_PGERR--------��FLASH�ı������Ϊ0XFFFFʱӲ����1,����д1����
        //  FLASH_SR_WRPERR-------��FLASH��д����������ʱӲ����1,����д1����

        HAL_FLASHEx_Erase(&f, &PageError); //�����洢��
        //���������Ĵ洢��������֤�Ƿ�Ϊ0xffff;
        for(addr = 0;addr < 512; addr++)    //���ֶ�
        {
            ReadValue = *(u16*)(FLASH_TWO_START_ADDR+(addr*2));    //������һҳ
            if(ReadValue != 0xffff)
            {
            HAL_FLASH_Lock();    //����FLASH 
            return HAL_ERROR;    //����������з�0xFFFF�����򷵻ر�̴����־
            }
        }
        erase_times = 0;
    }		

    /*д��洢������*/
    Address = FLASH_TWO_START_ADDR;                      //д��洢ҳ�׵�ַ
    j = sizeof(SystemParameter_TypeDef) / ADDR_SIZE;     //�������ݸ���
    i = 0;
    pSystemParameter = &SSPT.System_ID;   //��ָ�����pSystemParameterָ��ϵͳ�����ṹ���׵�ַ
    Flash_XorCRC(&SSPT);
    for(i = 0;i<j;i++)
    {
        while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY));	//���FLASH��FLASH->SRæ��־Ϊ1����һֱ�ȴ�	  

        ST_Flash = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,Address,*pSystemParameter); 

        if(ST_Flash == HAL_OK)
        {
            Address = Address + 4;
            pSystemParameter++;
            __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
        }				
        else
        { 
            HAL_FLASH_Lock();                     //����FLASH 
            return HAL_ERROR;                 //����������з�0xFFFF�����򷵻�SAVE_ERR
        }
    }

    /*���д�����Ƿ�ͽṹ���в���һ��*/
    pSystemParameter = &SSPT.System_ID;   //��ָ�����pSystemParameterָ��ϵͳ�����ṹ���׵�ַ
    for(i = 0;i < j;i ++)
    {
        SysParBuff = *(__IO uint16_t *)(FLASH_TWO_START_ADDR + 2 + i * 4);   //�ȶ�����16λ
        SysParBuff = (SysParBuff << 16) & 0xffff0000;                                     //����16λ�����16λ
        SysParBuff = SysParBuff | ((*(__IO uint16_t *)(FLASH_TWO_START_ADDR + i * 4)) & 0x0000ffff);  //������16λ�͸�16λ�ϲ�
        if(SysParBuff != *pSystemParameter)
        {
            HAL_FLASH_Lock();                 //����FLASH 
            return HAL_ERROR;                 //����������з�0xFFFF�����򷵻�SAVE_ERR				
        }
        else
        {
            pSystemParameter++;
        }
    }

    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);  //���FLASH״̬��־
    HAL_FLASH_Lock();               //����FLASH
    return HAL_OK;                   //����洢��������ȷ����SAVE_OK	
}
/*========================================================
�������ƣ�u8 LoadOnePage(SystemParameter_TypeDef *SSPT)
�������ܣ��Ӵ洢������ϵͳ����
��    ����SSPT��StrSystemParameterTemp����д
�� �� ֵ��HAL_OK���ɹ�    HAL_ERROR��ʧ��
����ʱ�䣺2017.6.7
��ע��
========================================================*/
u8 LoadOnePage(SystemParameter_TypeDef *SSPT)
{
    u8 i,j;
    u32 *sysbuf;
    u32 xor_crc = 0;
    u32 sysbufdata;

    sysbuf = &(SSPT ->System_ID);  //ָ��ṹ��
    j = sizeof(SystemParameter_TypeDef) / ADDR_SIZE;        //�������ݸ����������ƶ������ݸ���

    /*��������*/
    for(i = 0;i < j;i ++)
    {
        //���ж�FLASH_SR_BSYλΪ0���ٶ�PGERR����
        while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY));	//���FLASH��FLASH->SRæ��־Ϊ1����һֱ�ȴ�	  

        sysbufdata = *(__IO uint16_t *)(FLASH_ONE_START_ADDR + 2 + i * 4);      //�ȶ�����16λ
        sysbufdata = (sysbufdata << 16) & 0xffff0000;                           //����16λ�����16λ
        sysbufdata = sysbufdata | ((*(__IO uint16_t *)(FLASH_ONE_START_ADDR + i * 4)) & 0x0000ffff);  //������16λ�͸�16λ�ϲ�
        *sysbuf = sysbufdata;  //�������������η���StrSystemParameterTemp�ṹ����
        sysbuf++;              //ָ���Լ�
    }

    /*���������ȷ���*/
    sysbuf = &(SSPT ->System_ID);  //ָ��ṹ��
    for(i = 0;i < (j-1);i ++)
    {
        xor_crc = xor_crc ^ (*sysbuf);         //���ն�����j-1�������������
        sysbuf++;
    }

    /*���͵�j+1�����Ƚ��Ƿ����	,ͬʱ�ж϶����ĵ�һ�����Ƿ�ΪFF*/
    if((xor_crc == SSPT ->Xor_CRC) && (SSPT ->Phone_Number != 0xffffffff))
    {
        return HAL_OK;                     //����0		
    }
    else
    {
        return HAL_ERROR;                  //����HAL_ERROR
    }
}
/*========================================================
�������ƣ�u8 LoadTwoPage(SystemParameter_TypeDef *SSPT)
�������ܣ��ӱ���������ϵͳ����
��    ����StrSystemParameterTemp
�� �� ֵ��HAL_OK=0  ���ɹ�    HAL_ERROR��ʧ��
����ʱ�䣺2017.6.7
��ע��
========================================================*/
u8 LoadTwoPage(SystemParameter_TypeDef *SSPT)
{
    u8 i,j;
    u32 *sysbuf;
    u32 xor_crc = 0;
    u32 sysbufdata;

    sysbuf = &(SSPT ->System_ID);  //ָ��ṹ��
    j = sizeof(SystemParameter_TypeDef) / ADDR_SIZE;        //�������ݸ����������ƶ������ݸ���

    /*��������*/
    for(i = 0;i < j;i ++)
    {
        //���ж�FLASH_SR_BSYλΪ0���ٶ�PGERR����
        while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY));	//���FLASH��FLASH->SRæ��־Ϊ1����һֱ�ȴ�	  

        sysbufdata = *(__IO uint16_t *)(FLASH_TWO_START_ADDR + 2 + i * 4);      //�ȶ�����16λ
        sysbufdata = (sysbufdata << 16) & 0xffff0000;                           //����16λ�����16λ
        sysbufdata = sysbufdata | ((*(__IO uint16_t *)(FLASH_TWO_START_ADDR + i * 4)) & 0x0000ffff);  //������16λ�͸�16λ�ϲ�
        *sysbuf = sysbufdata;  //�������������η���StrSystemParameterTemp�ṹ����
        sysbuf++;              //ָ���Լ�
    }

    /*���������ȷ���*/
    sysbuf = &(SSPT ->System_ID);  //ָ��ṹ��
    for(i = 0;i < (j-1);i ++)
    {
        xor_crc = xor_crc ^ (*sysbuf);         //���ն�����j-1�������������
        sysbuf++;
    }

    /*���͵�j+1�����Ƚ��Ƿ����	,ͬʱ�ж϶����ĵ�һ�����Ƿ�ΪFF*/
    if((xor_crc == SSPT ->Xor_CRC) && (SSPT ->Phone_Number != 0xffffffff))
    {
        return HAL_OK;                     //����0		
    }
    else
    {
        return HAL_ERROR;                  //����HAL_ERROR
    }
}
/*========================================================
�������ƣ�u8 DataBackup(void)	
�������ܣ��ϵ�ʱ���Ե�һ���������ݽ��м�鱸��
          ������ݺϷ��������ڶ�������������Ϸ�
          ����صڶ�������������Ϊ��Ч����
��    ������
�� �� ֵ��HAL_OK      �洢�����ݶ��ɹ�    
          HAL_BUSY    �洢�����󣬱�����������ȷ�����Ǵ洢��
          HAL_ERROR   �洢�� ���������ݾ�����ȷ������ϵͳ��ʼ����
          HAL_TIMEOUT ���������ݴ���
����ʱ�䣺2016.8.10
��ע��
========================================================*/
HAL_StatusTypeDef DataBackup(void)		
{
    u8 i=5;
    i=LoadOnePage(&SystemPar);
    if(i == HAL_OK)				                         //�����һ��������У��ɹ�
    {
        i = FlashSaveTwoPage(SystemPar);	 //�����ݴӵ�һ����ת�浽�ڶ�����
        if(i == HAL_OK)
        {
            return HAL_OK;                          //����ڶ������洢��ȷ����HAL_OK
        }
        else
        {
            return HAL_TIMEOUT;                     //����ڶ������洢����ȷ����HAL_TIMEOUT
        }
    }
    else								                      //��һ�������ݴ��󣬼��صڶ���������
    {
        i = LoadTwoPage(&SystemPar);
        if(i == HAL_OK)			                          //����ڶ���������У��ɹ�
        {
            i = FlashSaveOnePage(SystemPar); //���ڶ���������ת�浽��һ�������Ա���Ϊ�����������д���
            if(i == HAL_OK)
            {
                return HAL_OK;
            }
            else
            {
                return HAL_TIMEOUT;
            }
        }
        else							                      //����ڶ���������Ҳ�������п�����ϵͳ��һ���ϵ磬��Ҫ����Ĭ�ϲ�����д��
        {
            return HAL_ERROR;					            //������Ҫ�Բ������г�ʼ��
        }
    }
}

#endif

void Sys_Par_Init(void)
{
    u8 k = 5;   //0֮�������8λ��
    u8 i,j;
    u32 crcvalue = 0;   //������ݻ���
    u32 *sysbuf;          

    sysbuf =(u32*) &SystemPar;  //ָ��ṹ��

    j = sizeof(SystemParameter_TypeDef) / ADDR_SIZE;        //�������ݸ����������ƶ������ݸ���

    memset((void *)&SystemPar, 0, sizeof(SystemPar));
    SystemPar.System_ID = 0;          
    SystemPar.System_Mode = 0;          
    SystemPar.Alarm_Type = 0;         	
    SystemPar.Cumulative_Time = 0;    	
    SystemPar.Available_time = 3000;     	
    SystemPar.Uncap_Times = 0;        	
    memset(SystemPar.Phone_Number, 0, sizeof(SystemPar.Phone_Number));       	
    SystemPar.ThresholdTemp = 70;        	
    SystemPar.ThresholdLowPress = 0;         	
    SystemPar.ValveOpenDelay = 880;      	
    SystemPar.Breath_Open_Press = 0;  	
    SystemPar.Breath_Close_Time = 80;  	
    SystemPar.Breath_Close_Press = 0; 	
    SystemPar.EnerjySave_Delay = 180;   	
    SystemPar.Alarm_Detec_Time = 180;   	
    SystemPar.FrontHalfPeriodTime = 660;	
    SystemPar.AfterHalfPeriodTime = 680;
	SystemPar.OffsetTime = 80;
    SystemPar.Longitude = 0;          	
    SystemPar.Latitude = 0;           
    SystemPar.Altitude = 0;           
    memset(SystemPar.Sos_Password, 0, sizeof(SystemPar.Sos_Password));    
    SystemPar.Xor_CRC = 0;            

    for(i = 0; i < (j - 1); i ++)
    {
        crcvalue = crcvalue ^ (*sysbuf);
        sysbuf++;
    }

    SystemPar.Xor_CRC = crcvalue;    //���У���	

//    k = FlashSaveOnePage(SystemPar);  //���дʧ�ܣ���д1��
//    if(k != HAL_OK)
//    {
//        FlashSaveOnePage(SystemPar);
//    }			
}
/*========================================================
�������ƣ�void Flash_Check(void)
�������ܣ�FLASH��飬�����һ�ο�����д���ʼ����
��    ������
�� �� ֵ��
����ʱ�䣺2019.12.24
��    ע��
========================================================*/
void Flash_Check(void)
{
    main_readSysPar(0x01);
    if(SystemPar.System_ID != 0xffffffff)
    {
        return ;
    }
    else       //����洢���ͱ��������ݶ���������д���ʼ��������λϵͳ
    {
        Sys_Par_Init();
        main_saveSysPar();
        HAL_Delay(50);
        SW_RESET();                      //ϵͳ��λ  
    }    
}
/*========================================================
�������ƣ�void Flash_XorCRC(SystemParameter_TypeDef *SSPT)
�������ܣ�����CRCֵ
��    ������
�� �� ֵ��*SSPT  ϵͳ����
����ʱ�䣺2017.6.9
��    ע��
========================================================*/
void Flash_XorCRC(SystemParameter_TypeDef *SSPT)
{
    u8 i,j;
    u32 *sysSSPT;
    u32 xorcrcdata = 0;
    sysSSPT = &(SSPT ->System_ID);
    j = sizeof(SystemParameter_TypeDef) / ADDR_SIZE;        //�������ݸ����������ƶ�ȡ���ݸ���
    for(i = 0; i < (j-1);i ++)
    {
        xorcrcdata = xorcrcdata ^(*sysSSPT);
        sysSSPT++;
    }
    SSPT->Xor_CRC = xorcrcdata;
}

