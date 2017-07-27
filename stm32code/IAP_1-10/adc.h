#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

extern float temp[20],val,max,min;
extern u16 adcx;
u16 Get_Adc_Average(u8 ch,u8 times);
void ADC_GPIO_config();
void ADC_config();
u16 Get_Adc(u8 ch);  
float Analyze_Adc_I_Data();
float Analyze_Adc_U_Data();
#endif 