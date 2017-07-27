#include "rs485.h"	 
#include "delay.h"
#ifdef EN_USART2_RX   	//如果使能了接收   	  
//接收缓存区 	
u8 RS485_RX_BUF[7];  	//接收缓冲,最大7个字节.
//接收到的数据长度
u8 RS485_RX_CNT=0;   
void USART2_IRQHandler(void)
{
	u8 res;	 
	//printf("receive data\n");
	//
	if(USART2->SR&(1<<5))//接收到数据
	{	 
		res=USART2->DR;
		if(RS485_RX_CNT<7)
		{
			RS485_RX_BUF[RS485_RX_CNT]=res;		//记录接收到的值
			RS485_RX_CNT++;						//接收数据增加1 
//			if(RS485_RX_CNT>=7)
//				RS485_RX_CNT=0;
		}
	}  											 
} 
#endif										 
//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void RS485_Init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   	//使能PORTA口时钟  
  
 	GPIOA->CRL&=0XFFF0FFFF;	//IO状态设置
	GPIOA->CRL|=0X00030000;	//IO状态设置PA4	  
 
	
	GPIOA->CRL&=0XFFFF00FF;	//IO状态设置
	GPIOA->CRL|=0X00008B00;	//IO状态设置PA2\PA3	  

	RCC->APB1ENR|=1<<17;  	//使能串口时钟 
		  
	RCC->APB1RSTR|=1<<17;   //复位串口2
	RCC->APB1RSTR&=~(1<<17);//停止复位	   	   
	//波特率设置
 	USART2->BRR=mantissa; 	// 波特率设置	 
	USART2->CR1|=0X200C;  	//1位停止,无校验位.
#ifdef EN_USART2_RX		  	//如果使能了接收
	//使能接收中断
	USART2->CR1|=1<<8;    	//PE中断使能
	USART2->CR1|=1<<5;    	//接收缓冲区非空中断使能	    	
	//	MY_NVIC_Init(3,3,USART2_IRQn,2);//组2，最低优先级 
#endif
		//RS485_TX_EN=0;			//默认为接收模式	
		GPIOA->BSRR&=~(0x00100010);
		GPIOA->BSRR|=0x00000001<<20;	
}

void USART2_NVIC_INIT()
{
	SCB->AIRCR &=0x05faf8ff;
	SCB->AIRCR |=0x05fa0400;
	
	NVIC->ISER[1] |=1<<6;
  NVIC->IP[38]=0x30;
}
//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
//	RS485_TX_EN=1;			//设置为发送模式
		GPIOA->BSRR&=~(0x00100010);
		GPIOA->BSRR|=0x00000001<<4;	
	
  	for(t=0;t<len;t++)		//循环发送数据
	{
		while((USART2->SR&0X40)==0);//等待发送结束		  
		USART2->DR=buf[t];
	}	 
	while((USART2->SR&0X40)==0);//等待发送结束	
	RS485_RX_CNT=0;	  
//	RS485_TX_EN=0;				//设置为接收模式	
	GPIOA->BSRR&=~(0x00100010);
		GPIOA->BSRR|=0x00000001<<20;	
}

void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//默认为0
	Delayms(10);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
	if(rxlen==RS485_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//记录本次数据长度
		RS485_RX_CNT=0;		//清零
	}
}
