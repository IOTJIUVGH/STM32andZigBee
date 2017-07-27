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
#include "delay.h"

#include "string.h"
#include <stdlib.h>

#define USART_HANDLE
#define TIMER_RTC
#define KEY
#define STM32FLASH
#define ADC
//#define DOG
#define PWM
#define RS485

u16 line1[30]={0};//接收曲线参数数组

int main()
{
	
	float I,U;//ADC变量
	u16 *p,*q,leak_i1,leak_i2,*l,*name;
	u8 i,crc,wet;	
	name=(u16*)0X0800FC00;//ID地址
	l=(u16*)0x0800F800;//曲线ID
	q=(u16*)0x0800F802;//曲线点数地址
	p=(u16*)0x0800F804;//曲线参数起始地址
	
	Init_SyStemClock_inside();
	uart_init(72,9600);
	USART1_NVIC_Init();
	
	Bkp_Init();
	printf("GO");
	
	#ifdef ADC
	ADC_GPIO_config();
	ADC_config();
	#endif
	
	#ifdef PWM
	Init_Gpio();
	PWM_TIM1_Init();
	LedToggle('n');
	pwm=30;
	#endif
	
	#ifdef STM32FLASH
	FlashunInit();
	
	#endif
	
	#ifdef TIMER_RTC
	cnt1 = RTC->CNTH;  
  cnt1 <<= 16;  
  cnt1 += RTC->CNTL;  
	if(cnt1)
	{
		while(!(RTC->CRL&(1<<3)));//等待RTC寄存器同步  
		RTC->CRH|=0X01;  		  //允许秒中断
    while(!(RTC->CRL&(1<<5)));//等待RTC寄存器操作完成
	}
	RTC__NVIC_Init();
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
		
		//根据曲线对等操作
		Rtc_Get();
		if(*q<10)
		{
			for(i=0;i<*q;i++)
			{
			if((timer.hour==*(p+3*i))&&(timer.minute==*(p+1+3*i))&&(timer.second==1))
			{
				analyze_PWMcxe();
				if(*(p+2+3*i)==0)
				{
					LedToggle('f');
					pwm=0;
				}else
				{
					LedToggle('n');
					pwm=*(p+2+3*i);
					pwmval((u16)((pwm_coe/100.0)*(pwm)));
				}
			}
			}
		}
		
		if(bkp_flag==1)
		{
			u16 Accum_time;
			
			Accum_time=BKP->DR1;
			Accum_time++;
			BKP->DR1&=0x0000;
			BKP->DR1|=Accum_time;	

			bkp_flag=0;
		}
		
	//每隔20秒上传一次数据
		if(send_flag==20)
		{
			
			#ifdef ADC
			U=Analyze_Adc_U_Data();
			I=Analyze_Adc_I_Data();
			//printf("%.4f\r\n%.4f\r\n",I,U);
			#endif
			
//			for(i=0;i<7;i++)
//			{
//				printf("%d\r\n",RS485_RX_BUF[i]);
//			}
			crc=Get_Crc16(RS485_RX_BUF,7);
			if(crc==0)
			{
				leak_i1=RS485_RX_BUF[0];
				leak_i1=leak_i1<<8;
				leak_i1=RS485_RX_BUF[1];
				
				leak_i2=RS485_RX_BUF[2];
				leak_i2=leak_i2<<8;
				leak_i2=RS485_RX_BUF[3];
				
				wet=RS485_RX_BUF[4];
				
			}
			
			printf("{\"ID\":\"%d\",\"U\":\"%.4f\",\"I\":\"%.4f\",\"PWM\":\"%d\",\"LEAK_I1\":\"%d\"}",*name,U,I,pwm,leak_i1);
			Delayms(500);
			printf("{\"ID\":\"%d\",\"LEAK_I2\":\"%d\",\"LINE\":\"%d\",\"WET\":\"%d\",\"OPEN_TIME\":\"%d\"}",*name,leak_i2,*l,wet,opentime);

			send_flag=0;
			for(i=0;i<7;i++)
			{	
				RS485_RX_BUF[i]=0x00;
			}
			RS485_RX_CNT=0;
		}
		
		//处理接收到的数据
		#ifdef USART_HANDLE
		if(flag)
		{
			u8 i;	
			
			Analyze_Rec_Json(buf,cnt);
			for(i=0;i<cnt;i++)
			{	
				buf[i]='\0';
			}
			cnt=0;
			flag=0;
		}	
		#endif
		
		//调试数据处理
		if((GPIOB->IDR&0x0400)==0)
		{
			u8 i;	
			float adc_i,adc_u;
			u16 byte0,byte1;
			u16 *a;
			int id,y,m,d,h,mi,s;
			
			Analyze_rec_data(buf,cnt);
			id=(u16)atoi(debug_data[0]);
			adc_u=atof(debug_data[1]);
			adc_i=atof(debug_data[2]);
			y=atoi(debug_data[3]);
			m=atoi(debug_data[4]);
			d=atoi(debug_data[5]);
			h=atoi(debug_data[6]);
			mi=atoi(debug_data[7]);
			s=atoi(debug_data[8]);
			byte0=(u16)(adc_i*1000);
			byte1=(u16)(adc_u*400);
			
			a=(u16*)0x0800FC00;
			DelPage(0x0800FC00);
			WriteData(id,a);
			WriteData(byte0,a+1);
			WriteData(byte1,a+2);
			printf("%d",*(a));
			
			#ifdef TIMER_RTC
			Rtc_Init();
			RTC__NVIC_Init();
			Rtc_TIME_Set(y,m,d,h,mi,s);			
			#endif
				
			for(i=0;i<cnt;i++)
			{	
				buf[i]='\0';
			}
			cnt=0;
			
		}
		
		
	}
}	


