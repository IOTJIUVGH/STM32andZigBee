/***************************************************************************************************
  Filename:       MT_UART.c
  Revised:        $Date: 2009-03-12 16:25:22 -0700 (Thu, 12 Mar 2009) $
  Revision:       $Revision: 19404 $

  Description:  This module handles anything dealing with the serial port.

  Copyright 2007 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED 揂S IS?WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.

***************************************************************************************************/

/***************************************************************************************************
 * INCLUDES
 ***************************************************************************************************/
#include "ZComDef.h"
#include "OSAL.h"
#include "hal_uart.h"
#include "MT.h"
#include "MT_UART.h"
#include "OSAL_Memory.h"


/***************************************************************************************************
 * MACROS
 ***************************************************************************************************/

/***************************************************************************************************
 * CONSTANTS
 ***************************************************************************************************/
/* State values for ZTool protocal */
#define SOP_STATE      0x00
#define CMD_STATE1     0x01
#define CMD_STATE2     0x02
#define LEN_STATE      0x03
#define DATA_STATE     0x04
#define FCS_STATE      0x05

/***************************************************************************************************
 *                                         GLOBAL VARIABLES
 ***************************************************************************************************/
/* Used to indentify the application ID for osal task */
byte App_TaskID;

/* ZTool protocal parameters */
uint8 state;
uint8  CMD_Token[2];
uint8  LEN_Token;
uint8  FSC_Token;
mtOSALSerialData_t  *pMsg;
uint8  tempDataLen;

unsigned long int gcdm(unsigned long int a,unsigned long int b);
unsigned long int getDm(unsigned long int Fn,unsigned long int e1);
unsigned long int getEm(unsigned long int Fn);
uint8 primem(uint8 i);
uint8 getpm(uint8 i);
uint8 getqm(uint8 p1);
unsigned long int RSAm(unsigned long int a,unsigned long int b,unsigned long int c);

#if defined (ZAPP_P1) || defined (ZAPP_P2)
uint16  MT_UartMaxZAppBufLen;
bool    MT_UartZAppRxStatus;
#endif


/***************************************************************************************************
 *                                          LOCAL FUNCTIONS
 ***************************************************************************************************/

/***************************************************************************************************
 * @fn      MT_UartInit
 *
 * @brief   Initialize MT with UART support
 *
 * @param   None
 *
 * @return  None
***************************************************************************************************/
void MT_UartInit ()
{
  halUARTCfg_t uartConfig;

  /* Initialize APP ID */
  App_TaskID = 0;

  /* UART Configuration */
  uartConfig.configured           = TRUE;
  uartConfig.baudRate             = MT_UART_DEFAULT_BAUDRATE;
  uartConfig.flowControl          = MT_UART_DEFAULT_OVERFLOW;
  uartConfig.flowControlThreshold = MT_UART_DEFAULT_THRESHOLD;
  uartConfig.rx.maxBufSize        = MT_UART_DEFAULT_MAX_RX_BUFF;
  uartConfig.tx.maxBufSize        = MT_UART_DEFAULT_MAX_TX_BUFF;
  uartConfig.idleTimeout          = MT_UART_DEFAULT_IDLE_TIMEOUT;
  uartConfig.intEnable            = TRUE;
#if defined (ZTOOL_P1) || defined (ZTOOL_P2)
  uartConfig.callBackFunc         = MT_UartProcessZToolData;
#elif defined (ZAPP_P1) || defined (ZAPP_P2)
  uartConfig.callBackFunc         = MT_UartProcessZAppData;
#else
  uartConfig.callBackFunc         = NULL;
#endif

  /* Start UART */
#if defined (MT_UART_DEFAULT_PORT)
  HalUARTOpen (MT_UART_DEFAULT_PORT, &uartConfig);
#else
  /* Silence IAR compiler warning */
  (void)uartConfig;
#endif

  /* Initialize for ZApp */
#if defined (ZAPP_P1) || defined (ZAPP_P2)
  /* Default max bytes that ZAPP can take */
  MT_UartMaxZAppBufLen  = 1;
  MT_UartZAppRxStatus   = MT_UART_ZAPP_RX_READY;
#endif

}

/***************************************************************************************************
 * @fn      MT_SerialRegisterTaskID
 *
 * @brief   This function registers the taskID of the application so it knows
 *          where to send the messages whent they come in.
 *
 * @param   void
 *
 * @return  void
 ***************************************************************************************************/
void MT_UartRegisterTaskID( byte taskID )
{
  App_TaskID = taskID;
}

/***************************************************************************************************
 * @fn      SPIMgr_CalcFCS
 *
 * @brief   Calculate the FCS of a message buffer by XOR'ing each byte.
 *          Remember to NOT include SOP and FCS fields, so start at the CMD field.
 *
 * @param   byte *msg_ptr - message pointer
 * @param   byte len - length (in bytes) of message
 *
 * @return  result byte
 ***************************************************************************************************/
byte MT_UartCalcFCS( uint8 *msg_ptr, uint8 len )
{
  byte x;
  byte xorResult;

  xorResult = 0;

  for ( x = 0; x < len; x++, msg_ptr++ )
    xorResult = xorResult ^ *msg_ptr;

  return ( xorResult );
}
   

void MT_UartProcessZToolData ( uint8 port, uint8 event )
{
  uint8 flag=0,i,j=0;   //flag是判断有没有收到数据，j记录数据长度
  uint8 buf[128];     //串口buffer最大缓冲默认是128，我们这里用128.
  (void)event;        // Intentionally unreferenced parameter  
  
  uint8 r=4;
  uint8 k,y;
  uint8 p,q;
  unsigned long int k_e,k_d,k_n,k_yn;
  
  while (Hal_UART_RxBufLen(port)) //检测串口数据是否接收完成

  {
    HalUARTRead (port,&buf[j], 1);  //把数据接收放到buf中
    j++;                           //记录字符数
    flag=1;                         //已经从串口接收到信息
  } 
  for(i=0;i<4;i++)
    if(buf[i]=='k')
      r--;
 
    if(r==0)       //已经从串口接收到信息

  { 
    uint8 send[30];
    uint8 ii;
    p=getpm(1);
    q=getqm(p);
    k_n=p*q;
    k_yn=(p-1)*(q-1);
    k_e=getEm(k_yn);
    k_d=getDm(k_yn,k_e);
    RSA_d=k_d;
    RSA_e=k_e;
    RSA_n=k_n;
    for(i=0;i<4;i++)
      send[i]='x';
    for(i=0,ii=4;i<4;i++,ii++)
      send[ii]=k_e>>(i*8);
    for(i=0,ii=8;i<4;i++,ii++)
      send[ii]=k_n>>(i*8);
    HalUARTWrite(0,"发送keye\n",7);
   // HalUARTWrite(0,(uint8*)&RSA_e,4);
    //HalUARTWrite(0,(uint8*)&RSA_n,4);
	    //分配内存空间，为机构体内容+数据内容+1个记录长度的数据
   pMsg = (mtOSALSerialData_t *)osal_msg_allocate( sizeof  
          ( mtOSALSerialData_t )+8+1+4);
  //事件号用原来的CMD_SERIAL_MSG
  pMsg->hdr.event = CMD_SERIAL_MSG;
  pMsg->msg = (uint8*)(pMsg+1);  // 把数据定位到结构体数据部分
  pMsg->msg [0]= 12;              //给上层的数据第一个是长度
  for(i=0;i<12;i++)                //从第二个开始记录数据 
  pMsg->msg [i+1]= send[i];   
  osal_msg_send( App_TaskID, (byte *)pMsg );  //登记任务，发往上层
  
  osal_msg_deallocate ( (uint8 *)pMsg );      //释放内存
  }
  
  else if(flag==1&&r!=0)       //已经从串口接收到信息

  {     /* Allocate memory for the data */
	    //分配内存空间，为机构体内容+数据内容+1个记录长度的数据
    uint8 ii=0,jj=4;
    uint8 text[128];
    unsigned long int textRSA[30];
    for(i=0;i<j;i++)
      textRSA[i]=RSAm((unsigned long int)buf[i],RSA_e,RSA_n);
    for(i=0;i<j;i++)
    {
      for(jj=0;jj<4;jj++)
      {
        text[ii]=(uint8)textRSA[i];
        textRSA[i]=textRSA[i]>>8;
        ii++;
      }
    }
    //for(i=0;i<j;i++)
     // for(ii=0)
     // text[i]=(uint8)*(&textRSA[0]+i);
   pMsg = (mtOSALSerialData_t *)osal_msg_allocate( sizeof  
          ( mtOSALSerialData_t )+j*4+1);
  //事件号用原来的CMD_SERIAL_MSG
  pMsg->hdr.event = CMD_SERIAL_MSG;
  pMsg->msg = (uint8*)(pMsg+1);  // 把数据定位到结构体数据部分
  pMsg->msg [0]= j*4;              //给上层的数据第一个是长度
  for(i=0;i<j*4;i++)                //从第二个开始记录数据 
  pMsg->msg [i+1]= text[i];   
  osal_msg_send( App_TaskID, (byte *)pMsg );  //登记任务，发往上层
  /* deallocate the msg */
  osal_msg_deallocate ( (uint8 *)pMsg );      //释放内存
  }
}


/***************************************************************************************************
 * @fn      MT_UartProcessZToolData
 *
 * @brief   | SOP | Data Length  |   CMD   |   Data   |  FCS  |
 *          |  1  |     1        |    2    |  0-Len   |   1   |
 *
 *          Parses the data and determine either is SPI or just simply serial data
 *          then send the data to correct place (MT or APP)
 *
 * @param   port     - UART port
 *          event    - Event that causes the callback
 *
 *
 * @return  None
 ***************************************************************************************************/


#if defined (ZAPP_P1) || defined (ZAPP_P2)
/***************************************************************************************************
 * @fn      MT_UartProcessZAppData
 *
 * @brief   | SOP | CMD  |   Data Length   | FSC  |
 *          |  1  |  2   |       1         |  1   |
 *
 *          Parses the data and determine either is SPI or just simply serial data
 *          then send the data to correct place (MT or APP)
 *
 * @param   port    - UART port
 *          event   - Event that causes the callback
 *
 *
 * @return  None
 ***************************************************************************************************/
void MT_UartProcessZAppData ( uint8 port, uint8 event )
{

  osal_event_hdr_t  *msg_ptr;
  uint16 length = 0;
  uint16 rxBufLen  = Hal_UART_RxBufLen(MT_UART_DEFAULT_PORT);

  /*
     If maxZAppBufferLength is 0 or larger than current length
     the entire length of the current buffer is returned.
  */
  if ((MT_UartMaxZAppBufLen != 0) && (MT_UartMaxZAppBufLen <= rxBufLen))
  {
    length = MT_UartMaxZAppBufLen;
  }
  else
  {
    length = rxBufLen;
  }

  /* Verify events */
  if (event == HAL_UART_TX_FULL)
  {
    // Do something when TX if full
    return;
  }

  if (event & ( HAL_UART_RX_FULL | HAL_UART_RX_ABOUT_FULL | HAL_UART_RX_TIMEOUT))
  {
    if ( App_TaskID )
    {
      /*
         If Application is ready to receive and there is something
         in the Rx buffer then send it up
      */
      if ((MT_UartZAppRxStatus == MT_UART_ZAPP_RX_READY ) && (length != 0))
      {
        /* Disable App flow control until it processes the current data */
         MT_UartAppFlowControl (MT_UART_ZAPP_RX_NOT_READY);

        /* 2 more bytes are added, 1 for CMD type, other for length */
        msg_ptr = (osal_event_hdr_t *)osal_msg_allocate( length + sizeof(osal_event_hdr_t) );
        if ( msg_ptr )
        {
          msg_ptr->event = SPI_INCOMING_ZAPP_DATA;
          msg_ptr->status = length;

          /* Read the data of Rx buffer */
          HalUARTRead( MT_UART_DEFAULT_PORT, (uint8 *)(msg_ptr + 1), length );

          /* Send the raw data to application...or where ever */
          osal_msg_send( App_TaskID, (uint8 *)msg_ptr );
        }
      }
    }
  }
}

/***************************************************************************************************
 * @fn      SPIMgr_ZAppBufferLengthRegister
 *
 * @brief
 *
 * @param   maxLen - Max Length that the application wants at a time
 *
 * @return  None
 *
 ***************************************************************************************************/
void MT_UartZAppBufferLengthRegister ( uint16 maxLen )
{
  /* If the maxLen is larger than the RX buff, something is not right */
  if (maxLen <= MT_UART_DEFAULT_MAX_RX_BUFF)
    MT_UartMaxZAppBufLen = maxLen;
  else
    MT_UartMaxZAppBufLen = 1; /* default is 1 byte */
}

/***************************************************************************************************
 * @fn      SPIMgr_AppFlowControl
 *
 * @brief
 *
 * @param   status - ready to send or not
 *
 * @return  None
 *
 ***************************************************************************************************/
void MT_UartAppFlowControl ( bool status )
{

  /* Make sure only update if needed */
  if (status != MT_UartZAppRxStatus )
  {
    MT_UartZAppRxStatus = status;
  }

  /* App is ready to read again, ProcessZAppData have to be triggered too */
  if (status == MT_UART_ZAPP_RX_READY)
  {
    MT_UartProcessZAppData (MT_UART_DEFAULT_PORT, HAL_UART_RX_TIMEOUT );
  }

}

#endif //ZAPP

/***************************************************************************************************
***************************************************************************************************/


unsigned long int RSA(unsigned long int a,unsigned long int b,unsigned long int c);
//产生随机素数，uint8 p,q;
uint8 getpm(uint8 i)
{
  uint8 p1;
  do{
    p1=osal_rand()%255+1;
  }while(p1%2==0||!primem(p1));
  return p1;
}
uint8 getqm(uint8 p1)
{
  uint8 q1;
  do{
    q1=osal_rand()%255+1;
  }while(q1%2==0||p1==q1||!primem(q1));
  return q1;
}
//判断是否为素数
uint8 primem(uint8 i)
{
  uint8 j;
  if(i<1)
    return 0;
  for(j=3;j<i;j++){
    if(i%j==0)
      return 0;
  }
  return 1;//确定是素数
}
//产生公钥E，unsigned long int e
unsigned long int getEm(unsigned long int Fn)
{
  unsigned long int e1=2;
  while(e1<Fn&&(!gcdm(Fn,e1)))
  {
    e1=(osal_rand()*osal_rand())%Fn+1;
  }
  return e1;
}
//产生私钥D unsigned long int d
unsigned long int getDm(unsigned long int Fn,unsigned long int e1)
{
  unsigned long int d1=2;
    while(((e1*d1)%Fn)!=1)
    {
      d1++;
    }
  return d1;
}
//判断是否互素
unsigned long int gcdm(unsigned long int a,unsigned long int b)
{
  if(a%b==0)
    return 0;
  else if(a%b==1)
    return 1;
  else 
    return gcdm(b,a%b);
}
//核心加密解密算法
unsigned long int RSAm(unsigned long int a,unsigned long int b,unsigned long int c)
{
  unsigned long int r=1;
  b=b+1;
  while(b!=1)
  {
    
    r=r*a;
    r=r%c;
    b--;
   // HalUARTWrite(0,"T",1 );
   // if(b==1)
   // HalUARTWrite(0,(unsigned char *)&r,4 );
  }
   //HalUARTWrite(0,(unsigned char *)&r,5 );
  return r;
}