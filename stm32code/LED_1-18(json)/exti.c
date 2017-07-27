#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "Usart_Data_analyze.h"
#include "rtc.h"

u8 debug_flag=0;
//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{
	
	Delayms(10);//����
	printf("exti0");
	debug_flag=1;
	EXTI->PR=1<<0;  //���LINE0�ϵ��жϱ�־λ  
}

void EXTIX_Init(void)
{
	RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��
	RCC->APB2ENR|=0x01;//ʹ��io����ʱ��		
	GPIOA->CRL&=0XFFFFFFF0;//PA0���ó����� 
	GPIOA->CRL|=0X00000008; 

	AFIO->EXTICR[0]&=~(0x000F<<0);   //�Ըö˿����ԭ���ж������ã������һ�£�ȷ����ʼ��Ϊ�ɾ���״̬��
	AFIO->EXTICR[0]|=0<<0;//EXTI.BITx=EXTI.BIT0ӳ�䵽GPIOA��0�ܽ���,�����ж������õ�PA0�ܽ���
	
	EXTI->IMR|=1<<0; //����PA0�ϵ��жϣ�ʵ����ͬʱҲ������PB0��PC0��PD0..PG0�ϵ��ж��ߣ�ϸ������ֲᣩ
	EXTI->FTSR|=1<<0; //����PA0���½��ش����ж�
}

void exti0_UP__NVIC_Init()
{	
	SCB->AIRCR |= 0x5 << 8; 
	NVIC->IP[29] |= 0;   
	NVIC->ISER[0] |=1<<6; 
	
}
void KEY_Init(void)
{
//	RCC->APB2ENR|=1<<0;     //��������ʱ��	   
//	AFIO->MAPR&=0XF8FFFFFF; //���MAPR��[26:24]
//	AFIO->MAPR|=0X02<<25;       //����jtagģʽ
	
	RCC->APB2ENR|=1<<3;     //ʹ��PORTBʱ��
	GPIOB->CRH&=0XFFFFF0FF;//PB10���ó����� 
	GPIOB->CRH|=0X00000800;
	GPIOB->ODR|=0x001<<10;
}
