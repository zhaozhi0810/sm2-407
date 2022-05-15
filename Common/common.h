#include "stm32f4xx.h"
	   	
//定义系统文件夹是否支持UCOSII   0表示不支持UCOSII  1表示支持UCOSII
#define OS_UCOS		0	
																	    	 
//IO位带操作宏定义,方便调用和使用
#define BITBAND(addr, bit) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bit<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bit)   MEM_ADDR(BITBAND(addr, bit)) 

//IO口地址映射
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
 
//控制单个的IO口管脚输入和输出操作
#define PAOUT(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAIN(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBOUT(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBIN(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCOUT(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCIN(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDOUT(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDIN(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEOUT(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEIN(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFOUT(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFIN(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGOUT(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGIN(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入


void delay_init(void);
void NVIC_Configuration(void);

