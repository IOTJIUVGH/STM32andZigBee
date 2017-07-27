#include "i2c.h"

//PB6 -- I2C1_SCL    PB7 -- I2C1_SDA

void init_i2c()
{
	RCC->APB2ENR|=1<<3;   //CLOCK GPIOB
	GPIOB->CRL &= 0X00FFFFFF;//PB6/7 输出
	GPIOB->CRL |= 0X33000000;	   
	GPIOB->ODR |= 3 << 6;     //PB6/7 输出高
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA(0x00000080);	  	  
	IIC_SCL(0x00000040);
	delay_us(4);
 	IIC_SDA(0x00800000);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL(0x00400000);//钳住I2C总线,准备发送或者接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL(0x00400000);
	IIC_SDA(0x00800000);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SDA(0x00000080);	  	  
	IIC_SCL(0x00000040);
	delay_us(4);							   	
}
//等待应答信号到来
//返回值:1,接受应答失败
//        0,接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入
	
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
//		SDA_IN();      //SDA设置为输入
//	if(READ_SDA==0)
//     ucErrTime = 1;
//   else 
//     ucErrTime = 0;
//	IIC_SCL(0x00400000);//0 	   
//	return 0;  
//} 
//产生ACK应答
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
//不产生ACK应答		    
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

/****接收应答信号**********/
u8 IIC_RecvACK()					//接收从机应答信号
{
	u8 CY = 0;		//应答标志0应答1无应答
	SDA_IN();
	IIC_SCL(0x00000040); 
	delay_us(5);	           
	CY = READ_SDA;           
	IIC_SCL(0x00400000);	 
	delay_us(5);	          
	return CY;	
}

//IIC发送一个字节
//1,有应答
//0,无应答			  
void IIC_Send_Byte(u8 txd)
{                        
   u8 t;   
	SDA_OUT(); 	    
    IIC_SCL(0x00400000);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA((txd&0x80)>>7);
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL(0x00000040);	 
		delay_us(2); 
		IIC_SCL(0x00400000);	 
		delay_us(2);
    }	 
} 	    
//读1个字节,ack=1,发送ACK,ack=0,发送nACK   
u8 IIC_Read_Byte(u8 ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
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
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
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
