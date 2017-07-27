#include "stm32f10x.h"
#include "string.h"
#include "Usart_Data_analyze.h"
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

u8 id[5]={'\0'},debug_data[10][10]={NULL},RTC_Flag=0;;
u16 bb=0,pwm_coe=0;
u8 first=0,array_flag=3,pwm,dimmer_num,Dos_Flag=0;

void analyze_PWMcxe(void)
{
	u16 a,b,i;
	u16 *addr;
	
	
	u16  Bright[2][20]={{0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38},
											{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	addr=(u16*)0x0800F400;
												
	for(i=0;i<20;i++)
	{
		Bright[1][i]=*addr;
		addr++;
	}				
	
	b=BKP->DR1;	
	
	a=b/1000;
	if(b%1000>500)
	{
		a++;
	}
	if(a>38)
	{
		pwm_coe=100;
	}
	
	for(i=0;i<20;i++)
	{
		if(a==0)
		{
			pwm_coe=Bright[1][0];	
				break;
		}
		
		if(Bright[0][i]>a)
		{
			if(i==0)
			{
				pwm_coe=Bright[1][0];	
				break;
			}
			pwm_coe=Bright[1][i-1];
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
	for(i=0;i<8;i++)
	printf("AW%sAW",debug_data[i]);
}

//自定义格式数据处理函数
void Analyze_Rec_Frame(u8 data[],u8 num)
{
	struct Frame frame={0,0,0,0};
	u8 i;

	frame.dev_addr|=data[3];
	frame.dev_addr=frame.dev_addr<<8;
	frame.dev_addr|=data[2];
	frame.command=data[4];
	frame.data_len=data[5];
	for(i=0;i<data[5];i++)
	frame.Use_data[i]=data[6+i];
	
	Rec_Data_Handle(frame);
}


void Pack_Data(u8* data,u8 len,u8 cmd)
{
	u8 packdata[50],x,y=6;
	u16 crc;
	
	packdata[0]=0x5a;
	packdata[1]=0x3c;
	if(cmd==0x00)
	{
		packdata[2]=0xff;
		packdata[3]=0xff;
	}else
	{
		packdata[2]=BKP->DR2&0xff;
		packdata[3]=(BKP->DR2>>8)&0xff;
	}
	packdata[4]=cmd;
	packdata[5]=len;
	
	for(x=0;x<len;x++)
	{
		packdata[y]=data[x];
		y++;
	}
	crc=Get_Crc16(packdata,y);

	packdata[y]=crc&0xff;
	y++;
	packdata[y]=(crc>>8)&0xff;
	y++;
	if((packdata[2]==0xff)&&(packdata[3]==0xff))
		{
			if((cmd==0x00)||(cmd==0x17)||(cmd==0x19))
			{
				for(x=0;x<y;x++)
				Uart1_PutChar(packdata[x]);
			}
		}else
			{
				for(x=0;x<y;x++)
				Uart1_PutChar(packdata[x]);
			}
}
//处理后的数据具体操作
void Rec_Data_Handle(struct Frame frame1)
{
	u8 i=0;
	u16 *addr,*addr1,short_id;
	addr=(u16*)0x0800F800;
	addr1=(u16*)0x0800F400;
	
	if(frame1.dev_addr==BKP->DR2||frame1.dev_addr==0x0000||frame1.dev_addr==0xffff)
	{
		if(frame1.command==0x01)//分配设备ID
		{
			u8 i,j=0,id1[12];
			u16 id=0;
			u32 U_ID[4];
			
			U_ID[0]=(*( u32 *)0x1FFFF7E8); 
			U_ID[1]=(*( u32 *)0x1FFFF7EC);
			U_ID[2]=(*( u32 *)0x1FFFF7EF);//获取器件唯一标识符
			
			for(i=0;i<3;i++)
			{
				
				id1[j]=U_ID[i]&0xff;
				j++;
				id1[j]=(U_ID[i]>>8)&0xff;
				j++;
				id1[j]=(U_ID[i]>>16)&0xff;
				j++;
				id1[j]=(U_ID[i]>>24)&0xff;
				j++;
			}
			for(i=0;i<12;i++)
			{
				if(frame1.Use_data[i]==id1[i])
				{}
				else
					break;
			}
			if(i==12)
			{
				id|=frame1.Use_data[13];
				id=id<<8;
				id|=frame1.Use_data[12];
				BKP->DR2&=0x0000;
				BKP->DR2|=id;
				//printf("0x%x",BKP->DR2);
			}
		}
		
		if(frame1.command==0x02)//拒绝访问
		{
			Dos_Flag=1;
		}
		
		if(frame1.command==0x16)//电流电压系数
	{
		u16 debug_data[2]={0},*a;
		u8 back[4]={0};
		
		for(i=0;i<8;i++)
			{
				debug_data[i]|=frame1.Use_data[2*i+1];
				debug_data[i]=debug_data[i]<<8;
				debug_data[i]|=frame1.Use_data[2*i];
			}
			a=(u16*)0x0800FC02;
			DelPage(0x0800FC00);//调试数据存放地址
			WriteData(debug_data[0],a);
			WriteData(debug_data[1],a+1);		

			back[0]=(*a)&0xff;
			back[1]=((*a)>>8)&0xff;
			back[2]=(*(a+1))&0xff;
			back[3]=((*(a+1))>>8)&0xff;
		
			Pack_Data(back,0x04,0x17);
		
	}
	
	if(frame1.command==0x18)//设置时间
	{
		u16 debug_data1[6]={0};
		u8 back[7]={0};
		
		for(i=0;i<6;i++)
			{
				debug_data1[i]|=frame1.Use_data[2*i+1];
				debug_data1[i]=debug_data1[i]<<8;
				debug_data1[i]|=frame1.Use_data[2*i];
			}
			
			Rtc_Init();
			RTC__NVIC_Init();
			Rtc_TIME_Set(debug_data1[0],(u8)debug_data1[1],(u8)debug_data1[2],(u8)debug_data1[3],(u8)debug_data1[4],(u8)debug_data1[5]);		
			
			Rtc_Get();  
		
			back[0]=timer.year&0xff;
			back[1]=(timer.year>>8)&0xff;
			back[2]=timer.month;
			back[3]=timer.date;
			back[4]=timer.hour;
			back[5]=timer.minute;
			back[6]=timer.second;
		
			Pack_Data(back,0x07,0x19);
		}
	
	if(frame1.command==0x12)//初始化短地址
	{
		BKP->DR2|=0xffff;
	}
}
	
	short_id=BKP->DR2;
	if(short_id!=0xffff)
	{
		if(frame1.dev_addr==BKP->DR2||frame1.dev_addr==0x0000)
		{
			if(frame1.command==0x0a)//调光曲线
			{
				u8 back2[1];
				
				dimmer_num=(frame1.data_len-1)/0x3;
				BKP->DR3&=0x0000;
				BKP->DR3|=dimmer_num;	
			
				STMFLASH_ErasePage(0x0800F800);
				for(i=0;i<frame1.data_len;i++)
				{
					STMFLASH_WriteHalfWord((u16)frame1.Use_data[i],addr);
					addr++;
				}
	
				back2[0]=0x00;
				Pack_Data(back2,0x01,0x0b);
				
			}
		
		if(frame1.command==0x0e)//实时控制
		{
			u8 back3[1];

			RTC_Flag=1;
			if(RTC_Flag==1)
			{
				if(frame1.Use_data[0]==0)
				{
					pwm=0;
					LedToggle('f');
				}else
				{
					analyze_PWMcxe();
					pwm=frame1.Use_data[0];
					LedToggle('n');
					pwmval((u16)((pwm_coe/100.0)*((u16)pwm)));
				}	
			
				back3[0]=pwm;
				Pack_Data(back3,0x01,0x0f);
	
			}
		}
		
	
	if(frame1.command==0x04)//设备ID查询(96)
	{
		u8 i,j=0,id1[12];
		u32 U_ID[4]; 
		
			U_ID[0]=(*( u32 *)0x1FFFF7E8); 
			U_ID[1]=(*( u32 *)0x1FFFF7EC);
			U_ID[2]=(*( u32 *)0x1FFFF7EF);//获取器件唯一标识符
			
			for(i=0;i<3;i++)
			{
				id1[j]=U_ID[i]&0xff;
				j++;
				id1[j]=(U_ID[i]>>8)&0xff;
				j++;
				id1[j]=(U_ID[i]>>16)&0xff;
				j++;
				id1[j]=(U_ID[i]>>24)&0xff;
				j++;
			}
			Pack_Data(id1,0x0c,0x05);
	}		
	if(frame1.command==0x06)//恒照度曲线
	{
			u8 back[1];
	
			STMFLASH_ErasePage(0x0800F400);
			for(i=0;i<frame1.data_len;i++)
			{
				STMFLASH_WriteHalfWord((u16)frame1.Use_data[i],addr1);
				addr1++;
			}
			back[0]=0x00;
			Pack_Data(back,0x01,0x07);
	}
	
	if(frame1.command==0x08)//恒照度曲线查询
	{
		u8 back1[28];
		
		addr1++;
		for(i=0;i<20;i++)
		{
			back1[i]=*addr1;
			addr1++;
		}
		Pack_Data(back1,0x14,0x09);
	
	}
	
	
	
if(frame1.command==0x13)//查询实时时间
{
		u8 back[20]={0};	
	
		Rtc_Get();  

		back[0]=timer.year&0xff;
		back[1]=(timer.year>>8)&0xff;
		back[2]=timer.month;
		back[3]=timer.date;
		back[4]=timer.hour;
		back[5]=timer.minute;
		back[6]=timer.second;
	
		Pack_Data(back,0x07,0x15);
}
	
	if(frame1.command==0x14)//关闭实时控制
	{
		RTC_Flag=0;
	}
	
		
}
}
}	

void Register_shortaddr()//设备短地址注册
{
	u32 U_ID[3];
	u8 crc_cheak[20],a,b,c,d,i,j=0;

	if((BKP->DR2==0xffff)&&(id_flag==3)&&(Dos_Flag==0))//Dos_Flag是拒绝服务标志
		{
			U_ID[0]=(*( u32 *)0x1FFFF7E8); 
			U_ID[1]=(*( u32 *)0x1FFFF7EC);
			U_ID[2]=(*( u32 *)0x1FFFF7EF);//获取器件唯一标识符
	
			for(i=0;i<3;i++)
			{
				a=b=c=d=0xff;
				a&=U_ID[i];
				crc_cheak[j]=a;
				j++;
				b&=U_ID[i]>>8;
				crc_cheak[j]=b;
				j++;
				c&=U_ID[i]>>16;
				crc_cheak[j]=c;
				j++;
				d&=U_ID[i]>>24;
				crc_cheak[j]=d;
				j++;
			}
			Pack_Data(crc_cheak,0x0c,0x00);
			id_flag=0;
		}
}

void Operate_LED()//根据调光曲线操作LED灯
{
	u8 i;
	u16 *p;
	p=(u16*)0x0800F802;//曲线参数起始地址
	
	Rtc_Get();
//		for(i=0;i<dimmer_num;i++)
//		{
//			if((timer.hour==*(p+3*i))&&(timer.minute==*(p+1+3*i))&&(timer.second==1))
//			{
//				analyze_PWMcxe();
//				if(*(p+2+3*i)==0)
//				{
//					LedToggle('f');
//					pwm=0;
//				}else
//				{
//					LedToggle('n');
//					pwm=*(p+2+3*i);
//					pwmval((u16)((pwm_coe/100.0)*(pwm)));
//				}
//			}
//		}
		if(RTC_Flag==0)
		{
			u32 line_time[10][2]={0},now_time=0;
			
			//存储曲线
			if(BKP->DR3<=10)
			{
				for(i=0;i<BKP->DR3;i++)
				{
					line_time[i][0]=(*(p+3*i))*3600+(*(p+1+3*i))*60;
					line_time[i][1]=*(p+2+3*i);
				}
			}
			//排序
//			for(i=0;i<dimmer_num;i++)
//			{
//				for(x=i+1;x<5;j++)
//				if(line_time[i][0]<line_time[x][0])
//				{
//					a=line_time[i][0];
//					line_time[i][0]=line_time[x][0];
//					line_time[x][0]=a;
//					
//					b=line_time[i][1];
//					line_time[i][1]=line_time[x][1];
//					line_time[x][1]=b;
//				}
//			}
			
			now_time=timer.hour*3600+timer.minute*60;
			analyze_PWMcxe();
			for(i=0;i<BKP->DR3;i++)
			{
				u8 a;
				a=BKP->DR3;
				if(i==a-1)
				{
					if(now_time>=line_time[i][0]&&now_time<=(u32)86400)
					{
						if(line_time[i][1]==0)
						{
							LedToggle('f');
							pwm=0;
							pwmval(0);
						}
						else
						{
							LedToggle('n');
							pwm=line_time[i][1];
							pwmval((u16)((pwm_coe/100.0)*(line_time[i][1])));
						}
					}
					break;
				}
				if(now_time>=line_time[i][0]&&now_time<line_time[i+1][0])
				{	
					if(line_time[i][1]==0)
					{
						LedToggle('f');
						pwm=0;
					}
					else
					{
						LedToggle('n');
						pwm=line_time[i][1];
						pwmval((u16)((pwm_coe/100.0)*(line_time[i][1])));
					}
				}	
			}
		}
	}

void Total_worktime()//累计工作时间存储
{	
	if(bkp_flag==1)//累计工作时间
		{
			u16 Accum_time;
			
			Accum_time=BKP->DR1;
			Accum_time++;
			BKP->DR1&=0x0000;
			BKP->DR1|=Accum_time;	
			bkp_flag=0;
		}
}

void Upload_HeartbeatData()//上传心跳数据
{
	u8 i,crc_cheak1[50];
	u16 crc,*l;
	float I,U;//ADC变量
	
	l=(u16*)0x0800F800;//曲线ID
	if(send_flag==5)
		{
			u16 totaltime;
		
			U=Analyze_Adc_U_Data();
			I=Analyze_Adc_I_Data();
			
			U=U*10;
			I=I*10000;
			//printf("%d\r\n%d\r\n",(u16)I,(u16)U);
			 totaltime=BKP->DR1;
			
			Rtc_Get();
//			for(i=0;i<7;i++)
//			{
//				printf("%d\r\n",RS485_RX_BUF[i]);
//			}
		crc_cheak1[0]=0x01;
		crc_cheak1[1]=*l;	
		crc_cheak1[2]=((u16)U)&0xff;
		crc_cheak1[3]=((u16)U)>>8&0xff;
		crc_cheak1[4]=((u16)I)&0xff;
		crc_cheak1[5]=((u16)I)>>8&0xff;	
		crc_cheak1[6]=pwm&0xff;
		crc_cheak1[7]=totaltime&0xff;
		crc_cheak1[8]=totaltime>>8&0xff;
		crc_cheak1[9]=timer.hour&0xff;
		crc_cheak1[10]=timer.minute&0xff;
		crc_cheak1[11]=timer.second&0xff;
		
		crc=Get_Crc(RS485_RX_BUF,7);
			if(crc==0)
			{
				crc_cheak1[12]=RS485_RX_BUF[4];
				crc_cheak1[13]=RS485_RX_BUF[1];
				crc_cheak1[14]=RS485_RX_BUF[0];
				crc_cheak1[15]=RS485_RX_BUF[3];
				crc_cheak1[16]=RS485_RX_BUF[2];
			}
			Pack_Data(crc_cheak1,0x11,0x03);
//			printf("{\"ID\":\"%d\",\"U\":\"%.4f\",\"I\":\"%.4f\",\"PWM\":\"%d\",\"LEAK_I1\":\"%d\"}",*name,U,I,pwm,leak_i1);
//			Delayms(500);
//			printf("{\"ID\":\"%d\",\"LEAK_I2\":\"%d\",\"LINE\":\"%d\",\"WET\":\"%d\",\"OPEN_TIME\":\"%d\"}",*name,leak_i2,*l,wet,opentime);

			send_flag=0;
			for(i=0;i<7;i++)
			{	
				RS485_RX_BUF[i]=0x00;
			}
			RS485_RX_CNT=0;
		}
	}

void Handle_ReceiveData()//处理接收到的数据
{
	u8 i;
	u16 crc;
	
	if(TIMER3_flag==1)//定时控制数据的接收
	{
			
//			for(i=0;i<cnt;i++)
//		{
//			Uart1_PutChar(buf[i]);
//		
//		}
		if((buf[0]==0x5a)&&(buf[1]==0x3c))
		{
			u8 i;	
			
			crc=Get_Crc16(buf,cnt);
			if(crc==0)
			{
				
				Analyze_Rec_Frame(buf,cnt);
			}
			for(i=0;i<cnt;i++)
			{	
				buf[i]='\0';
			}
			cnt=0;
		}
		else
		{
			if(buf[0]=='a')
			{
				Delayms(500);
			}
			else
			{
				for(i=0;i<cnt;i++)
				{	
					buf[i]='\0';
				}
			cnt=0;
			}
		}
		TIMER3_flag=0;
	}
}
