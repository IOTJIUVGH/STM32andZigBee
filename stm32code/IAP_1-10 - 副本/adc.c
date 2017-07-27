#include "stm32f10x.h"
#include "delay.h"
#include "adc.h"
#include "usart.h"

float temp[20],val,max,min;
u16 adcx;

void ADC_GPIO_config()
{
		RCC->APB2ENR |=0x0001<<2;//enable GPIOA
	
		GPIOC->CRL |=0x000fff00;//ģ������ģʽPA0,PA1
}

void ADC_config()
{
	RCC->APB2ENR |=0x0001<<9;//enable ADC1 timer
	RCC->APB2RSTR |=0x0001<<9;//��λADC1��ʱ�� 
	RCC->APB2RSTR &=~(0x0001<<9);//ADC1��λ����
	
	RCC->CFGR &=~(0x0003<<14);//ADCԤ��Ƶ����
	RCC->CFGR |=0x0003<<14;//����ADCԤ��Ƶ�˷�Ƶ
  
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
     
 
   ADC1->SMPR2&=~(0x01ff03f<<0);  //����ʱ�����
   ADC1->SMPR2|=0x01ff03f<<0;     //ͨ��1/2/5/6/7 Ϊ239.5����
     
   ADC1->CR2|=1<<0; //����ADת����     
   ADC1->CR2|=1<<3; //ʹ�ܸ�λУ׼    
   while(ADC1->CR2&1<<3); //�ȴ�У׼����         
          
   ADC1->CR2|=1<<2;      //����ADУ׼     
   while(ADC1->CR2&1<<2);  //�ȴ�У׼����
}

u16 Get_Adc(u8 ch)   
 {
   u16 a;           
     
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
     }

     return temp_val/times;
} 
 
float Analyze_Adc_U_Data()
{
	u8 i;
	float sub=0;
	
		for(i=0;i<100;i++)
		{
			adcx=Get_Adc_Average(1,10);
			temp[i]=(float)adcx*(3.3/4096);
		
		}
		max=min=temp[0];
	
		for(i=0;i<100;i++)
		{
			if(temp[i]>max)
			{
				max=temp[i];
			}
		}
	
		for(i=0;i<100;i++)
		{
			if(temp[i]<min)
			{
				min=temp[i];
			}
		}

		
		val=140.1234*(max-min);	

	return val;
		
		
}


float Analyze_Adc_I_Data()
{
	u8 i;
	float sub=0;
	
		for(i=0;i<100;i++)
		{
			adcx=Get_Adc_Average(0,10);
			temp[i]=(float)adcx*(3.3/4096);
		
		}
		max=min=temp[0];
	
		for(i=0;i<100;i++)
		{
			if(temp[i]>max)
			{
				max=temp[i];
			}
		}
	
		for(i=0;i<100;i++)
		{
			if(temp[i]<min)
			{
				min=temp[i];
			}
		}

		
		val=0.7115*(max-min);	

	return val;
		
		
}