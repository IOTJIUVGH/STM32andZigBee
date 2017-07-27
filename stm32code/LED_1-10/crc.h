#ifndef __CRC8_16_H
#define __CRC8_16_H
#include "stm32f10x.h"	

void CRC_Init();
u32 CRC_CalculateCRC(u32 data);
u32 CRC_CalculateBlockCRC(u8 pBuffer[], u32 BufferLength);
u16 Get_Crc16(u8 *puchMsg,u16 usDataLen);
u8 Get_Crc8(u8 *ptr,u16 len);

#endif