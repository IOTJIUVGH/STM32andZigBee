#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "Usart_Data_analyze.h"
#include "rtc.h"

u8 debug_flag=0;
//外部中断0服务程序
void EXTI0_IRQHandler(void)
{
	
	Delayms(10);//消抖
	printf("exti0");
	debug_flag=1;
	EXTI->PR=1<<0;  //清除LINE0上的中断标志位  
}

void EXTIX_Init(void)
{
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟
	RCC->APB2ENR|=0x01;//使能io复用时钟		
	GPIOA->CRL&=0XFFFFFFF0;//PA0设置成输入 
	GPIOA->CRL|=0X00000008; 

	AFIO->EXTICR[0]&=~(0x000F<<0);   //对该端口清除原来中断线设置（先清除一下，确保初始化为干净的状态）
	AFIO->EXTICR[0]|=0<<0;//EXTI.BITx=EXTI.BIT0映射到GPIOA的0管脚上,即将中断线设置到PA0管脚上
	
	EXTI->IMR|=1<<0; //开启PA0上的中断（实际上同时也开启了PB0，PC0，PD0..PG0上的中断线，细节请见手册）
	EXTI->FTSR|=1<<0; //设置PA0上下降沿触发中断
}

void exti0_UP__NVIC_Init()
{	
	SCB->AIRCR |= 0x5 << 8; 
	NVIC->IP[29] |= 0;   
	NVIC->ISER[0] |=1<<6; 
	
}
void KEY_Init(void)
{
//	RCC->APB2ENR|=1<<0;     //开启辅助时钟	   
//	AFIO->MAPR&=0XF8FFFFFF; //清除MAPR的[26:24]
//	AFIO->MAPR|=0X02<<25;       //设置jtag模式
	
	RCC->APB2ENR|=1<<3;     //使能PORTB时钟
	GPIOB->CRH&=0XFFFFF0FF;//PB10设置成输入 
	GPIOB->CRH|=0X00000800;
	GPIOB->ODR|=0x001<<10;
}
