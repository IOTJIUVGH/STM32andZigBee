#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f10x.h"

void Flash_unlock(void);
void WriteData(u16 Data,u16* Address);
void DelPage(u32 Page_Address);
void WriteString(u16* data,u32 Address,u16 num);

void STMFLASH_Lock(void);
u8 STMFLASH_GetStatus(void);
u8 STMFLASH_WaitDone(u16 time);
u8 STMFLASH_ErasePage(u32 Page_Address);
u8 STMFLASH_WriteHalfWord(u16 Data,u16* Address);
#endif
