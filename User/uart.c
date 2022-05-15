

#include <stm32f4xx.h>
#include "uart.h"

typedef char * va_list;
#define _INTSIZEOF(n)   ((sizeof(n)+sizeof(int)-1)&~(sizeof(int) - 1) )
#define va_start(ap,fmt) ( ap = (va_list)&fmt + _INTSIZEOF(fmt) )
#define va_end(ap)    ( ap = (va_list)0 )
#define NULL (void*)0
int vsnDebug_printf(char*, int, char*, va_list);

/*
20191219  ��ԭ�����жϷ�ʽ����Ϊ   
	1.����DMA�����ڼ�����buff֮��Ĺ�ϵ��
	2.USART_IT_IDLE�����жϽ�����һ֡���ݣ�������buff���ָ��,֪ͨ�û����������һ�����ݣ�
	3.�������ݣ�ֱ�ӽ�����������ΪDMA��Դ��ַ����ʹ����ӦDMAͨ�������ݴ���TC�ж��жϷ�����ɣ�
	4.�û������ж�ȡFIF0��
*/


uint8_t uart1_recv_buf[BUF_SIZE];   //���ջ���
uint8_t uart1_send_buf[BUF_SIZE];   //���ջ���
FIFO_t uart1_rfifo;

uint8_t recv_flag = 0;  //Ϊ1ʱ��ʾ���յ�һ֡����
uint8_t send_flag = 0;  //����ʱ��־��1�����ͽ���ʱ����

void uart1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	DMA_InitTypeDef DMA_InitStruct;
	
	//0.GPIO������صĳ�ʼ����io��ʱ��ʹ�ܣ�io��ģʽ����Ϊ���ù��ܣ����ø��ù�����ʲô��
	//0.1 �˿�A��9��10����������
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	//0.3 ����Ϊ�����ĳһ������
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
		
	//0.2
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;   //���ù���
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;	  //115200
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
	//1.ʹ�ܴ���1�ⲿ�豸ʱ�ӣ�������ʹ��
	//1.1.uartʱ�ӳ�ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  //ʹ��ʱ��
	
	//1.2 ���ڲ���������
	USART_StructInit(&USART_InitStruct);
	USART_InitStruct.USART_BaudRate = 115200;	
	USART_Init(USART1, &USART_InitStruct);
	
	//1.3 ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);
	
	
	//�ж���صĳ�ʼ��
	USART_ITConfig(USART1, USART_IT_IDLE , ENABLE);  //�����ж�
	
	//NVIC����
	//4.nvic arm��������жϵĿ�����
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;   //����1���ж�����
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(& NVIC_InitStruct);
	
	
	//DMA����
	//��������DMA
	USART_DMACmd(USART1, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);   //���ڷ��ͺͽ��ն�ʹ��DMA  | USART_DMAReq_Tx
	
	//ʱ��ʹ�� RCC_AHB1Periph_DMA2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	//DMA2 ������5ͨ��4��Ӧ���գ�������7ͨ��4��Ӧ����
	DMA_DeInit(DMA2_Stream5);
	DMA_DeInit(DMA2_Stream7);
	
	DMA_StructInit(&DMA_InitStruct);
	DMA_InitStruct.DMA_Channel = DMA_Channel_4;  //ͨ��4
	DMA_InitStruct.DMA_BufferSize = BUF_SIZE ;   //�����С
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)uart1_recv_buf;  //������׵�ַ
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR); //���ڽ��ջ���ĵ�ַ
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Զ�����
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;     //ѭ���洢��FIFO������ DMA_Mode_Circular
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;  //���ȼ����ȷ������ȼ���
	DMA_Init(DMA2_Stream5, &DMA_InitStruct);  //����ͨ��	
	DMA_Cmd(DMA2_Stream5,ENABLE);
	
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;    		 //��������
	DMA_InitStruct.DMA_BufferSize = 0;            		 //���ͻ���Ϊ��
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;  //���䷽�������赽����
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)uart1_send_buf; //���͵Ļ�����ʱ������
	DMA_InitStruct.DMA_Priority = DMA_Priority_Low; 	 //���͵����ȼ���
	DMA_Init(DMA2_Stream7, &DMA_InitStruct);  			 //����ͨ��

	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);   //DMA��������ж�

	//NVIC���� DMA�ж�
	//4.nvic arm��������жϵĿ�����
	NVIC_InitStruct.NVIC_IRQChannel = DMA2_Stream7_IRQn;   //����1���ж�����
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(& NVIC_InitStruct);

	Fifobuff_Init(&uart1_rfifo,uart1_recv_buf,BUF_SIZE);

}



////�����ַ���
//void send_str(uint8_t * str)
//{
//	//�жϲ����Ƿ�Ϸ�
//	if(str == NULL)
//	{
//		return;
//	}
//		
//	while(*str != 0)
//	{
//		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//		USART_SendData(USART1, *str++);	//�����ж��Ƿ��ͽ���		
//	}
//}


void uart1_send(uint16_t len)
{

	if(len == 0)
	{
		return;
	}
		
	send_flag = 1; //��ʼ���ͱ�־
	DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_FEIF7 | DMA_FLAG_DMEIF7 | DMA_FLAG_TEIF7 | DMA_FLAG_HTIF7 | DMA_FLAG_TCIF7);
	DMA_Cmd(DMA2_Stream7,DISABLE);
	DMA2_Stream7->NDTR  = len;//�������ݴ�С
	DMA_Cmd(DMA2_Stream7,ENABLE);

}




void Debug_printf(const char* fmt, ...)
{	
	int n;
	va_list args;
	
	while(send_flag == 1);  //���ڷ��ͣ��ȴ�ǰ��ķ�����
	
    
    
    va_start(args, fmt);
    n = vsnprintf((char*)uart1_send_buf, 256, (void*)fmt, args);
    va_end(args);
    	
	uart1_send(n);
	
	return;
}




//void send_char(uint8_t dat)
//{
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//	USART_SendData(USART1, dat);	//�����ж��Ƿ��ͽ���	
//}




//void fputc(uint8_t dat)
//{
//	send_char(dat);
//}



///* retarget the C library Debug_printf function to the USART */
//int fputc(int ch, FILE *f)
//{
//    UART1_TX(ch);
//    return ch;
//}




//�����жϴ�����
//ÿ�յ�һ���ֽھͻ�����ж�һ��
void USART1_IRQHandler(void)
{

	if(USART_GetITStatus(USART1, USART_IT_IDLE))  //�����ж�
	{
		uart1_rfifo.in = BUF_SIZE - DMA_GetCurrDataCounter(DMA2_Stream5);   //���յ����ٸ�����
		
		USART_ReceiveData(USART1);  //��ȡ���ж�
		
		recv_flag = 1;   //���յ�һ������������
	}
	//�����־
	//��ȡ���ݼĴ��������ݣ��Զ����жϱ�־���
}


void DMA2_Stream7_IRQHandler(void)
{
	send_flag = 0;    //�������
	
	DMA_Cmd(DMA2_Stream7, DISABLE);   //����DMAͨ����ֹ
		
	DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
}

