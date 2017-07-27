#include "cmd.h"

u16 *m_ConIllAddr;
u8 g_DviceSleep = 0;

void Allot_Id()                 //分配ID
{
	u8 i, j=0, id1[12];
	u16 id = 0;
	u32 U_ID[4];
	
	U_ID[0] = (*( u32 *)0x1FFFF7E8); 
	U_ID[1] = (*( u32 *)0x1FFFF7EC);
	U_ID[2] = (*( u32 *)0x1FFFF7EF);//获取器件唯一标识符
	
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
		if(g_Data_Deal.Use_data[i]==id1[i])
		{
			
		}
		else
			break;
	}
	if(i==12)
	{
		id |= g_Data_Deal.Use_data[13];
		id = id<<8;
		id |= g_Data_Deal.Use_data[12];
		BKP->DR2 &= 0x0000;
		BKP->DR2 |= id;
		//printf("0x%x",BKP->DR2);
	}
}

void Refuse_Call()                 //拒绝访问
{
	g_REFU_ENROLL = 1;
}

void Vc_Val()                     //电压电流系数
{
	u16 debug_data[2]={0},*a;
	u8 back[4]={0};
	u8 i;
	
	for(i=0; i<2; i++)
	{
		debug_data[i] |= g_Data_Deal.Use_data[2*i+1];
		debug_data[i] = debug_data[i]<<8;
		debug_data[i] |= g_Data_Deal.Use_data[2*i];
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

void Set_Time()                   //设置时间
{
	u16 debug_data1[6]={0};
	u8 back[7]={0};
	u8 i;

	for(i=0;i<6;i++)
	{
		debug_data1[i] |= g_Data_Deal.Use_data[2*i+1];
		debug_data1[i] = debug_data1[i]<<8;
		debug_data1[i] |= g_Data_Deal.Use_data[2*i];
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

void Init_Addr()                 //初始化短地址
{
	g_REFU_ENROLL = 0;
	BKP->DR2 = 0xffff;
//	g_EnRgstShrtAddr_flag = 0;
}

void Find_Id()                   //设备ID查询
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

void Constant_Illum()             //恒照度曲线
{
	u8 back[1];
	u8 i;
	
	m_ConIllAddr = (u16*)0x0800F400;
	
	STMFLASH_ErasePage(0x0800F400);
	for(i=0;i<g_Data_Deal.data_len;i++)
	{
		STMFLASH_WriteHalfWord((u16)g_Data_Deal.Use_data[i],m_ConIllAddr);
		m_ConIllAddr++;
	}
	back[0]=0x00;
	Pack_Data(back,0x01,0x07);
}

void Find_Illum()
{
	u8 back1[28];
	u8 i;
	
	m_ConIllAddr = (u16*)0x0800F400;
		
//	m_ConIllAddr++;
	for(i=0;i<20;i++)
	{
		back1[i]=*m_ConIllAddr;
		m_ConIllAddr++;
	}
	Pack_Data(back1,0x14,0x09);
}

void Adjust_Curve()              //调光曲线
{
	u8 back2[1];
	u16 *addr;
	u8 i;
			
	addr = (u16*)0x0800F800;
	g_DimNum = (g_Data_Deal.data_len-1)/0x3;
	BKP->DR3 &= 0x0000;
	BKP->DR3 |= g_DimNum;	

	STMFLASH_ErasePage(0x0800F800);
	for(i=0; i<g_Data_Deal.data_len; i++)
	{
		STMFLASH_WriteHalfWord((u16)g_Data_Deal.Use_data[i], addr);
		addr++;
	}

	back2[0]=0x00;
	Pack_Data(back2,0x01,0x0b);
}

void Ctrl_Real_Time()             //实时控制
{
	u8 back3[1];

	g_DimCrv_flag = 1;

	if(g_Data_Deal.Use_data[0]==0)
	{
		g_PWMValue = 0;
		Led_Toggle('f');
	}else
	{
		analyze_PWMcxe();
		g_PWMValue = g_Data_Deal.Use_data[0];
		Led_Toggle('n');
		pwmval((u16)((g_PWMCoe/100.0)*((u16)g_PWMValue)));
	}	

	back3[0] = g_PWMValue;
	Pack_Data(back3,0x01,0x0f);
}

void Find_Time()
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

void Close_Ctrl()
{
	g_DimCrv_flag = 0;
}

void Find_Curve()           //查询调光曲线
{
	u16 *addr;
	u8 i;
	u8 back1[31];
	
	addr = (u16*)0x0800F802;
		
	for(i=0; i<(BKP->DR3 * 3 + 1); i++)
	{
		back1[i] = *addr;
		addr++;
	}
	Pack_Data(back1,(BKP->DR3 * 3),0x0d);
}

void Device_Sleep()         //设备休眠
{
	if(g_Data_Deal.Use_data[0] == 0x35)
	{
		vtimer_KillTimer(VTIM2);
		g_EnHrtBtDat_flag = 0;
	}
	if(g_Data_Deal.Use_data[0] == 0x86)
	{
		vtimer_KillTimer(VTIM2);
		g_EnHrtBtDat_flag = 0;
		g_DviceSleep = 1;
	}
}

void Device_wake()          //设备唤醒
{
	u16 heartdata_uploadtime=0;
	
	g_DviceSleep = 0;
	heartdata_uploadtime=BKP->DR4;
	vtimer_SetTimer(VTIM2,heartdata_uploadtime,Heart_Timer);
}

void set_hertdatauptime()//设置上传数据时间和注册时间
{
  u16 heartdata_uploadtime=0;
	
	heartdata_uploadtime=g_Data_Deal.Use_data[1];
	heartdata_uploadtime=(heartdata_uploadtime<<8)&0xff00;
	
	heartdata_uploadtime|=g_Data_Deal.Use_data[0];
	BKP->DR4 &= 0x0000;
	BKP->DR4 |= heartdata_uploadtime;	
	
}

void backhertdatatime()//查询上传数据时间和注册时间
{
  u16 heartdata_uploadtime=0;
	u8 back1[2];
	
	heartdata_uploadtime=BKP->DR4;
	back1[0]=heartdata_uploadtime&0xff;
	back1[1]=(heartdata_uploadtime>>8)&0xff;
	
	Pack_Data(back1,0x02,0x1c);
}
