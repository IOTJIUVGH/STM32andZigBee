#include "stm32f10x.h"
#include "rcc.h"
#include "usart.h"
#include "delay.h"
#include "gprs.h"
#include "timer.h"
int main()
{	
	u8 i,a[5]={0x42,0x44,0x45,0x46,0x48};
	Init_SyStemClock();
	uart_init(72,9600);
	USART1_NVIC_Init();
	uart2_init(72,9600*2);
	USART2_NVIC_Init();
		
	TIM3_Int_Init(9,7199);
	TIMER3_UP__NVIC_Init();
	Uart1_PutChar('B');
	//GSM_Init();
	TIM2_Int_Init(9,7199);
	TIMER2_UP__NVIC_Init();
//Uart2_PutChar('E');
	
	while(1)
	{
		if(TIMER3_flag==1)
		{
		//	u8 i;
			for(i=0;i<Rx_cnt;i++)
			{//Uart1_PutChar('D');
				Delayms(5);
			Uart2_PutChar(Rx_buf[i]);
			}
		for(i=0;i<Rx_cnt;i++)
		Rx_buf[i]=0;//»º´æ
	  
			Rx_cnt=0;
		TIMER3_flag=0;
	//	Send_Str(a,5);
		}
		
		if(TIMER2_flag==1)
		{		
			for(i=0;i<cnt;i++)	
			Uart1_PutChar(buf[i]);
			
			clear_buffer();
			TIMER2_flag=0;
			cnt=0;
		}

	}

}

