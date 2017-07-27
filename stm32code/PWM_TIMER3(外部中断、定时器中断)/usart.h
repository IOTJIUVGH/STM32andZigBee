#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "stm32f10x.h"
extern u8 com;
extern u16 cnt;
extern u8 buf[100];
void uart_init(u32 pclk2,u32 bound);
void Uart1_PutChar(u8 ch);
void USART1_NVIC_Init();
void printintputdata();
#endif	   
















