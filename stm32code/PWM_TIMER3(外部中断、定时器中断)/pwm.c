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
   
    TIM3->DIER|=1<<0;   //允许更新中断				
	TIM3->DIER|=1<<6;   //允许触发中断
	
	TIM3->CR1 = 0X0080;//自动重装载预装载使能
    TIM3->CR1 |= 0X01;//使能定时器3

}

void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{
		TIM3->CCR1 =500;			   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}

void TIMER3_UP__NVIC_Init()
{
//	SCB->AIRCR &=0x05faf8ff;
//	SCB->AIRCR |=0x05fa0400;
//	
//	NVIC->ISER[1] |=0x19;
//  NVIC->IP[37]=0x30;
		
	 SCB->AIRCR |= 0x5 << 8; 
    NVIC->IP[29] |= 0;   
      
    	printf("TT");
    NVIC->ISER[0] |=1<<29; 
	printf("AA");
}

