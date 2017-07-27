#include "stm32f10x.h"
#include "usart.h"
#include "V_Timer.h"
#include "crc.h"
#include "gprs.h"
#include "timer.h"

u8 link_flag=0;

void Ser1Data(void)
{
	TIMER3_flag=1;
	//vtimer_SetTimer(VTIM2,1000,Ser1Data);	
}

void Ser2Data(void)
{
	TIMER4_flag=1;
	//vtimer_SetTimer(VTIM3,1000,Ser2Data);
	//T1=0;
}

void CheckLink(void)
{
  link_flag=1;
	vtimer_SetTimer(VTIM4,10000,CheckLink);
}


void Pack_Data(u8* data,u8 len,u8 fformat)
{
	u8 packdata[100]={0}, x, y=7, i;
	u16 crc;
	for(i=0;i<3;i++)
	packdata[i] = 0x3c;
	
	packdata[3] = 0x5a;
	packdata[4] = fformat;
	packdata[5] = 0x02;
	
	packdata[6] = len;
	
	for(x=0;x<len;x++)
	{
		packdata[y] = data[x];
		y++;
	}
	crc = Get_Crc16(packdata,y);

	packdata[y]=crc&0xff;
	y++;
	packdata[y]=(crc>>8)&0xff;
	y++;
	packdata[y]=0xed;
	y++;
	//for(i=0;i<y;i++)	
					//Uart1_PutChar(packdata[i]);
	Send_Str(packdata,y);
}	

