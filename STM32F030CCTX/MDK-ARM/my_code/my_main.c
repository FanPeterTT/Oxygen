#include <stdio.h>
#include <string.h>

#include "adc.h"
#include "my_main.h"
#include "fonts.h"
#include "my_bmp.h"
#include "my_key.h"
#include "my_motor.h"

#include "my_global.h"
#include "my_switch.h"
#include "my_uart.h"
#include "my_key.h"
#include "my_flash.h"
#include "my_standby.h"
#include "my_sensor.h"
#include "my_tim.h"
#include "my_fifo.h"
#include "bc20.h"
#include "my_oled.h"
#include "my_rtc.h"
#include "my_motor.h"

static unsigned char nowPicTick = PIC_MODE_MAIN;        //主界面

void main_init(void)
{
    rtc_init();             //RTC初始化
    tim_enableIt();         //初始话定时器中断
  
    oled_init();
    uart_enableIt();        //串口中断使能
    //bc20_init();               
    //mqtt_init();
    //oled_test();
    rtc_printfDataTime();

    main_picture();
    motor_enable(0x01);     //使能电机，并设置为高速模式
    mian_picMotorSpeed();
    
    Flash_Check();
    adcDmaStart();
}

/* 
 * 显示adc值
 * chn： 0， 12V
 *       1， xx
 *       2,  制氧压力
*        3， 呼吸压力
 **/
void main_showAdcValue(unsigned char chn) 
{
    int i = 0;
    char buf[12] = {'\0'};
    //static int adcChn0 = 0, adcChn1 = 0, adcChn2 = 10000, adcChn3 = 10000;
    
    if (nowPicTick != PIC_MODE_MAIN)
        return;
     
    float adcTmp = 0;
    for (i = 0; i < 100;) {
        adcTmp += adcValue[i + chn];
        i += 4;
    }

    adcTmp = adcTmp / 25;               //求平均值
    adcTmp = adcTmp / 4095 * 3.3;       //转换为电压值
    
    switch (chn) {
        case 0:
            break;
        case 1:
            break;
        case 2:         //制氧压力 
            adcTmp = (adcTmp - 0.2) / 0.005;
            adcTmp += 7;          //校准，误差
//            if (adcChn2 == (int )adcTmp)
//                break;
//            adcChn2 = (int )adcTmp;
            oled_clearRegion(4, 5, 72);
            sprintf(buf, "%dKPa", (int )adcTmp);
            display_string_8x16(4, 72, (unsigned char *)buf);
            break;
        case 3:         //呼吸压力
            adcTmp = (adcTmp - 1.38) / 1.25;
            adcTmp *= 1000;           //单位转换
//            if (adcChn3 == (int )adcTmp)
//                break;
//            adcChn3 = (int )adcTmp;
            oled_clearRegion(2, 3, 72);
            sprintf(buf, "%dPa", (int )adcTmp);
            display_string_8x16(2, 72, (unsigned char *)buf);
            break;
        default:break;
    }
}

void oled_test(void) 
{
    display_graphic(0, 1, (unsigned char *)signal0);
    display_graphic(0, 17, (unsigned char *)signal1);
    display_graphic(0, 34, (unsigned char *)signal2);
    display_graphic(0, 51, (unsigned char *)signal3);
    display_graphic(0, 68, (unsigned char *)signal4);
    oled_clearRegion(0, 0, 17);
    
    display_graphic(2, 1, (unsigned char *)signal0);
    display_graphic(2, 17, (unsigned char *)signal1);
    display_graphic(2, 34, (unsigned char *)signal2);
    display_graphic(2, 51, (unsigned char *)signal3);
    display_graphic(2, 68, (unsigned char *)signal4);
    oled_clearRegion(2, 3, 0);
    
    display_graphic(4, 1, (unsigned char *)signal0);
    display_graphic(4, 17, (unsigned char *)signal1);
    display_graphic(4, 34, (unsigned char *)signal2);
    display_graphic(4, 51, (unsigned char *)signal3);
    display_graphic(4, 68, (unsigned char *)signal4);
    oled_clearRegion(4, 5, 0);
    
    display_graphic(6, 1, (unsigned char *)signal0);
    display_graphic(6, 17, (unsigned char *)signal1);
    display_graphic(6, 34, (unsigned char *)signal2);
    display_graphic(6, 51, (unsigned char *)signal3);
    display_graphic(6, 68, (unsigned char *)signal4);  
    oled_clear();
}



void cfgShowCom(void)
{
    display_graphic(2, 0, (unsigned char *)she);
    display_graphic(2, 16, (unsigned char *)ding);
    display_graphic(2, 32, (unsigned char *)zhi);
    display_graphic(2, 48, (unsigned char *)maohao);
}

void cfgClearDiff(void)
{
    oled_clearRegion(0, 1, 0);  //清除第一 二行
    oled_clearRegion(2, 3, 0);
    oled_clearRegion(4, 5, 0);
}
/* 配置界面公用函数 */
unsigned int cfgShowDiff(CfgPars *cfg)
{
    char bufTmp[16] = {0};
    unsigned int valueTmp = *(cfg->value);

    //(默认值)600ms
    sprintf(bufTmp, "%d%s", valueTmp, cfg->uint);
    display_string_8x16(0, 70, (unsigned char *)bufTmp);
    //设定值:
    sprintf(bufTmp, "%d%s", valueTmp, cfg->uint);
    display_string_8x16(2, 70, (unsigned char *)bufTmp);
        
    return 0;

}
/* 参数配置界面 */
/* 画面1：阀门1时序：660ms */
char cfg_picture1(CfgPars *cfg)
{    
    cfgClearDiff();
    
    //阀A时序:600ms
    display_graphic(0, 0, (unsigned char *)fa);        
    display_string_8x16(0, 16, (unsigned char *)"A");
    display_graphic(0, 24, (unsigned char *)shi);      
    display_graphic(0, 40, (unsigned char *)xu);      
    display_graphic(0, 56, (unsigned char *)maohao);
    
    cfgShowCom();
    
    cfg->min = 460;
    cfg->max = 760;
    cfg->value = (unsigned int *)&SystemPar.FrontHalfPeriodTime;
    cfg->uint = "ms";
    cfg->tick = 1;
    cfgShowDiff(cfg);
    
    return 0;
}

/* 画面2：阀门B时序：680ms */
char cfg_picture2(CfgPars *cfg)
{    
    cfgClearDiff();
    //阀A时序:600ms
    display_graphic(0, 0, (unsigned char *)fa);        
    display_string_8x16(0, 16, (unsigned char *)"B");
    display_graphic(0, 24, (unsigned char *)shi);      
    display_graphic(0, 40, (unsigned char *)xu);      
    display_graphic(0, 56, (unsigned char *)maohao);
    
    cfgShowCom();
    
    cfg->min = 580;
    cfg->max = 780;
    cfg->value = (unsigned int *)&SystemPar.AfterHalfPeriodTime;
    cfg->uint = "ms";
    cfg->tick = 1;
    
    cfgShowDiff(cfg);
    
    return 0;
}

/* 画面3:排氮时序：80ms */
char cfg_picture3(CfgPars *cfg)
{   
    cfgClearDiff();
    //阀A时序:1000ms
    display_graphic(0, 0, (unsigned char *)pai);        
    display_graphic(0, 16, (unsigned char *)dan);
    display_graphic(0, 32, (unsigned char *)shi);      
    display_graphic(0, 48, (unsigned char *)xu);      
    display_graphic(0, 64, (unsigned char *)maohao);
    
    cfgShowCom();
    
    cfg->min = 20;
    cfg->max = 200;
    cfg->value = (unsigned int *)&SystemPar.OffsetTime;
    cfg->uint = "ms";
    cfg->tick = 1;
    cfgShowDiff(cfg);
    
    return 0;
}

/* 画面4：呼吸阀开800ms */
char cfg_picture4(CfgPars *cfg)
{
    cfgClearDiff();
    //阀A时序:1000ms
    display_graphic(0, 0, (unsigned char *)hu);        
    display_graphic(0, 16, (unsigned char *)xi);
    display_graphic(0, 32, (unsigned char *)fa);      
    display_graphic(0, 48, (unsigned char *)kai);      
    display_graphic(0, 64, (unsigned char *)maohao);
    
    cfgShowCom();
    
    cfg->min = 400;
    cfg->max = 1200;
    cfg->value = (unsigned int *)&SystemPar.Breath_Open_Press;
    cfg->uint = "ms";
    cfg->tick = 1;
    cfgShowDiff(cfg);

    return 0;
}

/* 显示主界面：制氧模式 */
char main_picture(void)
{
    display_graphic(0, 0, (unsigned char *)zhi4);
    display_graphic(0, 16, (unsigned char *)yang); 
    display_graphic(0, 32, (unsigned char *)mo); 
    display_graphic(0, 48, (unsigned char *)mshi); 
    display_graphic(0, 64, (unsigned char *)maohao);   
    
    display_graphic(2, 0, (unsigned char *)hu);
    display_graphic(2, 16, (unsigned char *)xi);
    display_graphic(2, 32, (unsigned char *)ya);
    display_graphic(2, 48, (unsigned char *)li);
    display_graphic(2, 64, (unsigned char *)maohao);  
    
    display_graphic(4, 0, (unsigned char *)zhi4);
    display_graphic(4, 16, (unsigned char *)yang);
    display_graphic(4, 32, (unsigned char *)ya);
    display_graphic(4, 48, (unsigned char *)li);
    display_graphic(4, 64, (unsigned char *)maohao);  
    return 0;
}

/* 显示主界面：电机功率 */
char mian_picMotorSpeed(void)
{
    char bufTmp[8] = {0};
    char valueTmp = 0;
    
    if (motorSpeed == MOTOR_STA_HIGH)
        valueTmp = 25;
    else if (motorSpeed == MOTOR_STA_LOW)
        valueTmp = 17;
    else if (motorSpeed == MOTOR_STA_STOP)
        valueTmp = 0;
    
    oled_clearRegion(0, 1, 72);
    sprintf(bufTmp, "%dW", valueTmp);
    display_string_8x16(0, 72, (unsigned char *)bufTmp);
    return 0;
}

/* 主按键响应函数 */
void main_keyAction(void)
{
    static unsigned char keyValue = NonKey;
    static CfgPars cfg;
    char bufTmp[16] = {0};
    static unsigned int valueTmp = 0;
    
    /* 如果没有按键按下，返回 */
    if (KeyFlagStructure.KeyValue == NonKey)
        return;
    
    /* 取出按键值，并清除按键结构 */
    keyValue = KeyFlagStructure.KeyValue;
    KeyFlagStructure.KeyValue = NonKey;
    
    /* 短按键 */
    if (KeyFlagStructure.ShortKeyPressFlag == 0x01) {
        if (keyValue == ModKey) {
            debug_str("ModKey");
            
            /* 更新系统参数 */
            if (nowPicTick != PIC_MODE_MAIN)   {
                *(cfg.value) = valueTmp;
            } 
            
            /* 画面切换 */
            nowPicTick++;
            if (nowPicTick >= 5) nowPicTick = 0;
            
            if (nowPicTick == PIC_MODE_MAIN) {
                /* 写入flash */
                main_saveSysPar();
            }
            switch(nowPicTick) {
                case PIC_MODE_MAIN:
                    main_picture();
                    mian_picMotorSpeed();
                    break;
                case PIC_MODE_PIC1:
                    cfg_picture1(&cfg);
                    break;
                case PIC_MODE_PIC2:
                    cfg_picture2(&cfg);
                    break;
                case PIC_MODE_PIC3:
                    cfg_picture3(&cfg);
                    break;
                case PIC_MODE_PIC4:
                    cfg_picture4(&cfg);
                    break;
                default:
                    break;
            }
            /* 获取系统的配置 */
            if (nowPicTick != PIC_MODE_MAIN)
                valueTmp = *cfg.value;
        } else if (keyValue == IncKey) {
            debug_str("IncKey");
            if (nowPicTick == PIC_MODE_MAIN) {
                motor_speedCtl(0x02);
                mian_picMotorSpeed();
            } else {
                valueTmp++;
                if (valueTmp < cfg.min)
                    valueTmp = cfg.max;
                if (valueTmp > cfg.max)
                    valueTmp = cfg.min;
                oled_clearRegion(2, 3, 70);  
                memset(bufTmp, 0, sizeof(bufTmp));
                sprintf(bufTmp, "%d%s", valueTmp, cfg.uint);
                debug_str(bufTmp);
                display_string_8x16(2, 70, (unsigned char *)bufTmp);
            }
        } else if (keyValue == DecKey) {
            debug_str("DecKey");
            if (nowPicTick == PIC_MODE_MAIN) {
                motor_speedCtl(0x02);
                mian_picMotorSpeed();
            } else {
                valueTmp--;
                if (valueTmp < cfg.min)
                    valueTmp = cfg.max;
                if (valueTmp > cfg.max)
                    valueTmp = cfg.min;
                oled_clearRegion(2, 3, 70);  
                memset(bufTmp, 0, sizeof(bufTmp));
                sprintf(bufTmp, "%d%s", valueTmp, cfg.uint);
                display_string_8x16(2, 70, (unsigned char *)bufTmp);
            }
        }
    }
    keyValue = NonKey;  
}

/* 异或校验:单字节 */
static uint8_t xor(uint8_t *buf, uint8_t num)
{
    int i = 0;
    uint8_t tmp = buf[0];
    for (i = 1; i < num; i++)
        tmp ^= buf[i];
    
    return tmp;
}

#if 1
/* 周期保存系统参数
 * 参数： 周期，单位 分钟
 */
char main_saveSysParCycle(uint8_t cycle)
{
    static uint8_t min = 0;
    uint8_t tmp = rtc_getMin();
    
    if (((tmp - min) >= cycle) || (tmp == 0)) {
        min = tmp;
        debug_str("save data to flash");
        main_saveSysPar();
    }
    
    return 0;
}


char main_saveSysPar(void)
{
    uint32_t size = sizeof(SystemParameter_TypeDef);
    uint8_t buf[128] = {0};
    uint32_t tmp = 0;
    
    SystemPar.Xor_CRC = xor((uint8_t *)&SystemPar, size - 1);

    stmFlash_write(FLASH_ONE_START_ADDR, (uint16_t *)&SystemPar, size / 2);
    #if 0
    stmFlash_read(FLASH_ONE_START_ADDR, (uint16_t *)buf, size / 2);
    tmp = xor((uint8_t *)buf, size - 4);
    
    if (tmp == ((SystemParameter_TypeDef *)buf)->Xor_CRC) {
        debug_str("xxxxxxx6xxxxxxxx\r\n");
        debug_str("write sucess\r\n");
    } else {
        //error
        debug_str("write error----------------------------\r\n");
    }
    #endif
    /* 写入第二备份区域 */
    stmFlash_write(FLASH_TWO_START_ADDR, (uint16_t *)&SystemPar, size / 2);
#if 0
    stmFlash_read(FLASH_TWO_START_ADDR, (uint16_t *)buf, size / 2);
    tmp = xor((uint8_t *)buf, size - 4);
    
    if (tmp == ((SystemParameter_TypeDef *)buf)->Xor_CRC) {
        debug_str("xxxxxxx6xxxxxxxx\r\n");
        debug_str("write sucess\r\n");
    } else {
        //error
        debug_str("write error----------------------------\r\n");
    }
#endif
    
    debug_data((char *)buf, 88);
    return 0;
}
#endif

char main_readSysPar(char isFirst)
{
    uint8_t tmp = 0;
    unsigned char buf[128] = {0};
    uint32_t size = sizeof(SystemParameter_TypeDef);
    
    if (isFirst == 0x01)
        stmFlash_read(FLASH_ONE_START_ADDR, (uint16_t *)buf, size / 2);
    else stmFlash_read(FLASH_TWO_START_ADDR, (uint16_t *)buf, size / 2);
    tmp = xor((uint8_t *)buf, size - 1);
    
    if (tmp == ((SystemParameter_TypeDef *)buf)->Xor_CRC) {
        debug_str("xxxxxxx6xxxxxxxx\r\n");
        debug_str("read sucess\r\n");
        memcpy((unsigned char *)&SystemPar, buf, size);
        return 0;
    } else {
        //error
        debug_str("read error----------------------------\r\n");
        return -1;
    }
}
