#include "my_oled.h"
#include "fonts.h"
#include "my_bmp.h"

void display_graphic(unsigned char page,unsigned char column,unsigned char *dp);

void oled_resetCtrl(char isReset)
{
    HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, isReset == 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void oled_powerCtrl(char isOn)
{
    HAL_GPIO_WritePin(OLED_PWOER_EN_GPIO_Port, OLED_PWOER_EN_Pin, isOn == 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void oled_send(unsigned char data)
{
    IIC_Send_Byte(data);
}

void oled_sendCmd(unsigned char cmd)
{
    IIC_Start();
    oled_send(OLED_ADDRESS);
    oled_send(0x00);
    oled_send(cmd);
    IIC_Stop();
}

void oled_sendData(unsigned char data)
{
    IIC_Start();
    oled_send(OLED_ADDRESS);
    oled_send(0x40);
    oled_send(data);
    IIC_Stop();
}

void oled_address(unsigned char page,unsigned char column)
{
//	column=column-1;
//	page=page-1;
	oled_sendCmd(0xb0+page);
	oled_sendCmd(((column>>4)&0x0f)+0x10);
	oled_sendCmd(column&0x0f); 
}

void oled_clear(void)
{
    unsigned char i, j;
    
    for (i = 0; i < 8; i++) {
        oled_address(i, 0);
        for (j = 0; j < 128; j++)
            oled_sendData(0x00);
    }
}

/* 清除y2 - y1行， x-128数据 */
void oled_clearRegion(unsigned char y1, unsigned char y2, unsigned char x)
{
    unsigned char i, j;
    for (i = y1; i < (y2 + 1); i++) {
        oled_address(i, x);
        for (j = x; j < 128; j++)
            oled_sendData(0x00);
    }
}

void full_display(unsigned char data1, unsigned char data2)
{
    int i, j;
    
    for (i = 0; i < 8; i++) {
        oled_address(i + 1, 1);
        for (j = 0; j < 64; j++) {
            oled_sendData(data1);
            oled_sendData(data2);
        }
    }
}


void oled_init(void)
{
    oled_powerCtrl(0x01);   //上电
    HAL_Delay(50);
    oled_resetCtrl(0x01);
    HAL_Delay(200);
    oled_resetCtrl(0x00);
    HAL_Delay(1);
    
    oled_sendCmd(0xFD);	//Command Lock  ( SSD1309 use, SSD1305 no use )
   	oled_sendCmd(0x12);	// Unlock Driver IC (0x12/0x16)	   

	oled_sendCmd(0xAE);	// Set Display Offset

	oled_sendCmd(0xD5);	 //Set Display Clock Divide Ratio/Oscillator Frequency
	oled_sendCmd(0xA0);	//Oscillator freq;-->0xf0

	oled_sendCmd(0xA8);	//Set Multiplex Ratio 
	oled_sendCmd(0x3F);	// 

	oled_sendCmd(0xD3);	//Set Display Offset
	oled_sendCmd(0x00);	//      --->0x40

	oled_sendCmd(0x40);	// Set Display Start Line

    oled_sendCmd(0xA1);	// Set SEG/Column Mapping (0xA0/0xA1)
	oled_sendCmd(0xC8);	// Set COM/Row Scan Direction (0xC0/0xC8)

	oled_sendCmd(0xDA);	//Set COM Pins Hardware Configuration
	oled_sendCmd(0x12);	// Set Alternative Configuration (0x02/0x12)
	
	oled_sendCmd(0x81);	//Set Current Control 
    oled_sendCmd(0xDF);  // 
	
	oled_sendCmd(0xD9);	//Set Pre-Charge Period 
	oled_sendCmd(0x82);	// ---->0xf1

	oled_sendCmd(0xDB);	//Set VCOM Deselect Level
	oled_sendCmd(0x34);	// 

	oled_sendCmd(0x20);
	oled_sendCmd(0x02);	// Set Page Addressing Mode (0x00/0x01/0x02)

	oled_sendCmd(0xA4);	// Set Entire Display On/Off (0xA4/0xA5)
	oled_sendCmd(0xA6);	// Set Normal/Inverse Display (0xA6/0xA7)
    
    oled_clear();
    HAL_Delay(10);
    oled_sendCmd(0xAF); //Set Displat On
    HAL_Delay(100);
}

void display_graphic_128x64(unsigned char page,unsigned char column,unsigned char *dp)
{
	int i,j;
	for(j=0;j<8;j++)
	{
		oled_address(page+j,column);
		for(i=0;i<128;i++)
		{
			oled_sendData(*dp);   
			dp++;
		}
	}          
}


void display_graphic_32x32(unsigned char page,unsigned char column,unsigned char *dp)
{
	unsigned char i,j;
	for(j=0;j<4;j++) {
		oled_address(page+j,column);
		for (i=0;i<32;i++) {
			oled_sendData(*dp);
			dp++;
        }
    }
}

//??16x16 ????????????16x16 ???????
void display_graphic_16x16(unsigned char page,unsigned char column,unsigned char *dp)
{
	unsigned char i,j;
	for(j=0;j<2;j++)
    {
        oled_address(page+j,column);
        for (i=0;i<16;i++)
        {
            oled_sendData(*dp); //????LCD,?????8 ???????????1
            dp++;
        }
    }
}

void display_graphic(unsigned char page,unsigned char column,unsigned char *dp)
{
	unsigned char i,j;
    unsigned char h, v;
    h = *dp;
    dp++;
    v = *dp;
    dp++;
    
	for(j = 0; j < (h / 8); j++)
    {
        oled_address(page+j,column);
        for (i = 0; i < v; i++)
        {
            oled_sendData(*dp);
            dp++;
        }
    }
}


/*??16x16????????????16x16???????*/
void display_graphic_16x16_2(unsigned char reverse,unsigned char page,unsigned char column,unsigned char *dp)
{
	unsigned char i,j;
	for(j=0;j<2;j++)
	{
		oled_address(page+j,column);
		for (i=0;i<16;i++)
		{	
			if(reverse==1)
				oled_sendData(*dp);
			else
				oled_sendData(~*dp);
			dp++;
		}
	}
}

void display_string_8x16(unsigned char page, unsigned char column, unsigned char *text)
{
	unsigned char i=0,j,k,n;
	if(column > 123) {
		column=0;
		page+=2;
    }
	while(text[i] > 0x00) {
        if((text[i]>=0x20)&&(text[i]<=0x7e)) {
            j=text[i]-0x20;
            for(n=0;n<2;n++) {
                oled_address(page+n,column);
                for(k=0;k<8;k++)
                    oled_sendData(ascii_table_8x16[j][k+8*n]);
            }
            i++;
            column += 8;
        } else
            i++;
    }
}

