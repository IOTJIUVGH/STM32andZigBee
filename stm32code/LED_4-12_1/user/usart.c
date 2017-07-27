#include "usart.h"

u16 g_URT1_RxLen = 0;
u16 g_URT1_TxLen = 0;
u8 g_URT1_RxBuf[50] = {0};
u8 g_URT1_TxBuf[50];
u8 m_URT1_TxNum = 1;
u8 g_URT1_RxFlag = 0;
u8 g_URT1_TxEn = 0;

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
	while((USART1->SR&0X40) == 0){}//等待发送结束
    USART1->DR = ch;
	g_URT1_TxEn = 1;
	USART1->CR1 |= 1<<6;    //发送完成中断使能
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

//	USART1->CR1|=1<<6;    //发送完成中断使能
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
}


void USART1_NVIC_Init()
{
	SCB->AIRCR &=0x05faf8ff;
	SCB->AIRCR |=0x05fa0400;
	NVIC->IP[37]=0x30;
	NVIC->ISER[1] |=1<<5;
}

void over_time()
{
	g_URT1_RxFlag = 1;
	vtimer_KillTimer(VTIM1);
}

/*************
串口1中断函数
接收中断   发送中断
*************/

void USART1_IRQHandler(void)
{
	u8 temp;
	if(USART1->SR & 0x20)
	{
		if(g_URT1_RxFlag == 0)
		{
			g_URT1_RxBuf[g_URT1_RxLen] = USART1->DR;
			g_URT1_RxLen++;
			vtimer_SetTimer(VTIM1, 100, over_time);
		}
		else
			temp = USART1->DR;
			
		
	}
	
	if((USART1->SR & 0X40) && g_URT1_TxEn)
	{
		USART1->DR = g_URT1_TxBuf[m_URT1_TxNum];

		m_URT1_TxNum++;
		if(m_URT1_TxNum >= g_URT1_TxLen)
		{
			USART1->CR1 &= ~(1<<6);    
			m_URT1_TxNum = 1;
			g_URT1_TxEn = 0;
			g_URT1_TxLen = 0;
		}
	}

}

