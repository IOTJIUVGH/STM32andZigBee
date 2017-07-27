#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "string.h"
#include "gprs.h"
#include <stdlib.h>
#include <stdio.h>

char *strx=0,*stry=0,*Readystrx;

void clear_buffer(void)//��ջ���
{
		u8 i;
		//UART1_SendStr((char*)buf);
		for(i=0;i<cnt;i++)
		buf[i]=0;//����
		cnt=0;
		
}

void GSM_Init(void)
{
	//int i;
	printf("ATE0&W\r\n"); //�رջ���
	Delayms(100);
	printf("AT\r\n");
	Delayms(100);
	strx=strstr((const char*)buf,(const char*)"OK");//����OK
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT\r\n"); 
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//����OK
		}
		
	clear_buffer();
	printf("AT+CSQ\r\n"); //���CSQ�����������ֻ�APP�鿴CSQֵ
	Delayms(100);
		/////////////////////////////////
		printf("AT+CPIN?\r\n");//���SIM���Ƿ���λ
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"+CPIN: READY");//�鿴�Ƿ񷵻�ready
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+CPIN?\r\n");
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"+CPIN: READY");//���SIM���Ƿ���λ���ȴ�����λ�������ʶ�𲻵���ʣ��Ĺ�����û������
		}
		
		clear_buffer();	
	
		///////////////////////////////////
		/*printf(" AT+CREG?\r\n");//�鿴�Ƿ�ע��GSM����
		Delayms(500);
		strx=strstr((const char*)buf,(const char*)"+CREG: 0,0");//��������
		while(strx==NULL)
		{
				clear_buffer();
				printf(" AT+CREG?\r\n");//�鿴�Ƿ�ע��GSM����
				Delayms(500);
				strx=strstr((const char*)buf,(const char*)"+CREG: 0,0");//��������
		}*/
		clear_buffer();
		/////////////////////////////////////
		/*printf(" AT+CGREG?\r\n");//�鿴�Ƿ�ע��GPRS����
		Delayms(500);
		strx=strstr((const char*)buf,(const char*)"+CGREG: 0,2");//��������Ҫ��ֻ��ע��ɹ����ſ��Խ���GPRS���ݴ��䡣
		while(strx==NULL)
		{
				clear_buffer();
				printf(" AT+CGREG?\r\n");//�鿴�Ƿ�ע��GPRS����
				Delayms(500);
				strx=strstr((const char*)buf,(const char*)"+CGREG: 0,2");//��������Ҫ��ֻ��ע��ɹ����ſ��Խ���GPRS���ݴ��䡣
		}*/
		clear_buffer();
		printf("AT+QIDEACT\r\n");//�ر��ƶ���������˼���ǹر���ص���������//�ر�GPRS/CSD PDP����
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//�鿴�Ƿ�ɹ��ر�
		while(strx==NULL)//�ر�ʧ�ܣ������ر�
		{
				clear_buffer();
				printf("AT+QIDEACT\r\n");
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//�رճɹ�
		}
		clear_buffer();
		
		printf("AT+QIFGCNT=0\r\n");//����ǰ�ó�����Ϊ�������������������
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//�����ɹ�
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIFGCNT=0\r\n");
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//�����ɹ�
		}
		clear_buffer();
		printf("AT+QICSGP=1,\042CMIOT\042\r\n");//����APN��
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//�����ɹ�
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QICSGP=1,\042CMIOT\042\r\n");//����APN��
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");////�����ɹ�
		}
		clear_buffer();
		printf("AT+QIREGAPP\r\n");//�����ƶ�����
			Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//�����ɹ�
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIREGAPP\r\n");//�����ƶ�����//��������
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//�����ɹ�
		}
		clear_buffer();
		printf("AT+QIACT\r\n");//PDP�����ļ���//�����ƶ���������GPRS��CSD��������
			Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//�����ɹ�
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIACT\r\n");//PDP����
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//�����ɹ�
		}
		clear_buffer();
		printf("AT+QIHEAD=1\r\n");//��IPD����ͷ
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//�����ɹ�
				while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIHEAD=1\r\n");//��IPD����ͷ
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//�����ɹ�
		}
		clear_buffer();
		printf("AT+QIDNSIP=0\r\n");//ʹ��IP���ӻ����������ӣ�0��IP 1������
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//����ok
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIDNSIP=0\r\n");
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//����ok
		}
		clear_buffer();
		//printf("AT+QIOPEN=\042TCP\042,\04247.92.7.47\042,\0428999\042\r\n");//��������Ҫ��½��IP���룬���빫����IP�Լ��˿ںžͿ����ˡ�
		//printf("AT+QIOPEN=\042UDP\042,\042121.40.103.153\042,\04212307\042\r\n");
		//printf("AT+QIOPEN=\042UDP\042,\04247.92.30.216\042,\04212307\042\r\n");
		printf("AT+QIOPEN=\042UDP\042,\042106.15.178.106\042,\0428999\042\r\n");
		Delayms(100);
		
		strx=strstr((const char*)buf,(const char*)"OK");//����Ƿ��½�ɹ�
		while(strx==NULL&&Readystrx==NULL)
		{
			strx=strstr((const char*)buf,(const char*)"CONNECT OK");//�ж�����״̬
			Readystrx=strstr((const char*)buf,(const char*)"ALREADY CONNECT"); //�Ѿ�����״̬
			Delayms(100);
		}
		clear_buffer();
}

void TcpReLink(void)
{			
		clear_buffer();
		printf("AT+QIDEACT\r\n");//�ر��ƶ���������˼���ǹر���ص���������//�ر�GPRS/CSD PDP����
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//�鿴�Ƿ�ɹ��ر�
		while(strx==NULL)//�ر�ʧ�ܣ������ر�
		{
				clear_buffer();
				printf("AT+QIDEACT\r\n");
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//�رճɹ�
		}
		clear_buffer();
		
		printf("AT+QIFGCNT=0\r\n");//����ǰ�ó�����Ϊ�������������������
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//�����ɹ�
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIFGCNT=0\r\n");
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//�����ɹ�
		}
		clear_buffer();
		printf("AT+QICSGP=1,\042CMIOT\042\r\n");//����APN��
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//�����ɹ�
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QICSGP=1,\042CMIOT\042\r\n");//����APN��
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");////�����ɹ�
		}
		clear_buffer();
		printf("AT+QIREGAPP\r\n");//�����ƶ�����
			Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//�����ɹ�
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIREGAPP\r\n");//�����ƶ�����//��������
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//�����ɹ�
		}
		clear_buffer();
		printf("AT+QIACT\r\n");//PDP�����ļ���//�����ƶ���������GPRS��CSD��������
			Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//�����ɹ�
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIACT\r\n");//PDP����
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//�����ɹ�
		}
		clear_buffer();
		printf("AT+QIHEAD=1\r\n");//��IPD����ͷ
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//�����ɹ�
				while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIHEAD=1\r\n");//��IPD����ͷ
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//�����ɹ�
		}
		clear_buffer();
		printf("AT+QIDNSIP=0\r\n");//ʹ��IP���ӻ����������ӣ�0��IP 1������
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"OK");//����ok
		while(strx==NULL)
		{
				clear_buffer();
				printf("AT+QIDNSIP=0\r\n");
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//����ok
		}
		clear_buffer();
		//printf("AT+QIOPEN=\042TCP\042,\04247.92.7.47\042,\0428999\042\r\n");//��������Ҫ��½��IP���룬���빫����IP�Լ��˿ںžͿ����ˡ�
		//printf("AT+QIOPEN=\042UDP\042,\042121.40.103.153\042,\04212307\042\r\n");
		//printf("AT+QIOPEN=\042UDP\042,\04247.92.30.216\042,\04212307\042\r\n");
		printf("AT+QIOPEN=\042UDP\042,\042106.15.178.106\042,\0428999\042\r\n");
		Delayms(100);
		
		strx=strstr((const char*)buf,(const char*)"OK");//����Ƿ��½�ɹ�
		while(strx==NULL&&Readystrx==NULL)
		{
			strx=strstr((const char*)buf,(const char*)"CONNECT OK");//�ж�����״̬
			Readystrx=strstr((const char*)buf,(const char*)"ALREADY CONNECT"); //�Ѿ�����״̬
			Delayms(100);
		}
		clear_buffer();
}

void Send_Str(u8* data,u8 len)
{
	u8 i;
	char str[3];
	//u8 untildata=0xff;//����ʣ���ֽ���
	clear_buffer();
	sprintf(str,"%d",len);
	//itoa(len, str, 10);
	RCC->APB1ENR&=~(1<<2);
	Uart1_PutChar('G');
	printf("AT+QISEND=%s\r\n",str);//���ͺ���
	Delayms(100);
	for(i=0;i<len;i++)
	{
		Uart2_PutChar(data[i]);
		Delayms(100);	
//		USART_SendData(USART2, (u8) 0x1a);//������ɺ���
//		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
//		{

//		}
  }
//		Delayms(100);	
//		USART_SendData(USART2, (u8) 0x1a);//������ɺ���
//		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
//		{

//		}
Delayms(100);
strx=strstr((char*)buf,(char*)"SEND OK");//�Ƿ���ȷ����
while(strx==NULL)
{
		strx=strstr((char*)buf,(char*)"SEND OK");//�Ƿ���ȷ����
		Delayms(10);
}
Delayms(100);
clear_buffer();
RCC->APB1ENR|=1<<2;	//TIM4ʱ��ʹ�� 

//printf("AT+QISACK\r\n");
//strx=strstr((char*)buf,(char*)"+QISACK");//����ʣ���ֽ�����
//	while(untildata)
//	{
//		printf("AT+QISACK\r\n");
//		Delayms(100);
//		strx=strstr((char*)buf,(char*)"+QISACK");//����ʣ���ֽ�����
//		if(strx)
//		{
//				strx=strstr((char*)strx,(char*)",");//��ȡ��һ��,
//				strx=strstr((char*)(strx+1),(char*)",");//��ȡ�ڶ���,
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
		strx=strstr((const char*)buf,(const char*)"IP CLOSE");//�鿴�Ƿ�ɹ��ر�
	  stry=strstr((const char*)buf,(const char*)"PDP DEACT");
	
	for(i=0;i<cnt;i++)
	  Uart1_PutChar(buf[i]);
		if((strx!=NULL)||(stry!=NULL))//�ر�ʧ�ܣ������ر�
		{
		    TcpReLink();
				Uart1_PutChar('D');
		}

		clear_buffer();
//		RCC->APB1ENR|=1<<2;	//TIM4ʱ��ʹ�� 
//		Delayms(100);
		T1=0;
}

void DisLink()
{
	clear_buffer();
		printf("AT+QICLOSE\r\n");//�ر�tcp����
		Delayms(100);
		strx=strstr((const char*)buf,(const char*)"CLOSE OK");//�鿴�Ƿ�ɹ��ر�
		while(strx==NULL)//�ر�ʧ�ܣ������ر�
		{
				clear_buffer();
				printf("AT+QIDEACT\r\n");
				Delayms(100);
				strx=strstr((const char*)buf,(const char*)"OK");//�رճɹ�
		}
		clear_buffer();
		Delayms(100);
}