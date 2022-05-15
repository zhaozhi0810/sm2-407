
#include <stm32f4xx.h>


//delay�ĳ�ʼ������Ҫ��������ʱ��Դ
void delay_init(void)
{
	SysTick->CTRL = 0;   //ʱ��Դ��AHB/8���������жϣ���ʹ�ܶ�ʱ��
}

//��ʱ΢��ĺ���
//���� nus ��ʾ��ʱ�ĺ�������0-798915us��
void delay_us(uint32_t nus)
{
	if(nus < 798000)
	{
		SysTick->LOAD = nus * 21;		
		SysTick->VAL = 0;   			//д����Ĵ���������ֵ��������üĴ���
		SysTick->CTRL |= 1;				//��ʼ����ʱ��
		
		while((SysTick->CTRL & (1<<16)) == 0);  //�ȴ�����������ݼ���0	
		
		SysTick->CTRL = 0;	           //�رն�ʱ��	
	}
	return ;
}



//��ʱ΢��ĺ���
//���� nus ��ʾ��ʱ�ĺ�������0-798ms��
void delay_ms(uint32_t nms)
{
	if(nms < 798)
	{
		delay_us(nms*1000);
	}
	
}


