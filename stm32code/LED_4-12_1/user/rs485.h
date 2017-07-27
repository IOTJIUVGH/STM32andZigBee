#ifndef __RS485_H
#define __RS485_H	

#include "stm32f10x.h"
#include "V_Timer.h"
#include "stdio.h"
#include "crc.h"
 	
extern u8 g_RS485_RxBuf[7]; 		//接收缓冲,最大64个字节
extern u8 g_RS485_RxCnt;   			//接收到的数据长度
extern u8 g_RS485_RxFlag;



void USART2_NVIC_INIT(void);														 
void RS485_Init(u32 pclk2,u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);
void RS485_OverTim(void);




#endif
