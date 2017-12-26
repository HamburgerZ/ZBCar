/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"
#include "ZGlobals.h"
#include "AF.h"
#include "aps_groups.h"
#include "ZDApp.h"

#include "SampleApp.h"
#include "SampleAppHw.h"

#include "OnBoard.h"

/* HAL */
#include "hal_car.h"
#include "hal_track.h"
#include "hal_led.h"
#include "hal_uart.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */
#if !defined( SERIAL_APP_PORT )
#define SERIAL_APP_PORT  0
#endif

#if !defined( SERIAL_APP_BAUD )
  //#define SERIAL_APP_BAUD  HAL_UART_BR_38400
  #define SERIAL_APP_BAUD  HAL_UART_BR_115200
#endif

// When the Rx buf space is less than this threshold, invoke the Rx callback.
#if !defined( SERIAL_APP_THRESH )
#define SERIAL_APP_THRESH  64
#endif

#if !defined( SERIAL_APP_RX_SZ )
#define SERIAL_APP_RX_SZ  128
#endif

#if !defined( SERIAL_APP_TX_SZ )
#define SERIAL_APP_TX_SZ  128
#endif

// Millisecs of idle time after a byte is received before invoking Rx callback.
#if !defined( SERIAL_APP_IDLE )
#define SERIAL_APP_IDLE  6
#endif

// Loopback Rx bytes to Tx for throughput testing.
#if !defined( SERIAL_APP_LOOPBACK )
#define SERIAL_APP_LOOPBACK  FALSE
#endif

// This is the max byte count per OTA message.
#if !defined( SERIAL_APP_TX_MAX )
#define SERIAL_APP_TX_MAX  80
#endif

#define SERIAL_APP_RSP_CNT  4

/*********************************************************************
 * TYPEDEFS
 */
const cId_t SampleApp_ClusterList[SAMPLEAPP_MAX_CLUSTERS] =
{
  SAMPLEAPP_P2P_CLUSTERID, 
  SAMPLEAPP_PERIODIC_CLUSTERID
};
                        
const SimpleDescriptionFormat_t SampleApp_SimpleDesc =
{
    SAMPLEAPP_ENDPOINT,              //  int Endpoint;
    SAMPLEAPP_PROFID,                //  uint16 AppProfId[2];
    SAMPLEAPP_DEVICEID,              //  uint16 AppDeviceId[2];
    SAMPLEAPP_DEVICE_VERSION,        //  int   AppDevVer:4;
    SAMPLEAPP_FLAGS,                 //  int   AppFlags:4;
    SAMPLEAPP_MAX_CLUSTERS,          //  byte  AppNumInClusters;
    (cId_t *) SampleApp_ClusterList,  //  byte *pAppInClusterList;
    SAMPLEAPP_MAX_CLUSTERS,          //  byte  AppNumInClusters;
    (cId_t *) SampleApp_ClusterList   //  byte *pAppInClusterList;
};

endPointDesc_t SampleApp_epDesc;
devStates_t SampleApp_NwkState;
uint8 SampleApp_TaskID;
uint8 SampleApp_TransID;
afAddrType_t SampleApp_P2P_DstAddr;
afAddrType_t SampleApp_Periodic_DstAddr;

uint8 SampleAppDirection = 0;
uint8 SampleAppState = 0;

void SampleApp_MessageMSGCB(afIncomingMSGPacket_t *pkt);
void SampleApp_Send_P2P_Message( byte* SendData, byte len );
void SampleApp_SendPeriodicMessage( byte* SendData, byte len );
void SampleApp_ControlCar(void);
void SampleApp_DetectBlack(void);
void SampleApp_SendMessageGo(void);
void SampleApp_SendMessageStop(void);

void SampleApp_Init( uint8 task_id )
{
  SampleApp_TaskID = task_id;
  SampleApp_TransID = 0;
  
  //通信部分配置
  SampleApp_NwkState=DEV_INIT;
  
  //定义本设备用来通信的APS层端点描述符
  SampleApp_epDesc.endPoint = SAMPLEAPP_ENDPOINT;//应用程序的端口号
  SampleApp_epDesc.task_id = &SampleApp_TaskID;  //描述符的任务ID
  SampleApp_epDesc.simpleDesc                     //简单描述符
      = (SimpleDescriptionFormat_t *)&SampleApp_SimpleDesc;
  SampleApp_epDesc.latencyReq = noLatencyReqs;    //延时策略
  
  afRegister( &SampleApp_epDesc );                //向AF层登记描述符
  RegisterForKeys( task_id );
  
  //点播配置
  SampleApp_P2P_DstAddr.addrMode = (afAddrMode_t)Addr16Bit; //点播 
  SampleApp_P2P_DstAddr.endPoint = SAMPLEAPP_ENDPOINT; 
  SampleApp_P2P_DstAddr.addr.shortAddr = 0x0000;            //发给协调器
  
  //广播配置
  SampleApp_Periodic_DstAddr.addrMode = (afAddrMode_t)AddrBroadcast;//广播
  SampleApp_Periodic_DstAddr.endPoint = SAMPLEAPP_ENDPOINT;
  SampleApp_Periodic_DstAddr.addr.shortAddr = 0xFFFF;
  
  //串口配置
  halUARTCfg_t uartConfig;
  
  uartConfig.configured           = TRUE;              // 2x30 don't care - see uart driver.
  uartConfig.baudRate             = SERIAL_APP_BAUD;
  uartConfig.flowControl          = FALSE;
  uartConfig.flowControlThreshold = SERIAL_APP_THRESH; // 2x30 don't care - see uart driver.
  uartConfig.rx.maxBufSize        = SERIAL_APP_RX_SZ;  // 2x30 don't care - see uart driver.
  uartConfig.tx.maxBufSize        = SERIAL_APP_TX_SZ;  // 2x30 don't care - see uart driver.
  uartConfig.idleTimeout          = SERIAL_APP_IDLE;   // 2x30 don't care - see uart driver.
  uartConfig.intEnable            = TRUE;              // 2x30 don't care - see uart driver.
  //uartConfig.callBackFunc         = SerialApp_CallBack;
  
  HalUARTOpen (SERIAL_APP_PORT, &uartConfig);
  
  //执行部分配置
  hal_car_configure();
  hal_track_configure();
  
//  osal_set_event( SampleApp_TaskID, SAMPLEAPP_ARRIVE_EVT );
//  osal_set_event( SampleApp_TaskID, SAMPLEAPP_CAR_EVT );
//  osal_set_event( SampleApp_TaskID, SAMPLEAPP_COLOR_EVT );
  
}

uint16 SampleApp_ProcessEvent( uint8 task_id, uint16 events )
{
  afIncomingMSGPacket_t *MSGpkt;
  (void)task_id;  // Intentionally unreferenced parameter
  
//自行定义的事件处理函数
  if ( events & SAMPLEAPP_CAR_EVT )
  {
    SampleApp_ControlCar();  
    return( events ^ SAMPLEAPP_CAR_EVT );
  }
//自行定义的事件处理函数
  else if( events & SAMPLEAPP_COLOR_EVT ) //此时检测到黑带
  {
    SampleApp_DetectBlack();  
    return( events ^ SAMPLEAPP_COLOR_EVT );
  }
  else if( events & SAMPLEAPP_SENDGO_EVT )
  {
    SampleApp_SendMessageGo();
    return( events ^ SAMPLEAPP_SENDGO_EVT );
  }
  else if ( events & SYS_EVENT_MSG )
    {
        MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( SampleApp_TaskID );
        while ( MSGpkt )
        {
            switch ( MSGpkt->hdr.event )
            {
            case ZDO_STATE_CHANGE:
                  SampleApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
                  if(SampleApp_NwkState == DEV_END_DEVICE)
                  {
                  }
                  break;
            case AF_INCOMING_MSG_CMD:
                SampleApp_MessageMSGCB(MSGpkt);
                break;
            default:
                break;
            }
            osal_msg_deallocate( (uint8 *)MSGpkt );
            MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( SampleApp_TaskID );
        }
        return (events ^ SYS_EVENT_MSG);
    }
  return 0;
}

void SampleApp_SendPeriodicMessage( byte* SendData, byte len )
{
  if ( AF_DataRequest( &SampleApp_Periodic_DstAddr, &SampleApp_epDesc,
                       SAMPLEAPP_PERIODIC_CLUSTERID,
                       len,
                       SendData,
                       &SampleApp_TransID,
                       AF_DISCV_ROUTE,
                       AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
  }
  else
  {
    // Error occurred in request to send.
  }
}

void SampleApp_Send_P2P_Message( byte* SendData, byte len )
{
  
  if ( AF_DataRequest( &SampleApp_P2P_DstAddr, &SampleApp_epDesc,
                       SAMPLEAPP_P2P_CLUSTERID,
                       len,
                       SendData,
                       &SampleApp_TransID,
                       AF_DISCV_ROUTE,
                       AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
    HalLedBlink(HAL_LED_1, 0, 50, 500);
  }
  else
  {
    HalLedBlink(HAL_LED_2, 0, 50, 500);
    // Error occurred in request to send.
  }
}

void SampleApp_MessageMSGCB(afIncomingMSGPacket_t *pkt)
{
    unsigned char buf[3]; 
    
    switch ( pkt->clusterId )
    {
    case SAMPLEAPP_PERIODIC_CLUSTERID:
        osal_memset(buf, 0 , 3);
        osal_memcpy(buf, pkt->cmd.Data, 2);
        
        if( buf[0] == '2' && buf[1] == '1' )       
        {
            //osal_set_event( SampleApp_TaskID, SAMPLEAPP_CAR_EVT );
            //osal_set_event( SampleApp_TaskID, SAMPLEAPP_COLOR_EVT );
            osal_set_event( SampleApp_TaskID, SAMPLEAPP_CAR_EVT );
            osal_set_event( SampleApp_TaskID, SAMPLEAPP_COLOR_EVT );
            osal_set_event( SampleApp_TaskID, SAMPLEAPP_SENDGO_EVT );
            HalLedBlink(HAL_LED_1, 0, 50, 500);            
        }
        else if( buf[0] == '0' && buf[1] ==  '1' )
        {
         //   HalLedBlink(HAL_LED_2, 0, 50, 500);
        }
        break;
    }
}

void SampleApp_ControlCar(void)
{
      if( SampleAppState == 0  )
      {
        if( SampleAppDirection == 1 )
        {
          hal_car_turnleft();
          osal_start_timerEx( SampleApp_TaskID, SAMPLEAPP_CAR_EVT, 5 );
        }
        if( SampleAppDirection == 2 )
        {
          hal_car_turnright();
          osal_start_timerEx( SampleApp_TaskID, SAMPLEAPP_CAR_EVT, 5 );
        }
        if( SampleAppDirection == 0 )
        {
          hal_car_go();
          osal_start_timerEx( SampleApp_TaskID, SAMPLEAPP_CAR_EVT, 5 );
        }
        if( SampleAppDirection == 3 )
        {
          hal_car_stop();
        }
        SampleAppState = 1;
        
      }
      else if( SampleAppState == 1 )
      {
        hal_car_stop();
        SampleAppState = 0;
        osal_start_timerEx( SampleApp_TaskID, SAMPLEAPP_CAR_EVT, 15 );
      }    
}
void SampleApp_DetectBlack(void)
{
      if( Color_1 == 1 && Color_2 == 0 )
      {
        SampleAppDirection = 1;
        osal_start_timerEx( SampleApp_TaskID, SAMPLEAPP_COLOR_EVT, 100 );
      }
      else if( Color_1 == 0 && Color_2 == 1 )   
      {
        SampleAppDirection = 2;
        osal_start_timerEx( SampleApp_TaskID, SAMPLEAPP_COLOR_EVT, 100 );
      }
      else if( Color_1 == 0 && Color_2 == 0 )
      {
        SampleAppDirection = 0;
        osal_start_timerEx( SampleApp_TaskID, SAMPLEAPP_COLOR_EVT, 50 );
      }    
      else if( Color_1 == 1 && Color_2 == 1 )
      {
        SampleAppDirection = 3;
        SampleApp_SendMessageStop();
      }
}

void SampleApp_SendMessageGo(void)
{
      byte SendData[3] = "21";
      byte len = 2;
      SampleApp_Send_P2P_Message( SendData, len );
}

void SampleApp_SendMessageStop(void)
{
      byte SendData[3] = "20";
      byte len = 2;
      SampleApp_Send_P2P_Message( SendData, len );
}