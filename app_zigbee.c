
/* USER CODE BEGIN Header */

/*MY CODE ONOFF COORD*/
/**
  ******************************************************************************
  * @file    App/app_zigbee.c
  * @author  MCD Application Team
  * @brief   Zigbee Application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "app_entry.h"
#include "dbg_trace.h"
#include "app_zigbee.h"
#include "zigbee_interface.h"
#include "shci.h"
#include "stm_logging.h"
#include "app_conf.h"
#include "stm32wbxx_core_interface_def.h"
#include "zigbee_types.h"
#include "stm32_seq.h"

/* Private includes -----------------------------------------------------------*/
#include <assert.h>
#include "zcl/zcl.h"
#include "zcl/general/zcl.onoff.h"
#include "zcl/general/zcl.therm.h"


/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define APP_ZIGBEE_STARTUP_FAIL_DELAY               500U
#define CHANNEL                                     13

#define SW1_ENDPOINT                                17
#define ICON_ENDPOINT                               0x01

/* USER CODE BEGIN PD */
#define SW1_GROUP_ADDR                              0x0001
/** Device Temperature Cluster Attribute Ids */

#define ExternalMeasuredRoomSensor 					0x4015
#define SetpointChangeSource 						0x0030



#define ZCL_DEVICE_TEMP_MIN                         (-200)
#define ZCL_DEVICE_TEMP_MAX                         200
#define ZCL_DEVICE_TEMP_INI     					35


/* USER CODE END PAPP_ZIGBEE_ProcessNotifyM0ToM4D */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* External definition -------------------------------------------------------*/
enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config);

/* USER CODE BEGIN ED */
/* USER CODE END ED */

/* Private function prototypes -----------------------------------------------*/
static void APP_ZIGBEE_StackLayersInit(void);
static void APP_ZIGBEE_ConfigEndpoints(void);
static void APP_ZIGBEE_NwkForm(void);

static void APP_ZIGBEE_TraceError(const char *pMess, uint32_t ErrCode);
static void APP_ZIGBEE_CheckWirelessFirmwareInfo(void);

static void Wait_Getting_Ack_From_M0(void);
static void Receive_Ack_From_M0(void);
static void Receive_Notification_From_M0(void);

static void APP_ZIGBEE_ProcessNotifyM0ToM4(void);
static void APP_ZIGBEE_ProcessRequestM0ToM4(void);

/* USER CODE BEGIN PFP */
static void APP_ZIGBEE_ConfigGroupAddr(void);
//static void APP_ZIGBEE_APS_Conf_cb(struct ZbApsdeDataConfT *conf, void *arg);
//callback function prot.
static void APP_ZIGBEE_EP_cb(struct ZbZdoActiveEpRspT *rsp, void *cb_arg);
static void APP_ZIGBEE_discoverCluster_cb(struct ZbZdoSimpleDescRspT *rsp, void *cb_arg);
static void Shortrequest_cb(struct ZbZdoNwkAddrRspT  *rsp, void *cb_arg);
static void Bindrequest_cb(struct ZbZdoBindRspT *rsp, void *cb_arg);
static void ReportConfig_cb(struct ZbZclCommandRspT  *cmd_rsp, void *arg);
static void ReportRead_cb(struct ZbZclCommandRspT  *cmd_rsp, void *arg);
static void APP_ZIGBEE_getendpoints(void);
static void APP_ZIGBEE_discoverCluster(void);
static void APP_ZIGBEE_InitDevTemp(void);
static void APP_ZIGBEE_Bindrequest(void);
static void APP_ZIGBEE_ReadValue(void);
static void APP_ZIGBEE_ReportRead(void);
static void APP_ZIGBEE_ReportConfig(void);
static void APP_ZIGBEE_Bindrequest(void);
static void APP_ZIGBEE_Button1(void);
static void APP_ZIGBEE_Button3(void);
static void APP_ZIGBEE_asgn_dst_addr(void);
static void asgn_dst_addr_cb(struct ZbZdoNwkAddrRspT  *rsp, void *cb_arg);
static void device_temp_read_cb(const ZbZclReadRspT *readRsp, void *arg);

//function that returns Pan address of ZB instance
uint16_t ZbShortAddressPan(struct ZigBeeT *zb);
/* USER CODE END PFP */

/* Private variables ---------------------------------------------------------*/
static TL_CmdPacket_t   *p_ZIGBEE_otcmdbuffer;
static TL_EvtPacket_t   *p_ZIGBEE_notif_M0_to_M4;
static TL_EvtPacket_t   *p_ZIGBEE_request_M0_to_M4;
static __IO uint32_t    CptReceiveNotifyFromM0 = 0;
static __IO uint32_t    CptReceiveRequestFromM0 = 0;

PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_ZIGBEE_Config_t ZigbeeConfigBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t ZigbeeOtCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ZigbeeNotifRspEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ZigbeeNotifRequestBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];


struct ZbApsAddrT myaddr;

struct zigbee_app_info
{
  bool has_init;
  struct ZigBeeT *zb;
  enum ZbStartType startupControl;
  enum ZbStatusCodeT join_status;

  uint32_t join_delay;
  bool init_after_join;
  struct ZbStartupT config_info;
  uint16_t panID;
  uint16_t nwkAddr;
  uint64_t extaddr;
  uint16_t epCount;
  uint16_t ep[10];
  uint64_t thermextaddr;
  uint16_t thermshortaddr;
  uint16_t thermep;
  bool discoverClusterStatus;
  bool bindingfinished;
  bool reportConfigFinished;
  bool reportReadFinished;
  enum ZclStatusCodeT statusgetendpoint;
  struct ZbZclClusterT *device_therm_server;
  struct ZbZclClusterT *device_therm_client;


};
static struct zigbee_app_info zigbee_app_info;

//static enum ZclStatusCodeT thermsetpoint(struct ZbZclClusterT *cluster, void *arg, struct ZbZclThermCliSetpointT *req, struct ZbZclAddrInfoT *srcInfo);

/*static struct ZbZclThermServerCallbacksT  Therm_cb1 =
{
	.setpoint_raise_lower = thermsetpoint,
	.set_weekly =0x00,
	.get_weekly=0x00,
	.clear_weekly=0x00,
	.get_relay_status_log=0x00,
};*/

/*static enum ZclStatusCodeT thermsetpoint(struct ZbZclClusterT *cluster, void *arg, struct ZbZclThermCliSetpointT *req, struct ZbZclAddrInfoT *srcInfo)
//	        struct ZbZclThermCliSetpointT *req, struct ZbZclAddrInfoT *srcInfo);
{
	uint8_t endpoint;
	endpoint = ZbZclClusterGetEndpoint(cluster);
	if (endpoint == ICON_ENDPOINT)
	  {
		APP_DBG("thermsetpoint ICON_ENDPOINT recognized ");
		return ZCL_STATUS_SUCCESS;
		//(void)ZbZclAttrIntegerRead(cluster, 0x0000, 0);
		//Attribute id 0x0000 Local Temperature
	  }
	else
	{
		return ZCL_STATUS_FAILURE;
	}

}
*/

/**
 * @brief  Zigbee application initialization
 * @param  None
 * @retval None
 */
void APP_ZIGBEE_Init(void)
{
  SHCI_CmdStatus_t ZigbeeInitStatus;

  APP_DBG("APP_ZIGBEE_Init");

  /* Check the compatibility with the Coprocessor Wireless Firmware loaded */
  APP_ZIGBEE_CheckWirelessFirmwareInfo();
  APP_DBG("after APP_ZIGBEE_CheckWirelessFirmwareInfo()");


  /* Register cmdbuffer */
  APP_ZIGBEE_RegisterCmdBuffer(&ZigbeeOtCmdBuffer);
  APP_DBG("after APP_ZIGBEE_RegisterCmdBuffer()");

  /* Init config buffer and call TL_ZIGBEE_Init */
  APP_ZIGBEE_TL_INIT();
  APP_DBG("after APP_ZIGBEE_TL_INIT()");

  /* Register task */
  /* Create the different tasks */
  UTIL_SEQ_RegTask(1U << (uint32_t)CFG_TASK_NOTIFY_FROM_M0_TO_M4, UTIL_SEQ_RFU, APP_ZIGBEE_ProcessNotifyM0ToM4);
  APP_DBG("after APP_ZIGBEE_ProcessNotifyM0ToM4()");
  UTIL_SEQ_RegTask(1U << (uint32_t)CFG_TASK_REQUEST_FROM_M0_TO_M4, UTIL_SEQ_RFU, APP_ZIGBEE_ProcessRequestM0ToM4);
  APP_DBG("after APP_ZIGBEE_ProcessRequestM0ToM4()");
  /* Task associated with network creation process */

  UTIL_SEQ_RegTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, UTIL_SEQ_RFU, APP_ZIGBEE_NwkForm);
  UTIL_SEQ_RegTask(1U << CFG_TASK_mytask_getendpoints, UTIL_SEQ_RFU, APP_ZIGBEE_getendpoints);
  UTIL_SEQ_RegTask(1U << CFG_TASK_mytask_getclusters, UTIL_SEQ_RFU, APP_ZIGBEE_discoverCluster);

  /* Task associated with push button SW1 */
  UTIL_SEQ_RegTask( 1U << CFG_TASK_BUTTON_SW1, UTIL_SEQ_RFU, APP_ZIGBEE_Button1);

  /* Task associated with push button SW2*/
  UTIL_SEQ_RegTask( 1U << CFG_TASK_BUTTON_SW2, UTIL_SEQ_RFU, APP_ZIGBEE_ReadValue);

  /* Task associated with push button SW2*/
  UTIL_SEQ_RegTask( 1U << CFG_TASK_BUTTON_SW3, UTIL_SEQ_RFU, APP_ZIGBEE_Button3);
  APP_DBG("after register tasks");

  /* USER CODE BEGIN APP_ZIGBEE_INIT */
  /* USER CODE END APP_ZIGBEE_INIT */

  /* Start the Zigbee on the CPU2 side */
  ZigbeeInitStatus = SHCI_C2_ZIGBEE_Init();
  /* Prevent unused argument(s) compilation warning */
  APP_DBG("ZigbeeInitStatus, %d", ZigbeeInitStatus);
  UNUSED(ZigbeeInitStatus);

  /* Initialize Zigbee stack layers */
  APP_ZIGBEE_StackLayersInit();
  APP_DBG("after APP_ZIGBEE_StackLayersInit");

  //zigbee_app_info.addr = ZbShortAddress(zigbee_app_info.zb);

  //APP_DBG("zigbee_app_info.panID after nwkform 0x%02x",zigbee_app_info.addr);


}



/**
 * @brief  Initialize Zigbee stack layers
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_StackLayersInit(void)
{
  APP_DBG("internal APP_ZIGBEE_StackLayersInit");

  zigbee_app_info.zb = ZbInit(0U, NULL, NULL);
  APP_DBG("after ZbInit");
  assert(zigbee_app_info.zb != NULL);
  APP_DBG("ZB instance created");

  /* Create the endpoint and cluster(s) */
  APP_ZIGBEE_ConfigEndpoints();
  //APP_DBG("after APP_ZIGBEE_ConfigEndpoints");

  /* USER CODE BEGIN APP_ZIGBEE_StackLayersInit */
  BSP_LED_Off(LED_RED);
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_BLUE);
  /* USER CODE END APP_ZIGBEE_StackLayersInit */

  /* Configure the joining parameters */
  zigbee_app_info.join_status = (enum ZbStatusCodeT) 0x01; /* init to error status */
  zigbee_app_info.join_delay = HAL_GetTick(); /* now */
  //APP_DBG("zigbee_app_info.startupControl before assigning 0x%02x",zigbee_app_info.startupControl);
  //APP_DBG("zigbee_app_info.panID before assigning 0x%02x",zigbee_app_info.panId);
  zigbee_app_info.startupControl = ZbStartTypeForm;
  //APP_DBG("zigbee_app_info.startupControl after assigning  0x%02x",zigbee_app_info.startupControl);
  //APP_DBG("zigbee_app_info.panID before assigning 0x%02x",zigbee_app_info.panId);

  /* Initialization Complete */
  zigbee_app_info.has_init = true;
  zigbee_app_info.thermextaddr  = 0x003c84fffe02386a;
  zigbee_app_info.thermshortaddr = 0x0000U;
  zigbee_app_info.epCount = 0;
  zigbee_app_info.discoverClusterStatus=0;
  zigbee_app_info.bindingfinished =0;
  zigbee_app_info.reportConfigFinished = 0;
  zigbee_app_info.reportReadFinished = 0;

  //myaddr.mode = ZB_APSDE_ADDRMODE_EXT;
  //myaddr.extAddr = 0x003c84fffe02386a;


  /* run the task */
  APP_DBG("set CFG_TASK_ZIGBEE_NETWORK_FORM prio to 0");
  UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, CFG_SCH_PRIO_0);
}

/**
 * @brief  Configure Zigbee application endpoints
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ConfigEndpoints(void)
{
  struct ZbApsmeAddEndpointReqT req;
  struct ZbApsmeAddEndpointConfT conf;

  memset(&req, 0, sizeof(req));
 /* zigbee_app_info.device_temp_server = ZbZclThermServerAlloc (zigbee_app_info.zb, ICON_ENDPOINT, &Therm_cb1, NULL);
  assert(zigbee_app_info.device_temp_server != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.device_temp_server);
*/

  /* USER CODE BEGIN CONFIG_ENDPOINT */
  /* Endpoint: ICON_ENDPOINT */
	req.profileId = ZCL_PROFILE_HOME_AUTOMATION;
	req.deviceId = ZCL_DEVICE_THERMOSTAT;
	req.endpoint = ICON_ENDPOINT;
	ZbZclAddEndpoint(zigbee_app_info.zb, &req, &conf);
	assert(conf.status == ZB_STATUS_SUCCESS);

	zigbee_app_info.device_therm_client = ZbZclThermClientAlloc (zigbee_app_info.zb, ICON_ENDPOINT);
	assert(zigbee_app_info.device_therm_client != NULL);
	ZbZclClusterEndpointRegister(zigbee_app_info.device_therm_client);
  /* USER CODE END CONFIG_ENDPOINT */
}

/**
 * @brief  Handle Zigbee network forming and joining
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_NwkForm(void)
{
	APP_DBG("start APP_ZIGBEE_NwkForm");
  if ((zigbee_app_info.join_status != ZB_STATUS_SUCCESS) && (HAL_GetTick() >= zigbee_app_info.join_delay))
  {
    struct ZbStartupT config;
    enum ZbStatusCodeT status;

    /* Configure Zigbee Logging */
    ZbSetLogging(zigbee_app_info.zb, ZB_LOG_MASK_LEVEL_ALL, NULL);

    /* Attempt to join a zigbee network */
    APP_DBG("Attempt to join a zigbee network ");
    APP_DBG("Send configuration values to M0 ");
    ZbStartupConfigGetProDefaults(&config);

    /* Set the centralized network */
    APP_DBG("Network config : APP_STARTUP_CENTRALIZED_COORDINATOR");
    config.startupControl = zigbee_app_info.startupControl;

    /* Using the default HA preconfigured Link Key */
    memcpy(config.security.preconfiguredLinkKey, sec_key_ha, ZB_SEC_KEYSIZE);
    //APP_DBG("Security key preconfiguredLinkKey has been set");

    config.channelList.count = 1;
    config.channelList.list[0].page = 0;
    config.channelList.list[0].channelMask = 1 << CHANNEL; /*Channel in use */
    //APP_DBG("config channel details have been set");

    /* Using ZbStartupWait (blocking) */
    status = ZbStartupWait(zigbee_app_info.zb, &config);
    //APP_DBG("after ZbStartupWait");

    APP_DBG("ZbStartup Callback (status = 0x%02x)", status);
    //zigbee_app_info.panID = config.panId;
    //APP_DBG("zigbee_app_info.panID after ZbStartupWait 0x%02x",zigbee_app_info.panID);
    zigbee_app_info.join_status = status;

    if (status == ZB_STATUS_SUCCESS)
    {
      zigbee_app_info.join_delay = 0U;
      zigbee_app_info.init_after_join = true;
      APP_DBG("Startup done !\n");
      /* USER CODE BEGIN 3 */
      BSP_LED_On(LED_BLUE);
      zigbee_app_info.panID = ZbShortAddressPan(zigbee_app_info.zb);
      zigbee_app_info.extaddr = ZbExtendedAddress(zigbee_app_info.zb);
      zigbee_app_info.nwkAddr = ZbShortAddress(zigbee_app_info.zb);
      zigbee_app_info.statusgetendpoint = ZB_STATUS_ALLOC_FAIL;

      //zigbee_app_info.addr = ZbShortAddressPan(zigbee_app_info.zb);
      APP_DBG("Network created ");
      APP_DBG("zigbee_app_info.Network panID after request 0x%02x",zigbee_app_info.panID);
      APP_DBG("zigbee_app_info.Network extaddr after request 0x%22x",zigbee_app_info.extaddr);
      APP_DBG("zigbee_app_info.Network nwkAddr after request 0x%02x",zigbee_app_info.nwkAddr);

      /* USER CODE END 3 */
    }
    else
    {
      APP_DBG("Startup failed, attempting again after a short delay (%d ms)", APP_ZIGBEE_STARTUP_FAIL_DELAY);
      zigbee_app_info.join_delay = HAL_GetTick() + APP_ZIGBEE_STARTUP_FAIL_DELAY;
      /* USER CODE BEGIN 4 */

      /* USER CODE END 4 */
    }
  }

  /* If Network forming/joining was not successful reschedule the current task to retry the process */
  if (zigbee_app_info.join_status != ZB_STATUS_SUCCESS)
  {
	  APP_DBG("Network forming/joining was not successful, set CFG_TASK_ZIGBEE_NETWORK_FORM prio to 0");
    UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, CFG_SCH_PRIO_0);
  }
  /* USER CODE BEGIN NW_FORM */
  else
  {
    zigbee_app_info.init_after_join = false;

    /* Assign ourselves to the group addresses */
    APP_ZIGBEE_ConfigGroupAddr();
    APP_DBG("after ConfigGroupAddr");

    /* Since we're using group addressing (broadcast), shorten the broadcast timeout */
    uint32_t bcast_timeout = 3;
    ZbNwkSet(zigbee_app_info.zb, ZB_NWK_NIB_ID_NetworkBroadcastDeliveryTime, &bcast_timeout, sizeof(bcast_timeout));
    APP_DBG("after ZbNwkSet");
    //APP_ZIGBEE_InitDevTemp();

    }
  /* USER CODE END NW_FORM */
  APP_ZIGBEE_getendpoints();
}


/*************************************************************
 * ZbStartupWait Blocking Call
 *************************************************************/
struct ZbStartupWaitInfo
{
  bool active;
  enum ZbStatusCodeT status;
};

static void ZbStartupWaitCb(enum ZbStatusCodeT status, void *cb_arg)
{
  struct ZbStartupWaitInfo *info = cb_arg;

  info->status = status;
  info->active = false;
  UTIL_SEQ_SetEvt(EVENT_ZIGBEE_STARTUP_ENDED);
}

enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config)
{
	APP_DBG("start ZbStartupWait ");
  struct ZbStartupWaitInfo *info;
  enum ZbStatusCodeT status;

  info = malloc(sizeof(struct ZbStartupWaitInfo));
  if (info == NULL)
  {
    return ZB_STATUS_ALLOC_FAIL;
  }
  memset(info, 0, sizeof(struct ZbStartupWaitInfo));

  info->active = true;
  APP_DBG("start stack function ZbStartup ");
  status = ZbStartup(zb, config, ZbStartupWaitCb, info);
  if (status != ZB_STATUS_SUCCESS)
  {
    info->active = false;
    return status;
  }

  APP_DBG("before UTIL_SEQ_WaitEvt start EVENT_ZIGBEE_STARTUP_ENDED ");
  UTIL_SEQ_WaitEvt(EVENT_ZIGBEE_STARTUP_ENDED);
  APP_DBG("after UTIL_SEQ_WaitEvt start EVENT_ZIGBEE_STARTUP_ENDED ");
  status = info->status;
  free(info);
  return status;
}

/**
 * @brief  Trace the error or the warning reported.
 * @param  ErrId :
 * @param  ErrCode
 * @retval None
 */
void APP_ZIGBEE_Error(uint32_t ErrId, uint32_t ErrCode)
{
  switch (ErrId)
  {
    default:
      APP_ZIGBEE_TraceError("ERROR Unknown ", 0);
      break;
  }
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/**
 * @brief  Warn the user that an error has occurred.
 *
 * @param  pMess  : Message associated to the error.
 * @param  ErrCode: Error code associated to the module (Zigbee or other module if any)
 * @retval None
 */
static void APP_ZIGBEE_TraceError(const char *pMess, uint32_t ErrCode)
{
  APP_DBG("**** Fatal error = %s (Err = %d)", pMess, ErrCode);
  /* USER CODE BEGIN TRACE_ERROR */
  while (1U == 1U) 
  {
    BSP_LED_Toggle(LED1);
    HAL_Delay(500U);
    BSP_LED_Toggle(LED2);
    HAL_Delay(500U);
    BSP_LED_Toggle(LED3);
    HAL_Delay(500U);
  }
  /* USER CODE END TRACE_ERROR */

}

/**
 * @brief Check if the Coprocessor Wireless Firmware loaded supports Zigbee
 *        and display associated information
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_CheckWirelessFirmwareInfo(void)
{
  WirelessFwInfo_t wireless_info_instance;
  WirelessFwInfo_t *p_wireless_info = &wireless_info_instance;

  APP_DBG("internal APP_ZIGBEE_CheckWirelessFirmwareInfo");


  if (SHCI_GetWirelessFwInfo(p_wireless_info) != SHCI_Success)
  {
    APP_ZIGBEE_Error((uint32_t)ERR_ZIGBEE_CHECK_WIRELESS, (uint32_t)ERR_INTERFACE_FATAL);
  }
  else
  {
    APP_DBG("**********************************************************");
    APP_DBG("WIRELESS COPROCESSOR FW:");
    /* Print version */
    APP_DBG("VERSION ID = %d.%d.%d", p_wireless_info->VersionMajor, p_wireless_info->VersionMinor, p_wireless_info->VersionSub);

    switch (p_wireless_info->StackType)
    {
      case INFO_STACK_TYPE_ZIGBEE_FFD:
        APP_DBG("FW Type : FFD Zigbee stack");
        break;

      case INFO_STACK_TYPE_ZIGBEE_RFD:
        APP_DBG("FW Type : RFD Zigbee stack");
        break;

      default:
        /* No Zigbee device supported ! */
        APP_ZIGBEE_Error((uint32_t)ERR_ZIGBEE_CHECK_WIRELESS, (uint32_t)ERR_INTERFACE_FATAL);
        break;
    }

    /* print the application name */
    char *__PathProject__ = (strstr(__FILE__, "Zigbee") ? strstr(__FILE__, "Zigbee") + 7 : __FILE__);
    char *pdel = NULL;
    if((strchr(__FILE__, '/')) == NULL)
    {
      pdel = strchr(__PathProject__, '\\');
    }
    else
    {
      pdel = strchr(__PathProject__, '/');
    }

    int index = (int)(pdel - __PathProject__);
    APP_DBG("Application flashed: %*.*s", index, index, __PathProject__);

    /* print channel */
    APP_DBG("Channel used: %d", CHANNEL);
    /* print Link Key */
    APP_DBG("Link Key: %.16s", sec_key_ha);
    /* print Link Key value hex */
    char Z09_LL_string[ZB_SEC_KEYSIZE*3+1];
    Z09_LL_string[0] = 0;
    for (int str_index = 0; str_index < ZB_SEC_KEYSIZE; str_index++)
    {
      sprintf(&Z09_LL_string[str_index*3], "%02x ", sec_key_ha[str_index]);
    }

    APP_DBG("Link Key value: %s", Z09_LL_string);
    /* print clusters allocated */
    APP_DBG("Clusters allocated are:");
    APP_DBG("onOff Server on Endpoint %d", SW1_ENDPOINT);
    APP_DBG("**********************************************************");
  }
}

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/

void APP_ZIGBEE_RegisterCmdBuffer(TL_CmdPacket_t *p_buffer)
{
  p_ZIGBEE_otcmdbuffer = p_buffer;
  APP_DBG("internal APP_ZIGBEE_RegisterCmdBuffer");
}

Zigbee_Cmd_Request_t * ZIGBEE_Get_OTCmdPayloadBuffer(void)
{
  return (Zigbee_Cmd_Request_t *)p_ZIGBEE_otcmdbuffer->cmdserial.cmd.payload;
}

Zigbee_Cmd_Request_t * ZIGBEE_Get_OTCmdRspPayloadBuffer(void)
{
  return (Zigbee_Cmd_Request_t *)((TL_EvtPacket_t *)p_ZIGBEE_otcmdbuffer)->evtserial.evt.payload;
}

Zigbee_Cmd_Request_t * ZIGBEE_Get_NotificationPayloadBuffer(void)
{
  return (Zigbee_Cmd_Request_t *)(p_ZIGBEE_notif_M0_to_M4)->evtserial.evt.payload;
}

Zigbee_Cmd_Request_t * ZIGBEE_Get_M0RequestPayloadBuffer(void)
{
  return (Zigbee_Cmd_Request_t *)(p_ZIGBEE_request_M0_to_M4)->evtserial.evt.payload;
}

/**
 * @brief  This function is used to transfer the commands from the M4 to the M0.
 *
 * @param   None
 * @return  None
 */
void ZIGBEE_CmdTransfer(void)
{
  Zigbee_Cmd_Request_t *cmd_req = (Zigbee_Cmd_Request_t *)p_ZIGBEE_otcmdbuffer->cmdserial.cmd.payload;

  /* Zigbee OT command cmdcode range 0x280 .. 0x3DF = 352 */
  p_ZIGBEE_otcmdbuffer->cmdserial.cmd.cmdcode = 0x280U;
  /* Size = otCmdBuffer->Size (Number of OT cmd arguments : 1 arg = 32bits so multiply by 4 to get size in bytes)
   * + ID (4 bytes) + Size (4 bytes) */
  p_ZIGBEE_otcmdbuffer->cmdserial.cmd.plen = 8U + (cmd_req->Size * 4U);

  TL_ZIGBEE_SendM4RequestToM0();

  /* Wait completion of cmd */
  Wait_Getting_Ack_From_M0();
}

/**
 * @brief  This function is called when the M0+ acknowledge the fact that it has received a Cmd
 *
 *
 * @param   Otbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_ZIGBEE_CmdEvtReceived(TL_EvtPacket_t *Otbuffer)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Otbuffer);

  Receive_Ack_From_M0();
}

/**
 * @brief  This function is called when notification from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_ZIGBEE_NotReceived(TL_EvtPacket_t *Notbuffer)
{
  p_ZIGBEE_notif_M0_to_M4 = Notbuffer;

  Receive_Notification_From_M0();
}

/**
 * @brief  This function is called before sending any ot command to the M0
 *         core. The purpose of this function is to be able to check if
 *         there are no notifications coming from the M0 core which are
 *         pending before sending a new ot command.
 * @param  None
 * @retval None
 */
void Pre_ZigbeeCmdProcessing(void)
{
  UTIL_SEQ_WaitEvt(EVENT_SYNCHRO_BYPASS_IDLE);
}

/**
 * @brief  This function waits for getting an acknowledgment from the M0.
 *
 * @param  None
 * @retval None
 */
static void Wait_Getting_Ack_From_M0(void)
{
  UTIL_SEQ_WaitEvt(EVENT_ACK_FROM_M0_EVT);
}

/**
 * @brief  Receive an acknowledgment from the M0+ core.
 *         Each command send by the M4 to the M0 are acknowledged.
 *         This function is called under interrupt.
 * @param  None
 * @retval None
 */
static void Receive_Ack_From_M0(void)
{
  UTIL_SEQ_SetEvt(EVENT_ACK_FROM_M0_EVT);
}

/**
 * @brief  Receive a notification from the M0+ through the IPCC.
 *         This function is called under interrupt.
 * @param  None
 * @retval None
 */
static void Receive_Notification_From_M0(void)
{
  CptReceiveNotifyFromM0++;
  //APP_DBG("set CFG_TASK_NOTIFY_FROM_M0_TO_M4 prio to 0");
  UTIL_SEQ_SetTask(1U << (uint32_t)CFG_TASK_NOTIFY_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
}

/**
 * @brief  This function is called when a request from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_ZIGBEE_M0RequestReceived(TL_EvtPacket_t *Reqbuffer)
{
  p_ZIGBEE_request_M0_to_M4 = Reqbuffer;

  CptReceiveRequestFromM0++;
  //APP_DBG("set CFG_TASK_REQUEST_FROM_M0_TO_M4 prio to 0");
  UTIL_SEQ_SetTask(1U << (uint32_t)CFG_TASK_REQUEST_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
}

/**
 * @brief Perform initialization of TL for Zigbee.
 * @param  None
 * @retval None
 */
void APP_ZIGBEE_TL_INIT(void)
{
  ZigbeeConfigBuffer.p_ZigbeeOtCmdRspBuffer = (uint8_t *)&ZigbeeOtCmdBuffer;
  ZigbeeConfigBuffer.p_ZigbeeNotAckBuffer = (uint8_t *)ZigbeeNotifRspEvtBuffer;
  ZigbeeConfigBuffer.p_ZigbeeNotifRequestBuffer = (uint8_t *)ZigbeeNotifRequestBuffer;
  TL_ZIGBEE_Init(&ZigbeeConfigBuffer);
  APP_DBG("internal APP_ZIGBEE_Init");
}

/**
 * @brief Process the messages coming from the M0.
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ProcessNotifyM0ToM4(void)
{

	APP_DBG("start APP_ZIGBEE_ProcessNotifyM0ToM4");
	if (CptReceiveNotifyFromM0 != 0)
  {
    /* If CptReceiveNotifyFromM0 is > 1. it means that we did not serve all the events from the radio */
    if (CptReceiveNotifyFromM0 > 1U)
    {
      APP_ZIGBEE_Error(ERR_REC_MULTI_MSG_FROM_M0, 0);
    }
    else
    {
      Zigbee_CallBackProcessing();
    }

    /* Reset counter */
    CptReceiveNotifyFromM0 = 0;
  }
}

/**
 * @brief Process the requests coming from the M0.
 * @param
 * @return
 */
static void APP_ZIGBEE_ProcessRequestM0ToM4(void)
{
	APP_DBG("start APP_ZIGBEE_ProcessRequestM0ToM4");
	if (CptReceiveRequestFromM0 != 0)
  {
    Zigbee_M0RequestProcessing();
    //APP_DBG("CptReceiveRequestFromM0 is not empty");
    CptReceiveRequestFromM0 = 0;
  }
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */

/**
 * @brief  Set group addressing mode
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ConfigGroupAddr(void)
{
  struct ZbApsmeAddGroupReqT req;
  struct ZbApsmeAddGroupConfT conf;

  APP_DBG("start ConfigGroupAddr ");
  memset(&req, 0, sizeof(req));
  req.endpt = SW1_ENDPOINT;
  req.groupAddr = SW1_GROUP_ADDR;
  ZbApsmeAddGroupReq(zigbee_app_info.zb, &req, &conf);
}


/* Initialize current temperature */
static void APP_ZIGBEE_InitDevTemp()
{
  enum ZclStatusCodeT status;
  status = ZbZclAttrIntegerWrite(zigbee_app_info.device_therm_server, ZCL_DEV_TEMP_CURRENT, (int16_t)ZCL_DEVICE_TEMP_INI);
  if(status == ZCL_STATUS_SUCCESS)
  {
    APP_DBG("[DEV TEMP] Device Temperature initial value set at %d",(int16_t)ZCL_DEVICE_TEMP_INI);
    BSP_LED_On(LED_GREEN);
  }
  else
  {
    APP_DBG("[DEV TEMP]Failed to initialize initial Temperature");
  }

}

static void APP_ZIGBEE_EP_cb(struct ZbZdoActiveEpRspT *rsp, void *cb_arg)
{
	if (rsp->status == ZB_STATUS_SUCCESS)
	{
		APP_DBG("%s : EP_cb Callback success status = 0x%02x", __func__, rsp->status);
		APP_DBG("EP Callback success nwkAddr = 0x%02x",  rsp->nwkAddr);
		APP_DBG("Callback success endpoint count = 0x%02x",  rsp->activeEpCount);
		zigbee_app_info.epCount = rsp->activeEpCount;
		for (int i = 0; i<rsp->activeEpCount ; i++ )
			{
				APP_DBG("Callback success rsp endpoint = 0x%02x",  rsp->activeEpList[i]);
				zigbee_app_info.ep[i] = rsp->activeEpList[i];
			}
		zigbee_app_info.statusgetendpoint = ZB_STATUS_SUCCESS;

		APP_DBG("Callback success info.ep = %d",  zigbee_app_info.ep[0]);
		APP_DBG("Callback success info.ep = %d",  zigbee_app_info.ep[1]);
		//return ZB_STATUS_SUCCESS;
	}
	else
	{
		/* Unknown endpoint */
		//return ZCL_STATUS_FAILURE;
		APP_DBG("EP_cb Callback fail");
	}
}

static void Shortrequest_cb(struct ZbZdoNwkAddrRspT  *rsp, void *cb_arg)
{
	if (rsp->status == ZB_STATUS_SUCCESS)
	{
		APP_DBG("%s : Shortrequest_cb Callback success status = 0x%02x", __func__, rsp->status);
	    APP_DBG("Short request extAddr = 0x%02x",  rsp->extAddr);
		APP_DBG("Short request nwkAddr = 0x%02x",  rsp->nwkAddr);
		zigbee_app_info.thermshortaddr = rsp->nwkAddr;
	}
	else
	{
		APP_DBG("Short request  Callback fail");
	}
}

static void APP_ZIGBEE_getendpoints(void)
{
	APP_DBG("start APP_ZIGBEE_getendpoints");

	uint64_t epid = 0U;

	//For short address
	enum ZclStatusCodeT ShortrequestStatus;
	struct ZbZdoNwkAddrReqT shortrequest;
	shortrequest.extAddr = 0x003c84fffe02386a;
	shortrequest.dstNwkAddr = 0xfffd;
	shortrequest.reqType = ZB_ZDO_ADDR_REQ_TYPE_SINGLE;
	shortrequest.startIndex = 0;
	uint64_t extaddrTherm = 0x003c84fffe02386a;
	UNUSED(extaddrTherm);
	UNUSED(ShortrequestStatus);

	//for EP
    enum ZclStatusCodeT ActiveEPReqStatus;
    struct ZbZdoActiveEpReqT ActiveEPReq;
	memset(&ActiveEPReq, 0, sizeof(ActiveEPReq));

	  if(zigbee_app_info.zb == NULL){
		APP_DBG("getendpoints : zigbee_app_info.zb == NULL");
	    return;
	  }
	  /* Check if the router joined the network */
	  if (ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS) {
		APP_DBG("getendpoints : ZbNwkGet != ZB_STATUS_SUCCESS");
	    return;
	  }
	  if (epid == 0U) {
		APP_DBG("getendpoints : epid == 0U");
	    return;
	  }

	  ShortrequestStatus = ZbZdoNwkAddrReq(zigbee_app_info.zb, &shortrequest, &Shortrequest_cb, NULL);

	  ActiveEPReq.nwkAddrOfInterest = zigbee_app_info.thermshortaddr;
	  ActiveEPReq.dstNwkAddr = zigbee_app_info.thermshortaddr;
	  ActiveEPReqStatus =  ZbZdoActiveEpReq(zigbee_app_info.zb, &ActiveEPReq, &APP_ZIGBEE_EP_cb, NULL);
	  UNUSED(ActiveEPReqStatus);
	//APP_DBG("set CFG_TASK_mytask_getendpoints prio to 0");
	//UTIL_SEQ_SetTask(1U << (uint32_t)CFG_TASK_mytask_getendpoints, CFG_SCH_PRIO_0);


}

static void APP_ZIGBEE_discoverCluster_cb(struct ZbZdoSimpleDescRspT *rsp, void *cb_arg)
{
	/*ZbZdoSimpleDescRspT rsp structure:
	enum ZbStatusCodeT status;
	uint16_t nwkAddr;
	struct ZbSimpleDescriptorT simpleDesc;


	struct ZbSimpleDescriptorT {
    uint8_t endpoint;
    uint16_t profileId; < Application profile identifier
    uint16_t deviceId; < Application device identifier
    uint8_t version; < Application device version
    uint8_t inputClusterCount; < Application input cluster count
    uint16_t inputClusterList[ZB_ZDO_CLUSTER_LIST_MAX_SZ]; < Application input cluster list
    uint8_t outputClusterCount; < Application output cluster count
    uint16_t outputClusterList[ZB_ZDO_CLUSTER_LIST_MAX_SZ]; < Application output cluster list */

	struct ZbSimpleDescriptorT simpleDescReceived;

	if (rsp->status == ZB_STATUS_SUCCESS)
	{
		APP_DBG("%s : discoverCluster Callback success = 0x%02x", __func__, rsp->status);
		APP_DBG("discoverCluster Callback success endpoint = 0x%02x", rsp->simpleDesc.endpoint);
		APP_DBG("discoverCluster Callback success profileId = 0x%02x",  rsp->simpleDesc.profileId);
		APP_DBG("discoverCluster Callback success deviceId = 0x%02x",  rsp->simpleDesc.deviceId);
		APP_DBG("discoverCluster Callback success outputClusterCount = 0x%02x",  rsp->simpleDesc.outputClusterCount);

		for (int i = 0; i< rsp->simpleDesc.outputClusterCount ; i++ )
		{
			APP_DBG("discoverCluster Callback success outputCluster [i] = 0x%04x",  rsp->simpleDesc.outputClusterList[i]);
		}
		APP_DBG("discoverCluster Callback success inputClusterCount = 0x%02x",  rsp->simpleDesc.inputClusterCount);
		for (int i = 0; i< rsp->simpleDesc.inputClusterCount ; i++ )
				{
					APP_DBG("discoverCluster Callback success inputCluster [i] = 0x%04x",  rsp->simpleDesc.inputClusterList[i]);
				}
	}
	else
	{
		/* Unknown endpoint */
		//return ZCL_STATUS_FAILURE;
		APP_DBG("discoverCluster Callback fail");
	}

}

static void APP_ZIGBEE_discoverCluster(void)
{
	uint64_t epid = 0U;
	enum ZclDataTypeT type = ZCL_DATATYPE_SIGNED_16BIT;
	enum ZclStatusCodeT discoverClusterStatus;
    struct ZbZdoSimpleDescReqT discoverClusterReq;

    APP_DBG("start discoverCluster");
	memset(&discoverClusterReq, 0, sizeof(discoverClusterReq));
	discoverClusterReq.nwkAddrOfInterest = zigbee_app_info.thermshortaddr;
	discoverClusterReq.dstNwkAddr = zigbee_app_info.thermshortaddr;
	discoverClusterReq.endpt = zigbee_app_info.ep[0];

	  if(zigbee_app_info.epCount == 0)
	  {
		APP_DBG("getendpoints : zigbee_app_info.epCount == 0");
	    return;
	  }
	  APP_DBG("For EP [0] dicoverCluster:");
	  discoverClusterStatus =  ZbZdoSimpleDescReq(zigbee_app_info.zb, &discoverClusterReq, &APP_ZIGBEE_discoverCluster_cb, NULL);
	  UNUSED(discoverClusterStatus);
}


/*find the short address of the thermostat*/
static void asgn_dst_addr_cb(struct ZbZdoNwkAddrRspT  *rsp, void *cb_arg)
	{
		if (rsp->status == ZB_STATUS_SUCCESS)
		{
			APP_DBG("%s : Shortrequest_cb Callback success status = 0x%02x", __func__, rsp->status);
			zigbee_app_info.thermshortaddr = rsp->nwkAddr;
			zigbee_app_info.thermep = ICON_ENDPOINT;
		    APP_DBG("zigbee_app_info.thermextaddr extAddr = 0x%16x",  zigbee_app_info.thermextaddr);
			APP_DBG("zigbee_app_info.thermshortaddr nwkAddr = 0x%02x",  zigbee_app_info.thermshortaddr);
			APP_DBG("zigbee_app_info.thermep EP = 0x%02x",  zigbee_app_info.thermep);
		}
		else
		{
			APP_DBG("Short request  Callback fail");
		}

	}

static void APP_ZIGBEE_asgn_dst_addr(void)
{
	uint64_t epid = 0U;

	  if(zigbee_app_info.zb == NULL){
		APP_DBG("getendpoints : zigbee_app_info.zb == NULL");
		return;
	  }
	  /* Check if the router joined the network */
	  if (ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS) {
		APP_DBG("getendpoints : ZbNwkGet != ZB_STATUS_SUCCESS");
		return;
	  }
	  if (epid == 0U) {
		APP_DBG("getendpoints : epid == 0U");
		return;
	  }

	struct ZbZdoNwkAddrReqT shortrequest;
	shortrequest.extAddr = zigbee_app_info.thermextaddr;//0x003c84fffe02386a;
	shortrequest.dstNwkAddr = 0xfffd;
	shortrequest.reqType = ZB_ZDO_ADDR_REQ_TYPE_SINGLE;
	shortrequest.startIndex = 0;
	if (ZbZdoNwkAddrReq(zigbee_app_info.zb, &shortrequest, &asgn_dst_addr_cb, NULL)== ZB_STATUS_SUCCESS)
	{
		APP_DBG("Thermostat address assigned");
	}
}
/*END find short address of thermostat*/

/*BEGIN Readvalue*/

static void device_temp_read_cb(const ZbZclReadRspT *readRsp, void *arg)
{
  int i;
  int16_t current_temp;
  int16_t min_temp;
  int16_t max_temp;
  //int16_t info1;
  //int16_t info2;

  for(i=0;i<readRsp->count;i++)
  {
    if(readRsp->attr[i].status == ZCL_STATUS_SUCCESS)
    {
      switch(readRsp->attr[i].attrId)
      {
      case ZCL_THERM_SVR_ATTR_LOCAL_TEMP:
        current_temp = pletoh16(readRsp->attr[i].value);
        APP_DBG("[DEV TEMP]Current Temperature= %d,%dC",current_temp/100, current_temp%100);
        break;
      case ZCL_THERM_SVR_ATTR_OCCUP_COOL_SETPOINT:
        min_temp = pletoh16(readRsp->attr[i].value);
        APP_DBG("[DEV TEMP] Min set Temperature= %d,%dC",min_temp/100,min_temp%100);
        break;
      case ZCL_THERM_SVR_ATTR_OCCUP_HEAT_SETPOINT:
        max_temp = pletoh16(readRsp->attr[i].value);
        APP_DBG("[DEV TEMP] Max set Temperature= %d,%dC",max_temp/100,max_temp%100);
        break;
      /*case ExternalMeasuredRoomSensor:
    	info1 = pletoh16(readRsp->attr[i].value);
		APP_DBG("[DEV TEMP] External Measured Room Sensor= %dC",min_temp);
		break;
      case SetpointChangeSource:
    	info2 = pletoh16(readRsp->attr[i].value);
		APP_DBG("[DEV TEMP] Setpoint Change Source (0x00 - Manual, 0x02 - External attribute )= 0x%02x",max_temp);
		break;*/
      default:
        break;
      }
    }
    else
    {
      APP_DBG("[DEV TEMP] Failed to read attribute %d",readRsp->attr[i].attrId);
    }
  }
}


static void APP_ZIGBEE_ReadValue(void)
{
	ZbZclReadReqT readReq;
	uint64_t epid = 0U;
	if(zigbee_app_info.zb == NULL)
	{
		return;
	}
	  /* Check if the router joined the network */
	if (ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS) {
		return;
	}
	if (epid == 0U)
	{
		return;
	}
	memset(&readReq, 0, sizeof(readReq));
	readReq.dst.mode = ZB_APSDE_ADDRMODE_SHORT;
	readReq.dst.endpoint = ICON_ENDPOINT;
	readReq.dst.nwkAddr = zigbee_app_info.thermshortaddr;//zigbee_app_info.nwkAddr;//0x0000
	readReq.count = 3U;
	readReq.attr[0]=ZCL_THERM_SVR_ATTR_LOCAL_TEMP ;
	readReq.attr[1]=ZCL_THERM_SVR_ATTR_OCCUP_COOL_SETPOINT;
	readReq.attr[2]=ZCL_THERM_SVR_ATTR_OCCUP_HEAT_SETPOINT ;
	//readReq.attr[3]=ExternalMeasuredRoomSensor ;
	//readReq.attr[4]=SetpointChangeSource ;

	APP_DBG("[DEV TEMP] Read Coordinator Temperature Attributes");
	if(ZbZclReadReq(zigbee_app_info.device_therm_client, &readReq, device_temp_read_cb, zigbee_app_info.device_therm_client) != ZCL_STATUS_SUCCESS)
	{
	    APP_DBG("Error, ZbZclReadReq failed (ICON_ENDPOINT)");
	}
}
/*END Readvalue*/

/*BEGIN Bindrequest*/
static void Bindrequest_cb(struct ZbZdoBindRspT *rsp, void *cb_arg)
{
	APP_DBG("Start bindreq callback");
	if (rsp->status == ZB_STATUS_SUCCESS)
	{
		APP_DBG("%s : Bindrequest_cb success = 0x%02x", __func__, rsp->status);
		zigbee_app_info.bindingfinished = 1;
		APP_DBG("zigbee_app_info.bindingfinished  = %d", zigbee_app_info.bindingfinished);
	}
	else
	{
		APP_DBG("Bindrequest_cb Callback fail");
	}
}

static void APP_ZIGBEE_Bindrequest(void)
{
	APP_DBG("Start bindreq");
	enum ZbStatusCodeT bindStatus;

	struct ZbZdoBindReqT mybindreq;
	//struct ZbApsAddrT mybindaddr;

	mybindreq.target = (uint16_t)zigbee_app_info.thermshortaddr;
	mybindreq.srcExtAddr = (uint64_t)zigbee_app_info.thermextaddr;//0x003c84fffe02386a;
	mybindreq.srcEndpt = ICON_ENDPOINT; //0x00
	mybindreq.clusterId = ZCL_CLUSTER_HVAC_THERMOSTAT ;//0x0201
	mybindreq.dst.mode = ZB_APSDE_ADDRMODE_EXT;//zigbee.aps.h
	mybindreq.dst.endpoint = (uint16_t)ICON_ENDPOINT;
	mybindreq.dst.extAddr = (uint64_t)zigbee_app_info.extaddr;//ZbExtendedAddress(zigbee_app_info.zb);

	if (zigbee_app_info.bindingfinished == 0)
	{
		APP_DBG("Start binding");
		bindStatus = ZbZdoBindReq(zigbee_app_info.zb, &mybindreq, &Bindrequest_cb, NULL);
	}
	if (zigbee_app_info.bindingfinished == 1)
	{
		APP_DBG("Binding already done");
		return;
	}
	UNUSED(bindStatus);
	//UNUSED(mybindaddr);
}
/*END Bindrequest*/



/*BEGIN ReportConfig*/
static void ReportConfig_cb(struct ZbZclCommandRspT  *cmd_rsp, void *arg)
{
	APP_DBG("Start report configure callback");
	if (cmd_rsp->status == ZCL_STATUS_SUCCESS)
	{
		APP_DBG("%s : ReportConfig_cb success = 0x%02x", __func__, cmd_rsp->status);
		zigbee_app_info.reportConfigFinished = 1;
		APP_DBG("zigbee_app_info.bindingfinished  = %d", zigbee_app_info.reportConfigFinished);
	}
	else
	{
		APP_DBG("ReportConfig_cb Callback fail");
	}
}

static void APP_ZIGBEE_ReportConfig(void)
{
	if (zigbee_app_info.reportConfigFinished == 0 && zigbee_app_info.bindingfinished == 1)
	{
		APP_DBG("Start APP_ZIGBEE_ReportConfig");
		enum ZbStatusCodeT reportConfigStatus;
		struct ZbZclAttrReportConfigT reportConfig;
		reportConfig.dst.mode = ZB_APSDE_ADDRMODE_EXT;//zigbee.aps.h
		reportConfig.dst.endpoint = ICON_ENDPOINT; //0X01;
		reportConfig.dst.extAddr = (uint64_t)zigbee_app_info.thermextaddr;//0x003c84fffe02386a;
		reportConfig.num_records = (uint8_t)3;
		reportConfig.record_list[0].min = 5;
		reportConfig.record_list[0].max = 60;
		reportConfig.record_list[0].attr_id = ZCL_THERM_SVR_ATTR_LOCAL_TEMP;
		reportConfig.record_list[0].attr_type = ZCL_DATATYPE_SIGNED_16BIT;
		reportConfig.record_list[0].direction = ZCL_REPORT_DIRECTION_NORMAL;

		reportConfigStatus = ZbZclAttrReportConfigReq(zigbee_app_info.device_therm_client, &reportConfig, &ReportConfig_cb, NULL);
		// ZbZclAttrReportConfigReq(struct ZbZclClusterT *cluster, struct ZbZclAttrReportConfigT *config,cb,NULL)
		UNUSED(reportConfigStatus);
	}
	if (zigbee_app_info.reportConfigFinished == 1 && zigbee_app_info.bindingfinished == 1)
	{
		APP_DBG("APP_ZIGBEE_ReportConfig already done");
		return;
	}

}
/*END ReportConfig*/


/*BEGIN ReadRequest*/
static void ReportRead_cb(struct ZbZclCommandRspT  *cmd_rsp, void *arg)
{
	APP_DBG("Start ReportRead_cb callback");
	if (cmd_rsp->status == ZCL_STATUS_SUCCESS)
	{
		/*enum ZbZclClusterIdT clusterId;
		  uint8_t linkQuality;
		  struct ZbZclHeaderT hdr;
		  const uint8_t *payload;*/

		APP_DBG("%s : ReportRead_cb success = 0x%02x", __func__, cmd_rsp->status);
		APP_DBG("ReportRead_cb manufacturerCode = 0x%02x",  cmd_rsp->hdr.manufacturerCode);
		APP_DBG("ReportRead_cb payload = 0x%02x",  cmd_rsp->payload);

		enum ZbStatusCodeT AttrReadStatus;
		uint64_t epid = 0U;
		enum ZclDataTypeT type = ZCL_DATATYPE_SIGNED_16BIT;
		AttrReadStatus = ZbZclAttrRead(zigbee_app_info.device_therm_client, zigbee_app_info.thermshortaddr, &type, &epid, 40, 0);
		APP_DBG("Attribute value: %d", epid);
		UNUSED(AttrReadStatus);
	}
	else
	{
		APP_DBG("ReportRead_cb Callback fail");
	}
}

static void APP_ZIGBEE_ReportRead(void)
{
	if (zigbee_app_info.reportConfigFinished == 1)
	{
		enum ZbStatusCodeT reportReadStatus;
		struct ZbZclAttrReportReadT reportRead;

		reportRead.dst.mode = ZB_APSDE_ADDRMODE_EXT;//zigbee.aps.h
		reportRead.dst.endpoint = 0X01;
		reportRead.dst.extAddr = (uint64_t)zigbee_app_info.thermextaddr;//0x003c84fffe02386a;
		reportRead.num_records = (uint8_t)3;
		reportRead.record_list[0].min = 5;
		reportRead.record_list[0].max = 60;
		reportRead.record_list[0].attr_id = ZCL_THERM_SVR_ATTR_LOCAL_TEMP;
		reportRead.record_list[0].attr_type = ZCL_DATATYPE_SIGNED_16BIT;
		reportRead.record_list[0].direction = ZCL_REPORT_DIRECTION_NORMAL;

		reportReadStatus = ZbZclAttrReportReadReq(zigbee_app_info.device_therm_client, &reportRead, &ReportRead_cb,NULL);
		UNUSED(reportReadStatus);
	}
}
/*END ReportRead*/

/*BEGIN Push Button 1*/
static void APP_ZIGBEE_Button1(void)
{
	APP_ZIGBEE_asgn_dst_addr();
	APP_ZIGBEE_discoverCluster();


}
/*END Push Button 1*/

/*BEGIN Push Button 3*/
static void APP_ZIGBEE_Button3(void)
{
	APP_ZIGBEE_Bindrequest();
	APP_ZIGBEE_ReportConfig();
	APP_ZIGBEE_ReportRead();

}
/*END Push Button 3*/

uint16_t
ZbShortAddressPan(struct ZigBeeT *zb)
{
    uint16_t panId = 0U;

    (void)ZbNwkGet(zb, ZB_NWK_NIB_ID_PanId, &panId, sizeof(panId));
    return panId;
}

/* USER CODE END FD_LOCAL_FUNCTIONS */
