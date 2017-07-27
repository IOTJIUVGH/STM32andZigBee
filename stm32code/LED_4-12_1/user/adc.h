#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

void ADC_GPIO_config(void);
void ADC_config(void);

u16 Get_ADC(void); 
void ADC1_NVIC_Init(void);


#endif
