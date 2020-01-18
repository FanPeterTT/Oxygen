#ifndef __BC20_H__
#define __BC20_H__

typedef struct {
    unsigned char csq;
    unsigned char socketNum;   //���
    unsigned char recvLen;   //��ȡ�����ݵĳ���
    unsigned char res;
    unsigned char recvDataLen[10];
    unsigned char recvData[100];
    unsigned char netStatus;//����ָʾ��
}BC20;


void bc20_init(void);

void mqtt_init(void);

void mqtt_putData(unsigned char temp, unsigned char humi);

#endif
