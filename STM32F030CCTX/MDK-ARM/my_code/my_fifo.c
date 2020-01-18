#include "my_fifo.h"

int fifoAbs(int data)
{
    return (data > 0) ? data : (-data);
}

void fifoInitial(Fifo* fifo)
{
	fifo->writeIndex = 1;
	fifo->readIndex = 0;
}

unsigned int fifoCanReadSize(Fifo* fifo)
{
    int bufSize = 0; 
    bufSize = (fifo->writeIndex > fifo->readIndex) ? 0 : (FIFO_BUF_SIZE);
    return (fifo->writeIndex - fifo->readIndex - 1 + bufSize);
}

unsigned char fifoRead(Fifo* fifo)
{
    if(fifoCanReadSize(fifo) <=0) return 0;

    fifo->readIndex++;
    if(fifo->readIndex >= FIFO_BUF_SIZE) fifo->readIndex = 0;
    return fifo->buf[fifo->readIndex];
}

void fifoWrite(Fifo* fifo,unsigned char data)
{
    if((fifo->readIndex == fifo->writeIndex + 1) ||( (fifo->readIndex == 0 ) && (fifo->writeIndex == (FIFO_BUF_SIZE - 1))))
    {
        return;
    }
    
    fifo->buf[fifo->writeIndex] = data;

    fifo->writeIndex ++;
    if(fifo->writeIndex == FIFO_BUF_SIZE)
    {
        fifo->writeIndex = 0;
    }
}

unsigned char fifoCheckIndex(Fifo* fifo,unsigned int index)
{
    unsigned int tempReadIndex = fifo->readIndex + index + 1;
    
    if(fifoCanReadSize(fifo) < index + 1) return 0;
    if(tempReadIndex >= FIFO_BUF_SIZE) tempReadIndex -= FIFO_BUF_SIZE;
    
    return fifo->buf[tempReadIndex];
}

int fifoReadDataS(Fifo *fifo, unsigned char *buf)
{
    int i = 0;
    int len = fifoCanReadSize(fifo);

    for (i = 0; i < len; i++) {
        buf[i] = fifoRead(fifo);
    }
    return len;
}

void fifoClear(Fifo* fifo)
{
    fifo->writeIndex = 1;
	fifo->readIndex = 0;
}
