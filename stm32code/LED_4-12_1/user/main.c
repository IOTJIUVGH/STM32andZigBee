#include "stm32f10x.h"
#include "rcc.h"
#include "adc.h"
#include "usart.h"
#include "pwm.h"
#include "flash.h"
#include "Usart_Data_analyze.h"
#include "rtc.h"
#include "exti.h"
#include "rs485.h"
#include "dog.h"
#include "crc.h"

#include "string.h"
#include <stdlib.h>
#include "V_Timer.h"

#define USART_HANDLE
#define TIMER_RTC
#define KEY
#define STM32FLASH
#define ADC
#define DOG
#define PWM
#define RS485
#define TIMER

u16 line1[30]={0};//接收曲线参数数组


void test_fun(void)
{
//	if(USART1->SR & 0x40)
	{
	USART1->CR1|=1<<6;    //发送完成中断使能
	USART1->DR = 0x3c;
		
	while(!(USART1->SR & 0X40));
	vtimer_SetTimer(VTIM3, 5000, test_fun);
	}
}

int main()
{
	u16 heartdata_uploadtime=0;
	//	Init_SystemClock_outside();     //外部时钟
	SystemInit();
	
	uart_init(72,9600);            //串口1初始化
	USART1_NVIC_Init();            //中断优先级
	vtimer_init();
	Bkp_Init();                 //初始化备份区

	#ifdef ADC
	ADC_GPIO_config();
	ADC_config();
	ADC1_NVIC_Init();
	#endif
	
	#ifdef PWM
	Init_Gpio();
	PWM_TIM1_Init();
	Led_Toggle('f');           //关灯
	g_PWMValue = 0;
	#endif
	
	#ifdef STM32FLASH
	Flash_unlock();            //flash解锁
	#endif
	
	#ifdef TIMER_RTC
	Rtc_Init2();
	#endif
			
	#ifdef KEY
	KEY_Init();
	#endif
	
	#ifdef RS485
	RS485_Init(72,2400);
	USART2_NVIC_INIT();
	#endif
	//printf("GO");
//	#ifdef DOG
//	IWDG_Init(4,1000); 
//	#endif	

 heartdata_uploadtime=BKP->DR4;
 if(heartdata_uploadtime==0)
	BKP->DR4=5000;
	

	vtimer_SetTimer(VTIM2,1000,Heart_Timer);

	vtimer_SetTimer(VTIM3, 5000, En_GetData);
	
	vtimer_SetTimer(VTIM4, 1000, En_GetADC);
	
	while(1)
	{
//		#ifdef DOG
//		IWDG_Feed();
//		#endif
		
		//数据定时发送
		if(g_EnHrtBtDat_flag)  
		{
			if(CheckEnroll())  					//需要注册，则发注册申请
			{
				if(Register_shortaddr())
					g_EnHrtBtDat_flag = 0;	
			}
			else								//不需要注册，则发心跳包
			{
				if(Upload_HeartbeatData())
				g_EnHrtBtDat_flag = 0;
			}
		}
		
		//获取数据
		if(g_GetHrtBtData_flag == 1)
		{
			Get_HeartBeatData();
			
			g_GetHrtBtData_flag = 0;
		}
		
		//获取电压电流的值
		if(g_GetVolt_flag == 1)
		{
			Get_Volt();
			g_GetVolt_flag = 0;
		}
		if(g_GetCur_flag == 1)
		{
			Get_Cur();
			g_GetCur_flag = 0;
		}
		
		if(g_DviceSleep == 0)
		{
			//根据曲线对灯操作
			if(g_DimCrv_flag == 0)
			{
				Operate_LED();
			}
			
			//LED灯累计工作时间
			if(g_Bkp_flag == 1)
			{
				Total_worktime();
			}
		}
			
		//处理串口接收到的数据
		if(g_URT1_RxFlag == 1)
		{
			Handle_ReceiveData();
		}
		
		//处理485收到的数据
		if(g_RS485_RxFlag == 1)
		{
			RS485_RxDatDeal();
		}

	}
}	

