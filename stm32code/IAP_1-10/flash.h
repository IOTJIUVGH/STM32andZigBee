#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f10x.h"

void FlashunInit(void);
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);	
void DelPage(u32 Page_Address);
void Test_Write(u32 WriteAddr,u16 WriteData);
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead); 
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);  
u16 STMFLASH_ReadHalfWord(u32 faddr);
 void WriteData(u16* data,u32 Address,u16 num);	

#endif 
