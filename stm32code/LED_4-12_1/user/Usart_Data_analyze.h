#ifndef __USART_DATA_ANALYZE_H
#define __USART_DATA_ANALYZE_H	

#include "stm32f10x.h"
#include "cmd.h"

#define ALLOT_ID               0X01          //����ע��,����ID
#define REFUSE_CALL            0X02          //�ܾ�ע��
#define FIND_ID                0X04          //��ѯ�豸ID
#define CONSTANT_ILLUM         0X06          //���ն���������
#define FIND_ILLUM             0X08          //���ն����߲�ѯ
#define ADJUST_CURVE           0X0A          //������������
#define FIND_CURVE             0X0C          //�������߲�ѯ
#define CTRL_REAL_TIME         0X0E          //ʵʱ����
#define DEVICE_SLEEP           0x10          //�豸��������
#define DEVICE_WAKE            0x11          //�豸����
#define INIT_ADDR              0X12          //��ʼ��ID
#define VC_VAL                 0X16          //���õ�ѹ��������      
#define SET_TIME               0X18          //����ʱ��
#define FIND_TIME              0X13          //��ѯʱ��
#define CLOSE_CTRL             0X14          //�ر�ʵʱ����
#define SET_UPLOADTIME         0X1a          //�����ϴ��������ݺ�ע��ʱ��
#define FIND_UPLOADTIME        0X1b           //��ѯ�ϴ��������ݺ�ע��ʱ��

extern u8 g_PWMValue;
extern u8 g_DimNum;
extern u16 g_PWMCoe;
extern u8 g_REFU_ENROLL;
extern u8 g_DimCrv_flag;
extern u8 debug_data[10][10];
extern u8 g_GetHrtBtData_flag ;
extern u8 g_EnHrtBtDat_flag;
extern u8 g_GetVolt_flag;
extern u8 g_GetCur_flag ;

void Analyze_rec_data(u8 data[],u8 num);
void Analyze_Rec_Frame(u8 data[],u8 num);
void analyze_PWMcxe(void);
u8 Register_shortaddr(void);
void Operate_LED(void);
void Total_worktime(void);
u8 Upload_HeartbeatData(void);
void Handle_ReceiveData(void);
void Pack_Data(u8* data,u8 len,u8 cmd);
void Heart_Timer(void);
u8 CheckEnroll(void);
void Get_HeartBeatData(void);
void En_GetData(void);
void Get_Volt(void);
void Get_Cur(void);
void En_GetADC(void);
void En_GetVlot(void);
void En_GetCur(void);
void RS485_RxDatDeal(void);



#endif
