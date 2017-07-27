#ifndef __ADC_H
#define __ADC_H
#include "stm32f10x.h"
void EXTIX_Init(void);
void exti1_UP__NVIC_Init();
void exti0_UP__NVIC_Init();
void EXTI15_10_NVIC_Init();	
#endif