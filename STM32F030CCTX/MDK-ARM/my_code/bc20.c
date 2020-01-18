#include "bc20.h"
#include "my_uart.h"
#include "my_fifo.h"
#include "gpio.h"

#include <stdio.h>
#include <string.h>


/*
 *  测试地址
 */
#define ProductKey      "a1fAybihkrj"             //产品KEY
#define DeviceName      "dht11"      //
#define DeviceSecret    "qyvv2QFfOWfTpxkjO0BAUGQmO0A6c9Il"  //
#define PubTopic        "/sys/a1fAybihkrj/dht11/thing/event/property/post"
#define SubTopic        "/sys/a1NXoxeFw4c/mzh002/thing/service/property/set"

char *strPos = (void *)0;
char *strx = (void *)0;
int dataLen = 0;
unsigned char rxBuf[FIFO_BUF_SIZE] = {0};
BC20 bc20Sta;

void clearBuf(void)
{
    memset(rxBuf, 0, dataLen);
    dataLen = 0;
    strx = (void *)0;
}

char sendCmd(char *cmdBuf)
{
    printf("%s\r\n", cmdBuf);
    debug_str("send cmd: ");
    debug_str(cmdBuf);
    HAL_Delay(300);

    return 1;
}

char sendCmdAndCheck(char *cmdBuf, char *checkData)
{
    char ret = 0;

    clearBuf();
    printf("%s\r\n", cmdBuf);
    debug_str(cmdBuf);
    
    HAL_Delay(300);
    dataLen = fifoReadDataS(&uart2Fifo, rxBuf);
    debug_data((char *)rxBuf, dataLen);
    strx = strstr((const char*)rxBuf, (const char *)checkData);
    if (strx != (void *)0)
        ret = 1;

    return ret;
}

char sendCmdAndCheckWhile(char *cmdBuf, char *checkData)
{
    clearBuf();
    while(strx == (void *)0) {
        printf("%s\r\n", cmdBuf);
        HAL_Delay(300);
        dataLen = fifoReadDataS(&uart2Fifo, rxBuf);
        debug_data((char *)rxBuf, dataLen);
        strx = strstr((const char*)rxBuf, (const char *)checkData);
        if (strx != (void *)0) {
            debug_str("sendCmdAndCheckWhile:  ");
            debug_data((char *)rxBuf, dataLen);
            debug_str("\r\n");
        }
    }

    return 1;
}

char sendCmdAndWhileCheck(char *cmdBuf, char *checkData)
{
    unsigned char tick = 0;
    char *str = (void *)0;
    clearBuf();
    printf("%s\r\n", cmdBuf);
    HAL_Delay(300);
    while(strx == (void *)0) {
        dataLen = fifoReadDataS(&uart2Fifo, rxBuf);
        debug_data((char *)rxBuf, dataLen);
        str = strstr((const char *)rxBuf, (const char *)"ERROR");
        if (str != (void *)0) {
            str = (void *)0;
            printf("%s\r\n", cmdBuf);
            HAL_Delay(300);
        }
        strx = strstr((const char*)rxBuf, (const char *)checkData);
        HAL_Delay(300);
        tick++;
        if (tick >= 7) {
            sendCmd(cmdBuf);
            tick = 0;
        }
    }

    return 1;
}

void bc20_powerCtrl(char isPwerOn)
{
    HAL_GPIO_WritePin(BC20_ONOFF_GPIO_Port, BC20_ONOFF_Pin, isPwerOn == 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

//void bc20_reset(char isReset)
//{
//    HAL_GPIO_WritePin(BC20_RST_GPIO_Port, BC20_RST_Pin, (isReset == 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
//}

void bc20_reset(void)
{
    debug_str("bc20_reset\n");
    HAL_GPIO_WritePin(BC20_RST_GPIO_Port, BC20_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(1200);
    HAL_GPIO_WritePin(BC20_RST_GPIO_Port, BC20_RST_Pin, GPIO_PIN_SET);
}

void bc20_reboot(void)
{
    bc20_powerCtrl(0x00);
    HAL_Delay(1200);
    bc20_powerCtrl(0x01);
}

void bc20_at(void)
{
    int i = 0, ret = 0;
    
    for (i = 0; ; i++) {
        if (i % 3 == 0 && i != 0)
            ;//bc20_reset();
        ret = sendCmdAndCheck("AT", "OK");
        if (ret == 1) 
            return;
        
        HAL_Delay(300);
    }
}

void bc20_init(void)
{
    char sta = 0;
    int i = 0;
    
    bc20_powerCtrl(0x01);
    HAL_GPIO_WritePin(BC20_RST_GPIO_Port, BC20_RST_Pin, GPIO_PIN_SET);
    
    return;
    /* 进入AT指令模式 */
    //sendCmdAndCheckWhile("AT", "OK");
    bc20_at();
    debug_str("bc20_init: at sucess\n");
    
    /*  */
    sendCmd("ATe0&w");
    
    /* 获取卡号 */
    sendCmd("AT+CFUN=1");
    
    /* 对GNSS上电 */
    sendCmd("AT+QGNSSC=1");
    
    /* 关闭连接 */
    sendCmd("AT+QMTCLOSE=0");
    sendCmd("AT+QMTDISC=0");
    
    /* 获取卡号 */
    sendCmdAndCheckWhile("AT+CIMI", "460");

    /* 激活网络 */
    sendCmdAndCheck("AT+CGATT=1", "OK");
    debug_str("bc20_init: sucess\n");


    /* 查询激活状态 */
    sendCmdAndCheckWhile("AT+CGATT?", "+CGATT: 1");
#if 0
    /* 查看获取CSQ值 */
    sta = sendCmdAndCheck("AT+CESQ", "+CESQ");
    if (sta == 1) {
        bc20Sta.csq = (strx[7] - 0x30) * 10 + (strx[8] - 0x30);
        if ((bc20Sta.csq == 99) || (strx[7] - 0x30) == 0) {         //扫网失败
            while(1) {
                bc20Sta.netStatus = 0;
                printf("信号搜索失败， 请查看原因!\r\n");
                bc20_powerCtrl(0x01);
                HAL_Delay(300);
                HAL_Delay(300);
                bc20_powerCtrl(0x00);
                HAL_Delay(300);
            }
        } else
            bc20Sta.netStatus = 1;
    }
#endif
    clearBuf();
}

void bc20_udpClose(void)
{
    sendCmd("AT+QSOCL=0");
}

void bc20_tcpClose(void)
{
    sendCmd("AT+QICLOSE=0");
}

void bc20_createTcpSocket(void)
{
    sendCmdAndWhileCheck("AT+QIOPEN=1,0,\"TCP\",\"47.99.80.89\",14269,1234,1\r\n", "+QIOPEN: 0,0");
}

void mqtt_init(void)
{
    /* 发送产品KEY */
    char keyBuf[128] = {'\0'};
    sprintf(keyBuf, "AT+QMTCFG=\"aliauth\",0,\"%s\",\"%s\",\"%s\"\r\n", ProductKey, DeviceName, DeviceSecret);
    sendCmdAndCheckWhile(keyBuf, "OK");
//    printf("AT+QMTCFG=\"aliauth\",0,\"%s\",\"%s\",\"%s\"\r\n", ProductKey, DeviceName, DeviceSecret);
//    HAL_Delay(300);

    /* TCP连接mqtt服务器 */
    debug_str("TCP connect mqtty server..\r\n");
    sendCmdAndWhileCheck("AT+QMTOPEN=0,\"139.196.135.135\",1883\r\n", "+QMTOPEN: 0,0");
    debug_str("TCP connect mqtty server sucess....\r\n");
    
    /* 登录mqtt服务器 */
    debug_str("TCP denglu mqtty server ....\r\n");
    sendCmdAndWhileCheck("AT+QMTCONN=0,\"12345_33\"", "+QMTCONN: 0,0,0");
    debug_str("TCP denglu mqtty server .sucerss...\r\n");
}

unsigned char mqtt_saveData(char *payload, unsigned char temp, unsigned char humi)
{
    //          {"id":"123","version":"1.0","params":{"CurrentTemperature":{"value":11,"time":1575045955579},"RelativeHumidity":{"value":15,"time":1575045955579}},"method":"thing.event.property.post"}
    char json[]="{\"id\":\"26\",\"version\":\"1.0\",\"params\":{\"CurrentTemperature\":{\"value\":%d,\"time\":1575045955579},\"RelativeHumidity\":{\"value\":%d,\"time\":1575045955579}},\"method\":\"thing.event.property.post\"}";
//  char json[]="{\"datastreams\":[{\"id\":\"location\",\"datapoints\":[{\"value\":{\"lon\":%2.6f,\"lat\":%2.6f}}]}]}";
    char jsonData[200] = {0};
    int jsonDataLen = 0;

    sprintf(jsonData, json, temp, humi);
    jsonDataLen = strlen(jsonData) / sizeof(char);
    memcpy(payload, jsonData, jsonDataLen);
    return jsonDataLen;
}

void mqtt_putData(unsigned char temp, unsigned char humi)
{
    char data[200] = {0};
    unsigned char len = 0;
    char buf[12] = {'\0'};

    sprintf(data, "AT+QMTPUB=0,0,0,0,%s\r\n", PubTopic);
    if (sendCmdAndCheck(data, ">") == 1) {
        debug_str(">>>>>>>>>>\n");
        len = mqtt_saveData(data, temp, humi);
        data[len] = '\0';
        sendCmd(data);
        buf[0] = 0x1a;
        while(1) {
            HAL_UART_Transmit(&huart2, (unsigned char *)&buf[0], 1, 1000);
            HAL_Delay(300);
            dataLen = fifoReadDataS(&uart2Fifo, rxBuf);
            debug_data((char *)rxBuf, dataLen);
            strx = strstr((const char*)rxBuf, "+QMTPUB: 0,0,0");
            if (strx != (void *)0) {
                debug_str("send 1a sucess\n");
                break;
            }
        }
        clearBuf();
    }
}

