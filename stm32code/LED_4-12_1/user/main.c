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

u16 line1[30]={0};//�������߲�������


void test_fun(void)
{
//	if(USART1->SR & 0x40)
	{
	USART1->CR1|=1<<6;    //��������ж�ʹ��
	USART1->DR = 0x3c;
		
	while(!(USART1->SR & 0X40));
	vtimer_SetTimer(VTIM3, 5000, test_fun);
	}
}

int main()
{
	u16 heartdata_uploadtime=0;
	//	Init_SystemClock_outside();     //�ⲿʱ��
	SystemInit();
	
	uart_init(72,9600);            //����1��ʼ��
	USART1_NVIC_Init();            //�ж����ȼ�
	vtimer_init();
	Bkp_Init();                 //��ʼ��������

	#ifdef ADC
	ADC_GPIO_config();
	ADC_config();
	ADC1_NVIC_Init();
	#endif
	
	#ifdef PWM
	Init_Gpio();
	PWM_TIM1_Init();
	Led_Toggle('f');           //�ص�
	g_PWMValue = 0;
	#endif
	
	#ifdef STM32FLASH
	Flash_unlock();            //flash����
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
		
		//���ݶ�ʱ����
		if(g_EnHrtBtDat_flag)  
		{
			if(CheckEnroll())  					//��Ҫע�ᣬ��ע������
			{
				if(Register_shortaddr())
					g_EnHrtBtDat_flag = 0;	
			}
			else								//����Ҫע�ᣬ��������
			{
				if(Upload_HeartbeatData())
				g_EnHrtBtDat_flag = 0;
			}
		}
		
		//��ȡ����
		if(g_GetHrtBtData_flag == 1)
		{
			Get_HeartBeatData();
			
			g_GetHrtBtData_flag = 0;
		}
		
		//��ȡ��ѹ������ֵ
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
			//�������߶ԵƲ���
			if(g_DimCrv_flag == 0)
			{
				Operate_LED();
			}
			
			//LED���ۼƹ���ʱ��
			if(g_Bkp_flag == 1)
			{
				Total_worktime();
			}
		}
			
		//�����ڽ��յ�������
		if(g_URT1_RxFlag == 1)
		{
			Handle_ReceiveData();
		}
		
		//����485�յ�������
		if(g_RS485_RxFlag == 1)
		{
			RS485_RxDatDeal();
		}

	}
}	

