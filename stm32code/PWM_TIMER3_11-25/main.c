#include "stm32f10x.h"
#include "rcc.h"
#include "adc.h"
#include "usart.h"
#include "pwm.h"

int main()
{
	float temp;
	u16 adcx;
	
	Init_SyStemClock();
	
	//uart_init(72,9600);
	//USART1_NVIC_Init();
	
	PWM_TIM3_CH1_Init(999,9);
	PWM_TIM3_Init_CH2(999,9);
		printf("adfsadf");
	//Uart1_PutChar('a');
//	printf("adfsadf");
//	ADC_GPIO_config();
//	ADC_config();
	TIM3->CCR1 =100;
//	printintputdata();
	while(1)
	{
		
		//printf("GO");
//	TIM3->CCR1 +=1;
//	TIM3->CCR2 +=2;	
//		TIM3->CCR1 -=100;
//		TIM3->CCR2 =20;	
		//adcx=Get_Adc_Average(0x0A,10);
//		temp=(float)adcx*(3.3/4096);
//		adcx=temp;
	}

}

