#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "stm32f10x.h"
extern u8 com;
#define USART_REC_LEN  			64 //�����������ֽ��� 18K
extern u8  buf[USART_REC_LEN];
extern u16 USART_RX_CNT;				//���յ��ֽ���		
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 pclk2,u32 bound);
void Uart1_PutChar(u8 ch);
void USART1_NVIC_Init(void);

#endif	   
















