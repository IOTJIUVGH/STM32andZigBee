#include "stm32f10x.h"
#include "delay.h"

//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{
	//Delayms(10);//����
	printf("exti0");
	EXTI->PR=1<<0;  //���LINE0�ϵ��жϱ�־λ  
}
void EXTI1_IRQHandler(void)
{
	printf("exti1");
	EXTI->PR=1<<1;  //���LINE1�ϵ��жϱ�־λ  
}


void EXTIX_Init(void)
{
	RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��
	GPIOA->CRL&=0XF0000000;//PA0��PA1���ó����� 
	GPIOA->CRL|=0X08888888; 

	AFIO->EXTICR[0]&=~(0x000F<<0);   //�Ըö˿����ԭ���ж������ã������һ�£�ȷ����ʼ��Ϊ�ɾ���״̬��
	AFIO->EXTICR[0]&=~(0x000F<<1);   //�Ըö˿����ԭ���ж������ã������һ�£�ȷ����ʼ��Ϊ�ɾ���״̬��

	AFIO->EXTICR[0]|=0<<0;//EXTI.BITx=EXTI.BIT0ӳ�䵽GPIOA��0�ܽ���,�����ж������õ�PA0�ܽ���
	AFIO->EXTICR[0]|=0<<1;//EXTI.BITx=EXTI.BIT0ӳ�䵽GPIOA��1�ܽ���,�����ж������õ�PA0�ܽ���

	EXTI->IMR|=1<<0; //����PA0�ϵ��жϣ�ʵ����ͬʱҲ������PB0��PC0��PD0..PG0�ϵ��ж��ߣ�ϸ������ֲᣩ
 	EXTI->IMR|=1<<1; //����PA1�ϵ��жϣ�ʵ����ͬʱҲ������PB1��PC1��PD1..PG1�ϵ��ж��ߣ�ϸ������ֲᣩ

	EXTI->FTSR|=1<<0; //����PA0���½��ش����ж�
	EXTI->FTSR|=1<<1; //����PA1���½��ش����ж�

	
}

void exti0_UP__NVIC_Init()
{	
	 SCB->AIRCR |= 0x5 << 8; 
    NVIC->IP[29] |= 0;   
      
    	printf("TT");
    NVIC->ISER[0] |=1<<6; 
	printf("AA");
}
void exti1_UP__NVIC_Init()
{	
	 SCB->AIRCR |= 0x5 << 8; 
    NVIC->IP[29] |= 1;   
      
    	printf("TOT");
    NVIC->ISER[0] |=1<<7; 
	printf("AA");
}
