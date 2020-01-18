#ifndef __BC20_H__
#define __BC20_H__

typedef struct {
    unsigned char csq;
    unsigned char socketNum;   //编号
    unsigned char recvLen;   //获取到数据的长度
    unsigned char res;
    unsigned char recvDataLen[10];
    unsigned char recvData[100];
    unsigned char netStatus;//网络指示灯
}BC20;


void bc20_init(void);

void mqtt_init(void);

void mqtt_putData(unsigned char temp, unsigned char humi);

#endif
