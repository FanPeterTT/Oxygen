#ifndef __MY_MAIN_H__
#define __MY_MAIN_H__

typedef struct {
    unsigned char tick;
    unsigned int *value;
    unsigned int max;
    unsigned int min;
    char *uint;
}CfgPars;

typedef enum {
    PIC_MODE_MAIN,
    PIC_MODE_PIC1,
    PIC_MODE_PIC2,
    PIC_MODE_PIC3,
    PIC_MODE_PIC4,
}PIC_MODE;

void main_init(void);

char main_saveSysPar(void);
char main_readSysPar(char isFirst);

char main_saveSysParCycle(unsigned char cycle);

void oled_test(void);

void main_keyAction(void);
void main_showAdcValue(unsigned char chn);

char main_picture(void);
char mian_picMotorSpeed(void);

char cfg_picture1(CfgPars *cfg);
char cfg_picture2(CfgPars *cfg);
char cfg_picture3(CfgPars *cfg);
char cfg_picture4(CfgPars *cfg);

#endif
