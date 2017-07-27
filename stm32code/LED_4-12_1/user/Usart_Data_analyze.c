#include "stm32f10x.h"
#include "Usart_Data_analyze.h"
#include "string.h"
#include "pwm.h"
#include "flash.h"
#include "usart.h"
#include "crc.h"
#include "rtc.h"
#include "rs485.h"
#include "adc.h"
#include "string.h"
#include <stdlib.h>
#include "delay.h"
#include "rs485.h"

u8 id[5]={'\0'},debug_data[10][10]={NULL};
u8 g_DimCrv_flag = 0;
u16 bb = 0;
u16 g_PWMCoe = 0;
u8 g_PWMValue;
u8 first=0,array_flag=3,g_DimNum,g_REFU_ENROLL=0;
u8 g_GetHrtBtData_flag = 0;
u8 g_EnHrtBtDat_flag = 0;
u8 m_HrtBtData[50];
u8 m_GetADC_flag = 0;
u8 g_GetVolt_flag = 0;
u8 g_GetCur_flag = 0;

//获取PWM对应调光参数
void analyze_PWMcxe(void)  
{
	u16 a, b, i;
	u16 *addr;
	u16  Bright[2][20] = {{0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38},
						{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
							
	addr = (u16*)0x0800F400;   //恒照度曲线存储地址
												
	for(i=0; i<20; i++)
	{
		Bright[1][i] = *addr;
		addr++;
	}				
	
	b = BKP->DR1;	   //累计工作时间
	
	a = b / 1000;
	if(b%1000 > 500)
	{
		a++;
	}
	if(a > 38)
	{
		g_PWMCoe = 100;
	}
	
	for(i=0; i<20; i++)
	{
		if(a == 0)
		{
			g_PWMCoe = Bright[1][0];	
			break;
		}
		
		if(Bright[0][i] > a)
		{
			if(i == 0)
			{
				g_PWMCoe = Bright[1][0];	
				break;
			}
			g_PWMCoe = Bright[1][i-1];
			break;
		}
		i++;
	}

}

//调试数据处理函数
void Analyze_rec_data(u8 data[],u8 num)
{
	
	u8 *nod,i=0,j=0;
	
	nod=data+2;
	while(*nod!=' ')
	{	
		for(i=0;i<8;i++)
		{
			while(*nod!=',')
			{
				debug_data[i][j]=*nod;
				j++;
				nod++; 
			}
				nod++;
				j=0;
		}	
	}
	for(i=0;i<8;i++){}
//	printf("AW%sAW",debug_data[i]);
}



//自定义格式数据处理函数
void Analyze_Rec_Frame(u8 data[], u8 num)
{
	u8 i;
	u16   short_ID = BKP->DR2;
	u32   long_ID[6];
	
	g_Data_Deal.dev_addr = data[3];    //地址
	g_Data_Deal.dev_addr = (g_Data_Deal.dev_addr << 8) & 0xff00;
	g_Data_Deal.dev_addr |= data[2];    
	g_Data_Deal.command = data[4];      
	g_Data_Deal.data_len = data[5];
	memset(g_Data_Deal.Use_data, 0, sizeof(g_Data_Deal.Use_data));
	for(i=0; i<data[5]; i++)
	g_Data_Deal.Use_data[i] = data[6+i];
	
	if((g_Data_Deal.dev_addr == short_ID) || (g_Data_Deal.dev_addr == 0x0000))
	{
		if(short_ID == 0xffff)
		{
			//设备唯一标识符
			long_ID[0] = (*( u32 *)0x1FFFF7E8); 
			long_ID[1] = (*( u32 *)0x1FFFF7EC);
			long_ID[2] = (*( u32 *)0x1FFFF7EF);//获取器件唯一标识符
			
			//接收标识符
			long_ID[3] = (g_Data_Deal.Use_data[11] << 24) | (g_Data_Deal.Use_data[10] << 16)
						 | (g_Data_Deal.Use_data[9] << 8) | g_Data_Deal.Use_data[8];
			long_ID[4] = (g_Data_Deal.Use_data[7] << 24) | (g_Data_Deal.Use_data[6] << 16)
						 | (g_Data_Deal.Use_data[5] << 8) | g_Data_Deal.Use_data[4];
			long_ID[5] = (g_Data_Deal.Use_data[3] << 24) | (g_Data_Deal.Use_data[2] << 16)
						 | (g_Data_Deal.Use_data[1] << 8) | g_Data_Deal.Use_data[0];
			
			if((long_ID[0] == long_ID[5]) && (long_ID[1] == long_ID[4]) && (long_ID[2] == long_ID[3]))
			{
				switch(g_Data_Deal.command)
				{
					case ALLOT_ID	: 	Allot_Id();        //分配ID
										break;
					case REFUSE_CALL:   Refuse_Call();     //拒绝访问
										break;
				}
			}
		}
		else
		{
			if(g_Data_Deal.command == DEVICE_WAKE)
				Device_wake();      //设备唤醒
			if(g_DviceSleep == 0)
			{
				switch(g_Data_Deal.command)
				{
					case VC_VAL     : 	Vc_Val();          //电压电流值
										break;
					case SET_TIME	:	Set_Time();        //设置时间
										break;
					case INIT_ADDR	:   Init_Addr();       //初始化短地址
										break;
					case FIND_ID	   : Find_Id();          //设备ID查询(96)
										 break;
					case CONSTANT_ILLUM: Constant_Illum();   //恒照度曲线
										 break;
					case FIND_ILLUM    : Find_Illum();       //恒照度曲线查询
										 break;
					case ADJUST_CURVE  : Adjust_Curve();     //调光曲线
										 break;
					case CTRL_REAL_TIME: Ctrl_Real_Time();   //实时控制
										 break;
					case FIND_TIME     : Find_Time();        //查询实时时间
										 break;
					case CLOSE_CTRL    : Close_Ctrl();       //关闭实时控制
										 break;
					case FIND_CURVE    : Find_Curve();       //查询调光曲线
										 break;
					case DEVICE_SLEEP  : Device_Sleep();     //设备休眠
										 break;
					case SET_UPLOADTIME  : set_hertdatauptime();     //设置心跳数据上传时间和注册时间
										 break;					 
					case FIND_UPLOADTIME :backhertdatatime();//查询心跳数据上传时间和注册时间
					         break;
				}
			}
			
		}
	}
}


void Pack_Data(u8* data,u8 len,u8 cmd)
{
	u8 packdata[50], x, y=6, i;
	u16 crc;
	
	packdata[0] = 0x5a;
	packdata[1] = 0x3c;
	if(cmd==0x00)
	{
		packdata[2] = 0xff;
		packdata[3] = 0xff;
	}else
	{
		packdata[2] = BKP->DR2 & 0xff;
		packdata[3] = (BKP->DR2>>8) & 0xff;
	}
	packdata[4] = cmd;
	packdata[5] = len;
	
	for(x=0;x<len;x++)
	{
		packdata[y] = data[x];
		y++;
	}
	crc = Get_Crc16(packdata,y);

	packdata[y]=crc&0xff;
	y++;
	packdata[y]=(crc>>8)&0xff;
	y++;
	if((packdata[2]==0xff)&&(packdata[3]==0xff))
	{
		if((cmd==0x00)||(cmd==0x17)||(cmd==0x19))
		{
			i = USART1->SR;        //避免第一次数据丢失
			for(x=0;x<y;x++)
			{
				g_URT1_TxBuf[x] = packdata[x];
			}
			g_URT1_TxLen = y;
			Uart1_PutChar(g_URT1_TxBuf[0]);
		}
	}
	else
	{
		i = USART1->SR;         //避免第一次数据丢失
		for(x=0;x<y;x++)
		{
			g_URT1_TxBuf[x] = packdata[x];
		}
		g_URT1_TxLen = y;
		Uart1_PutChar(g_URT1_TxBuf[0]);
	}
}	

//判读是否要执行注册
//返回值1：需要注册
//返回值0：不需要注册
u8 CheckEnroll(void)
{
	u8 val;
	if(BKP->DR2 == 0xFFFF)
		if(g_REFU_ENROLL == 0)
			val=1;		//需要注册
		else 
			val=0;		//不需要注册
	else 
		val=0;			//不需要注册
	
	return val;
}

u8 Register_shortaddr()//设备短地址注册
{
	u32 U_ID[3];
	u8 crc_cheak[20],a,b,c,d,i,j=0;
	u8 val;

	U_ID[0]=(*( u32 *)0x1FFFF7E8); 
	U_ID[1]=(*( u32 *)0x1FFFF7EC);
	U_ID[2]=(*( u32 *)0x1FFFF7EF);//获取器件唯一标识符

	for(i=0; i<3; i++)
	{
		a=b=c=d=0xff;
		a &= U_ID[i];
		crc_cheak[j] = a;
		j++;
		b &= U_ID[i] >> 8;
		crc_cheak[j] = b;
		j++;
		c &= U_ID[i] >> 16;
		crc_cheak[j] = c;
		j++;
		d &= U_ID[i] >> 24;
		crc_cheak[j] = d;
		j++;
	}
	if(g_URT1_TxEn == 0)
	{
		Pack_Data(crc_cheak,0x0c,0x00);
		val = 1;
	}
	else 
		val = 0;
	
	return val;
}

void Heart_Timer(void)
{
	u16 heartdata_uploadtime=0;
	
	g_EnHrtBtDat_flag = 1;
	
	heartdata_uploadtime = BKP->DR4;
	vtimer_SetTimer(VTIM2,heartdata_uploadtime,Heart_Timer);
}

void Operate_LED()//根据调光曲线操作LED灯
{
	u8 i;
	u16 *p;
	u32 line_time[10][2] = {0};
	u32 now_time = 0;
		
	p=(u16*)0x0800F802;//曲线参数起始地址
	
	Rtc_Get();
		
	//存储曲线
	if(BKP->DR3 <= 10)
	{
		for(i=0; i<BKP->DR3; i++)
		{
			line_time[i][0] = (*(p+3*i))*3600 + (*(p+1+3*i))*60;  //时间
			line_time[i][1] = *(p+2+3*i);                         //PWM
		}
	}
		
	now_time = timer.hour*3600 + timer.minute*60;
	analyze_PWMcxe();
	for(i=0; i<BKP->DR3; i++)
	{
		u8 a;
		a = BKP->DR3;
		if(i == a-1)
		{
			if((now_time>=line_time[i][0]) && (now_time<=(u32)86400))
			{
				if(line_time[i][1]==0)
				{
					Led_Toggle('f');
					g_PWMValue = 0;
					pwmval(0);
				}
				else
				{
					Led_Toggle('n');
					g_PWMValue = line_time[i][1];
					pwmval((u16)((g_PWMCoe/100.0)*(line_time[i][1])));
				}
			}
			break;
		}
		if((now_time>=line_time[i][0]) && (now_time<line_time[i+1][0]))
		{	
			if(line_time[i][1]==0)
			{
				Led_Toggle('f');
				g_PWMValue = 0;
				pwmval(0);
			}
			else
			{
				Led_Toggle('n');
				g_PWMValue = line_time[i][1];
				pwmval((u16)((g_PWMCoe/100.0)*(line_time[i][1])));
			}
		}	
	}
}

void Total_worktime()//累计工作时间存储
{	

	u16 Accum_time;
	
	Accum_time = BKP->DR1;
	Accum_time++;
	BKP->DR1 &= 0x0000;
	BKP->DR1 |= Accum_time;	
	g_Bkp_flag = 0;

}

void Get_HeartBeatData()
{
	u8 i;
    u16 *l;
	u16 totaltime;
	
    l = (u16*)0x0800F800;//曲线ID
	
	totaltime = BKP->DR1;
		
	Rtc_Get();
	
	if(g_DimCrv_flag == 1)  m_HrtBtData[0] &= 0x0f;
	else m_HrtBtData[0] |= 0x10;
	if(GPIOB->ODR & 0X0100)   m_HrtBtData[0] &= 0xf0;
	else m_HrtBtData[0] |= 0x01;
	
	m_HrtBtData[1]=*l;	
	m_HrtBtData[6] = g_PWMValue & 0xff;
	m_HrtBtData[7] = totaltime & 0xff;
	m_HrtBtData[8] = totaltime>>8 & 0xff;
	m_HrtBtData[9] = timer.hour & 0xff;
	m_HrtBtData[10] = timer.minute & 0xff;
	m_HrtBtData[11] = timer.second & 0xff;
	
}

void RS485_RxDatDeal()
{
	u16 crc;
	
	crc = Get_Crc(g_RS485_RxBuf,7);
	if(crc==0)
	{
		m_HrtBtData[12] = g_RS485_RxBuf[4];
		m_HrtBtData[13] = g_RS485_RxBuf[1];
		m_HrtBtData[14] = g_RS485_RxBuf[0];
		m_HrtBtData[15] = g_RS485_RxBuf[3];
		m_HrtBtData[16] = g_RS485_RxBuf[2];
		RS485_Send_Data(g_RS485_RxBuf,g_RS485_RxCnt);
	}
	
	g_RS485_RxCnt = 0;	
	g_RS485_RxFlag = 0;
}

void En_GetData()
{
	g_GetHrtBtData_flag = 1;
	vtimer_SetTimer(VTIM3, 1000, En_GetData);
}

void Get_Volt()
{
	u16 U;
	u16 *flash_u;
	flash_u=(u16*)0X0800FC02;

	U = Get_ADC();
	U *= (float)(*flash_u) / 100;
//	U *= (float)1.12;

	m_HrtBtData[2]=((u16)U)&0xff;
	m_HrtBtData[3]=((u16)U)>>8&0xff;
}

void Get_Cur()
{
	u16 I;
	u16 *flash_i;
	flash_i=(u16*)0X0800FC04;
	
	I = Get_ADC();
	I *= (float)(*flash_i);

	m_HrtBtData[4]=((u16)I)&0xff;
	m_HrtBtData[5]=((u16)I)>>8&0xff;
}

void En_GetADC()
{
	if(m_GetADC_flag == 0) 
	{
		vtimer_KillTimer(VTIM6);
		ADC1->SQR3 &= 0XFFFFFFE0;//规则序列1 通道ch
		ADC1->SQR3 |= 0;                          
		ADC1->CR2 |= 1<<22;//启动规则转换通道
		m_GetADC_flag = 1;
		vtimer_SetTimer(VTIM5, 100, En_GetCur);
	}
	else
	{		
		vtimer_KillTimer(VTIM5);
		ADC1->SQR3 &= 0XFFFFFFE0;//规则序列1 通道ch
		ADC1->SQR3 |= 1;                          
		ADC1->CR2 |= 1<<22;//启动规则转换通道
		m_GetADC_flag = 0;
		vtimer_SetTimer(VTIM6, 100, En_GetVlot);
	}
	vtimer_SetTimer(VTIM4, 1000, En_GetADC);
}

void En_GetVlot()
{
	g_GetVolt_flag = 1;
	vtimer_SetTimer(VTIM5, 100, En_GetVlot);
}
void En_GetCur()
{
	g_GetCur_flag = 1;
	vtimer_SetTimer(VTIM6, 100, En_GetCur);
}

u8 Upload_HeartbeatData()//上传心跳数据
{
	u8 val;
	
	if(g_URT1_TxEn == 0)
	{
		Pack_Data(m_HrtBtData,0x11,0x03);
		val = 1;
	}
	else
		val = 0;
	return val;
}

void Handle_ReceiveData()//处理接收到的数据
{
	u8 i;
	u16 crc;
	
	
	if((g_URT1_RxBuf[0]==0x5a) && (g_URT1_RxBuf[1]==0x3c))
	{
		
		crc = Get_Crc16(g_URT1_RxBuf,g_URT1_RxLen);
		
		if(crc == 0)
		{
			Analyze_Rec_Frame(g_URT1_RxBuf,g_URT1_RxLen);
		}
		
		for(i=0; i<g_URT1_RxLen; i++)
		{	
			g_URT1_RxBuf[i]='\0';
		}
		
		g_URT1_RxLen = 0;
	}
	else
	{
		if(g_URT1_RxBuf[0]=='a')
		{
			Delayms(500);
		}
		else
		{
			for(i=0; i<g_URT1_RxLen; i++)
			{	
				g_URT1_RxBuf[i]='\0';
			}
			g_URT1_RxLen = 0;
		}
	}
	g_URT1_RxFlag = 0;
}
