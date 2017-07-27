#include "stm32f10x.h"
#include "usart.h"
u8 TIMER3_flag=0;
u8 USART_falg=0;

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

void LedToggle(u8 ch)
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

void TIM3_IRQHandler(void)
{ 		    		  			    
	
	if(TIM3->SR&0X0001)//溢出中断
	{
		if(USART_falg==1)
		{
			T--;
			if(T==0)
			{
				TIMER3_flag=1;
				USART_falg=0;
			}  
		
//		TIM3->PSC=7199; 	
		}				   
			TIM3->SR&=~(1<<0);//清除中断标志位 	    
	}
}
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;	//TIM3时钟使能    
 	TIM3->ARR=arr;  	//设定计数器自动重装值 
	TIM3->PSC=psc;  	//预分频器7200,得到10Khz的计数时钟		  
	TIM3->DIER|=1<<0;   //允许更新中断	 
	TIM3->CR1|=0x01;    //使能定时器3
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
   NVIC->ISER[0] |=1<<29; 
	
}
