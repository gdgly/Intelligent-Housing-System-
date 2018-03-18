#include "pbdata.h"
void system_init(void)
{
//	Rcc_init();
	GPIO_Config();
	USART1_config();
	USART2_Config();
	NVIC_Configuration();
}
//ϵͳʱ�ӳ�ʼ��
void Rcc_init(void)
{
	//RCC_DeInit();//��λRCC
	RCC_HSEConfig(RCC_HSE_ON);//ʹ���ⲿʱ��
	RCC_HSICmd(DISABLE);//�ر��ڲ�ʱ��
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);//���໷9��Ƶ��ѡ���ⲿʱ��,72M
	RCC_PLLCmd(ENABLE);//ʹ�����໷
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//�ⲿʱ����Ϊϵͳʱ��Դ,8M
	RCC_HCLKConfig(RCC_SYSCLK_Div1);//AHBʱ�ӵ���ϵͳʱ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);		
}


/*����������������ϵͳSYSTICKʱ�Ӷ�ʱ����ȷ���øĶ� */
void delay_ms(uint16_t nms)
{
	uint32_t temp;
	SysTick->LOAD=9000*nms;
	SysTick->VAL=0x00;
	SysTick->CTRL=0x01;
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&(!(temp&(1<<16))));
	SysTick->CTRL=0x00;
	SysTick->VAL=0x00;
}

void delay_us(uint32_t nus)
{
	uint32_t temp;
	SysTick->LOAD=9*nus;
	SysTick->VAL=0x00;
	SysTick->CTRL=0x01;
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&(!(temp&(1<<16))));
	SysTick->CTRL=0x00;
	SysTick->VAL=0x00;
}

