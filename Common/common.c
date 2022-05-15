
#include "common.h"


static u8  fac_us=0;//us΢����ʱ������
static u16 fac_ms=0;//ms������ʱ������

void delay_Init()	 
{

#ifdef OS_CRITICAL_UCOS 	//����OS_CRITICAL_UCOS������,˵��ʹ��ucosII��.
	u32 reload;
#endif
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//���Բ鿴�û��ֲᣬ�˴�Ϊѡ���ⲿʱ��  HCLK/8
	fac_us = SystemCoreClock/1000000;	//SystemCoreClockΪϵͳʱ�ӣ��˴�����fac_us��ϵͳʱ�ӵ�1/8  
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);    //ֱ����HCLKʱ�ӣ�
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000U )){
        /* capture error */
        return ;     //����ʧ��
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x02U);
/* reloadΪ24λ�Ĵ���,���ֵ:2��24�η�1024*1024*16=16777216,��72M��,Լ��1.864s����
  ��Ϊsystick��72M��8��֮һ��9M  ��ô16777216/9M = 1.864s*/	 
#ifdef OS_CRITICAL_UCOS 	//���OS_CRITICAL_UCOS������,˵��ʹ��ucosII��.
	reload = SystemCoreClock/8000000;		//ÿ���ӵļ������� ��λΪK	   
	reload* = 1000000/OS_TICKS_PER_SEC;     //����OS_TICKS_PER_SEC�趨���ʱ��
								
	fac_ms = 1000/OS_TICKS_PER_SEC;             //����ucos������ʱ�����ٵ�λ	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 	                    //ÿ1/OS_TICKS_PER_SEC���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK    
#else
	fac_ms = (u16)fac_us*1000; //��ucos��,����ÿ��ms��Ҫ��systickʱ����   
#endif
}

#ifdef OS_CRITICAL_UCOS	//�ж��Ƿ�ʹ����ucosʹ��ucos��ûʹ��ucos�ǲ�ͬ����ʱ����ʵ�ַ�ʽ
		    								   
void delay_us(u32 xus)	  //��ʱxus
{		
	u32 ALL_ticks;
	u32 T_old,T_now,numcnt=0;
	u32 reload=SysTick->LOAD;	//LOAD��ֵ	    	 
	ALL_ticks=xus*fac_us; 			//��Ҫ�Ľ�����	  		 
	numcnt=0;
	T_old=SysTick->VAL;        	//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		T_now=SysTick->VAL;	
		if(T_now!=T_old)
		{	    
			if(T_now<T_old)numcnt+=T_old-T_now;//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else numcnt+=reload-T_now+T_old;	    
			T_old=T_now;
			if(numcnt>=ALL_ticks)break;//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	}; 									    
}

void delay_ms(u16 xms)	   //��ʱxms
{						  
	if(TRUE == OSRunning)//���os�Ѿ�������	    
	{		  
		if(xms>=fac_ms)//��ʱ��ʱ�����ucos������ʱ������ 
		{
   			OSTimeDly(xms/fac_ms);//ucos��ʱ
		}
		xms%=fac_ms;				//ucos�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	delay_us((u32)(xms*1000));	//��ͨ��ʽ��ʱ,��ʱucos�޷���������.
}

#else
	    								   
void delay_us(u32 xus)	//��ʱxus��΢��
{		
	u32 temp;	    	 
	SysTick->LOAD=xus*fac_us;                         //ʱ�����	  		 
	SysTick->VAL=0x00;                                //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;       	  //���ÿ�ʼ������ʱ������

	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));            //�ȴ�ʱ�䵽��   
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����

	SysTick->VAL =0X00;                            //��ռ�����	 
}

//��ʱxms  ע��xms�ķ�Χ  SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:xms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,xms��λΪms   ��72M������,xms<=1864 
void delay_ms(u16 xms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)xms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  	    
} 
#endif


void NVIC_Configuration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
}



void SysTick_Handler(void)
{}
