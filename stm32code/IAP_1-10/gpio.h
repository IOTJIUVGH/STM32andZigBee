#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f10x.h"

void	GpioInit();
void pwmval(u16 num);
void PWM_TIM3_CH1_Init(u16 arr,u16 psc);
void LedToggle(u8 flag);
#endif 