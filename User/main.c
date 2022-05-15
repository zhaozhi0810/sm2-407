/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
* File Name          : main.c
* Author             : WWW.ARMJISHU.COM之STM32核心团队  JESSE 
* Version            : V2.0.1
* Date               : 01/2/2015
* Description        : STM32神舟IV号开发板    广州
*******************************************************************************/
#include <stm32f4xx.h>
#include <stdio.h>
#include <stdlib.h>

extern int sm2_test(void);
extern void usart1_init(uint32_t bandrate);
uint8_t UART1_Get_Ch(void);
int systick_config(void);
void UART1_TX_String(uint8_t *buf,uint8_t len);
void delay_1ms(uint32_t ncount);

void led_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//

	//GPIOE13,GPIOE14,GPIOE15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;    //2MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//不使用上拉电阻
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIO

	//引脚 PE13,PE14,PE15 输出高电平，熄灭LED
	GPIO_SetBits(GPIOE,GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15);
}


void led_flash(void)
{
//	uint16_t led[] = {GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15,
//						GPIO_Pin_13,GPIO_Pin_14,GPIO_Pin_15};
//						
	GPIO_ToggleBits(GPIOE,GPIO_Pin_13);
	
	return;
}



int main(void)
{
	uint8_t recv; 
	uint8_t runed = 0;
	
	//1.配置中断分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	led_init();
	//systick_config();
	delay_Init();

	//usart1_init(115200);
	uart1_init();
	
	Debug_printf("begin sm2 test...2022\n\n\n");
//	UART1_TX_String((uint8_t *)"begin sm2 test...2022\n",18);
	
	sm2_test();
	while(1)
	{		
//		recv = UART1_Get_Ch();
//		if((recv >= '0') && (recv <= '9'))
//		{
//			if(runed == 0)
//			{
//				Debug_printf("\n20220422 run sm2_test()\n");
//			//	sm2_test();
//				runed = 1;
//			}
//		}
//		
//		Debug_printf("\n20220422-recv is 0x%x \n",recv);
	}	
}




#if 0

int main(void)
{
	uint8_t recv; 
	uint8_t runed = 0;
	
	
	led_init();
	systick_config();
	
	usart1_init(115200);
	
	Debug_printf("begin sm2 test...\n");
	UART1_TX_String((uint8_t *)"begin sm2 test...2022\n",18);
	sm2_test();
	do
	{
//		recv = UART1_Get_Ch();
//		if((recv >= '0') && (recv <= '9'))
//		{
//			if(runed == 0)
//			{
//				Debug_printf("\n20220422 run sm2_test()\n");
//				sm2_test();
//				runed = 1;
//			}
//		}
//		Debug_printf("\n20220422-recv is %c \n",recv);
//		UART1_TX_String((uint8_t *)"\n20220422-recv\n",15);
	}while(1);
	
	

	Debug_printf("\n``````````````````this is end```````````````````````\n");
		
	while(1);  //单片机不能返回，只能死循环
	
	return 0;
}

void exit(int code)
{
	
}

#endif
