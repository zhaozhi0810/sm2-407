

#include <stm32f4xx.h>
#include "uart.h"

typedef char * va_list;
#define _INTSIZEOF(n)   ((sizeof(n)+sizeof(int)-1)&~(sizeof(int) - 1) )
#define va_start(ap,fmt) ( ap = (va_list)&fmt + _INTSIZEOF(fmt) )
#define va_end(ap)    ( ap = (va_list)0 )
#define NULL (void*)0
int vsnDebug_printf(char*, int, char*, va_list);

/*
20191219  从原来的中断方式调整为   
	1.配置DMA，串口及环形buff之间的关系；
	2.USART_IT_IDLE空闲中断接收完一帧数据，处理环形buff入口指针,通知用户程序接收完一次数据；
	3.发送数据，直接将待传数据作为DMA的源地址，再使能相应DMA通道，根据串口TC中断判断发送完成；
	4.用户程序中读取FIF0；
*/


uint8_t uart1_recv_buf[BUF_SIZE];   //接收缓存
uint8_t uart1_send_buf[BUF_SIZE];   //接收缓存
FIFO_t uart1_rfifo;

uint8_t recv_flag = 0;  //为1时表示接收到一帧数据
uint8_t send_flag = 0;  //发送时标志置1，发送结束时清零

void uart1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	DMA_InitTypeDef DMA_InitStruct;
	
	//0.GPIO进行相关的初始化（io的时钟使能，io的模式配置为复用功能，设置复用功能是什么）
	//0.1 端口A的9，10这两个引脚
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	//0.3 复用为具体的某一个功能
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
		
	//0.2
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;   //复用功能
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;	  //115200
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
	//1.使能串口1外部设备时钟，否则不能使用
	//1.1.uart时钟初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  //使能时钟
	
	//1.2 串口参数的配置
	USART_StructInit(&USART_InitStruct);
	USART_InitStruct.USART_BaudRate = 115200;	
	USART_Init(USART1, &USART_InitStruct);
	
	//1.3 使能串口
	USART_Cmd(USART1, ENABLE);
	
	
	//中断相关的初始化
	USART_ITConfig(USART1, USART_IT_IDLE , ENABLE);  //空闲中断
	
	//NVIC配置
	//4.nvic arm处理管理中断的控制器
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;   //串口1的中断允许
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(& NVIC_InitStruct);
	
	
	//DMA配置
	//串口允许DMA
	USART_DMACmd(USART1, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);   //串口发送和接收都使能DMA  | USART_DMAReq_Tx
	
	//时钟使能 RCC_AHB1Periph_DMA2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	//DMA2 数据流5通道4对应接收，数据流7通道4对应发送
	DMA_DeInit(DMA2_Stream5);
	DMA_DeInit(DMA2_Stream7);
	
	DMA_StructInit(&DMA_InitStruct);
	DMA_InitStruct.DMA_Channel = DMA_Channel_4;  //通道4
	DMA_InitStruct.DMA_BufferSize = BUF_SIZE ;   //缓存大小
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)uart1_recv_buf;  //缓存的首地址
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR); //串口接收缓存的地址
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址自动增加
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;     //循环存储到FIFO缓存中 DMA_Mode_Circular
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;  //优先级，比发送优先级高
	DMA_Init(DMA2_Stream5, &DMA_InitStruct);  //接收通道	
	DMA_Cmd(DMA2_Stream5,ENABLE);
	
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;    		 //正常发送
	DMA_InitStruct.DMA_BufferSize = 0;            		 //发送缓存为空
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;  //传输方向是外设到缓存
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)uart1_send_buf; //发送的缓存暂时不设置
	DMA_InitStruct.DMA_Priority = DMA_Priority_Low; 	 //发送的优先级低
	DMA_Init(DMA2_Stream7, &DMA_InitStruct);  			 //发送通道

	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);   //DMA发送完成中断

	//NVIC配置 DMA中断
	//4.nvic arm处理管理中断的控制器
	NVIC_InitStruct.NVIC_IRQChannel = DMA2_Stream7_IRQn;   //串口1的中断允许
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(& NVIC_InitStruct);

	Fifobuff_Init(&uart1_rfifo,uart1_recv_buf,BUF_SIZE);

}



////发送字符串
//void send_str(uint8_t * str)
//{
//	//判断参数是否合法
//	if(str == NULL)
//	{
//		return;
//	}
//		
//	while(*str != 0)
//	{
//		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//		USART_SendData(USART1, *str++);	//可以判断是否发送结束		
//	}
//}


void uart1_send(uint16_t len)
{

	if(len == 0)
	{
		return;
	}
		
	send_flag = 1; //开始发送标志
	DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_FEIF7 | DMA_FLAG_DMEIF7 | DMA_FLAG_TEIF7 | DMA_FLAG_HTIF7 | DMA_FLAG_TCIF7);
	DMA_Cmd(DMA2_Stream7,DISABLE);
	DMA2_Stream7->NDTR  = len;//发送数据大小
	DMA_Cmd(DMA2_Stream7,ENABLE);

}




void Debug_printf(const char* fmt, ...)
{	
	int n;
	va_list args;
	
	while(send_flag == 1);  //正在发送，等待前面的发送完
	
    
    
    va_start(args, fmt);
    n = vsnprintf((char*)uart1_send_buf, 256, (void*)fmt, args);
    va_end(args);
    	
	uart1_send(n);
	
	return;
}




//void send_char(uint8_t dat)
//{
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//	USART_SendData(USART1, dat);	//可以判断是否发送结束	
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




//串口中断处理函数
//每收到一个字节就会进入中断一次
void USART1_IRQHandler(void)
{

	if(USART_GetITStatus(USART1, USART_IT_IDLE))  //空闲中断
	{
		uart1_rfifo.in = BUF_SIZE - DMA_GetCurrDataCounter(DMA2_Stream5);   //接收到多少个数据
		
		USART_ReceiveData(USART1);  //读取清中断
		
		recv_flag = 1;   //接收到一串完整的数据
	}
	//清除标志
	//读取数据寄存器的内容，自动把中断标志清除
}


void DMA2_Stream7_IRQHandler(void)
{
	send_flag = 0;    //发送完成
	
	DMA_Cmd(DMA2_Stream7, DISABLE);   //发送DMA通道禁止
		
	DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
}

