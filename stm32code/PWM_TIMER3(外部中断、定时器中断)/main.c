#include "stm32f10x.h"
#include "rcc.h"
#include "adc.h"
#include "usart.h"
#include "pwm.h"
#include "exti.h"
int main()
{
	Init_SyStemClock();
	
	uart_init(72,9600);
	USART1_NVIC_Init();
	RCC->APB2ENR |= 0x0001<<3;	// enable IOPB 
	GPIOB->CRH &= ~0xffffffff;	// clean 
	GPIOB->CRH |= 0x00000003;    // set PB8 
	
//		GPIOB->BSRR&=~(0x01000100);
//			GPIOB->BSRR|=0x00000001<<8;	
	PWM_TIM3_CH1_Init(900,0);
	TIM3->CCR1 =100;
	
	TIMER3_UP__NVIC_Init();
	
	//Íâ²¿ÖÐ¶Ï
	EXTIX_Init();
	exti0_UP__NVIC_Init();
	exti1_UP__NVIC_Init();
//	NVIC->ISER[0] |=1<<8; 
//	NVIC->ISER[0] |=1<<9; 
//	NVIC->ISER[0] |=1<<10; 
//	NVIC->ISER[0] |=1<<23; 
//	NVIC->ISER[1] |=1<<8; 
	while(1)
	{
		//printf("11");
		printf("%s",buf);

	}

}

