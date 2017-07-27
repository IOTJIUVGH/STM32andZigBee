#include "stm32f10x.h"

void PWM_TIM3_CH1_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR |=1<<1;//ʹ��TIM3ʱ��
	//RCC->APB2ENR |=1<<2;//ʹ��GPIOA
	
	GPIOA->CRL &=0xf0ffffff;//PA6����
	GPIOA->CRL |=0x0b000000;//PA6���ù����������
	GPIOA->ODR |=1<<6;//PA6����
	
	TIM3->ARR=arr;//�趨�������Զ���װֵ��PWMƵ�ʣ�
	TIM3->PSC=psc;//Ԥ��Ƶ��0Ϊ����Ƶ
	
	TIM3->CCMR1 |=7<<4;//PWM2ģʽ
	TIM3->CCMR1 |=1<<3;//Ԥװ��ʹ��
	
	TIM3->CCER  |= 1 << 0;//���ʹ�����ã��ǲ���ʹ��
   
    TIM3->DIER|=1<<0;   //��������ж�				
	TIM3->DIER|=1<<6;   //�������ж�
	
	TIM3->CR1 = 0X0080;//�Զ���װ��Ԥװ��ʹ��
    TIM3->CR1 |= 0X01;//ʹ�ܶ�ʱ��3

}

void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//����ж�
	{
		TIM3->CCR1 =500;			   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}

void TIMER3_UP__NVIC_Init()
{
//	SCB->AIRCR &=0x05faf8ff;
//	SCB->AIRCR |=0x05fa0400;
//	
//	NVIC->ISER[1] |=0x19;
//  NVIC->IP[37]=0x30;
		
	 SCB->AIRCR |= 0x5 << 8; 
    NVIC->IP[29] |= 0;   
      
    	printf("TT");
    NVIC->ISER[0] |=1<<29; 
	printf("AA");
}

