#ifndef __HEART_HANDLE_H
#define __HEART_HANDLE_H	

#include "stm32f10x.h"

extern u8 link_flag;

void HeartDataUp_Timer(void);
void Pack_Data(u8* data,u8 len,u8 fformat);
void Ser1Data(void);
void Ser2Data(void);
void CheckLink(void);
#endif
