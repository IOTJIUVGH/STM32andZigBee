#include "stm32f10x.h"
#include "delay.h"

void ADC_GPIO_config()
{
		RCC->APB2ENR |=0x0001<<4;//enable GPIOC
	
		GPIOC->CRL |=0xffff0000;//Ä£ÄâÊäÈëÄ£Ê½PC0,PC1£¬PC2£¬PC3
}

void ADC_config()
{
	RCC->APB2ENR |=0x0001<<9;//enable ADC1 timer
	RCC->APB2RSTR |=0x0001<<9;//¸´Î»ADC1¶¨Ê±Æ÷ 
	RCC->APB2RSTR &=~(0x0001<<9);//ADC1¸´Î»½áÊø
	
	RCC->CFGR &=~(0x0003<<14);//ADCÔ¤·ÖÆµÇåÁã
	RCC->CFGR |=0x0002<<14;//ÅäÖÃADCÔ¤·ÖÆµÎªÁù·ÖÆµ
  
	ADC1->CR1 &=0xf0ffff;//¹¤×÷Ä£Ê½ÇåÁã
	ADC1->CR1|=0x0000000<<16; //¶ÀÁ¢¹¤×÷Ä£Ê½  
  ADC1->CR1 |=1<<8;   //É¨ÃèÄ£Ê½ 
  ADC1->CR2&=~(1<<1);    //µ¥´Î×ª»»Ä£Ê½
	
  ADC1->CR2&=~(7<<17);  
  ADC1->CR2|=7<<17;    //Ñ¡ÔñÆô¶¯¹æÔòÍ¨µÀ×é×ª»»µÄÍâ²¿ÊÂ¼þÎªSWSTART 
	ADC1->CR2|=1<<20;  //ÆôÓÃÍâ²¿´¥·¢
	
  ADC1->CR2 &=~(1<<11);  //ÓÒ¶ÔÆë  
     
  ADC1->SQR1 &=~(0XF<<20);   
	ADC1->SQR1 |=0<<20;     //£¨1¸ö×ª»»ÔÚ¹æÔòÐòÁÐÖÐ Ò²¾ÍÊÇÖ»×ª»»¹æÔòÐòÁÐ1£©¬´Ë´¦ÊÇ4¸ö	
     
 
   ADC1->SMPR1&=~(0x000fff<<0);  //²ÉÑùÊ±¼äÇå¿Õ
   ADC1->SMPR2|=0x000fff<<0;     //Í¨µÀ10/11/12/13 Îª239.5ÖÜÆÚ
     
   ADC1->CR2|=1<<0; //¿ªÆôAD×ª»»Æ÷     
   ADC1->CR2|=1<<3; //Ê¹ÄÜ¸´Î»Ð£×¼    
   while(ADC1->CR2&1<<3); //µÈ´ýÐ£×¼½áÊø         
          
   ADC1->CR2|=1<<2;      //¿ªÆôADÐ£×¼     
   while(ADC1->CR2&1<<2);  //µÈ´ýÐ£×¼½áÊø
}

u16 Get_Adc(u8 ch)   
 {
              
     ADC1->SQR3&=0XFFFFFFE0;//¹æÔòÐòÁÐ1 Í¨µÀch
     ADC1->SQR3|=ch;                          
     ADC1->CR2|=1<<22;//Æô¶¯¹æÔò×ª»»Í¨µÀ
     while(!(ADC1->SR&1<<1));   //µÈ´ý×ª»»½áÊø        
     return ADC1->DR;      //·µ»ØADCÖµ  
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