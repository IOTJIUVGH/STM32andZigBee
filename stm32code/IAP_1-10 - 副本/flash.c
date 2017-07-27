#include "stm32f10x.h"

#define STM_SECTOR_SIZE 1024 //�ֽ�
#define STM32_FLASH_BASE 0x08000000
u8 status;
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�
void FlashunInit()
{
	FLASH->KEYR=0x45670123;//���FLASH��
	FLASH->KEYR=0XCDEF89AB;

}


void DelPage(u32 Page_Address)
{
	status=FLASH->SR&0x00000001;
	if(status==0)//���Բ���״̬
	{
		FLASH->CR|=0x00000002;//����FALSH_CR�Ĵ�����PERλΪ1
		FLASH->AR=Page_Address;//��FALSH_AR�Ĵ���ѡ��Ҫɾ����ҳ
		FLASH->CR|=0x00000040;//STRTλΪ1
		
		//�ȴ�������ɣ��ȴ�BSYλ��Ϊ0��
		status=FLASH_WaitForLastOperation(0x000B0000);
		
		if(status!=1)//FLASH_BUSY
			FLASH->CR&=0x000016F4;//���������ɣ���ֹCR��PERΪ00x00001FFD
		
	
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
			FLASH->CR|=0x00000001;//����FALSH_CR�Ĵ�����PGλΪ1��
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
//	    WriteAddr+=2;//��ַ����2.
//	}  
//} 

//void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
//{
//	u32 secpos;	   //������ַ
//	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
//	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
// 	u16 i;    
//	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
//	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*64)))return;//�Ƿ���ַ
//	FLASH_Unlock();						//����
//	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
//	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
//	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
//	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
//	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
//	while(1) 
//	{	
////		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
////		for(i=0;i<secremain;i++)//У������
////		{
////			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
////		}
//		if(i<secremain)//��Ҫ����
//		{
//			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
//			for(i=0;i<secremain;i++)//����
//			{
//				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
//			}
//			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
//		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
//		if(NumToWrite==secremain)break;//д�������
//		else//д��δ����
//		{
//			secpos++;				//������ַ��1
//			secoff=0;				//ƫ��λ��Ϊ0 	 
//		   	pBuffer+=secremain;  	//ָ��ƫ��
//			WriteAddr+=secremain;	//д��ַƫ��	   
//		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
//			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
//			else secremain=NumToWrite;//��һ����������д����
//		}	 
//	};	
//	
//}


//////��ָ����ַ��ʼ����ָ�����ȵ�����
//////ReadAddr:��ʼ��ַ
//////pBuffer:����ָ��
//////NumToWrite:����(16λ)��
////void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
////{
////	u16 i;
////	for(i=0;i<NumToRead;i++)
////	{
////		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
////		ReadAddr+=2;//ƫ��2���ֽ�.	
////	}
////}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////WriteAddr:��ʼ��ַ
////WriteData:Ҫд�������
//void Test_Write(u32 WriteAddr,u16 WriteData)   	
//{
//	STMFLASH_Write(WriteAddr,&WriteData,1);//д��һ���� 
//}
