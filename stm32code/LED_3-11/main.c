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

int main()
{
	Init_SyStemClock_inside();
	
	uart_init(72,9600);
	USART1_NVIC_Init();
	
	Bkp_Init();

	#ifdef ADC
	ADC_GPIO_config();
	ADC_config();
	#endif
	
	#ifdef PWM
	Init_Gpio();
	PWM_TIM1_Init();
	LedToggle('f');
	pwm=0;
	#endif
	
	#ifdef TIMER
	TIM3_Int_Init(9,7199);//1毫秒4999 500毫秒
	TIMER3_UP__NVIC_Init();
	#endif
	
	#ifdef STM32FLASH
	FlashunInit();
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
	
	#ifdef DOG
	IWDG_Init(4,1000); 
	#endif	

	while(1)
	{
		#ifdef DOG
		IWDG_Feed();
		#endif
		
		Register_shortaddr();//注册设备短地址
		
		//根据曲线对灯操作
		Operate_LED();
		
		//LED灯累计工作时间
		Total_worktime();
		
		//上传心跳数据
		Upload_HeartbeatData();
		
		//处理接收到的数据
		Handle_ReceiveData();
	}
}	

