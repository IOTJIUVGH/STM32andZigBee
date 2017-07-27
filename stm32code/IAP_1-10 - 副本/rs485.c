#include "rs485.h"	 
#include "delay.h"
#ifdef EN_USART2_RX   	//Èç¹ûÊ¹ÄÜÁË½ÓÊÕ   	  
//½ÓÊÕ»º´æÇø 	
u8 RS485_RX_BUF[15*1024];  	//½ÓÊÕ»º³å,×î´15*1024¸ö×Ö½Ú.
//½ÓÊÕµ½µÄÊý¾Ý³¤¶È
u16 RS485_RX_CNT=0;   
void USART2_IRQHandler(void)
{
	u8 res;	 
	if(USART2->SR&(1<<5))//½ÓÊÕµ½Êý¾Ý
	{	 
		res=USART2->DR;
		RS485_RX_BUF[RS485_RX_CNT]=res;		//¼ÇÂ¼½ÓÊÕµ½µÄÖµ
		RS485_RX_CNT++;						//½ÓÊÕÊý¾ÝÔö¼Ó1
	}  											 
} 
#endif										 
//³õÊ¼»¯IO ´®¿Ú2
//pclk1:PCLK1Ê±ÖÓÆµÂÊ(Mhz)
//bound:²¨ÌØÂÊ	  
void RS485_Init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//µÃµ½USARTDIV
	mantissa=temp;				 //µÃµ½ÕûÊý²¿·Ö
	fraction=(temp-mantissa)*16; //µÃµ½Ð¡Êý²¿·Ö	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   	//Ê¹ÄÜPORTA¿ÚÊ±ÖÓ  
  
 	GPIOA->CRL&=0XFFF0FFFF;	//IO×´Ì¬ÉèÖÃ
	GPIOA->CRL|=0X00030000;	//IO×´Ì¬ÉèÖÃPA4	  
 
	
	GPIOA->CRL&=0XFFFF00FF;	//IO×´Ì¬ÉèÖÃ
	GPIOA->CRL|=0X00008B00;	//IO×´Ì¬ÉèÖÃPA2\PA3	  

	RCC->APB1ENR|=1<<17;  	//Ê¹ÄÜ´®¿ÚÊ±ÖÓ 
		  
	RCC->APB1RSTR|=1<<17;   //¸´Î»´®¿Ú2
	RCC->APB1RSTR&=~(1<<17);//Í£Ö¹¸´Î»	   	   
	//²¨ÌØÂÊÉèÖÃ
 	USART2->BRR=mantissa; 	// ²¨ÌØÂÊÉèÖÃ	 
	USART2->CR1|=0X200C;  	//1Î»Í£Ö¹,ÎÞÐ£ÑéÎ».
#ifdef EN_USART2_RX		  	//Èç¹ûÊ¹ÄÜÁË½ÓÊÕ
	//Ê¹ÄÜ½ÓÊÕÖÐ¶Ï
	USART2->CR1|=1<<8;    	//PEÖÐ¶ÏÊ¹ÄÜ
	USART2->CR1|=1<<5;    	//½ÓÊÕ»º³åÇø·Ç¿ÕÖÐ¶ÏÊ¹ÄÜ	    	
	//	MY_NVIC_Init(3,3,USART2_IRQn,2);//×é2£¬×îµÍÓÅÏÈ¼¶ 
#endif
		//RS485_TX_EN=0;			//Ä¬ÈÏÎª½ÓÊÕÄ£Ê½	
		GPIOA->BSRR&=~(0x00100010);
		GPIOA->BSRR|=0x00000001<<20;	
}

void USART2_NVIC_INIT()
{
	SCB->AIRCR &=0x05faf8ff;
	SCB->AIRCR |=0x05fa0400;
	
	NVIC->ISER[1] |=1<<6;
  NVIC->IP[37]=0x30;
}
//RS485·¢ËÍlen¸ö×Ö½Ú.
//buf:·¢ËÍÇøÊ×µØÖ·
//len:·¢ËÍµÄ×Ö½ÚÊý(ÎªÁËºÍ±¾´úÂëµÄ½ÓÊÕÆ¥Åä,ÕâÀï½¨Òé²»Òª³¬¹ý64¸ö×Ö½Ú)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
//	RS485_TX_EN=1;			//ÉèÖÃÎª·¢ËÍÄ£Ê½
		GPIOA->BSRR&=~(0x00100010);
		GPIOA->BSRR|=0x00000001<<4;	
	
  	for(t=0;t<len;t++)		//Ñ­»··¢ËÍÊý¾Ý
	{
		while((USART2->SR&0X40)==0);//µÈ´ý·¢ËÍ½áÊø		  
		USART2->DR=buf[t];
	}	 
	while((USART2->SR&0X40)==0);//µÈ´ý·¢ËÍ½áÊø	
	RS485_RX_CNT=0;	  
//	RS485_TX_EN=0;				//ÉèÖÃÎª½ÓÊÕÄ£Ê½	
	GPIOA->BSRR&=~(0x00100010);
		GPIOA->BSRR|=0x00000001<<20;	
}

void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//Ä¬ÈÏÎª0
	Delayms(10);		//µÈ´ý10ms,Á¬Ðø³¬¹ý10msÃ»ÓÐ½ÓÊÕµ½Ò»¸öÊý¾Ý,ÔòÈÏÎª½ÓÊÕ½áÊø
	if(rxlen==RS485_RX_CNT&&rxlen)//½ÓÊÕµ½ÁËÊý¾Ý,ÇÒ½ÓÊÕÍê³ÉÁË
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//¼ÇÂ¼±¾´ÎÊý¾Ý³¤¶È
		RS485_RX_CNT=0;		//ÇåÁã
	}
}
