#ifndef __USART_DATA_ANALYZE_H
#define __USART_DATA_ANALYZE_H	

#include "stm32f10x.h"
#include "cmd.h"

#define ALLOT_ID               0X01          //允许注册,分配ID
#define REFUSE_CALL            0X02          //拒绝注册
#define FIND_ID                0X04          //查询设备ID
#define CONSTANT_ILLUM         0X06          //恒照度曲线设置
#define FIND_ILLUM             0X08          //恒照度曲线查询
#define ADJUST_CURVE           0X0A          //调光曲线设置
#define FIND_CURVE             0X0C          //调光曲线查询
#define CTRL_REAL_TIME         0X0E          //实时控制
#define DEVICE_SLEEP           0x10          //设备休眠设置
#define DEVICE_WAKE            0x11          //设备唤醒
#define INIT_ADDR              0X12          //初始化ID
#define VC_VAL                 0X16          //设置电压电流参数      
#define SET_TIME               0X18          //设置时间
#define FIND_TIME              0X13          //查询时间
#define CLOSE_CTRL             0X14          //关闭实时控制
#define SET_UPLOADTIME         0X1a          //设置上传心跳数据和注册时间
#define FIND_UPLOADTIME        0X1b           //查询上传心跳数据和注册时间

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
