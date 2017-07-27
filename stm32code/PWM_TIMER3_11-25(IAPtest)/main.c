#include "stm32f10x.h"
#include "usart.h"

void Init_SyStemClock(void);

void Init_SyStemClock(void)
{
	RCC->CR |= 0x0001;								// HSION 内部8MHz时钟开启
	while(!(RCC->CR & (0x0001<<1)));
	RCC->CFGR |=  0x0001<<16;					// set PLLSRC HSE作为时钟源
	RCC->CFGR |= 0x0007<<18;					// PLLMUL: PLL倍频系数 9
	RCC->CR |= 0x0001<<24;						//PLLON: PLL使能
	while(!(RCC->CR & (0x0001<<25)));	
	RCC->CFGR |= 0x0001<<1;						//SW:系统时钟切换 PLL输出作为系统时钟源
	while(!(RCC->CFGR & (0x0001<<3)));
}

int main()
{
	SCB->VTOR = 0X08000000|0x3000;
	
	Init_SyStemClock();
	
	uart_init(72,9600);
	USART1_NVIC_Init();
	printf("adfsadf");
	while(1)
	{
		
		printf("GO");

	}

}

