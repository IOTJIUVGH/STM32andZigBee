#ifndef __TIMER3_H__
#define __TIMER3_H__
#include "stm32f10x.h"

extern u8 flag;
void TIM3_Int_Init(u16 arr,u16 psc);
void TIMER3_UP__NVIC_Init(void);

#endif
