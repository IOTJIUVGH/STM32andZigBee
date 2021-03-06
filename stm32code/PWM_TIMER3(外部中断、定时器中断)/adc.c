#include "stm32f10x.h"
#include "delay.h"

void ADC_GPIO_config()
{
		RCC->APB2ENR |=0x0001<<4;//enable GPIOC
	
		GPIOC->CRL |=0xffff0000;//模拟输入模式PC0,PC1，PC2，PC3
}

void ADC_config()
{
	RCC->APB2ENR |=0x0001<<9;//enable ADC1 timer
	RCC->APB2RSTR |=0x0001<<9;//复位ADC1定时器 
	RCC->APB2RSTR &=~(0x0001<<9);//ADC1复位结束
	
	RCC->CFGR &=~(0x0003<<14);//ADC预分频清零
	RCC->CFGR |=0x0002<<14;//配置ADC预分频为六分频
  
	ADC1->CR1 &=0xf0ffff;//工作模式清零
	ADC1->CR1|=0x0000000<<16; //独立工作模式  
  ADC1->CR1 |=1<<8;   //扫描模式 
  ADC1->CR2&=~(1<<1);    //单次转换模式
	
  ADC1->CR2&=~(7<<17);  
  ADC1->CR2|=7<<17;    //选择启动规则通道组转换的外部事件为SWSTART 
	ADC1->CR2|=1<<20;  //启用外部触发
	
  ADC1->CR2 &=~(1<<11);  //右对齐  
     
  ADC1->SQR1 &=~(0XF<<20);   
	ADC1->SQR1 |=0<<20;     //（1个转换在规则序列中 也就是只转换规则序列1）舜κ�4个	
     
 
   ADC1->SMPR1&=~(0x000fff<<0);  //采样时间清空
   ADC1->SMPR2|=0x000fff<<0;     //通道10/11/12/13 为239.5周期
     
   ADC1->CR2|=1<<0; //开启AD转换器     
   ADC1->CR2|=1<<3; //使能复位校准    
   while(ADC1->CR2&1<<3); //等待校准结束         
          
   ADC1->CR2|=1<<2;      //开启AD校准     
   while(ADC1->CR2&1<<2);  //等待校准结束
}

u16 Get_Adc(u8 ch)   
 {
              
     ADC1->SQR3&=0XFFFFFFE0;//规则序列1 通道ch
     ADC1->SQR3|=ch;                          
     ADC1->CR2|=1<<22;//启动规则转换通道
     while(!(ADC1->SR&1<<1));   //等待转换结束        
     return ADC1->DR;      //返回ADC值  
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