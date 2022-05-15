
#ifndef __FIFOBUFF_H__
#define __FIFOBUFF_H__

typedef struct fifo
{
	uint8_t * buffer;   //buffer的起始地址
	uint8_t in;         //buffer输入的下标
	uint8_t out;		//buffer输出的下标
	uint16_t size;      //buffer的大小，最好是2的倍数,取256最好
}FIFO_t;



void Fifobuff_Init(FIFO_t *fifo,uint8_t *buf,uint16_t len);
uint16_t Fifobuff_Get_Lenght(FIFO_t *fifo);
uint16_t Fifobuff_Get_Data(FIFO_t *fifo,uint8_t *buf,uint16_t len);
uint16_t Fifobuff_Put_Data(FIFO_t *fifo,uint8_t *buf,uint16_t len);

#endif

