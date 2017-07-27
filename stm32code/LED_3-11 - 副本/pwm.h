#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"
extern u8 TIMER3_flag;
extern u8 USART_falg;;
void Init_Gpio(void);
void PWM_TIM1_Init(void);
void pwmval(u16 pwm);
void LedToggle(u8 ch);
void TIM3_IRQHandler(void);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIMER3_UP__NVIC_Init(void);
#endif
