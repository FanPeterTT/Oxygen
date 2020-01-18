#include "my_iic.h"

void IIC_SCL(char isHigh)
{
    HAL_GPIO_WritePin(GPIOB, OLED_I2C_SCL_Pin, isHigh == 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void IIC_SDA(char isHigh)
{
    HAL_GPIO_WritePin(GPIOB, OLED_I2C_SDA_Pin, isHigh == 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET); 
}

GPIO_PinState READ_SDA(void)
{
    return HAL_GPIO_ReadPin(GPIOB, OLED_I2C_SDA_Pin);
}

void delay(int i)
{
    int j, k;
    for (j = 0; j < i; j++)
        for (k = 0; k < 1; k++)
            ;
}

void IIC_Init(void)
{
    SDA_OUT();
    IIC_SDA(0x01);
    IIC_SCL(0x01);
}

void IIC_Start(void)
{
    SDA_OUT();     //sda线输出
	IIC_SDA(0x01);
	IIC_SCL(0x01);
	delay(4);
 	IIC_SDA(0x00);
	delay(4);
	IIC_SCL(0x00);
}
void IIC_Stop(void)
{
    SDA_OUT();//sda线输出
	IIC_SCL(0x00);
	IIC_SDA(0x00);
 	delay(4);
	IIC_SCL(0x01);
	IIC_SDA(0x01);
	delay(4);	
}

unsigned char IIC_Wait_Ack(void)
{
    unsigned char ucErrTime=0;
	SDA_IN();
	IIC_SDA(0x01);
    delay(1);	   
	IIC_SCL(0x01);
    delay(1); 
	while(READ_SDA())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL(0x00);
	return 0;  
}

void IIC_Ack(void)
{
    IIC_SCL(0x00);
	SDA_OUT();
	IIC_SDA(0x00);
	delay(2);
	IIC_SCL(0x01);
	delay(2);
	IIC_SCL(0x00);
}
void IIC_NAck(void)
{
    IIC_SCL(0x00);
	SDA_OUT();
	IIC_SDA(0x01);
	delay(2);
	IIC_SCL(0x01);
	delay(2);
	IIC_SCL(0x00);
}

#if 0
void IIC_Send_Byte(unsigned char txd)
{
    unsigned char t;   
	SDA_OUT(); 	    
    IIC_SCL(0x00);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA((txd&0x80)>>7);
        txd<<=1; 	  
		delay(2);
		IIC_SCL(0x01);
		delay(2); 
		IIC_SCL(0x00);	
		delay(2);
    }
}
#else
void IIC_Send_Byte(unsigned char txd)
{
    unsigned char t;   
	SDA_OUT(); 	    

    for(t=0;t<8;t++)
    {      
        IIC_SCL(0x00); 
        if (txd & 0x80) IIC_SDA(0x01);
        else IIC_SDA(0x00);
        txd <<= 1;
        delay(2);
        IIC_SCL(0x01);
        delay(2);
    }
    IIC_SCL(0x00);
    IIC_SCL(0x01);
}
#endif
unsigned char IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL(0x00); 
        delay(2);
		IIC_SCL(0x01);
        receive<<=1;
        if(READ_SDA()) receive++;   
		delay(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}





