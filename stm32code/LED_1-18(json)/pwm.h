#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"
void Init_Gpio(void);
void PWM_TIM1_Init(void);
void pwmval(u16 pwm);
void LedToggle(u8 ch);
#endif
