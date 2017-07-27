#include "stm32f10x.h"
#include "rcc.h"
#include "adc.h"
#include "usart.h"
#include "pwm.h"
#include "crc.h"

int main()
{
	u16 crc,p;
	u8 a[22]={0x7e,0xbd,0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 
						0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0xC1, 0x81,0x44};
	Init_SyStemClock();
	uart_init(72,9600);
	USART1_NVIC_Init();
						
	crc=Get_Crc16(a,22);
				printf("%x\n",crc);		
	RCC->APB2ENR |= 0x0001<<3;	// enable IOPB 
	GPIOB->CRH &= ~0xffffffff;	// clean 
	GPIOB->CRH |= 0x00000003;    // set PB8 
	
		GPIOB->BSRR&=~(0x01000100);
			GPIOB->BSRR|=0x00000001<<8;	
						
	 RCC->APB1RSTR |= 1<<27;        //??BKP???  
    RCC->APB1RSTR &= ~(1<<27);  
  
    RCC->APB1ENR|=1<<28;     //??????        
    RCC->APB1ENR|=1<<27;     //??BKP??     
   PWR->CR|=1<<8;//?????????
						
		BKP->DR1&=0x0000;
		BKP->DR1|=0x2000;	
		
			p=BKP->DR1;		
	while(1)
	{

		printf("%x\r\n",BKP->DR1);	
//		printf("%s",buf);
//		printf("\"");

	}

}

