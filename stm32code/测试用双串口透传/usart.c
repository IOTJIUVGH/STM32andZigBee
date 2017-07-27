#include "usart.h"
#include "stm32f10x.h"
#include "timer.h"

u16 cnt=0,Rx_cnt=0;
u8 getdata,buf[100]={0},Rx_buf[100]={0},T=0,T1=0;

//�ض���fputc���� 
//int fputc(int ch, FILE *f)
//{      
//	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
//	USART2->DR = (u8) ch;      
//	return ch;
//}

//end
//////////////////////////////////////////////////////////////////

void Uart1_PutChar(u8 ch)
{
  USART1->DR=ch;
  while((USART1->SR&0X40)==0);//�ȴ����ͽ���
}

void Uart2_PutChar(u8 ch)
{
  USART2->DR=ch;
  while((USART1->SR&0X40)==0);//�ȴ����ͽ���
}

void UART1_SendStr(char* s)
{
	s--;
	while(*s != '\0')
	{
		//while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); 
		Uart1_PutChar(*s);
		s++;
	}
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

void uart2_init(u32 pclk2,u32 bound)
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
	RCC->APB1ENR|=1<<17;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRL&=0XFFFF00FF; 
	GPIOA->CRL|=0X00008B00;//IO״̬����PA2,PA3
		  
	RCC->APB1RSTR|=1<<17;   //��λ����1
	RCC->APB1RSTR&=~(1<<17);//ֹͣ��λ	   	   
	//����������
 	USART2->BRR=mantissa; // ����������	 
	USART2->CR1|=0X200C;  //1λֹͣ,��У��λ.

	
//	USART1->CR1|=1<<8;    //PE�ж�ʹ��
	USART2->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	 	
}

void USART2_NVIC_Init()
{
	SCB->AIRCR &=0x05faf8ff;
	SCB->AIRCR |=0x05fa0400;
	
	NVIC->ISER[1] |=1<<6;
  NVIC->IP[38]=0x30;
}

void USART2_IRQHandler(void)
{
//	if( USART2 -> SR & 0x20 )
//			{
//				if(TIMER3_flag==0)
//				{
//						getdata = USART2 -> DR;
//					//Uart1_PutChar(getdata);
//						buf[cnt]=getdata;
//					cnt++;
//		
//					USART_falg=1;
//					T=100;
//				}
//		}
	//Uart1_PutChar('B');
	if( USART2 -> SR & 0x20 )
	{
		if(TIMER2_flag==0)
		{
			getdata = USART2 -> DR;
			//Uart1_PutChar(getdata);
			buf[cnt]=getdata;
			cnt++;
			USART2_falg=1;
				T1=100;
		}	
	}		
}

void USART1_NVIC_Init()
{
	SCB->AIRCR &=0x05faf8ff;
	SCB->AIRCR |=0x05fa0400;
	
	NVIC->ISER[1] |=1<<5;
  NVIC->IP[37]=0x30;
}

void USART1_IRQHandler( void )
{
	if( USART1 -> SR & 0x20)
	{
		if(TIMER3_flag==0)
		{	
			getdata = USART1 -> DR;
			//Uart1_PutChar(getdata);
				Rx_buf[Rx_cnt]=getdata;
				Rx_cnt++;
				USART_falg=1;
				T=100;
		}
		
	}
		
//	if( USART1 -> SR & 0x20 )
//	{
//		getdata = USART1-> DR;
//		//Uart1_PutChar(getdata);
//		Rx_buf[Rx_cnt]=getdata;
//		Rx_cnt++;
//	}		
}

