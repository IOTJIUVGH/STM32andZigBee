#ifndef __USART_DATA_ANALYZE_H
#define __USART_DATA_ANALYZE_H	

#include "stm32f10x.h"
struct Frame
{
	u16 dev_addr;
	u8 command;
	u8 data_len;
	u8 Use_data[40];
};//Ò»Ö¡Êý¾Ý
extern u8 pwm;
extern u8 dimmer_num;
extern u16 pwm_coe;
extern u8 Dos_Flag;
extern u8 debug_data[10][10];
void Analyze_rec_data(u8 data[],u8 num);
void Analyze_Rec_Frame(u8 data[],u8 num);
void Rec_Data_Handle(struct Frame frame1);
void analyze_PWMcxe(void);
void Register_shortaddr(void);
void Operate_LED(void);
void Total_worktime(void);
void Upload_HeartbeatData(void);
void Handle_ReceiveData(void);
void Pack_Data(u8* data,u8 len,u8 cmd);
#endif
