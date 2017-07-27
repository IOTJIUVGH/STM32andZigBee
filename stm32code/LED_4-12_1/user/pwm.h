#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"

typedef struct 
{
	u16 dev_addr;
	u8 command;
	u8 data_len;
	u8 Use_data[40];
}Frame;//Ò»Ö¡Êý¾Ý

extern Frame g_Data_Deal ;

void Init_Gpio(void);
void PWM_TIM1_Init(void);
void pwmval(u16 pwm);
void Led_Toggle(u8 ch);
void TIM3_IRQHandler(void);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIMER3_UP__NVIC_Init(void);




#endif
