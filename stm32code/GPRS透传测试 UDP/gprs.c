#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "string.h"
#include "gprs.h"
#include <stdlib.h>
#include <stdio.h>

char *strx=0,*stry=0,*Readystrx;

void clear_buffer(void)//清空缓存
{
		u8 i;
		//UART1_SendStr((char*)buf);
		for(i=0;i<cnt;i++)
		buf[i]=0;//缓存
		cnt=0;
		
}

void GSM_Init(void)
{
	//int i;
	printf("ATE0&W\r\n"); //关闭回显
	Delayms(100);
	printf("AT\r\n");
	Delayms(100);
	strx=strstr((const char*)buf,(const char*)"OK");//返回OK
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT\r\n"); 
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//返回OK
		}
		
	clear_buffer();
	printf("AT+CSQ\r\n"); //检查CSQ，可以利用手机APP查看CSQ值
	Delayms(100);
		/////////////////////////////////
		printf("AT+CPIN?\r\n");//检查SIM卡是否在位
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"+CPIN: READY");//查看是否返回ready
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+CPIN?\r\n");
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"+CPIN: READY");//检查SIM卡是否在位，等待卡在位，如果卡识别不到，剩余的工作就没法做了
		}
		
		clear_buffer();	
	
		///////////////////////////////////
		/*printf(" AT+CREG?\r\n");//查看是否注册GSM网络
		Delayms(500);
		strx=strstr((const char*)buf,(const char*)"+CREG: 0,0");//返回正常
		while(strx==NULL)
		{
				clear_buffer();
				printf(" AT+CREG?\r\n");//查看是否注册GSM网络
				Delayms(500);
				strx=strstr((const char*)buf,(const char*)"+CREG: 0,0");//返回正常
		}*/
		clear_buffer();
		/////////////////////////////////////
		/*printf(" AT+CGREG?\r\n");//查看是否注册GPRS网络
		Delayms(500);
		strx=strstr((const char*)buf,(const char*)"+CGREG: 0,2");//，这里重要，只有注册成功，才可以进行GPRS数据传输。
		while(strx==NULL)
		{
				clear_buffer();
				printf(" AT+CGREG?\r\n");//查看是否注册GPRS网络
				Delayms(500);
				strx=strstr((const char*)buf,(const char*)"+CGREG: 0,2");//，这里重要，只有注册成功，才可以进行GPRS数据传输。
		}*/
		clear_buffer();
		printf("AT+QIDEACT\r\n");//关闭移动场景，意思就是关闭相关的数据连接//关闭GPRS/CSD PDP场景
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//查看是否成功关闭
		while(strx==NULL)//关闭失败，继续关闭
		{
				clear_buffer();
				printf("AT+QIDEACT\r\n");
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//关闭成功
		}
		clear_buffer();
		
		printf("AT+QIFGCNT=0\r\n");//配置前置场景，为后面的数据连接做基础
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//开启成功
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIFGCNT=0\r\n");
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//开启成功
		}
		clear_buffer();
		printf("AT+QICSGP=1,\042CMIOT\042\r\n");//接入APN，
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//开启成功
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QICSGP=1,\042CMIOT\042\r\n");//接入APN，
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");////开启成功
		}
		clear_buffer();
		printf("AT+QIREGAPP\r\n");//开启移动场景
			Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//开启成功
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIREGAPP\r\n");//开启移动场景//启动任务
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//开启成功
		}
		clear_buffer();
		printf("AT+QIACT\r\n");//PDP上下文激活//激活移动场景发起GPRS、CSD无线连接
			Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//开启成功
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIACT\r\n");//PDP激活
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//开启成功
		}
		clear_buffer();
		printf("AT+QIHEAD=1\r\n");//打开IPD接收头
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//开启成功
				while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIHEAD=1\r\n");//打开IPD接收头
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//开启成功
		}
		clear_buffer();
		printf("AT+QIDNSIP=0\r\n");//使用IP连接还是域名连接，0是IP 1是域名
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//设置ok
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIDNSIP=0\r\n");
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//设置ok
		}
		clear_buffer();
		//printf("AT+QIOPEN=\042TCP\042,\04247.92.7.47\042,\0428999\042\r\n");//这里是需要登陆的IP号码，输入公网的IP以及端口号就可以了。
		//printf("AT+QIOPEN=\042UDP\042,\042121.40.103.153\042,\04212307\042\r\n");
		//printf("AT+QIOPEN=\042UDP\042,\04247.92.30.216\042,\04212307\042\r\n");
		printf("AT+QIOPEN=\042UDP\042,\042106.15.178.106\042,\0428999\042\r\n");
		Delayms(100);
		
		strx=strstr((const char*)buf,(const char*)"OK");//检查是否登陆成功
		while(strx==NULL&&Readystrx==NULL)
		{
			strx=strstr((const char*)buf,(const char*)"CONNECT OK");//判断连接状态
			Readystrx=strstr((const char*)buf,(const char*)"ALREADY CONNECT"); //已经连接状态
			Delayms(100);
		}
		clear_buffer();
}

void TcpReLink(void)
{			
		clear_buffer();
		printf("AT+QIDEACT\r\n");//关闭移动场景，意思就是关闭相关的数据连接//关闭GPRS/CSD PDP场景
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//查看是否成功关闭
		while(strx==NULL)//关闭失败，继续关闭
		{
				clear_buffer();
				printf("AT+QIDEACT\r\n");
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//关闭成功
		}
		clear_buffer();
		
		printf("AT+QIFGCNT=0\r\n");//配置前置场景，为后面的数据连接做基础
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//开启成功
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIFGCNT=0\r\n");
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//开启成功
		}
		clear_buffer();
		printf("AT+QICSGP=1,\042CMIOT\042\r\n");//接入APN，
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//开启成功
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QICSGP=1,\042CMIOT\042\r\n");//接入APN，
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");////开启成功
		}
		clear_buffer();
		printf("AT+QIREGAPP\r\n");//开启移动场景
			Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//开启成功
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIREGAPP\r\n");//开启移动场景//启动任务
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//开启成功
		}
		clear_buffer();
		printf("AT+QIACT\r\n");//PDP上下文激活//激活移动场景发起GPRS、CSD无线连接
			Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//开启成功
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIACT\r\n");//PDP激活
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//开启成功
		}
		clear_buffer();
		printf("AT+QIHEAD=1\r\n");//打开IPD接收头
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//开启成功
				while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIHEAD=1\r\n");//打开IPD接收头
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//开启成功
		}
		clear_buffer();
		printf("AT+QIDNSIP=0\r\n");//使用IP连接还是域名连接，0是IP 1是域名
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//设置ok
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIDNSIP=0\r\n");
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//设置ok
		}
		clear_buffer();
		//printf("AT+QIOPEN=\042TCP\042,\04247.92.7.47\042,\0428999\042\r\n");//这里是需要登陆的IP号码，输入公网的IP以及端口号就可以了。
		//printf("AT+QIOPEN=\042UDP\042,\042121.40.103.153\042,\04212307\042\r\n");
		//printf("AT+QIOPEN=\042UDP\042,\04247.92.30.216\042,\04212307\042\r\n");
		printf("AT+QIOPEN=\042UDP\042,\042106.15.178.106\042,\0428999\042\r\n");
		Delayms(100);
		
		strx=strstr((const char*)buf,(const char*)"OK");//检查是否登陆成功
		while(strx==NULL&&Readystrx==NULL)
		{
			strx=strstr((const char*)buf,(const char*)"CONNECT OK");//判断连接状态
			Readystrx=strstr((const char*)buf,(const char*)"ALREADY CONNECT"); //已经连接状态
			Delayms(100);
		}
		clear_buffer();
}

void Send_Str(u8* data,u8 len)
{
	u8 i;
	char str[3];
	//u8 untildata=0xff;//发送剩余字节数
	clear_buffer();
	sprintf(str,"%d",len);
	//itoa(len, str, 10);
	RCC->APB1ENR&=~(1<<2);
	Uart1_PutChar('G');
	printf("AT+QISEND=%s\r\n",str);//发送函数
	Delayms(100);
	for(i=0;i<len;i++)
	{
		Uart2_PutChar(data[i]);
		Delayms(100);	
//		USART_SendData(USART2, (u8) 0x1a);//发送完成函数
//		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
//		{

//		}
  }
//		Delayms(100);	
//		USART_SendData(USART2, (u8) 0x1a);//发送完成函数
//		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
//		{

//		}
Delayms(100);
strx=strstr((char*)buf,(char*)"SEND OK");//是否正确发送
while(strx==NULL)
{
		strx=strstr((char*)buf,(char*)"SEND OK");//是否正确发送
		Delayms(10);
}
Delayms(100);
clear_buffer();
RCC->APB1ENR|=1<<2;	//TIM4时钟使能 

//printf("AT+QISACK\r\n");
//strx=strstr((char*)buf,(char*)"+QISACK");//发送剩余字节数据
//	while(untildata)
//	{
//		printf("AT+QISACK\r\n");
//		Delayms(100);
//		strx=strstr((char*)buf,(char*)"+QISACK");//发送剩余字节数据
//		if(strx)
//		{
//				strx=strstr((char*)strx,(char*)",");//获取第一个,
//				strx=strstr((char*)(strx+1),(char*)",");//获取第二个,
//				untildata=*(strx+2)-0x30;
//		}
//	}
		//clear_buffer();	
	T1=0;
}  

void ConnectJudgment()
{
	u8 i,a[15];
	
//		RCC->APB1ENR&=~(1<<2);
	printf("AT+QISTAT\r\n");
	
		Delayms(500);
		strx=strstr((const char*)buf,(const char*)"IP CLOSE");//查看是否成功关闭
	  stry=strstr((const char*)buf,(const char*)"PDP DEACT");
	
	for(i=0;i<cnt;i++)
	  Uart1_PutChar(buf[i]);
		if((strx!=NULL)||(stry!=NULL))//关闭失败，继续关闭
		{
		    TcpReLink();
				Uart1_PutChar('D');
		}

		clear_buffer();
//		RCC->APB1ENR|=1<<2;	//TIM4时钟使能 
//		Delayms(100);
		T1=0;
}

void DisLink()
{
	clear_buffer();
		printf("AT+QICLOSE\r\n");//关闭tcp链接
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"CLOSE OK");//查看是否成功关闭
		while(strx==NULL)//关闭失败，继续关闭
		{
				clear_buffer();
				printf("AT+QIDEACT\r\n");
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//关闭成功
		}
		clear_buffer();
		Delayms(100);
}