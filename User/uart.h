
#ifndef __UART_H__
#define __UART_H__

#include "fifobuff.h"

void uart1_init(void);

void Debug_printf(const char* fmt, ...);

#define BUF_SIZE 256
extern uint8_t uart1_recv_buf[BUF_SIZE]; //´®¿Ú½ÓÊÕ»º´æ

extern FIFO_t uart1_rfifo;
extern uint8_t recv_flag;

#endif
