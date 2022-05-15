
#include <stm32f4xx.h>


//delay的初始化，主要就是设置时钟源
void delay_init(void)
{
	SysTick->CTRL = 0;   //时钟源是AHB/8，不产生中断，不使能定时器
}

//延时微秒的函数
//参数 nus 表示延时的毫秒数（0-798915us）
void delay_us(uint32_t nus)
{
	if(nus < 798000)
	{
		SysTick->LOAD = nus * 21;		
		SysTick->VAL = 0;   			//写这个寄存器的任意值都会清理该寄存器
		SysTick->CTRL |= 1;				//开始倒计时了
		
		while((SysTick->CTRL & (1<<16)) == 0);  //等待这个计数器递减到0	
		
		SysTick->CTRL = 0;	           //关闭定时器	
	}
	return ;
}



//延时微秒的函数
//参数 nus 表示延时的毫秒数（0-798ms）
void delay_ms(uint32_t nms)
{
	if(nms < 798)
	{
		delay_us(nms*1000);
	}
	
}


