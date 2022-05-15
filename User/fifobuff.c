
#include <stm32f4xx.h>
#include "fifobuff.h"




void Fifobuff_Init(FIFO_t *fifo,uint8_t *buf,uint16_t len)
{
	fifo-> buffer = buf;    //指向存在的缓存
	fifo->in = 0;
	fifo->out = 0;
	fifo->size = len;
}


//获取缓存中的数据个数
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
	
	//计算一下buff中的字节数
	lenght = Fifobuff_Get_Lenght(fifo);
	if(lenght == 0)  //缓存中没有数据
	{
		return 0;
	}
	
	if(len < lenght)   //FIFO接收缓存的数据比多，而我现在的缓存小
	{
		lenght = len;   
	}
	
	if(fifo->size == 256)
	{
		for(i=0;i<lenght;i++) //读取缓存的数据
		{
			buf[i] = fifo->buffer[(fifo->out)++];
		}
		
	}
	else
	{
		for(i=0;i<lenght;i++) //读取缓存的数据
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
		for(i=0;i<len;i++) //读取缓存的数据
		{
			fifo->buffer[(fifo->in)++] = buf[i] ;
		}		
	}
	else
	{
		for(i=0;i<len;i++) //读取缓存的数据
		{
			fifo->buffer[(fifo->in+i)%fifo->size] = buf[i] ;
		//	buf[i] = fifo->buffer[((fifo->out)+i)%fifo->size];
		}
		fifo->in = (fifo->in+len)%fifo->size;
	}

	return len;
}
