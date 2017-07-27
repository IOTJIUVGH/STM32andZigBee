#include "stm32f10x.h"

#define STM_SECTOR_SIZE 1024 //字节
#define STM32_FLASH_BASE 0x08000000
u8 status;
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节
void FlashunInit()
{
	FLASH->KEYR=0x45670123;//解除FLASH锁
	FLASH->KEYR=0XCDEF89AB;

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



void WriteData(u16* data,u32 Address,u16 num)
{
	u16 *a,*p,i;
	a=(u16 *)Address;
	
	DelPage(Address);
	for(i=0;i<num;i++)
	{
		status=FLASH->SR&0x00000001;
		if(status == 0)//FLASH_COMPLETE
		{
			FLASH->CR|=0x00000001;//设置FALSH_CR寄存器的PG位为1；
			*(a+i)=data[i];
			p=a+i;
			status=FLASH->SR&0x00000001;
			if(status !=1)	
			{	
				FLASH->CR &=0x000016F6;// FLASH->CR &= CR_PG_Reset;
			}
		}
	}
	
}
//u16 STMFLASH_ReadHalfWord(u32 faddr)
//{
//	return *(vu16*)faddr; 
//}

//void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
//{ 			 		 
//	u16 i;
//	for(i=0;i<NumToWrite;i++)
//	{
//		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
//	    WriteAddr+=2;//地址增加2.
//	}  
//} 

//void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
//{
//	u32 secpos;	   //扇区地址
//	u16 secoff;	   //扇区内偏移地址(16位字计算)
//	u16 secremain; //扇区内剩余地址(16位字计算)	   
// 	u16 i;    
//	u32 offaddr;   //去掉0X08000000后的地址
//	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*64)))return;//非法地址
//	FLASH_Unlock();						//解锁
//	offaddr=WriteAddr-STM32_FLASH_BASE;		//实际偏移地址.
//	secpos=offaddr/STM_SECTOR_SIZE;			//扇区地址  0~127 for STM32F103RBT6
//	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
//	secremain=STM_SECTOR_SIZE/2-secoff;		//扇区剩余空间大小   
//	if(NumToWrite<=secremain)secremain=NumToWrite;//不大于该扇区范围
//	while(1) 
//	{	
////		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//读出整个扇区的内容
////		for(i=0;i<secremain;i++)//校验数据
////		{
////			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//需要擦除  	  
////		}
//		if(i<secremain)//需要擦除
//		{
//			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区
//			for(i=0;i<secremain;i++)//复制
//			{
//				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
//			}
//			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//写入整个扇区  
//		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
//		if(NumToWrite==secremain)break;//写入结束了
//		else//写入未结束
//		{
//			secpos++;				//扇区地址增1
//			secoff=0;				//偏移位置为0 	 
//		   	pBuffer+=secremain;  	//指针偏移
//			WriteAddr+=secremain;	//写地址偏移	   
//		   	NumToWrite-=secremain;	//字节(16位)数递减
//			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//下一个扇区还是写不完
//			else secremain=NumToWrite;//下一个扇区可以写完了
//		}	 
//	};	
//	
//}


//////从指定地址开始读出指定长度的数据
//////ReadAddr:起始地址
//////pBuffer:数据指针
//////NumToWrite:半字(16位)数
////void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
////{
////	u16 i;
////	for(i=0;i<NumToRead;i++)
////	{
////		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
////		ReadAddr+=2;//偏移2个字节.	
////	}
////}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////WriteAddr:起始地址
////WriteData:要写入的数据
//void Test_Write(u32 WriteAddr,u16 WriteData)   	
//{
//	STMFLASH_Write(WriteAddr,&WriteData,1);//写入一个字 
//}
