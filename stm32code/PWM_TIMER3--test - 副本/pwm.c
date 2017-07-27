#include "stm32f10x.h"

void PWM_TIM3_CH1_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR |=1<<1;//ʹ��TIM3ʱ��
	//RCC->APB2ENR |=1<<2;//ʹ��GPIOA
	
	GPIOA->CRL &=0xf0ffffff;//PA6����
	GPIOA->CRL |=0x0b000000;//PA6���ù����������
	GPIOA->ODR |=1<<6;//PA6����
	
	TIM3->ARR=arr;//�趨�������Զ���װֵ��PWMƵ�ʣ�
	TIM3->PSC=psc;//Ԥ��Ƶ��0Ϊ����Ƶ
	
	TIM3->CCMR1 |=7<<4;//PWM2ģʽ
	TIM3->CCMR1 |=1<<3;//Ԥװ��ʹ��
	
	TIM3->CCER  |= 1 << 0;//���ʹ�����ã��ǲ���ʹ��
   
    TIM3->CR1 = 0X0080;//�Զ���װ��Ԥװ��ʹ��
    TIM3->CR1 |= 0X01;//ʹ�ܶ�ʱ��3

}

void PWM_TIM3_Init_CH2(u16 arr,u16 psc)
{
	RCC->APB1ENR |=1<<1;//ʹ��TIM3ʱ��
	//RCC->APB2ENR |=1<<2;//ʹ��GPIOA
	
	GPIOA->CRL &=0x0fffffff;//PA7����
	GPIOA->CRL |=0xb0000000;//PA7���ù����������
	GPIOA->ODR |=1<<7;//PA7����
	
	TIM3->ARR=arr;//�趨�������Զ���װֵ��PWMƵ�ʣ�
	TIM3->PSC=psc;//Ԥ��Ƶ��0Ϊ����Ƶ
	
	TIM3->CCMR1 |=7<<12;//PWM2ģʽ
	TIM3->CCR1 |=1<<11;//Ԥװ��ʹ��
	
	TIM3->CCER  |= 1 << 4;//���ʹ�����ã��ǲ���ʹ��
   
    TIM3->CR1 = 0X0080;//�Զ���װ��Ԥװ��ʹ��
    TIM3->CR1 |= 0X01;//ʹ�ܶ�ʱ��3

}