#include "stm32f10x.h"


void	GpioInit()
{
	RCC->APB2ENR|=1<<3;      //PORTB       
		//           
   GPIOB->CRH&=~(0xffffffff);//PB8
  GPIOB->CRH|=0x00000003;//PB8     
}

void LedToggle(u8 flag)
{
		if(flag=='f')
		{
			GPIOB->BSRR&=~(0x00000001<<8);
			GPIOB->BSRR|=0x00000001<<8;
		}
		if(flag=='n')
		{
			GPIOB->BSRR&=~(0x00000001<<24);
			GPIOB->BSRR|=0x00000001<<24;
		
		}
}

void PWM_TIM3_CH1_Init(u16 arr,u16 psc)
{
	RCC->APB2ENR |=1<<11;//ʹ��TIM1ʱ��
	//RCC->APB2ENR |=1<<2;//ʹ��GPIOA
	
	GPIOB->CRH &=0xfffffff0;//PB8����
	GPIOB->CRH |=0x0000000b;//PB8���ù����������
	GPIOB->ODR |=1<<8;//PB8����
	
	TIM1->ARR=arr;//�趨�������Զ���װֵ��PWMƵ�ʣ�
	TIM1->PSC=psc;//Ԥ��Ƶ��0Ϊ����Ƶ
	
	TIM1->CCMR1 |=7<<4;//PWM2ģʽ
	TIM1->CCMR1 |=1<<3;//Ԥװ��ʹ��
	
	TIM1->CCER  |= 1 << 0;//���ʹ�����ã��ǲ���ʹ��
   
    TIM1->CR1 = 0X0080;//�Զ���װ��Ԥװ��ʹ��
    TIM1->CR1 |= 0X01;//ʹ�ܶ�ʱ��3

}

void pwmval(u16 num)
{

	TIM3->CCR1 =num;

}