#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"
void ADC_GPIO_config(void);
void ADC_config(void);
u16 Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times);
float Analyze_Adc_I_Data(void);
float Analyze_Adc_U_Data(void);
#endif
