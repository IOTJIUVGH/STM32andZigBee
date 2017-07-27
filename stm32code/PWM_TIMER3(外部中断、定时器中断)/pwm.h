#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"
void PWM_TIM3_CH1_Init(u16 arr,u16 psc);

void TIMER3_UP__NVIC_Init();
#endif