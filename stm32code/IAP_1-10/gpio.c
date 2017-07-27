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
	RCC->APB2ENR |=1<<11;//使能TIM1时钟
	//RCC->APB2ENR |=1<<2;//使能GPIOA
	
	GPIOB->CRH &=0xfffffff0;//PB8清零
	GPIOB->CRH |=0x0000000b;//PB8复用功能推挽输出
	GPIOB->ODR |=1<<8;//PB8上拉
	
	TIM1->ARR=arr;//设定计数器自动重装值（PWM频率）
	TIM1->PSC=psc;//预分频器0为不分频
	
	TIM1->CCMR1 |=7<<4;//PWM2模式
	TIM1->CCMR1 |=1<<3;//预装栽使能
	
	TIM1->CCER  |= 1 << 0;//输出使能设置，非捕获使能
   
    TIM1->CR1 = 0X0080;//自动重装载预装载使能
    TIM1->CR1 |= 0X01;//使能定时器3

}

void pwmval(u16 num)
{

	TIM3->CCR1 =num;

}