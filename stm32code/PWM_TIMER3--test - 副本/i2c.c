#include "i2c.h"

//PB6 -- I2C1_SCL    PB7 -- I2C1_SDA

void init_i2c()
{
	RCC->APB2ENR|=1<<3;   //CLOCK GPIOB
	GPIOB->CRL &= 0X00FFFFFF;//PB6/7 ���
	GPIOB->CRL |= 0X33000000;	   
	GPIOB->ODR |= 3 << 6;     //PB6/7 �����
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA(0x00000080);	  	  
	IIC_SCL(0x00000040);
	delay_us(4);
 	IIC_SDA(0x00800000);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL(0x00400000);//ǯסI2C����,׼�����ͻ��߽������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL(0x00400000);
	IIC_SDA(0x00800000);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SDA(0x00000080);	  	  
	IIC_SCL(0x00000040);
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ:1,����Ӧ��ʧ��
//        0,����Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����
	
	IIC_SDA(0x00000080);delay_us(1);	   
	IIC_SCL(0x00000040);delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL(0x00400000);//0 	   
	return 0;  
} 
//u8 IIC_Wait_Ack(void)
//{
//	u8 ucErrTime=0;
//	
//	IIC_SCL(0x00400000);
//	IIC_SDA(0x00000080);delay_us(5);	   
//	IIC_SCL(0x00000040);delay_us(5);	
//		SDA_IN();      //SDA����Ϊ����
//	if(READ_SDA==0)
//     ucErrTime = 1;
//   else 
//     ucErrTime = 0;
//	IIC_SCL(0x00400000);//0 	   
//	return 0;  
//} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL(0x00400000);
	SDA_OUT();
	IIC_SDA(0x00800000);
	delay_us(2);
	IIC_SCL(0x00000040);
	delay_us(2);
	IIC_SCL(0x00400000);
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL(0x00400000);
	SDA_OUT();
	IIC_SDA(0x00000080);
	delay_us(2);
	IIC_SCL(0x00000040);
	delay_us(2);
	IIC_SCL(0x00400000);
}					 				  

/****����Ӧ���ź�**********/
u8 IIC_RecvACK()					//���մӻ�Ӧ���ź�
{
	u8 CY = 0;		//Ӧ���־0Ӧ��1��Ӧ��
	SDA_IN();
	IIC_SCL(0x00000040); 
	delay_us(5);	           
	CY = READ_SDA;           
	IIC_SCL(0x00400000);	 
	delay_us(5);	          
	return CY;	
}

//IIC����һ���ֽ�
//1,��Ӧ��
//0,��Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
   u8 t;   
	SDA_OUT(); 	    
    IIC_SCL(0x00400000);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA((txd&0x80)>>7);
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL(0x00000040);	 
		delay_us(2); 
		IIC_SCL(0x00400000);	 
		delay_us(2);
    }	 
} 	    
//��1���ֽ�,ack=1,����ACK,ack=0,����nACK   
u8 IIC_Read_Byte(u8 ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL(0x00400000);
        delay_us(2);
		IIC_SCL(0x00000040);	 
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}

void IIC_SendACK(u8 ack)
{
	  SDA_OUT();//SDA??
		if(ack==0)		//?ack?0???????
		{	IIC_SDA(0x00800000);}
		else 
		{IIC_SDA(0x00000080); }	//?ack?1????????    						
			IIC_SCL(0x00000040);               //?????
			delay_us(5);                //??
			IIC_SCL(0x00400000);                     //?????
			delay_us(5);              //??
}
