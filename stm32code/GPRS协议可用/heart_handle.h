#ifndef __HEART_HANDLE_H
#define __HEART_HANDLE_H	

#include "stm32f10x.h"

extern u8 heartDataRequest;

void HeartDataUp_Timer(void);
void Pack_Data(u8* data,u8 len,u8 fformat);

#endif
