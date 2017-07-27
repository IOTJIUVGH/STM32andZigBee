#include "usart.h"
#include "stm32f10x.h"
#include "Usart_Data_analyze.h"
#include <string.h>
u16 cnt=0;
u8 getdata,buf[100];
u8 flag=0;
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}


//////////////////////////////////////////////////////////////////

void Uart1_PutChar(u8 ch)
{
  USART1->DR=ch;
  while((USART1->SR&0X40)==0);//�ȴ����ͽ���
}

//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������
//CHECK OK
//091209
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<14;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRH&=0XFFFFF00F; 
	GPIOA->CRH|=0X000008B0;//IO״̬����PA9,PA10
		  
	RCC->APB2RSTR|=1<<14;   //��λ����1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=mantissa; // ����������	 
	USART1->CR1|=0X200C;  //1λֹͣ,��У��λ.

	
//	USART1->CR1|=1<<8;    //PE�ж�ʹ��
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
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
