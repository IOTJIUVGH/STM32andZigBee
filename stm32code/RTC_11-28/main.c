#include "stm32f10x.h"
#include "rcc.h"
#include "adc.h"
#include "usart.h"
	int a;
	float b;
	
int main()
{
	u16 i,j;
	
	Init_SyStemClock();
	
//	uart_init(72,9600);
//	ADC_GPIO_config();
//	ADC_config();
	
	RCC->APB2ENR|=1<<2;
	GPIOA->CRH |= 0X03<<8;
	while(1)
	{
		
			GPIOA->BSRR |= 0x01<<10;
		
			for(i=0;i<1000;i++)
				for(j=0;j<1000;j++){}
					
			GPIOA->BSRR |= 0x01<<26;
					
			for(i=0;i<1000;i++)
				for(j=0;j<1000;j++){}
		
//		a=Get_Adc_Average(0,10);
//		b=(float)a*(3.3/4096);
//		printf("B%.4f",b);
	//	printf("**********\n\n");
//	Analyze_AdcData(0);
	}

}
	