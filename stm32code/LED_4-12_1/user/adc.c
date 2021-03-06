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
	
	GPIOC->CRL |=0x000fff00;//模拟输入模式PA0,PA1
}

void ADC_config()
{
	RCC->APB2ENR |=0x0001<<9;//enable ADC1 timer
	RCC->APB2RSTR |=0x0001<<9;//复位ADC1定时器 
	RCC->APB2RSTR &=~(0x0001<<9);//ADC1复位结束
	
	RCC->CFGR &=~(0x0003<<14);//ADC预分频清零
	RCC->CFGR |=0x0003<<14;//配置ADC预分频八分频
  
	ADC1->CR1 &=0xf0ffff;//工作模式清零
	ADC1->CR1|=0x0000000<<16; //独立工作模式  
	ADC1->CR1 |=1<<8;   //扫描模式 
	//ADC1->CR2&=~(1<<1);    //单次转换模式
	ADC1->CR2 |= 0x01 << 1;
	
	ADC1->CR2&=~(7<<17);  
	ADC1->CR2|=7<<17;    //选择启动规则通道组转换的外部事件为SWSTART 
	ADC1->CR2|=1<<20;  //启用外部触发
	
	ADC1->CR2 &=~(1<<11);  			//右对齐  
     
	ADC1->SQR1 &=~(0XF<<20);   
	ADC1->SQR1 |=0<<20;     		//（1个转换在规则序列中 也就是只转换规则序列1）舜κ�4个	
     
 
	ADC1->SMPR2&=~(0x01ff03f<<0);  //采样时间清空
	ADC1->SMPR2|=0x01ff03f<<0;     //通道1/2/5/6/7 为239.5周期
     
	ADC1->CR2|=1<<0; 				//开启AD转换器     
	ADC1->CR2|=1<<3; 				//使能复位校准    
	while(ADC1->CR2&1<<3); 			//等待校准结束         
          
	ADC1->CR2|=1<<2;      			//开启AD校准     
	while(ADC1->CR2&1<<2);  			//等待校准结束
   
	ADC1->CR1 |= 0X01<<5;			//启动ADC转换结束中断
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
	ADC中断处理函数
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
		
	//	ADC1->SQR3&=0XFFFFFFE0;//规则序列1 通道ch
	//	ADC1->SQR3|=channel;  
	//   ADC1->CR2|=1<<22;//启动规则转换通道
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
