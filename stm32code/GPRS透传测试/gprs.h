#ifndef __GPRS_H
#define __GPRS_H

#include "stdio.h"	
#include "stm32f10x.h"

void clear_buffer(void);
void GSM_Init(void);
void Send_Str(u8* data,u8 len);
void ConnectJudgment(void);
void TcpReLink(void);
void DisLink(void);
#endif	
