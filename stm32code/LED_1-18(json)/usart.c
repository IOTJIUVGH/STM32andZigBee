#include "usart.h"
#include "stm32f10x.h"
#include "Usart_Data_analyze.h"
#include <string.h>
u16 cnt=0;
u8 getdata,buf[100];
u8 flag=0;
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}


//////////////////////////////////////////////////////////////////

void Uart1_PutChar(u8 ch)
{
  USART1->DR=ch;
  while((USART1->SR&0X40)==0);//等待发送结束
}

//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率
//CHECK OK
//091209
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	GPIOA->CRH&=0XFFFFF00F; 
	GPIOA->CRH|=0X000008B0;//IO状态设置PA9,PA10
		  
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X200C;  //1位停止,无校验位.

	
//	USART1->CR1|=1<<8;    //PE中断使能
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
}


void USART1_NVIC_Init()
{
	SCB->AIRCR &=0x05faf8ff;
	SCB->AIRCR |=0x05fa0400;
	NVIC->IP[29] |= 0; 
	//NVIC->IP[37]=0x30;
	NVIC->ISER[1] |=1<<5;
  
}

void USART1_IRQHandler( void )
{
	
	if( USART1 -> SR & 0x20 )
	{
		getdata = USART1 -> DR;
		//Uart1_PutChar(getdata);
		if(getdata=='}')
			flag=1;
		buf[cnt]=getdata;
		cnt++;
	}

}
