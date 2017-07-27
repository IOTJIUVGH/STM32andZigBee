#ifndef __USART_DATA_ANALYZE_H
#define __USART_DATA_ANALYZE_H	

#include "stm32f10x.h"
struct Json
{
	u8 string[10];
	u8 value[10];
};
extern u16 pwm;
extern u16 pwm_coe;
extern u8 debug_data[10][10];
void Analyze_rec_data(u8 data[],u8 num);
void Analyze_Rec_Json(u8 data[],u8 num);
void Rec_Data_Handle(struct Json json1[],u16 num);
void analyze_PWMcxe(void);
#endif
