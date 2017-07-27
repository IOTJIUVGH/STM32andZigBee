#include "usart.h"

u16 g_URT1_RxLen = 0;
u16 g_URT1_TxLen = 0;
u8 g_URT1_RxBuf[50] = {0};
u8 g_URT1_TxBuf[50];
u8 m_URT1_TxNum = 1;
u8 g_URT1_RxFlag = 0;
u8 g_URT1_TxEn = 0;

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
	while((USART1->SR&0X40) == 0){}//�ȴ����ͽ���
    USART1->DR = ch;
	g_URT1_TxEn = 1;
	USART1->CR1 |= 1<<6;    //��������ж�ʹ��
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

//	USART1->CR1|=1<<6;    //��������ж�ʹ��
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
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
����1�жϺ���
�����ж�   �����ж�
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

