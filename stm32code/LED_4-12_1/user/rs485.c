#include "rs485.h"	 
#include "delay.h"
  	  	
u8 g_RS485_RxBuf[7];  	//���ջ���,���7���ֽ�.
u8 g_RS485_RxCnt = 0;      //���յ������ݳ���
u8 g_RS485_RxFlag = 0;

void USART2_IRQHandler(void)
{
	u8 res;	 

	if(USART2->SR & (1<<5))//���յ�����
	{	 
		if(g_RS485_RxFlag == 0)
		{
			res = USART2->DR;
			g_RS485_RxBuf[g_RS485_RxCnt] = res;		//��¼���յ���ֵ
			g_RS485_RxCnt++;						//������������1 
			vtimer_SetTimer(VTIM7, 10, RS485_OverTim);
		}
		else
			res = USART2->DR;
	}  											 
} 

void RS485_OverTim()
{
	g_RS485_RxFlag = 1;
	vtimer_KillTimer(VTIM7);
}
									 
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void RS485_Init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   	//ʹ��PORTA��ʱ��  
  
 	GPIOA->CRL&=0XFFF0FFFF;	//IO״̬����
	GPIOA->CRL|=0X00030000;	//IO״̬����PA4	  
 
	
	GPIOA->CRL&=0XFFFF00FF;	//IO״̬����
	GPIOA->CRL|=0X00008B00;	//IO״̬����PA2\PA3	  

	RCC->APB1ENR|=1<<17;  	//ʹ�ܴ���ʱ�� 
		  
	RCC->APB1RSTR|=1<<17;   //��λ����2
	RCC->APB1RSTR&=~(1<<17);//ֹͣ��λ	   	   
	//����������
 	USART2->BRR=mantissa; 	// ����������	 
	USART2->CR1|=0X200C;  	//1λֹͣ,��У��λ.

	//ʹ�ܽ����ж�
	USART2->CR1|=1<<8;    	//PE�ж�ʹ��
	USART2->CR1|=1<<5;    	//���ջ������ǿ��ж�ʹ��	    	

	GPIOA->BSRR&=~(0x00100010);
	GPIOA->BSRR|=0x00000001<<20;	
}

void USART2_NVIC_INIT()
{
	SCB->AIRCR &=0x05faf8ff;
	SCB->AIRCR |=0x05fa0400;
	
	NVIC->ISER[1] |=1<<6;
	NVIC->IP[38]=0x30;
}
//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;

	GPIOA->BSRR&=~(0x00100010);
	GPIOA->BSRR|=0x00000001<<4;	
	
  	for(t=0;t<len;t++)		//ѭ����������
	{
		while((USART2->SR&0X40)==0);//�ȴ����ͽ���		  
		USART2->DR=buf[t];
	}	 
	while((USART2->SR & 0X40)==0);//�ȴ����ͽ���	  
	GPIOA->BSRR &= ~(0x00100010);
	GPIOA->BSRR |= 0x00000001<<20;	
}

