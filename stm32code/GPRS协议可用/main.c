#include "stm32f10x.h"
#include "rcc.h"
#include "usart.h"
#include "delay.h"
#include "gprs.h"
#include "timer.h"
#include "V_Timer.h"
#include "heart_handle.h"
#include "crc.h"

int main()
{	
	u8 i;
	//u32 heartdata_uploadtime=0;
	
	Init_SyStemClock();
	uart_init(72,9600);
	USART1_NVIC_Init();
	uart2_init(72,9600*2);
	USART2_NVIC_Init();
	
	//vtimer_init();
	//vtimer_SetTimer(VTIM2,10000,HeartDataUp_Timer);
	TIM3_Int_Init(9,7199);
	TIMER3_UP__NVIC_Init();
	
	GSM_Init();
	
	TIM4_Int_Init(9,7199);
	TIMER4_UP__NVIC_Init();
	
	Uart1_PutChar('C');
	while(1)
	{
		if(TIMER3_flag==1)
		{
			Pack_Data(Rx_buf,Rx_cnt,0x03);
			for(i=0;i<Rx_cnt;i++)
		   Rx_buf[i]=0;//»º´æ
		  Rx_cnt=0;
		  TIMER3_flag=0;
		}
//		if(TIMER3_flag==1)
//		{
//		//	u8 i;
//			for(i=0;i<Rx_cnt;i++)
//				a[i]=Rx_buf[i];
//				//clear_buffer();
//				//UART1_SendStr((char*)buf);
//		Send_Str(a,Rx_cnt);
//		for(i=0;i<Rx_cnt;i++)
//		Rx_buf[i]=0;//»º´æ
//		Rx_cnt=0;
//		TIMER3_flag=0;
//		}
		
		if(TIMER4_flag==1)
		{		
			//Uart1_PutChar('B');
			for(i=0;i<cnt;i++)
			{
				if(buf[i]==':')
				{
					u16 crc;
					u8 j;
					i++;
					crc = Get_Crc16(&buf[i],cnt-i-1);
					if(crc==0)
					{
						if(buf[i++]==0x3c&&buf[i++]==0x3c&&buf[i++]==0x3c&&buf[i++]==0x5a)
					 {
							i+=3;
						  for(;i<cnt-3;i++)	
					     Uart1_PutChar(buf[i]);
						 
					 }
				}
				}
			}
			clear_buffer();
			TIMER4_flag=0;
			cnt=0;
		}

	}

}

