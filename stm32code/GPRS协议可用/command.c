#include "stm32f10x.h"

void Second_AT_Command(char *b,char *a,u8 wait_time)         
{
	u8 i;
	char *c;
	c = b;										//保存字符串地址到c
	CLR_Buf(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find(a)) 
		{
				b = c;							//将字符串地址给b
				for (; *b!='\0';b++)
				{
					//while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
					Uart1_PutChar(*b);//UART2_SendData(*b);
				}
				
				Times = 0;
				shijian = wait_time;
				Timer0_start = 1;
		   
    }
 	  else
		{
			i = 1;
			Timer0_start = 0;
		}
	}
	CLR_Buf(); 
}

void CLR_Buf(void)
{
	u16 k;
	for(k=0;k<100;k++)      //将缓存内容清零
	{
		buf[k] = 0x00;
	}
}

u8 Find(char *a)
{ 
  if(strstr(buf,a)!=NULL)
	    return 1;
	else
			return 0;
}