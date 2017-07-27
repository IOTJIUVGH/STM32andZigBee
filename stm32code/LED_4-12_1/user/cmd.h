#ifndef __CMD_H
#define __CMD_H

#include "stm32f10x.h"
#include "Usart_Data_analyze.h"
#include "pwm.h"
#include "flash.h"
#include "rtc.h"
#include "V_Timer.h"

extern u8 g_DviceSleep;

void Allot_Id(void);
void Refuse_Call(void);
void Vc_Val(void);
void Set_Time(void);
void Init_Addr(void);
void Find_Id(void);
void Constant_Illum(void);
void Find_Illum(void);
void Adjust_Curve(void);
void Ctrl_Real_Time(void);
void Find_Time(void);
void Close_Ctrl(void);
void Find_Curve(void);
void Device_Sleep(void);
void Device_wake(void);
void set_hertdatauptime();
void backhertdatatime();
#endif
