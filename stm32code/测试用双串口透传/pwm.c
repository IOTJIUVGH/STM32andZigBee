#include "stm32f10x.h"

void PWM_TIM3_CH1_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR |=1<<1;//使能TIM3时钟
	//RCC->APB2ENR |=1<<2;//使能GPIOA
	
	GPIOA->CRL &=0xf0ffffff;//PA6清零
	GPIOA->CRL |=0x0b000000;//PA6复用功能推挽输出
	GPIOA->ODR |=1<<6;//PA6上拉
	
	TIM3->ARR=arr;//设定计数器自动重装值（PWM频率）
	TIM3->PSC=psc;//预分频器0为不分频
	
	TIM3->CCMR1 |=7<<4;//PWM2模式
	TIM3->CCMR1 |=1<<3;//预装栽使能
	
	TIM3->CCER  |= 1 << 0;//输出使能设置，非捕获使能
   
    TIM3->CR1 = 0X0080;//自动重装载预装载使能
    TIM3->CR1 |= 0X01;//使能定时器3

}

void PWM_TIM3_Init_CH2(u16 arr,u16 psc)
{
	RCC->APB1ENR |=1<<1;//使能TIM3时钟
	//RCC->APB2ENR |=1<<2;//使能GPIOA
	
	GPIOA->CRL &=0x0fffffff;//PA7清零
	GPIOA->CRL |=0xb0000000;//PA7复用功能推挽输出
	GPIOA->ODR |=1<<7;//PA7上拉
	
	TIM3->ARR=arr;//设定计数器自动重装值（PWM频率）
	TIM3->PSC=psc;//预分频器0为不分频
	
	TIM3->CCMR1 |=7<<12;//PWM2模式
	TIM3->CCR1 |=1<<11;//预装栽使能
	
	TIM3->CCER  |= 1 << 4;//输出使能设置，非捕获使能
   
    TIM3->CR1 = 0X0080;//自动重装载预装载使能
    TIM3->CR1 |= 0X01;//使能定时器3

}