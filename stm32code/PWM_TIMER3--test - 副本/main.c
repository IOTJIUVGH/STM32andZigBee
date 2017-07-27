#include "stm32f10x.h"
#include "rcc.h"
#include "adc.h"
#include "usart.h"
#include "pwm.h"
#include "crc.h"
#include <stdlib.h>
#include <stdio.h>
#include "timer.h"

void Delayms(u16 ms);
int main()
{
	u8 a[10]={0x12,0x23,0x34,0x45,0x56,0x67,0x78,0x89,0x90,0xff},i=0;
	
	Init_SyStemClock();
	uart_init(72,9600);
	USART1_NVIC_Init();
	TIM3_Int_Init(999,7199);
	TIMER3_UP__NVIC_Init();
	
	TIM4_Int_Init(9,7199);
	TIMER4_UP__NVIC_Init();
	//printf("GO");
	
	while(1)
	{
		if(TIMER4_flag==1)
		{
			u8 i=0;
			Delayms(100);
			
			for(i=0;i<cnt;i++)
			Uart1_PutChar(buf[i]);
			
			for(i=0;i<cnt;i++)
				{	
					buf[i]='\0';
				}
			cnt=0;
			TIMER4_flag=0;
		}
		
		if(TIMER3_flag==200)
		{
		  for(i=0;i<10;i++)
			Uart1_PutChar(a[i]);
			
		  TIMER3_flag=0;
		}
	}

}
void Delayms(u16 ms)
{
	unsigned int i = 0;
	while (ms--)
	{
		for (i = 0; i < 8000; ++i)
		{
			;
		}
	}
}

