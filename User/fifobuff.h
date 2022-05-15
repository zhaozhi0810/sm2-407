
#ifndef __FIFOBUFF_H__
#define __FIFOBUFF_H__

typedef struct fifo
{
	uint8_t * buffer;   //buffer����ʼ��ַ
	uint8_t in;         //buffer������±�
	uint8_t out;		//buffer������±�
	uint16_t size;      //buffer�Ĵ�С�������2�ı���,ȡ256���
}FIFO_t;



void Fifobuff_Init(FIFO_t *fifo,uint8_t *buf,uint16_t len);
uint16_t Fifobuff_Get_Lenght(FIFO_t *fifo);
uint16_t Fifobuff_Get_Data(FIFO_t *fifo,uint8_t *buf,uint16_t len);
uint16_t Fifobuff_Put_Data(FIFO_t *fifo,uint8_t *buf,uint16_t len);

#endif

