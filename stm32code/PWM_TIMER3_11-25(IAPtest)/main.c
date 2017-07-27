#include "stm32f10x.h"
#include "usart.h"

void Init_SyStemClock(void);

void Init_SyStemClock(void)
{
	RCC->CR |= 0x0001;								// HSION �ڲ�8MHzʱ�ӿ���
	while(!(RCC->CR & (0x0001<<1)));
	RCC->CFGR |=  0x0001<<16;					// set PLLSRC HSE��Ϊʱ��Դ
	RCC->CFGR |= 0x0007<<18;					// PLLMUL: PLL��Ƶϵ�� 9
	RCC->CR |= 0x0001<<24;						//PLLON: PLLʹ��
	while(!(RCC->CR & (0x0001<<25)));	
	RCC->CFGR |= 0x0001<<1;						//SW:ϵͳʱ���л� PLL�����Ϊϵͳʱ��Դ
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

