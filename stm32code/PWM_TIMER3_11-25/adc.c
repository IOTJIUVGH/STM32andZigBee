#include "stm32f10x.h"
#include "delay.h"

void ADC_GPIO_config()
{
		RCC->APB2ENR |=0x0001<<4;//enable GPIOC
	
		GPIOC->CRL |=0xffff0000;//ģ������ģʽPC0,PC1��PC2��PC3
}

void ADC_config()
{
	RCC->APB2ENR |=0x0001<<9;//enable ADC1 timer
	RCC->APB2RSTR |=0x0001<<9;//��λADC1��ʱ�� 
	RCC->APB2RSTR &=~(0x0001<<9);//ADC1��λ����
	
	RCC->CFGR &=~(0x0003<<14);//ADCԤ��Ƶ����
	RCC->CFGR |=0x0002<<14;//����ADCԤ��ƵΪ����Ƶ
  
	ADC1->CR1 &=0xf0ffff;//����ģʽ����
	ADC1->CR1|=0x0000000<<16; //��������ģʽ  
  ADC1->CR1 |=1<<8;   //ɨ��ģʽ 
  ADC1->CR2&=~(1<<1);    //����ת��ģʽ
	
  ADC1->CR2&=~(7<<17);  
  ADC1->CR2|=7<<17;    //ѡ����������ͨ����ת�����ⲿ�¼�ΪSWSTART 
	ADC1->CR2|=1<<20;  //�����ⲿ����
	
  ADC1->CR2 &=~(1<<11);  //�Ҷ���  
     
  ADC1->SQR1 &=~(0XF<<20);   
	ADC1->SQR1 |=0<<20;     //��1��ת���ڹ��������� Ҳ����ֻת����������1����˴���4��	
     
 
   ADC1->SMPR1&=~(0x000fff<<0);  //����ʱ�����
   ADC1->SMPR2|=0x000fff<<0;     //ͨ��10/11/12/13 Ϊ239.5����
     
   ADC1->CR2|=1<<0; //����ADת����     
   ADC1->CR2|=1<<3; //ʹ�ܸ�λУ׼    
   while(ADC1->CR2&1<<3); //�ȴ�У׼����         
          
   ADC1->CR2|=1<<2;      //����ADУ׼     
   while(ADC1->CR2&1<<2);  //�ȴ�У׼����
}

u16 Get_Adc(u8 ch)   
 {
              
     ADC1->SQR3&=0XFFFFFFE0;//��������1 ͨ��ch
     ADC1->SQR3|=ch;                          
     ADC1->CR2|=1<<22;//��������ת��ͨ��
     while(!(ADC1->SR&1<<1));   //�ȴ�ת������        
     return ADC1->DR;      //����ADCֵ  
 }
 
 u16 Get_Adc_Average(u8 ch,u8 times)
 {
     u32 temp_val=0;
     u8 t;
     for(t=0;t<times;t++)
     {
        temp_val+=Get_Adc(ch);
				Delayms(5);
     }
     return temp_val/times;
 } 