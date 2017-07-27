#include "stm32f10x.h"
#include "rcc.h"
#include "usart.h"
#include "delay.h"
#include "flash.h"	
#include "iap.h"
#include "timer3.h"
#include "rs485.h"
int main()
{
	u16 oldcount=0;	//老的串口接收数据值
	u16 applenth=0;	//接收到的app代码长度
	u8 i;
	
  Init_SyStemClock_inside();
	
	uart_init(72,9600);
	USART1_NVIC_Init();
	
	RS485_Init(36,2400);
	USART2_NVIC_INIT();
	
	TIM3_Int_Init(4999,7199);
	TIMER3_UP__NVIC_Init();
	while(1)
	{
//		if(RS485_RX_CNT)
//		{
//			if(oldcount==RS485_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
//			{
//				applenth=RS485_RX_CNT;
//				oldcount=0;
//				RS485_RX_CNT=0;
//			}else oldcount=RS485_RX_CNT;	
//		}			
//		Delayms(100);
//		if(flag==20)
//		{
//			flag=0;
//			TIM3->CR1&=~(0x01);    //关闭定时器3
//			for(i=0;i<RS485_RX_CNT;i++)
//			{
//			
//				printf("%c",RS485_RX_BUF[i]);
//			}
//		}
		
		if(flag==40)
		{
			flag=0;
			TIM3->CR1&=~(0x01);    //关闭定时器3
			
			if(RS485_RX_CNT)
			{
					printf("%d",RS485_RX_CNT);
//						if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)
					{
						iap_write_appbin(FLASH_APP1_ADDR,RS485_RX_BUF,RS485_RX_CNT);//更新FLASH代码   
						Delayms(100);
						printf("AA");	
					}
					RS485_RX_CNT=0;
					
					if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)
					{
						iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码		 
					}
					//printf("GO8");	
			}else{
			
					if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)
					{
						iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码		 
					}
		
			}
		}	
		
 }

}
