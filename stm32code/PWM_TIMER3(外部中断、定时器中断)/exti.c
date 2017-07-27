#include "stm32f10x.h"
#include "delay.h"

//外部中断0服务程序
void EXTI0_IRQHandler(void)
{
	//Delayms(10);//消抖
	printf("exti0");
	EXTI->PR=1<<0;  //清除LINE0上的中断标志位  
}
void EXTI1_IRQHandler(void)
{
	printf("exti1");
	EXTI->PR=1<<1;  //清除LINE1上的中断标志位  
}


void EXTIX_Init(void)
{
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟
	GPIOA->CRL&=0XF0000000;//PA0和PA1设置成输入 
	GPIOA->CRL|=0X08888888; 

	AFIO->EXTICR[0]&=~(0x000F<<0);   //对该端口清除原来中断线设置（先清除一下，确保初始化为干净的状态）
	AFIO->EXTICR[0]&=~(0x000F<<1);   //对该端口清除原来中断线设置（先清除一下，确保初始化为干净的状态）

	AFIO->EXTICR[0]|=0<<0;//EXTI.BITx=EXTI.BIT0映射到GPIOA的0管脚上,即将中断线设置到PA0管脚上
	AFIO->EXTICR[0]|=0<<1;//EXTI.BITx=EXTI.BIT0映射到GPIOA的1管脚上,即将中断线设置到PA0管脚上

	EXTI->IMR|=1<<0; //开启PA0上的中断（实际上同时也开启了PB0，PC0，PD0..PG0上的中断线，细节请见手册）
 	EXTI->IMR|=1<<1; //开启PA1上的中断（实际上同时也开启了PB1，PC1，PD1..PG1上的中断线，细节请见手册）

	EXTI->FTSR|=1<<0; //设置PA0上下降沿触发中断
	EXTI->FTSR|=1<<1; //设置PA1上下降沿触发中断

	
}

void exti0_UP__NVIC_Init()
{	
	 SCB->AIRCR |= 0x5 << 8; 
    NVIC->IP[29] |= 0;   
      
    	printf("TT");
    NVIC->ISER[0] |=1<<6; 
	printf("AA");
}
void exti1_UP__NVIC_Init()
{	
	 SCB->AIRCR |= 0x5 << 8; 
    NVIC->IP[29] |= 1;   
      
    	printf("TOT");
    NVIC->ISER[0] |=1<<7; 
	printf("AA");
}
