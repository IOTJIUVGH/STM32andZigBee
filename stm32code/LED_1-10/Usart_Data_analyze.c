#include "stm32f10x.h"
#include "string.h"
#include "Usart_Data_analyze.h"
#include "pwm.h"
#include "flash.h"
#include "usart.h"
u8 id[5]={'\0'},debug_data[10][10]={'\0'};
u16 pwm,bb=0;
u8 first=0;

//调试数据处理函数
void Analyze_rec_data(u8 data[],u8 num)
{
	
	u8 *nod,i=0,j=0;
	
	nod=data;
	while(*nod!=' ')
	{	
		for(i=0;i<9;i++)
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
}

//JSON格式数据处理函数
void Analyze_Rec_Json(u8 data[],u8 num)
{
	u8 *nod,i=0,j=0,n=0,T=0;
	//u16* a=(u16*)0X0800FC00;
	struct Json json[10]={'\0','\0'};
	
	nod=data;
	while(*nod!='{')
	{
		nod++;
	}
		nod++;
	//printf("%s",data);
	while(*nod!=('\0'))
	{
		while(*nod!=',' && *nod!='}')
		{
			if(*nod=='"')
			{
			nod++;
			}else
			{
			switch(T)
			{
				case 0:json[i].string[j++]=*nod;
					nod++;break;

				case 1: json[i].value[n++]=*nod;
					nod++;break;
			}
			
			}
			if(*nod==':')
			{
				u8 *a;
				a=nod;
				if(*(++a)=='"')
				{
					nod=nod+2;
					T++;
				}
			}
			
		}
		nod++;
		i++;
		T=0;
		j=0;
		n=0;
	}
	Rec_Data_Handle(json,10);
	
}


//处理后的数据具体操作
void Rec_Data_Handle(struct Json json1[],u16 num)
{
	u8 i=0,j=0,x=0,z=0,stop_flag=0;
	extern u16 line1[30];
	static u8 c=1;
	u16* a=(u16*)0X0800FC00;
	u16 *addr;
	addr=(u16*)0x0800F800;
	
	for(i=0;i<10;i++)
	{
		if(strcmp(json1[i].string,"ID")==0)
		{
			if((u16)atoi(json1[i].value)==*a||(u16)atoi(json1[i].value)==0)
			{
				for(j=0;j<10;j++)
				{
					if(strcmp(json1[j].string,"TYPE")==0)
					{
						if(strcmp(json1[j].value,"CTRL")==0)
						{
							for(z=0;z<10;z++)
							{
								if(strcmp(json1[z].string,"PWM")==0)
								{
									if(strcmp(json1[z].value,"10")==0)
									{
										pwm=10;
										LedToggle('n');
										pwmval(pwm);
									}
									if(strcmp(json1[z].value,"20")==0)
									{
										pwm=20;
										LedToggle('n');
										pwmval(pwm);
									}
									if(strcmp(json1[z].value,"30")==0)
									{
										pwm=30;
										LedToggle('n');
										pwmval(pwm);
									}
									if(strcmp(json1[z].value,"40")==0)
									{
										pwm=40;
										LedToggle('n');
										pwmval(pwm);
									}
									if(strcmp(json1[z].value,"50")==0)
									{
										pwm=50;
										
										LedToggle('n');
										pwmval(pwm);
										
									}
									if(strcmp(json1[z].value,"60")==0)
									{
										pwm=60;
										LedToggle('n');
										pwmval(pwm);
									}
									if(strcmp(json1[z].string,"70")==0)
									{
										pwm=70;
										LedToggle('n');
										pwmval(pwm);
									}
									if(strcmp(json1[z].value,"80")==0)
									{
										pwm=80;
										LedToggle('n');
										pwmval(pwm);
									}
									if(strcmp(json1[z].value,"90")==0)
									{
										pwm=90;
										LedToggle('n');
										pwmval(pwm);
									}
									if(strcmp(json1[z].value,"100")==0)
									{
										pwm=100;
										LedToggle('n');
										pwmval(pwm);
									}
									if(strcmp(json1[z].value,"0")==0)
									{
										pwm=0;						
										LedToggle('f');
							
									}
								}
							}
						}
						
						if(strcmp(json1[j].value,"SET")==0)
						{
							
							for(x=0;x<10;x++)
							{
								if(strcmp(json1[x].string,"LINE")==0)
								{
									line1[0]=(u16)atoi(json1[x].value);
									
								}
								if(strcmp(json1[x].string,"NUM")==0)
								{
									line1[1]=(u16)atoi(json1[x].value);
								}
								if(strcmp(json1[x].string,"TIME")==0)
								{	
									u8 time[2][5]={'\0'},*p,i=0,j=0;
									p=json1[x].value;
									while(*p!='\0')
									{
										if(*p==':')
										{
											i=0;
											j++;
											p++;
										}
										time[j][i]=*p;
										p++;
										i++;
									}
									c++;
									line1[c]=(u16)atoi(time[0]);
									c++;		
									line1[c]=(u16)atoi(time[1]);							
								}
								if(strcmp(json1[x].string,"PWM")==0)
								{
									c++;
									line1[c]=(u16)atoi(json1[x].value);	
								}
								if(strcmp(json1[x].string,"FLAG")==0)
								{
										if(strcmp(json1[x].value,"STOP")==0)
										{	
											stop_flag=1;
											
										}
										if(strcmp(json1[x].value,"START")==0)
										{
											c=1;
											for(i=0;i<30;i++)
											{
												line1[i]=0;
											}
										}	
								}
								
							}
							if(stop_flag==1)
							{
								if(c==(line1[1]*3+1))
								{
									c=0;
									STMFLASH_ErasePage(0x0800F800);
									while(c!=29)
									{
										//printf("%d\r\n",line1[c]);
										STMFLASH_WriteHalfWord(line1[c],addr);
										addr++;
										c++;
									}	
									for(i=0;i<30;i++)
									{
										line1[i]=0;
									}
									c=1;
								}
							}
						}
						
					}
				}
			}
		}	
	}
 
}
