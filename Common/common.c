
#include "common.h"


static u8  fac_us=0;//us微秒延时倍乘数
static u16 fac_ms=0;//ms毫秒延时倍乘数

void delay_Init()	 
{

#ifdef OS_CRITICAL_UCOS 	//假如OS_CRITICAL_UCOS定义了,说明使用ucosII了.
	u32 reload;
#endif
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//可以查看用户手册，此处为选择外部时钟  HCLK/8
	fac_us = SystemCoreClock/1000000;	//SystemCoreClock为系统时钟，此处定义fac_us是系统时钟的1/8  
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);    //直接是HCLK时钟，
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000U )){
        /* capture error */
        return ;     //设置失败
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x02U);
/* reload为24位寄存器,最大值:2的24次方1024*1024*16=16777216,在72M下,约合1.864s左右
  因为systick是72M的8分之一即9M  那么16777216/9M = 1.864s*/	 
#ifdef OS_CRITICAL_UCOS 	//如果OS_CRITICAL_UCOS定义了,说明使用ucosII了.
	reload = SystemCoreClock/8000000;		//每秒钟的计数次数 单位为K	   
	reload* = 1000000/OS_TICKS_PER_SEC;     //根据OS_TICKS_PER_SEC设定溢出时间
								
	fac_ms = 1000/OS_TICKS_PER_SEC;             //代表ucos可以延时的最少单位	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 	                    //每1/OS_TICKS_PER_SEC秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK    
#else
	fac_ms = (u16)fac_us*1000; //非ucos下,代表每个ms需要的systick时钟数   
#endif
}

#ifdef OS_CRITICAL_UCOS	//判断是否使用了ucos使用ucos和没使用ucos是不同的延时函数实现方式
		    								   
void delay_us(u32 xus)	  //延时xus
{		
	u32 ALL_ticks;
	u32 T_old,T_now,numcnt=0;
	u32 reload=SysTick->LOAD;	//LOAD的值	    	 
	ALL_ticks=xus*fac_us; 			//需要的节拍数	  		 
	numcnt=0;
	T_old=SysTick->VAL;        	//刚进入时的计数器值
	while(1)
	{
		T_now=SysTick->VAL;	
		if(T_now!=T_old)
		{	    
			if(T_now<T_old)numcnt+=T_old-T_now;//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else numcnt+=reload-T_now+T_old;	    
			T_old=T_now;
			if(numcnt>=ALL_ticks)break;//时间超过/等于要延迟的时间,则退出.
		}  
	}; 									    
}

void delay_ms(u16 xms)	   //延时xms
{						  
	if(TRUE == OSRunning)//如果os已经在跑了	    
	{		  
		if(xms>=fac_ms)//延时的时间大于ucos的最少时间周期 
		{
   			OSTimeDly(xms/fac_ms);//ucos延时
		}
		xms%=fac_ms;				//ucos已经无法提供这么小的延时了,采用普通方式延时    
	}
	delay_us((u32)(xms*1000));	//普通方式延时,此时ucos无法启动调度.
}

#else
	    								   
void delay_us(u32 xus)	//延时xus个微秒
{		
	u32 temp;	    	 
	SysTick->LOAD=xus*fac_us;                         //时间加载	  		 
	SysTick->VAL=0x00;                                //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;       	  //设置开始倒数计时的掩码

	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));            //等待时间到达   
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器

	SysTick->VAL =0X00;                            //清空计数器	 
}

//延时xms  注意xms的范围  SysTick->LOAD为24位寄存器,所以,最大延时为:xms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,xms单位为ms   对72M条件下,xms<=1864 
void delay_ms(u16 xms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)xms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
} 
#endif


void NVIC_Configuration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
}



void SysTick_Handler(void)
{}
