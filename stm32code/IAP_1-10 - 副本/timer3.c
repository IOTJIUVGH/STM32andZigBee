#include "stm32f10x.h"
u8 flag=0;
void TIM3_IRQHandler(void)
{ 		    		  			    
	
	if(TIM3->SR&0X0001)//����ж�
	{
		
		flag++;
		TIM3->ARR=4999;  	//�趨�������Զ���װֵ//�պ�1ms    
//		TIM3->PSC=7199; 	
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;	//TIM3ʱ��ʹ��    
 	TIM3->ARR=arr;  	//�趨�������Զ���װֵ//�պ�1ms    
	TIM3->PSC=psc;  	//Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��		  
	TIM3->DIER|=1<<0;   //��������ж�	  
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
  									 
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
	
}
	
