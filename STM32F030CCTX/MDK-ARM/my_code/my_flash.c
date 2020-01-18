#include "my_flash.h"
#include "my_main.h"
#include <string.h>
/*========================================================
函数名称：u8 FlashSaveOnePage(SystemParameter_TypeDef SSPT)
函数功能：系统参数存储于第1页
参数：SSPT  StrSystemParameterTemp->系统参数
返回值：无
创建时间：2019.11.5
__enable_irq();    //开启总中断指令
__disable_irq();   //关闭总中断指令

FLASH编程步骤：
1.关中断
2.解锁FLASH
3.擦除    
4.擦除验证 
4.存储     
5.存储验证 
6.锁定FLASH
7.开中断  
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
	    writeAddr+=2;//地址增加2.
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
    u8 erase_times = 2;  //擦除重复次数
    u8 i = 0,j;
    uint32_t Address = 0x00;     //存储页FLASH首地址
    uint32_t *pSystemParameter;  //定义指针变量
    uint16_t  ReadValue,addr;    //读出的数据和读页的首地址
    uint32_t  SysParBuff;        //读出写入数据缓存
    HAL_StatusTypeDef  ST_Flash;         //FLASH状态-->枚举类型
    
    //擦除FLASH使用的结构体定义
    FLASH_EraseInitTypeDef f;   
    f.TypeErase = FLASH_TYPEERASE_PAGES;   //擦除页
    f.PageAddress = FLASH_ONE_START_ADDR;  //也地址
    f.NbPages = 1;                         //页数
    //设置PageError
    uint32_t PageError = 0;
    
    HAL_FLASH_Unlock();              //解锁FLASH
    
    /*擦除存储区*/
    while(erase_times)
    {
        //先判断FLASH_SR_BSY位为0后再对PGERR清零
        while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY)); //如果FLASH的FLASH->SR忙标志为1，则一直等待
        
        
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
        //	FLASH_SR_EOP----------对FLASH的擦除或写操作顺利完成时硬件置1,对其写1清零
        //  FLASH_SR_PGERR--------对FLASH的编程区域不为0XFFFF时硬件置1,对其写1清零
        //  FLASH_SR_WRPERR-------对FLASH的写保护区域编程时硬件置1,对其写1清零
        
        if (HAL_OK != HAL_FLASHEx_Erase(&f, &PageError)) //擦除存储区
            return HAL_ERROR;
        
        
        //读出擦除的存储区内容验证是否为0xffff;
        for(addr = 0;addr < 512; addr++)    //半字读
        {
            ReadValue = *(u16*)(FLASH_ONE_START_ADDR+(addr*2));    //擦除第一页
            if(ReadValue != 0xffff)
            {
                HAL_FLASH_Lock();    //锁定FLASH 
                return HAL_ERROR;    //如果擦除后还有非0xFFFF数据则返回编程错误标志
            }
        }
        erase_times = 0;
    }		
    
    /*写入存储区数据*/
    Address = FLASH_ONE_START_ADDR;                      //写入存储页首地址
    j = sizeof(SystemParameter_TypeDef) / ADDR_SIZE;     //计算数据个数
    i = 0;
    pSystemParameter = &SSPT.System_ID;   //将指针变量pSystemParameter指向系统参数结构体首地址
    Flash_XorCRC(&SSPT);
    for(i = 0;i<j;i++)
    {
        while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY));	//如果FLASH的FLASH->SR忙标志为1，则一直等待	  
        
        ST_Flash = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,Address,*pSystemParameter);    //以字的类型写入FLASH			
        if(ST_Flash == HAL_OK)
        {
            Address = Address + 4;
            pSystemParameter++;
            __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
        }				
        else
        { 
            HAL_FLASH_Lock();                     //锁定FLASH 
            return HAL_ERROR;                 //如果擦除后还有非0xFFFF数据则返回SAVE_ERR
        }
    }
    
    /*检查写入结果是否和结构体中参数一致*/
    pSystemParameter = &SSPT.System_ID;   //将指针变量pSystemParameter指向系统参数结构体首地址
    for(i = 0;i < j;i ++)
    {
        SysParBuff = *(__IO uint16_t *)(FLASH_ONE_START_ADDR + 2 + i * 4);   //先读出高16位
        SysParBuff = (SysParBuff << 16) & 0xffff0000;                                     //左移16位放入高16位
        SysParBuff = SysParBuff | ((*(__IO uint16_t *)(FLASH_ONE_START_ADDR + i * 4)) & 0x0000ffff);  //读出低16位和高16位合并
        if(SysParBuff != *pSystemParameter)
        {
            HAL_FLASH_Lock();                 //锁定FLASH 
            return HAL_ERROR;                 //如果擦除后还有非0xFFFF数据则返回SAVE_ERR				
        }
        else
        {
            pSystemParameter++;
        }
    }
    
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);  //清除FLASH状态标志
    HAL_FLASH_Lock();               //锁定FLASH
    return HAL_OK;                   //存入存储区数据正确返回SAVE_OK	
}
/*========================================================
函数名称：u8 FlashSaveTwoPage(SystemParameter_TypeDef SSPT)
函数功能：系统参数存储于第2页(备份)
参数：SSPT   StrSystemParameterTemp->系统参数
返回值：无
创建时间：2017.6.7
========================================================*/
u8 FlashSaveTwoPage(SystemParameter_TypeDef SSPT)
{
    u8 erase_times = 2;  //擦除重复次数
    u8 i = 0,j;
    uint32_t Address = 0x00;     //存储页FLASH首地址
    uint32_t *pSystemParameter;  //定义指针变量
    uint16_t  ReadValue,addr;    //读出的数据和读页的首地址
    uint32_t  SysParBuff;        //读出写入数据缓存
    HAL_StatusTypeDef  ST_Flash;         //FLASH状态-->枚举类型

    //擦除FLASH使用的结构体定义
    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.PageAddress = FLASH_TWO_START_ADDR;
    f.NbPages = 1;
    //设置PageError
    uint32_t PageError = 0;

    HAL_FLASH_Unlock();              //解锁FLASH

    /*擦除存储区*/
    while(erase_times)
    {
    //先判断FLASH_SR_BSY位为0后再对PGERR清零
        while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY)); //如果FLASH的FLASH->SR忙标志为1，则一直等待


        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
        //	FLASH_SR_EOP----------对FLASH的擦除或写操作顺利完成时硬件置1,对其写1清零
        //  FLASH_SR_PGERR--------对FLASH的编程区域不为0XFFFF时硬件置1,对其写1清零
        //  FLASH_SR_WRPERR-------对FLASH的写保护区域编程时硬件置1,对其写1清零

        HAL_FLASHEx_Erase(&f, &PageError); //擦除存储区
        //读出擦除的存储区内容验证是否为0xffff;
        for(addr = 0;addr < 512; addr++)    //半字读
        {
            ReadValue = *(u16*)(FLASH_TWO_START_ADDR+(addr*2));    //擦除第一页
            if(ReadValue != 0xffff)
            {
            HAL_FLASH_Lock();    //锁定FLASH 
            return HAL_ERROR;    //如果擦除后还有非0xFFFF数据则返回编程错误标志
            }
        }
        erase_times = 0;
    }		

    /*写入存储区数据*/
    Address = FLASH_TWO_START_ADDR;                      //写入存储页首地址
    j = sizeof(SystemParameter_TypeDef) / ADDR_SIZE;     //计算数据个数
    i = 0;
    pSystemParameter = &SSPT.System_ID;   //将指针变量pSystemParameter指向系统参数结构体首地址
    Flash_XorCRC(&SSPT);
    for(i = 0;i<j;i++)
    {
        while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY));	//如果FLASH的FLASH->SR忙标志为1，则一直等待	  

        ST_Flash = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,Address,*pSystemParameter); 

        if(ST_Flash == HAL_OK)
        {
            Address = Address + 4;
            pSystemParameter++;
            __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
        }				
        else
        { 
            HAL_FLASH_Lock();                     //锁定FLASH 
            return HAL_ERROR;                 //如果擦除后还有非0xFFFF数据则返回SAVE_ERR
        }
    }

    /*检查写入结果是否和结构体中参数一致*/
    pSystemParameter = &SSPT.System_ID;   //将指针变量pSystemParameter指向系统参数结构体首地址
    for(i = 0;i < j;i ++)
    {
        SysParBuff = *(__IO uint16_t *)(FLASH_TWO_START_ADDR + 2 + i * 4);   //先读出高16位
        SysParBuff = (SysParBuff << 16) & 0xffff0000;                                     //左移16位放入高16位
        SysParBuff = SysParBuff | ((*(__IO uint16_t *)(FLASH_TWO_START_ADDR + i * 4)) & 0x0000ffff);  //读出低16位和高16位合并
        if(SysParBuff != *pSystemParameter)
        {
            HAL_FLASH_Lock();                 //锁定FLASH 
            return HAL_ERROR;                 //如果擦除后还有非0xFFFF数据则返回SAVE_ERR				
        }
        else
        {
            pSystemParameter++;
        }
    }

    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);  //清除FLASH状态标志
    HAL_FLASH_Lock();               //锁定FLASH
    return HAL_OK;                   //存入存储区数据正确返回SAVE_OK	
}
/*========================================================
函数名称：u8 LoadOnePage(SystemParameter_TypeDef *SSPT)
函数功能：从存储区读出系统参数
参    数：SSPT是StrSystemParameterTemp的缩写
返 回 值：HAL_OK读成功    HAL_ERROR读失败
创建时间：2017.6.7
备注：
========================================================*/
u8 LoadOnePage(SystemParameter_TypeDef *SSPT)
{
    u8 i,j;
    u32 *sysbuf;
    u32 xor_crc = 0;
    u32 sysbufdata;

    sysbuf = &(SSPT ->System_ID);  //指向结构体
    j = sizeof(SystemParameter_TypeDef) / ADDR_SIZE;        //计算数据个数用来控制读书数据个数

    /*读出数据*/
    for(i = 0;i < j;i ++)
    {
        //先判断FLASH_SR_BSY位为0后再对PGERR清零
        while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY));	//如果FLASH的FLASH->SR忙标志为1，则一直等待	  

        sysbufdata = *(__IO uint16_t *)(FLASH_ONE_START_ADDR + 2 + i * 4);      //先读出高16位
        sysbufdata = (sysbufdata << 16) & 0xffff0000;                           //左移16位放入高16位
        sysbufdata = sysbufdata | ((*(__IO uint16_t *)(FLASH_ONE_START_ADDR + i * 4)) & 0x0000ffff);  //读出低16位和高16位合并
        *sysbuf = sysbufdata;  //读出的数据依次放入StrSystemParameterTemp结构体中
        sysbuf++;              //指针自加
    }

    /*检查数据正确与否*/
    sysbuf = &(SSPT ->System_ID);  //指向结构体
    for(i = 0;i < (j-1);i ++)
    {
        xor_crc = xor_crc ^ (*sysbuf);         //将刚读出的j-1个数做异或运算
        sysbuf++;
    }

    /*并和第j+1个数比较是否相等	,同时判断读出的第一个数是否为FF*/
    if((xor_crc == SSPT ->Xor_CRC) && (SSPT ->Phone_Number != 0xffffffff))
    {
        return HAL_OK;                     //返回0		
    }
    else
    {
        return HAL_ERROR;                  //返回HAL_ERROR
    }
}
/*========================================================
函数名称：u8 LoadTwoPage(SystemParameter_TypeDef *SSPT)
函数功能：从备份区读出系统参数
参    数：StrSystemParameterTemp
返 回 值：HAL_OK=0  读成功    HAL_ERROR读失败
创建时间：2017.6.7
备注：
========================================================*/
u8 LoadTwoPage(SystemParameter_TypeDef *SSPT)
{
    u8 i,j;
    u32 *sysbuf;
    u32 xor_crc = 0;
    u32 sysbufdata;

    sysbuf = &(SSPT ->System_ID);  //指向结构体
    j = sizeof(SystemParameter_TypeDef) / ADDR_SIZE;        //计算数据个数用来控制读书数据个数

    /*读出数据*/
    for(i = 0;i < j;i ++)
    {
        //先判断FLASH_SR_BSY位为0后再对PGERR清零
        while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY));	//如果FLASH的FLASH->SR忙标志为1，则一直等待	  

        sysbufdata = *(__IO uint16_t *)(FLASH_TWO_START_ADDR + 2 + i * 4);      //先读出高16位
        sysbufdata = (sysbufdata << 16) & 0xffff0000;                           //左移16位放入高16位
        sysbufdata = sysbufdata | ((*(__IO uint16_t *)(FLASH_TWO_START_ADDR + i * 4)) & 0x0000ffff);  //读出低16位和高16位合并
        *sysbuf = sysbufdata;  //读出的数据依次放入StrSystemParameterTemp结构体中
        sysbuf++;              //指针自加
    }

    /*检查数据正确与否*/
    sysbuf = &(SSPT ->System_ID);  //指向结构体
    for(i = 0;i < (j-1);i ++)
    {
        xor_crc = xor_crc ^ (*sysbuf);         //将刚读出的j-1个数做异或运算
        sysbuf++;
    }

    /*并和第j+1个数比较是否相等	,同时判断读出的第一个数是否为FF*/
    if((xor_crc == SSPT ->Xor_CRC) && (SSPT ->Phone_Number != 0xffffffff))
    {
        return HAL_OK;                     //返回0		
    }
    else
    {
        return HAL_ERROR;                  //返回HAL_ERROR
    }
}
/*========================================================
函数名称：u8 DataBackup(void)	
函数功能：上电时，对第一扇区的数据进行检查备份
          如果数据合法，则存入第二扇区，如果不合法
          则加载第二扇区的数据作为有效参数
参    数：无
返 回 值：HAL_OK      存储区数据读成功    
          HAL_BUSY    存储区错误，备份区数据正确并覆盖存储区
          HAL_ERROR   存储区 备份区数据均不正确，调入系统初始参数
          HAL_TIMEOUT 备份区数据错误
创建时间：2016.8.10
备注：
========================================================*/
HAL_StatusTypeDef DataBackup(void)		
{
    u8 i=5;
    i=LoadOnePage(&SystemPar);
    if(i == HAL_OK)				                         //如果第一扇区数据校验成功
    {
        i = FlashSaveTwoPage(SystemPar);	 //将数据从第一扇区转存到第二扇区
        if(i == HAL_OK)
        {
            return HAL_OK;                          //如果第二扇区存储正确返回HAL_OK
        }
        else
        {
            return HAL_TIMEOUT;                     //如果第二扇区存储不正确返回HAL_TIMEOUT
        }
    }
    else								                      //第一扇区数据错误，加载第二扇区数据
    {
        i = LoadTwoPage(&SystemPar);
        if(i == HAL_OK)			                          //如果第二扇区数据校验成功
        {
            i = FlashSaveOnePage(SystemPar); //将第二扇区数据转存到第一扇区，以便作为工作参数进行处理
            if(i == HAL_OK)
            {
                return HAL_OK;
            }
            else
            {
                return HAL_TIMEOUT;
            }
        }
        else							                      //如果第二扇区数据也错误，则有可能是系统第一次上电，需要进行默认参数的写入
        {
            return HAL_ERROR;					            //表明需要对参数进行初始化
        }
    }
}

#endif

void Sys_Par_Init(void)
{
    u8 k = 5;   //0之外的任意8位数
    u8 i,j;
    u32 crcvalue = 0;   //异或数据缓存
    u32 *sysbuf;          

    sysbuf =(u32*) &SystemPar;  //指向结构体

    j = sizeof(SystemParameter_TypeDef) / ADDR_SIZE;        //计算数据个数用来控制读书数据个数

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

    SystemPar.Xor_CRC = crcvalue;    //异或校验和	

//    k = FlashSaveOnePage(SystemPar);  //如果写失败，再写1次
//    if(k != HAL_OK)
//    {
//        FlashSaveOnePage(SystemPar);
//    }			
}
/*========================================================
函数名称：void Flash_Check(void)
函数功能：FLASH检查，如果第一次开机则写入初始参数
参    数：无
返 回 值：
创建时间：2019.12.24
备    注：
========================================================*/
void Flash_Check(void)
{
    main_readSysPar(0x01);
    if(SystemPar.System_ID != 0xffffffff)
    {
        return ;
    }
    else       //如果存储区和备份区数据都错误，重新写入初始参数并复位系统
    {
        Sys_Par_Init();
        main_saveSysPar();
        HAL_Delay(50);
        SW_RESET();                      //系统复位  
    }    
}
/*========================================================
函数名称：void Flash_XorCRC(SystemParameter_TypeDef *SSPT)
函数功能：计算CRC值
参    数：无
返 回 值：*SSPT  系统参数
创建时间：2017.6.9
备    注：
========================================================*/
void Flash_XorCRC(SystemParameter_TypeDef *SSPT)
{
    u8 i,j;
    u32 *sysSSPT;
    u32 xorcrcdata = 0;
    sysSSPT = &(SSPT ->System_ID);
    j = sizeof(SystemParameter_TypeDef) / ADDR_SIZE;        //计算数据个数用来控制读取数据个数
    for(i = 0; i < (j-1);i ++)
    {
        xorcrcdata = xorcrcdata ^(*sysSSPT);
        sysSSPT++;
    }
    SSPT->Xor_CRC = xorcrcdata;
}

