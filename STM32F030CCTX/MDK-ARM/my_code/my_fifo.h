#ifndef _FIFO_H_
#define _FIFO_H_

#define FIFO_BUF_SIZE 256

typedef struct
{
    unsigned char buf[FIFO_BUF_SIZE];
    int readIndex;
    int writeIndex;
}Fifo;

int             fifoAbs(int data);
void            fifoInitial(Fifo* fifo);
unsigned int    fifoCanReadSize(Fifo* fifo);
unsigned char   fifoRead(Fifo* fifo);
void            fifoWrite(Fifo* fifo,unsigned char data);
unsigned char   fifoCheckIndex(Fifo* fifo,unsigned int index);
void            fifoClear(Fifo* fifo);
int             fifoReadDataS(Fifo *fifo, unsigned char *buf);

#endif
