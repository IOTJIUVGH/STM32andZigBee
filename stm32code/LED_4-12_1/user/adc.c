#include "stm32f10x.h"
#include "delay.h"
#include "adc.h"
#include "usart.h"
#include "flash.h"
#include "Usart_Data_analyze.h"

float temp[20],val1,max,min,byte0,byte1;
u16 adcx;

u16 m_I_ppVal[10]={0},m_U_ppVal[10]={0};


void ADC_GPIO_config()
{
	RCC->APB2ENR |=0x0001<<2;//enable GPIOA
	
	GPIOC->CRL |=0x000fff00;//Ä£ÄâÊäÈëÄ£Ê½PA0,PA1
}

void ADC_config()
{
	RCC->APB2ENR |=0x0001<<9;//enable ADC1 timer
	RCC->APB2RSTR |=0x0001<<9;//¸´Î»ADC1¶¨Ê±Æ÷ 
	RCC->APB2RSTR &=~(0x0001<<9);//ADC1¸´Î»½áÊø
	
	RCC->CFGR &=~(0x0003<<14);//ADCÔ¤·ÖÆµÇåÁã
	RCC->CFGR |=0x0003<<14;//ÅäÖÃADCÔ¤·ÖÆµ°Ë·ÖÆµ
  
	ADC1->CR1 &=0xf0ffff;//¹¤×÷Ä£Ê½ÇåÁã
	ADC1->CR1|=0x0000000<<16; //¶ÀÁ¢¹¤×÷Ä£Ê½  
	ADC1->CR1 |=1<<8;   //É¨ÃèÄ£Ê½ 
	//ADC1->CR2&=~(1<<1);    //µ¥´Î×ª»»Ä£Ê½
	ADC1->CR2 |= 0x01 << 1;
	
	ADC1->CR2&=~(7<<17);  
	ADC1->CR2|=7<<17;    //Ñ¡ÔñÆô¶¯¹æÔòÍ¨µÀ×é×ª»»µÄÍâ²¿ÊÂ¼þÎªSWSTART 
	ADC1->CR2|=1<<20;  //ÆôÓÃÍâ²¿´¥·¢
	
	ADC1->CR2 &=~(1<<11);  			//ÓÒ¶ÔÆë  
     
	ADC1->SQR1 &=~(0XF<<20);   
	ADC1->SQR1 |=0<<20;     		//£¨1¸ö×ª»»ÔÚ¹æÔòÐòÁÐÖÐ Ò²¾ÍÊÇÖ»×ª»»¹æÔòÐòÁÐ1£©¬´Ë´¦ÊÇ4¸ö	
     
 
	ADC1->SMPR2&=~(0x01ff03f<<0);  //²ÉÑùÊ±¼äÇå¿Õ
	ADC1->SMPR2|=0x01ff03f<<0;     //Í¨µÀ1/2/5/6/7 Îª239.5ÖÜÆÚ
     
	ADC1->CR2|=1<<0; 				//¿ªÆôAD×ª»»Æ÷     
	ADC1->CR2|=1<<3; 				//Ê¹ÄÜ¸´Î»Ð£×¼    
	while(ADC1->CR2&1<<3); 			//µÈ´ýÐ£×¼½áÊø         
          
	ADC1->CR2|=1<<2;      			//¿ªÆôADÐ£×¼     
	while(ADC1->CR2&1<<2);  			//µÈ´ýÐ£×¼½áÊø
   
	ADC1->CR1 |= 0X01<<5;			//Æô¶¯ADC×ª»»½áÊøÖÐ¶Ï
}

void ADC1_NVIC_Init()
{
	SCB->AIRCR &=0x05faf8ff;
	SCB->AIRCR |=0x05fa0400;
	NVIC->IP[18]=0x30;
	NVIC->ISER[0] |=1<<18;
}

 u16 Get_ADC(void)
{
	u8 i;
	u16 U_ppVal=0;
	
	for(i=0;i<10;i++)
	{
		U_ppVal += m_U_ppVal[i];
	}
	
	U_ppVal = U_ppVal/10;

	return U_ppVal;
}

/*
	ADCÖÐ¶Ï´¦Àíº¯Êý
*/
void ADC1_2_IRQHandler(void)
{
	static u8 u_pp_cnt=0;
	static u16 u_max=0,u_min=4095;
	static u16 u_cnt=0;
	u16 val;
	
	if(ADC1->SR & (0x01 <<1))
	{
		val = ADC1->DR;

		if(u_cnt >=1000)
		{
			m_U_ppVal[u_pp_cnt] = u_max - u_min;
			
			if(u_pp_cnt >=10)
				u_pp_cnt = 0;
			else 
				u_pp_cnt ++;
			
			u_cnt =0;
			u_max = 0;
			u_min=4095;
		}
		else
		{
			if(val>u_max) 
				u_max = val;
			if(val<u_min)
				u_min = val;
			u_cnt ++;
		}
		
	//	ADC1->SQR3&=0XFFFFFFE0;//¹æÔòÐòÁÐ1 Í¨µÀch
	//	ADC1->SQR3|=channel;  
	//   ADC1->CR2|=1<<22;//Æô¶¯¹æÔò×ª»»Í¨µÀ
	}
}



//		ADC1->SR &= ~(0x01 <<1);
//		if(channel==1)
//		{
//			channel=0;
//			GPIOB->BSRR&=~(0x01000100);
//			GPIOB->BSRR|=0x00000001<<24;	
//		}
//		else
//		{
//			channel=1;
//			GPIOB->BSRR&=~(0x01000100);
//			GPIOB->BSRR|=0x00000001<<8;	
//		}
