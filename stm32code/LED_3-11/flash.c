#include "stm32f10x.h"
#include "delay.h"
u8 status;

void FlashunInit()
{
	FLASH->KEYR=0x45670123;
	FLASH->KEYR=0XCDEF89AB;

}

//flash上锁
void STMFLASH_Lock(void)
{
	FLASH->CR|=1<<7;//上锁
}
//得到FLASH状态
u8 STMFLASH_GetStatus(void)
{	
	u32 res;		
	res=FLASH->SR; 
	if(res&(1<<0))return 1;		    //忙
	else if(res&(1<<2))return 2;	//编程错误
	else if(res&(1<<4))return 3;	//写保护错误
	return 0;						//操作完成
}
//等待操作完成
//time:要延时的长短
//返回值:状态.
u8 STMFLASH_WaitDone(u16 time)
{
	u8 res;
	do
	{
		res=STMFLASH_GetStatus();
		if(res!=1)break;//非忙,无需等待了,直接退出.
		Delayms(1);
		time--;
	 }while(time);
	 if(time==0)res=0xff;//TIMEOUT
	 return res;
}
//擦除页
//paddr:页地址
//返回值:执行情况
u8 STMFLASH_ErasePage(u32 Page_Address)
{
	u8 res=0;
	
	FlashunInit();
	
	res=STMFLASH_WaitDone(0X5FFF);//等待上次操作结束,>20ms   
	
	if(res==0)
	{ 
		FLASH->CR|=1<<1;//页擦除
		FLASH->AR=Page_Address;//设置页地址 
		FLASH->CR|=1<<6;//开始擦除		  
		res=STMFLASH_WaitDone(0X5FFF);//等待操作结束,>20ms  
		if(res!=1)//非忙
		{
			FLASH->CR&=~(1<<1);//清除页擦除标志.
		}
	}
	
	STMFLASH_Lock();
	return res;
}
//在FLASH指定地址写入半字
//faddr:指定地址(此地址必须为2的倍数!!)
//dat:要写入的数据
//返回值:写入的情况
u8 STMFLASH_WriteHalfWord(u16 Data,u16* Address)
{
	u8 res;	   	
	FlashunInit();
	res=STMFLASH_WaitDone(0XFF);	 
	if(res==0)//OK
	{
		FLASH->CR|=1<<0;		//编程使能
		*(vu16*)Address=Data;		//写入数据
		res=STMFLASH_WaitDone(0XFF);//等待操作完成
		if(res!=1)//操作成功
		{
			FLASH->CR&=~(1<<0);	//清除PG位.
		}
	} 
	STMFLASH_Lock();
	return res;
} 
void DelPage(u32 Page_Address)
{
	status=FLASH->SR&0x00000001;
	if(status==0)//可以操作状态
	{
		FLASH->CR|=0x00000002;//设置FALSH_CR寄存器的PER位为1
		FLASH->AR=Page_Address;//用FALSH_AR寄存器选择要删除的页
		FLASH->CR|=0x00000040;//STRT位为1
		
		//等待擦除完成（等待BSY位变为0）
		status=FLASH_WaitForLastOperation(0x000B0000);
		
		if(status!=1)//FLASH_BUSY
			FLASH->CR&=0x000016F4;//如果擦除完成，禁止CR的PER为00x00001FFD
		
	
	}
}



//void WriteData(u16 Data[],u32 Address,u16 num)
//{
//	u16 *a;
//	a=(u16*)Address;
//	
//	status=FLASH->SR&0x00000001;
//	if(status == 0)//FLASH_COMPLETE
//	{
//		FLASH->CR|=0x00000001;//设置FALSH_CR寄存器的PG位为1；
//		a=Data;
//	
//		status=FLASH->SR&0x00000001;
//	 if(status !=1)	
//		{	
//			FLASH->CR &=0x000016F6;// FLASH->CR &= CR_PG_Reset;
//		}
//	}
//}


void WriteData(u16 Data,u16* Address)
{
	u16 *a;
	a=(u16*)Address;
	
	FlashunInit();
	status=FLASH->SR&0x00000001;
	if(status == 0)//FLASH_COMPLETE
	{
		FLASH->CR|=0x00000001;//设置FALSH_CR寄存器的PG位为1；
		*a=Data;
	
		status=FLASH->SR&0x00000001;
	 if(status !=1)	
		{	
			FLASH->CR &=0x000016F6;// FLASH->CR &= CR_PG_Reset;
		}
	}
}
void WriteString(u16* data,u32 Address,u16 num)
{
	u16 *p,i;
	p=(u16 *)Address;
	
	DelPage(Address);
	for(i=0;i<num;i++)
	{
		status=FLASH->SR&0x00000001;
		if(status == 0)//FLASH_COMPLETE
		{
			FLASH->CR|=0x00000001;//设置FALSH_CR寄存器的PG位为1；
			*p=data[i];
			p++;
			status=FLASH->SR&0x00000001;
			if(status !=1)	
			{	
				FLASH->CR &=0x000016F6;// FLASH->CR &= CR_PG_Reset;
			}
		}
	}
}
