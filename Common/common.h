#include "stm32f4xx.h"
	   	
//����ϵͳ�ļ����Ƿ�֧��UCOSII   0��ʾ��֧��UCOSII  1��ʾ֧��UCOSII
#define OS_UCOS		0	
																	    	 
//IOλ�������궨��,������ú�ʹ��
#define BITBAND(addr, bit) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bit<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bit)   MEM_ADDR(BITBAND(addr, bit)) 

//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//���Ƶ�����IO�ڹܽ�������������
#define PAOUT(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAIN(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBOUT(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBIN(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCOUT(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCIN(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDOUT(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDIN(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEOUT(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEIN(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFOUT(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFIN(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGOUT(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGIN(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����


void delay_init(void);
void NVIC_Configuration(void);

