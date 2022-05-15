
#include <stm32f4xx.h>
#include "fifobuff.h"




void Fifobuff_Init(FIFO_t *fifo,uint8_t *buf,uint16_t len)
{
	fifo-> buffer = buf;    //ָ����ڵĻ���
	fifo->in = 0;
	fifo->out = 0;
	fifo->size = len;
}


//��ȡ�����е����ݸ���
uint16_t Fifobuff_Get_Lenght(FIFO_t *fifo)
{
	if(fifo->size == 256)
	{
		return fifo->in + fifo->size - fifo->out;
	}
	else
	{
		return (fifo->in + fifo->size - fifo->out)%fifo->size;
	}
}



uint16_t Fifobuff_Get_Data(FIFO_t *fifo,uint8_t *buf,uint16_t len)
{
	uint8_t lenght;
	uint8_t i;
	
	//����һ��buff�е��ֽ���
	lenght = Fifobuff_Get_Lenght(fifo);
	if(lenght == 0)  //������û������
	{
		return 0;
	}
	
	if(len < lenght)   //FIFO���ջ�������ݱȶ࣬�������ڵĻ���С
	{
		lenght = len;   
	}
	
	if(fifo->size == 256)
	{
		for(i=0;i<lenght;i++) //��ȡ���������
		{
			buf[i] = fifo->buffer[(fifo->out)++];
		}
		
	}
	else
	{
		for(i=0;i<lenght;i++) //��ȡ���������
		{
			buf[i] = fifo->buffer[((fifo->out)+i)%fifo->size];
		}
		fifo->out = (fifo->out+lenght)%fifo->size;
	}

	return lenght;
}


uint16_t Fifobuff_Put_Data(FIFO_t *fifo,uint8_t *buf,uint16_t len)
{
	uint8_t i;

	
	if(fifo->size == 256)
	{
		for(i=0;i<len;i++) //��ȡ���������
		{
			fifo->buffer[(fifo->in)++] = buf[i] ;
		}		
	}
	else
	{
		for(i=0;i<len;i++) //��ȡ���������
		{
			fifo->buffer[(fifo->in+i)%fifo->size] = buf[i] ;
		//	buf[i] = fifo->buffer[((fifo->out)+i)%fifo->size];
		}
		fifo->in = (fifo->in+len)%fifo->size;
	}

	return len;
}
