#include "stm32f10x.h"
typedef struct  
{  
    u8 hour;  
    u8 minute;  
    u8 second;  
  
    u16 year;  
    u8  month;  
    u8  date;  
    u8  week;  
}tm;  
  
extern tm timer;  
  
void Rtc_Init(void);  
void Rtc_TIME_Set(u16 year,u8 month,u8 date,u8 hour,u8 minute, u8 second);  
u8 Is_LeapYear(u16 year);  
u32  Date_TO_Sec(u16 year,u8 month,u8 date,u8 hour,u8 minute, u8 second);  
void Rtc_TIME_AutoSet(void);  
void Rtc_Get(void);  
void Rtc_ALARM_Set(u16 year,u8 month,u8 date,u8 hour,u8 minute, u8 second);  
u8 Rtc_DAY_Get(u16 year,u8 month,u8 day);  