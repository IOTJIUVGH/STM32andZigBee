#include "stm32f10x.h"
#include "usart.h"

Frame g_Data_Deal = {0,0,0,0};

void Init_Gpio(void)
{
	RCC->APB2ENR |=0x0001<<3;
	GPIOB->CRH &= ~0xffffffff;	// clean 
	GPIOB->CRH |= 0x00000003;    // set PB8 
}


void PWM_TIM1_Init()
{
	
	RCC->APB2ENR |=(0x0001<<2)|(0x0001<<11);
	
	GPIOA->CRH &= 0XFFFF0FFF;
	GPIOA->CRH |= 0X00009000;
	
	TIM1->CR1 |= 0x0001;
	TIM1->CCMR2 |= (0X0003 << 13);
	TIM1->PSC = 0X059f;//500hz
	TIM1->ARR = 0X0064;
	TIM1->CCER |= 0X3000;
	TIM1->CCR4 = 100;
	
	TIM1->BDTR |= (0X0001 << 15);

}

void pwmval(u16 pwm)
{
	static u16 pwm00=0;

	if(pwm != pwm00)
	{
		TIM1->BDTR &= ~(0X0001 << 15);
		TIM1->CCR4 = pwm;
		pwm00 = pwm;
		TIM1->EGR |= 0X0001;
		TIM1->BDTR |= 0X0001 << 15;
	}

}

void Led_Toggle(u8 ch)
{
	if(ch=='n')
	{
		GPIOB->BSRR&=~(0x01000100);
		GPIOB->BSRR|=0x00000001<<24;	
	}
	if(ch=='f')
	{
		GPIOB->BSRR&=~(0x01000100);
		GPIOB->BSRR|=0x00000001<<8;	
	}
}

