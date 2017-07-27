#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "stm32f10x.h"
extern u8 com,T,T1;
extern u16 cnt;
extern u16 Rx_cnt;
extern u8 buf[100];
extern u8 Rx_buf[100];
void USART1_NVIC_Init(void);
void uart_init(u32 pclk2,u32 bound);
void Uart2_PutChar(u8 ch);
void uart2_init(u32 pclk2,u32 bound);
void Uart1_PutChar(u8 ch);
void USART2_NVIC_Init(void);
void printintputdata(void);
void UART1_SendStr(char* s);
#endif	 
