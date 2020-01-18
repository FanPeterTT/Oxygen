#ifndef __MY_OLED_H__
#define __MY_OLED_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "my_iic.h"
  
#define OLED_ADDRESS    0x78        //8位地址：带读写位
void oled_init(void);
void oled_clear(void);

void display_graphic(unsigned char page,unsigned char column,unsigned char *dp);
void display_graphic_16x16(unsigned char page,unsigned char column,unsigned char *dp);
void display_string_8x16(unsigned char page, unsigned char column, unsigned char *text);
void oled_clearRegion(unsigned char y1, unsigned char y2, unsigned char x);
#ifdef __cplusplus
}
#endif

#endif
 
 
 
