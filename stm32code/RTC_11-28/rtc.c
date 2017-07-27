#include "stm32f10x.h"

#include "rtc.h"  
#include "stdio.h"  
  
tm timer;                   //����ʱ�ӽṹ�壬����������ֱ�ӵ��� 
  
//ƽ����·ݡ����ڱ�
const u8 Days_Table[12]={31,28,31,30,31,30,31,31,30,31,30,31};  
const u8 Month_Table[12][3]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};  
  
const u8* Week_Table[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};  
  
//���������ݱ�                                                                           
u8 const _Week[12]={0,3,3,6,1,4,6,2,5,0,3,5};   
  
void Rtc_Init(void)  
{  
    RCC->APB1ENR |= 1<<28; //ʹ�ܵ�Դʱ��  
    RCC->APB1ENR |= 1<<27;  //ʹ�ܱ���ʱ��
	
    PWR->CR |= 1<<8;       //ȡ��������д����
  
    //RCC->BDCR |= 1<<16;  //�����������λ
    //RCC->BDCR |= ~(1<<16);   //�����������λ����  
  
    RCC->BDCR |= 1<<0;     //�����ⲿ�������� 
  
    while(!(RCC->BDCR & 0x02));  //�ȴ��ⲿʱ�Ӿ���  
  
    RCC->BDCR |= 1<<8;         //LSE��ΪRTCʱ�� 
    RCC->BDCR |= 1<<15;            //RTCʱ��ʹ��  
  
    while(!(RTC->CRL & (1<<5)));   //�ȴ�RTC�Ĵ����������
    while(!(RTC->CRL & (1<<3)));   //�ȴ�RTC����ͬ��  
  
    RTC->CRH |= 0x01;                //�������ж�    
  
    while(!(RTC->CRL & (1<<5)));   //�ȴ�RTC�Ĵ���������� 
  
    RTC->CRL |=  1<<4;             //�������� 
		
    RTC->PRLH = 0x0000;                
    RTC->PRLL = 32767;               //ʱ����������
  
    //Rtc_TIME_AutoSet();              
    //Rtc_TIME_Set(2012,7,7,20,50,0);   
  
    RTC->CRL &= ~(1<<4);           //�˳�����ģʽ��ʼ����ʼ����RTC�Ĵ���  
    while(!(RTC->CRL & (1<<5)));   //�ȴ�RTC�Ĵ���������� 
  
}  
  
  
//����RTC��ʼ��ʱʱ�� 
void Rtc_TIME_Set(u16 year,u8 month,u8 date,u8 hour,u8 minute, u8 second)  
{  
        u32 sec;      
  
        sec = Date_TO_Sec(year,month,date,hour,minute,second);  
      
        //printf("\nRtc TIME Set  Sec = %x\n",sec);  
      
        RCC->APB1ENR |= 1<<28;                         //ʹ�ܵ�Դʱ�� 
        RCC->APB1ENR |= 1<<27;                         //ʹ�ܺ���ʱ�� 
        PWR->CR |= 1<<8;                               //ȡ��д���� 
      
        RTC-> CRL |= 1<<4;                             //�������� 
          
        RTC-> CNTL = sec&0xffff;                     //ȡ��16λ
        RTC-> CNTH = sec>>16;                          //ȡ��16λ 
      
        RTC-> CRL &= ~(1<<4);                          //��ʼRTC�Ĵ�������  
      
        while(!(RTC->CRL&(1<<5)));                     //�ȴ�RTC�Ĵ����������    
}  
  
  
  
  
//�ж��Ƿ�������
//����: 1,�ǵ�     0,����  
u8 Is_LeapYear(u16 year)  
{               
    if(year%4==0)               //????4??  
    {   
        if(year%100==0)   
        {   
            if(year%400==0)  
                return 1;       //???00??,????400??          
            else   
                return 0;     
        }else{   
            return 1;     
        }  
    }else{  
         return 0;    
    }  
}  
  
  
//��ʱ��ת����19701��һ�ŵ�������  
//Bugs:�˺�����������20���ң����Ժ�������ֵ���˸Ķ�
 
u32 Date_TO_Sec(u16 year,u8 month,u8 date,u8 hour,u8 minute, u8 second)  
{  
    u16 t;  
    u32 sec;  
  
    if(year >= 1970 && year<= 2106)        
    {  
        for(t= 1970 ;t<year;t++)           
        {  
            if(Is_LeapYear(t))            
  
                sec += 31622400;   
            else      
                sec += 31536000;          
        }     
  
  
        for(t=0;t<month-1;t++)                         
        {  
            sec += (u32) Days_Table[t]*86400;             
            if(Is_LeapYear(year) && t== 1)              
                sec += 86400;                             
      
        }  
      
        sec += (u32)(hour)*3600;  
        sec += (u32)(minute)*60;  
        sec += second;  
    }  
  
    return sec-20;                                     
  
}  
  
  
  
//�Զ���ȡ��ǰʱ������RTC
  
void Rtc_TIME_AutoSet()  
{  
    u16 year,i=0,j=0;  
    u8  mon,date,sec,min,hour;  
  
    u8 *_date = __DATE__;  
    u8 *_time = __TIME__;  
  
    for(i=0;i<12;i++)  
    {  
        for(j=0;j<3;j++)  
        {  
            if(Month_Table[i][j] == _date[j]) mon = i;  //�õ��·�
        }  
    }  
  
  
    if(_date[4]==' '){          //�õ����� 
        date=_date[5]-'0';      //eg '7'-'0' =7  
    }else{   
        date=10*(_date[4]-'0')+_date[5]-'0';  
    }  
            
  
    year=1000*(_date[7]-'0')+100*(_date[8]-'0')+10*(_date[9]-'0')+_date[10]-'0';  //�õ����       
    hour=10*(_time[0]-'0')+_time[1]-'0';                                          //�õ�Сʱ
    min=10*(_time[3]-'0')+_time[4]-'0';                                             
    sec=10*(_time[6]-'0')+_time[7]-'0';   
      
  
    //printf("\n%d-%d-%d  %d:%d:%d\n",year,mon,date,hour,min,sec);  
  
    Rtc_TIME_Set(year,mon,date,hour,min,sec);  
}  
  
  
//��ȡRTCʱ�� 
void Rtc_Get()  
{  
    u32 secs,days,temp,years = 1970,months = 0;       
  
    secs = RTC->CNTH;    //��ȡ��ǰ�ܵ�����  
    secs <<= 16;  
    secs += RTC->CNTL;  
  
    //printf("\nRtc_Get  Sec = %x\n",secs);  
  
    days = secs/86400;  
    if(days > 0)         
    {  
        temp = days;  
        while(temp >= 365)     
        {  
            if(Is_LeapYear(years))             
            {  
                if(temp >= 366)   
                    temp -= 366;   
                else  
                    break;  
            }else{  
                temp -= 365;  
            }             
            years++;  
        }  
          
        timer.year = years;          
  
        while(days >= 28)  
        {  
            if(Is_LeapYear(years) && months ==1)    
            {  
                if(temp >= 29)   
                    temp -= 29;   
                else  
                    break;  
            }else{  
                if(temp >= Days_Table[months])         
                    temp -= Days_Table[months];  
                else  
                    break;  
            }  
  
            months++;     
        }  
  
        timer.month = months+1;            
  
    temp = secs % 86400;                 
    timer.hour = temp/3600;                
    timer.minute = (temp%3600)/60;            
    timer.second = (temp%3600)%60;  
    timer.week = Rtc_DAY_Get(timer.year,timer.month,timer.date);  
  
                  
}  
  
//�жϵ�ǰ���ڼ�                   
  
u8 Rtc_DAY_Get(u16 year,u8 month,u8 day)  
{     
    u16 temp;  
    u8 yearH,yearL;  
      
    yearH = year/100;     
    yearL = year%100;   
  
      
    if( yearH > 19 ) yearL += 100;  
  
  
  
    temp = yearL+yearL/4;  
    temp = temp%7;   
    temp = temp + day + _Week[month-1];  
  
    if( yearL%4 == 0 && month < 3 ) temp--;  
  
    return(temp%7);  
}  
  
//��������ʱ��  
  
void Rtc_ALARM_Set(u16 year,u8 month,u8 date,u8 hour,u8 minute, u8 second)  
{  
  
        u32 sec;      
  
        sec = Date_TO_Sec(year,month,date,hour,minute,second);  
  
  
        RTC-> CRL |= 1<<4;                             //�������� 
  
        //while(!(RTC->CRL&(1<<5)));                       //RTOFF?1 ?????ALRL?ALRH???  
          
        RTC-> ALRL = sec&0xffff;                     //??16?  
        RTC-> ALRH = sec>>16;                          //??16?  
      
        RTC-> CRL &= ~(1<<4);                          //??RTC?????  
      
        while(!(RTC->CRL&(1<<5)));                     //??RTC???????  
  
}  