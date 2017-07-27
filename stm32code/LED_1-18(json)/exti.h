#ifndef __EXTI_H
#define __EXTI_H
#include "stm32f10x.h"
extern u8 debug_flag;
void EXTIX_Init(void);
void exti0_UP__NVIC_Init(void);
void KEY_Init(void);
#endif
