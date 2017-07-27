#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include "stdio.h"	
#include "V_Timer.h"
#include "Usart_Data_analyze.h"
#include "pwm.h"
#include <string.h>

extern u8 g_URT1_TxEn; 
extern u8 g_URT1_RxFlag;
extern u16 g_URT1_RxLen;
extern u16 g_URT1_TxLen;
extern u8 g_URT1_RxBuf[50];
extern u8 g_URT1_TxBuf[50];
void uart_init(u32 pclk2,u32 bound);
void Uart1_PutChar(u8 ch);
void USART1_NVIC_Init(void);

#endif	   
















