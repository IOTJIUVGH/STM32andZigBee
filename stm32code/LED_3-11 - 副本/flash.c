#include "stm32f10x.h"
#include "delay.h"
u8 status;

void FlashunInit()
{
	FLASH->KEYR=0x45670123;
	FLASH->KEYR=0XCDEF89AB;

}

//flash����
void STMFLASH_Lock(void)
{
	FLASH->CR|=1<<7;//����
}
//�õ�FLASH״̬
u8 STMFLASH_GetStatus(void)
{	
	u32 res;		
	res=FLASH->SR; 
	if(res&(1<<0))return 1;		    //æ
	else if(res&(1<<2))return 2;	//��̴���
	else if(res&(1<<4))return 3;	//д��������
	return 0;						//�������
}
//�ȴ��������
//time:Ҫ��ʱ�ĳ���
//����ֵ:״̬.
u8 STMFLASH_WaitDone(u16 time)
{
	u8 res;
	do
	{
		res=STMFLASH_GetStatus();
		if(res!=1)break;//��æ,����ȴ���,ֱ���˳�.
		Delayms(1);
		time--;
	 }while(time);
	 if(time==0)res=0xff;//TIMEOUT
	 return res;
}
//����ҳ
//paddr:ҳ��ַ
//����ֵ:ִ�����
u8 STMFLASH_ErasePage(u32 Page_Address)
{
	u8 res=0;
	
	FlashunInit();
	
	res=STMFLASH_WaitDone(0X5FFF);//�ȴ��ϴβ�������,>20ms   
	
	if(res==0)
	{ 
		FLASH->CR|=1<<1;//ҳ����
		FLASH->AR=Page_Address;//����ҳ��ַ 
		FLASH->CR|=1<<6;//��ʼ����		  
		res=STMFLASH_WaitDone(0X5FFF);//�ȴ���������,>20ms  
		if(res!=1)//��æ
		{
			FLASH->CR&=~(1<<1);//���ҳ������־.
		}
	}
	
	STMFLASH_Lock();
	return res;
}
//��FLASHָ����ַд�����
//faddr:ָ����ַ(�˵�ַ����Ϊ2�ı���!!)
//dat:Ҫд�������
//����ֵ:д������
u8 STMFLASH_WriteHalfWord(u16 Data,u16* Address)
{
	u8 res;	   	
	FlashunInit();
	res=STMFLASH_WaitDone(0XFF);	 
	if(res==0)//OK
	{
		FLASH->CR|=1<<0;		//���ʹ��
		*(vu16*)Address=Data;		//д������
		res=STMFLASH_WaitDone(0XFF);//�ȴ��������
		if(res!=1)//�����ɹ�
		{
			FLASH->CR&=~(1<<0);	//���PGλ.
		}
	} 
	STMFLASH_Lock();
	return res;
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



//void WriteData(u16 Data[],u32 Address,u16 num)
//{
//	u16 *a;
//	a=(u16*)Address;
//	
//	status=FLASH->SR&0x00000001;
//	if(status == 0)//FLASH_COMPLETE
//	{
//		FLASH->CR|=0x00000001;//����FALSH_CR�Ĵ�����PGλΪ1��
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
		FLASH->CR|=0x00000001;//����FALSH_CR�Ĵ�����PGλΪ1��
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
			FLASH->CR|=0x00000001;//����FALSH_CR�Ĵ�����PGλΪ1��
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
