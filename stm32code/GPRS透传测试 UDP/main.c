#include "stm32f10x.h"
#include "rcc.h"
#include "usart.h"
#include "delay.h"
#include "gprs.h"
#include "timer.h"
#include "V_Timer.h"
#include "heart_handle.h"
#include "crc.h"
#include "dog.h"

int main()
{	
	u8 i;
	u16 num=0;

	//u32 heartdata_uploadtime=0;
	
	Init_SyStemClock();
	uart_init(72,9600);
	USART1_NVIC_Init();
	uart2_init(72,9600*2);
	USART2_NVIC_Init();
	
	vtimer_init();
	vtimer_SetTimer(VTIM4,10000,CheckLink);
	GSM_Init();
	
	//TIM3_Int_Init(9,7199);
	//TIMER3_UP__NVIC_Init();
	
	//TIM4_Int_Init(9,7199);
	//TIMER4_UP__NVIC_Init();
//	Uart1_PutChar('C');
//	Delayms(500);
  //IWDG_Init(8,20000); 
	Uart1_PutChar('C');
	while(1)
	{				
		//IWDG_Feed();
		if(link_flag==1)
		{
		  ConnectJudgment();
			link_flag=0;
		}
		if(T1==1)
		{
		  vtimer_SetTimer(VTIM3,300,Ser2Data);
			T1=0;
		}
		
		if(T==1)
		{
		  vtimer_SetTimer(VTIM2,500,Ser1Data);
			T =0;
		}
		
		if(TIMER3_flag==1)
		{
			//Pack_Data(Rx_buf,Rx_cnt,0x03);
			Rx_buf[Rx_cnt]=num++;
			Send_Str(Rx_buf,Rx_cnt+1);
			
			Uart1_PutChar('B');
			for(i=0;i<Rx_cnt+1;i++)
		   Rx_buf[i]=0;//»º´æ
		  Rx_cnt=0;
		//DisLink();
		  TIMER3_flag=0;
		}
			
		if(TIMER4_flag==1)
		{		
			//Uart1_PutChar('B');
			for(i=0;i<cnt;i++)
			{
				if(buf[i]==':')
				{
						i++;
					  for(;i<cnt;i++)	
					    Uart1_PutChar(buf[i]);
				}
			}
			Uart1_PutChar('A');
			clear_buffer();
			    TIMER4_flag=0;
					cnt=0;
		}
		
	}

}

