
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/app_zigbee.c
  * @author  MCD Application Team
  * @brief   Zigbee Application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes -----------------------------------------------------------*/
#include <assert.h>
#include "zcl/zcl.h"
#include "zcl/general/zcl.onoff.h"

/* USER CODE BEGIN Includes */
#include "zcl/general/zcl.therm.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define APP_ZIGBEE_STARTUP_FAIL_DELAY               500U
#define CHANNEL                                     13

#define SW1_ENDPOINT                                17

/* USER CODE BEGIN PD */
#define SW1_GROUP_ADDR                              0x0001
#define ICON_ENDPOINT                               0x01
#define ExternalMeasuredRoomSensor 					0x4015
#define SetpointChangeSource 						0x0030
/* USER CODE END PD */

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

static void APP_ZIGBEE_ProcessNotifyM0ToM4(void * argument);
static void APP_ZIGBEE_ProcessRequestM0ToM4(void * argument);
static void APP_ZIGBEE_ProcessNwkForm(void * argument);

/* USER CODE BEGIN PFP */
static void APP_ZIGBEE_ConfigGroupAddr(void);

/*ADD ON*/
static void APP_ZIGBEE_ProcessPushButton(void *argument);
static void APP_ZIGBEE_ProcessGetThermAddr(void * argument);
static void APP_ZIGBEE_ProcessGetThermTemp(void * argument);

static void APP_ZIGBEE_GetThermAddr(void);
static void APP_ZIGBEE_GetThermTemp(void);

static void GetThermAddr_cb(struct ZbZdoNwkAddrRspT  *rsp, void *cb_arg);
static void GetThermTemp_cb(const ZbZclReadRspT *readRsp, void *arg);
/*ADD ON*/

static void APP_ZIGBEE_SW1_Process(void);
/* USER CODE END PFP */

/* Private variables ---------------------------------------------------------*/
static TL_CmdPacket_t   *p_ZIGBEE_otcmdbuffer;
static TL_EvtPacket_t   *p_ZIGBEE_notif_M0_to_M4;
static TL_EvtPacket_t   *p_ZIGBEE_request_M0_to_M4;
static __IO uint32_t    CptReceiveNotifyFromM0 = 0;
static __IO uint32_t    CptReceiveRequestFromM0 = 0;

static osThreadId_t   OsTaskNotifyM0ToM4Id;
static osThreadId_t   OsTaskRequestM0ToM4Id;
static osThreadId_t   OsTaskNwkFormId;
static osMutexId_t    MtxZigbeeId;
static osThreadId_t   OsTaskGetThermAddr;
static osThreadId_t   OsTaskGetThermTemp;

osSemaphoreId_t       TransferToM0Semaphore;
osSemaphoreId_t       StartupEndSemaphore;

PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_ZIGBEE_Config_t ZigbeeConfigBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t ZigbeeOtCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ZigbeeNotifRspEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ZigbeeNotifRequestBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];

struct zigbee_app_info
{
  bool has_init;
  struct ZigBeeT *zb;
  enum ZbStartType startupControl;
  enum ZbStatusCodeT join_status;
  uint32_t join_delay;
  bool init_after_join;
  /*ADDON*/
  struct ZbStartupT config_info;
  uint16_t nwkAddr;
  uint64_t extaddr;
  uint16_t epCount;
  uint16_t ep[10];
  uint64_t thermExtAddr;
  uint16_t thermShortAddr;
  uint16_t thermEP;
  bool discoverClusterStatus;
  bool bindingFinished;
  bool reportConfigFinished;
  bool reportReadFinished;
  bool ThermAddrFound;
  enum ZclStatusCodeT statusgetendpoint;
  struct ZbZclClusterT *device_therm_server;
  struct ZbZclClusterT *device_therm_client;

  struct ZbZclClusterT *onOff_server_1;
};
static struct zigbee_app_info zigbee_app_info;

/* FreeRtos stacks attributes */
const osThreadAttr_t TaskNotifyM0ToM4_attr =
{
    .name = CFG_TASK_NOTIFY_M0_TO_M4_PROCESS_NAME,
    .attr_bits = CFG_TASK_PROCESS_ATTR_BITS,
    .cb_mem = CFG_TASK_PROCESS_CB_MEM,
    .cb_size = CFG_TASK_PROCESS_CB_SIZE,
    .stack_mem = CFG_TASK_PROCESS_STACK_MEM,
    .priority = osPriorityBelowNormal,
    .stack_size = CFG_TASK_PROCESS_STACK_SIZE
};

const osThreadAttr_t TaskRequestM0ToM4_attr =
{
    .name = CFG_TASK_REQUEST_M0_TO_M4_PROCESS_NAME,
    .attr_bits = CFG_TASK_PROCESS_ATTR_BITS,
    .cb_mem = CFG_TASK_PROCESS_CB_MEM,
    .cb_size = CFG_TASK_PROCESS_CB_SIZE,
    .stack_mem = CFG_TASK_PROCESS_STACK_MEM,
    .priority = osPriorityBelowNormal,
    .stack_size = CFG_TASK_PROCESS_STACK_SIZE
};

const osThreadAttr_t TaskNwkForm_attr =
{
    .name = CFG_TASK_NWK_FORM_PROCESS_NAME,
    .attr_bits = CFG_TASK_PROCESS_ATTR_BITS,
    .cb_mem = CFG_TASK_PROCESS_CB_MEM,
    .cb_size = CFG_TASK_PROCESS_CB_SIZE,
    .stack_mem = CFG_TASK_PROCESS_STACK_MEM,
    .priority = osPriorityAboveNormal,
    .stack_size = CFG_TASK_PROCESS_STACK_SIZE
};

/* USER CODE BEGIN PV */

const osThreadAttr_t TaskGetThermAddr_attr =
{
    .name = CFG_TASK_GET_THERM_ADDR_PROCESS_NAME,
    .attr_bits = CFG_TASK_PROCESS_ATTR_BITS,
    .cb_mem = CFG_TASK_PROCESS_CB_MEM,
    .cb_size = CFG_TASK_PROCESS_CB_SIZE,
    .stack_mem = CFG_TASK_PROCESS_STACK_MEM,
    .priority = 18,
    .stack_size = CFG_TASK_PROCESS_STACK_SIZE
};

const osThreadAttr_t TaskGetThermTemp_attr =
{
    .name = CFG_TASK_GET_THERM_TEMP_PROCESS_NAME,
    .attr_bits = CFG_TASK_PROCESS_ATTR_BITS,
    .cb_mem = CFG_TASK_PROCESS_CB_MEM,
    .cb_size = CFG_TASK_PROCESS_CB_SIZE,
    .stack_mem = CFG_TASK_PROCESS_STACK_MEM,
    .priority = 17,
    .stack_size = CFG_TASK_PROCESS_STACK_SIZE
};
static osThreadId_t     OsTaskPushButtonId;
const osThreadAttr_t TaskPushButton_attributes =

{
    .name = CFG_TASK_PUSH_BUTTON_PROCESS_NAME,
    .attr_bits = CFG_TASK_PROCESS_ATTR_BITS,
    .cb_mem = CFG_TASK_PROCESS_CB_MEM,
    .cb_size = CFG_TASK_PROCESS_CB_SIZE,
    .stack_mem = CFG_TASK_PROCESS_STACK_MEM,
    //.priority = osPriorityNormal,
    .priority = osPriorityAboveNormal,
    .stack_size = CFG_TASK_PROCESS_STACK_SIZE
};

/* USER CODE END PV */
/* Functions Definition ------------------------------------------------------*/


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

  /* Register cmdbuffer */
  APP_ZIGBEE_RegisterCmdBuffer(&ZigbeeOtCmdBuffer);

  /* Init config buffer and call TL_ZIGBEE_Init */
  APP_ZIGBEE_TL_INIT();

  /* Initialize the mutex */
  MtxZigbeeId = osMutexNew( NULL );

  /* Initialize the semaphores */
  StartupEndSemaphore = osSemaphoreNew( 1, 0, NULL ); /*< Create the semaphore and make it busy at initialization */
  TransferToM0Semaphore = osSemaphoreNew( 1, 0, NULL );

  /* Register task */
  /* Create the different tasks */
  OsTaskNotifyM0ToM4Id = osThreadNew(APP_ZIGBEE_ProcessNotifyM0ToM4, NULL,&TaskNotifyM0ToM4_attr);
  OsTaskRequestM0ToM4Id = osThreadNew(APP_ZIGBEE_ProcessRequestM0ToM4, NULL,&TaskRequestM0ToM4_attr);

  /* Task associated with network creation process */
  OsTaskNwkFormId = osThreadNew(APP_ZIGBEE_ProcessNwkForm, NULL,&TaskNwkForm_attr);

  /* USER CODE BEGIN APP_ZIGBEE_INIT */



  /*ADD ON*/
   /* Task associated with getting zigbee network address */
 // OsTaskGetThermAddr = osThreadNew(APP_ZIGBEE_ProcessGetThermAddr, NULL,&TaskGetThermAddr_attr);

   /* Task associated with getting zigbee temperature data */
 // OsTaskGetThermTemp = osThreadNew(APP_ZIGBEE_ProcessGetThermTemp, NULL,&TaskGetThermTemp_attr);
   /*ADD ON*/


   /* Task associated with push button SW1 */
   OsTaskPushButtonId = osThreadNew(APP_ZIGBEE_ProcessPushButton, NULL, &TaskPushButton_attributes);

  /* USER CODE END APP_ZIGBEE_INIT */

  /* Start the Zigbee on the CPU2 side */
  ZigbeeInitStatus = SHCI_C2_ZIGBEE_Init();
  /* Prevent unused argument(s) compilation warning */
  UNUSED(ZigbeeInitStatus);

  /* Initialize Zigbee stack layers */
  APP_ZIGBEE_StackLayersInit();

}

/**
 * @brief  Initialize Zigbee stack layers
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_StackLayersInit(void)
{
  APP_DBG("APP_ZIGBEE_StackLayersInit");

  zigbee_app_info.zb = ZbInit(0U, NULL, NULL);
  assert(zigbee_app_info.zb != NULL);

  /* Create the endpoint and cluster(s) */
  APP_ZIGBEE_ConfigEndpoints();

  /* USER CODE BEGIN APP_ZIGBEE_StackLayersInit */
  BSP_LED_Off(LED_RED);
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_BLUE);
  /* USER CODE END APP_ZIGBEE_StackLayersInit */

  /* Configure the joining parameters */
  zigbee_app_info.join_status = (enum ZbStatusCodeT) 0x01; /* init to error status */
  zigbee_app_info.join_delay = HAL_GetTick(); /* now */
  zigbee_app_info.startupControl = ZbStartTypeForm;

  /* Initialization Complete */
  zigbee_app_info.has_init = true;
  zigbee_app_info.thermExtAddr  = 0x003c84fffe02386a;
  zigbee_app_info.thermShortAddr = 0x0000U;
  zigbee_app_info.ThermAddrFound = 0;
  zigbee_app_info.epCount = 0;
  zigbee_app_info.discoverClusterStatus=0;
  zigbee_app_info.bindingFinished =0;
  zigbee_app_info.reportConfigFinished = 0;
  zigbee_app_info.reportReadFinished = 0;

  /* run the task */
  osThreadFlagsSet(OsTaskNwkFormId,1);
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

  /* Endpoint: SW1_ENDPOINT */
  req.profileId = ZCL_PROFILE_HOME_AUTOMATION;
  req.deviceId = ZCL_DEVICE_ONOFF_SWITCH;
  req.endpoint = SW1_ENDPOINT;
  ZbZclAddEndpoint(zigbee_app_info.zb, &req, &conf);
  assert(conf.status == ZB_STATUS_SUCCESS);


  /* USER CODE BEGIN CONFIG_ENDPOINT */

  /* Endpoint: ICON_ENDPOINT */
	req.profileId = ZCL_PROFILE_HOME_AUTOMATION;
	req.deviceId = ZCL_DEVICE_THERMOSTAT;
	req.endpoint = ICON_ENDPOINT;
	ZbZclAddEndpoint(zigbee_app_info.zb, &req, &conf);
	assert(conf.status == ZB_STATUS_SUCCESS);

  /* Thermostat client */
	zigbee_app_info.device_therm_client = ZbZclThermClientAlloc (zigbee_app_info.zb, ICON_ENDPOINT);
	assert(zigbee_app_info.device_therm_client != NULL);
	ZbZclClusterEndpointRegister(zigbee_app_info.device_therm_client);
  /* USER CODE END CONFIG_ENDPOINT */
}

/**
 * @brief  Handle Zigbee network forming and joining task for FreeRTOS
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ProcessNwkForm(void *argument)
{
  UNUSED(argument);

  for(;;)
  {
    osThreadFlagsWait(1,osFlagsWaitAll,osWaitForever);
    APP_ZIGBEE_NwkForm();
  }
}

/**
 * @brief  Handle Zigbee network forming and joining
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_NwkForm(void)
{
  if ((zigbee_app_info.join_status != ZB_STATUS_SUCCESS) && (HAL_GetTick() >= zigbee_app_info.join_delay))
  {
    struct ZbStartupT config;
    enum ZbStatusCodeT status;

    /* Configure Zigbee Logging */
    ZbSetLogging(zigbee_app_info.zb, ZB_LOG_MASK_LEVEL_ALL, NULL);

    /* Attempt to join a zigbee network */
    ZbStartupConfigGetProDefaults(&config);

    /* Set the centralized network */
    APP_DBG("Network config : APP_STARTUP_CENTRALIZED_COORDINATOR");
    config.startupControl = zigbee_app_info.startupControl;

    /* Using the default HA preconfigured Link Key */
    memcpy(config.security.preconfiguredLinkKey, sec_key_ha, ZB_SEC_KEYSIZE);

    config.channelList.count = 1;
    config.channelList.list[0].page = 0;
    config.channelList.list[0].channelMask = 1 << CHANNEL; /*Channel in use */

    /* Using ZbStartupWait (blocking) */
    status = ZbStartupWait(zigbee_app_info.zb, &config);

    APP_DBG("ZbStartup Callback (status = 0x%02x)", status);
    zigbee_app_info.join_status = status;

    if (status == ZB_STATUS_SUCCESS)
    {
      zigbee_app_info.join_delay = 0U;
      zigbee_app_info.init_after_join = true;
      APP_DBG("Startup done !\n");
      /* USER CODE BEGIN 3 */
       BSP_LED_On(LED_BLUE);
       zigbee_app_info.extaddr = ZbExtendedAddress(zigbee_app_info.zb);
       zigbee_app_info.nwkAddr = ZbShortAddress(zigbee_app_info.zb);
       zigbee_app_info.statusgetendpoint = ZB_STATUS_ALLOC_FAIL;

 		//zigbee_app_info.addr = ZbShortAddressPan(zigbee_app_info.zb);
 		APP_DBG("Network created ");
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
    osThreadFlagsSet(OsTaskNwkFormId,1);
  }
  /* USER CODE BEGIN NW_FORM */
  else
  {
    zigbee_app_info.init_after_join = false;

    /* Assign ourselves to the group addresses */
    APP_ZIGBEE_ConfigGroupAddr();

    /* Since we're using group addressing (broadcast), shorten the broadcast timeout */
    uint32_t bcast_timeout = 3;
    ZbNwkSet(zigbee_app_info.zb, ZB_NWK_NIB_ID_NetworkBroadcastDeliveryTime, &bcast_timeout, sizeof(bcast_timeout));
  }
  /* USER CODE END NW_FORM */
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
  osSemaphoreRelease(StartupEndSemaphore);
}

enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config)
{
  struct ZbStartupWaitInfo *info;
  enum ZbStatusCodeT status;

  info = malloc(sizeof(struct ZbStartupWaitInfo));
  if (info == NULL)
  {
    return ZB_STATUS_ALLOC_FAIL;
  }
  memset(info, 0, sizeof(struct ZbStartupWaitInfo));

  info->active = true;
  status = ZbStartup(zb, config, ZbStartupWaitCb, info);
  if (status != ZB_STATUS_SUCCESS)
  {
    info->active = false;
    return status;
  }

  osSemaphoreAcquire(StartupEndSemaphore, osWaitForever);
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
  osMutexAcquire(MtxZigbeeId, osWaitForever);
}

/**
 * @brief  This function waits for getting an acknowledgment from the M0.
 *
 * @param  None
 * @retval None
 */
static void Wait_Getting_Ack_From_M0(void)
{
  osSemaphoreAcquire(TransferToM0Semaphore, osWaitForever);
  osMutexRelease(MtxZigbeeId);
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
  osSemaphoreRelease(TransferToM0Semaphore);
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
  osThreadFlagsSet(OsTaskNotifyM0ToM4Id,1);
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
  osThreadFlagsSet(OsTaskRequestM0ToM4Id,1);
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
}

/**
 * @brief Process the messages coming from the M0.
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ProcessNotifyM0ToM4(void * argument)
{
  UNUSED(argument);

  for(;;)
  {
    osThreadFlagsWait(1,osFlagsWaitAll,osWaitForever);
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
}

/**
 * @brief Process the requests coming from the M0.
 * @param
 * @return
 */
static void APP_ZIGBEE_ProcessRequestM0ToM4(void * argument)
{
  UNUSED(argument);

  for(;;)
  {
    osThreadFlagsWait(1,osFlagsWaitAll,osWaitForever);
    if (CptReceiveRequestFromM0 != 0)
    {
      Zigbee_M0RequestProcessing();
      CptReceiveRequestFromM0 = 0;
    }
  }
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */


void APP_ZIGBEE_LaunchPushButtonTask(void)
{
  osThreadFlagsSet(OsTaskPushButtonId,1);
}

static void APP_ZIGBEE_ProcessPushButton(void *argument)
{
  for(;;)
  {
	APP_ZIGBEE_GetThermAddr();
	APP_ZIGBEE_GetThermTemp();
    osThreadFlagsWait(1,osFlagsWaitAll,osWaitForever);
  }
}

/**
 * @brief  Set group addressing mode
 * @param  None
 * @retval None
 */

static void APP_ZIGBEE_ConfigGroupAddr(void)
{
  struct ZbApsmeAddGroupReqT req;
  struct ZbApsmeAddGroupConfT conf;

  memset(&req, 0, sizeof(req));
  req.endpt = SW1_ENDPOINT;
  req.groupAddr = SW1_GROUP_ADDR;
  ZbApsmeAddGroupReq(zigbee_app_info.zb, &req, &conf);

}


/**
 * @brief  Handle Zigbee get thermostat network short address for FreeRTOS
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ProcessGetThermAddr(void *argument)
{
  UNUSED(argument);

  for(;;)
  {
    osThreadFlagsWait(1,osFlagsWaitAll,osWaitForever);
    APP_ZIGBEE_GetThermAddr();
  }
}


/*find the short address of the thermostat */
static void APP_ZIGBEE_GetThermAddr(void)
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

	  if (zigbee_app_info.ThermAddrFound == 1) {
	  		APP_DBG("ThermAddress already assigned");
	  	return;
	  }

	struct ZbZdoNwkAddrReqT shortrequest;
	shortrequest.extAddr = zigbee_app_info.thermExtAddr;//0x003c84fffe02386a;
	shortrequest.dstNwkAddr = 0xfffd;
	shortrequest.reqType = ZB_ZDO_ADDR_REQ_TYPE_SINGLE;
	shortrequest.startIndex = 0;
	if (ZbZdoNwkAddrReq(zigbee_app_info.zb, &shortrequest, &GetThermAddr_cb, NULL)== ZB_STATUS_SUCCESS)
	{
		APP_DBG("Thermostat address assigned");
	}
}

/*find the short address of the thermostat CALLBACK*/
static void GetThermAddr_cb(struct ZbZdoNwkAddrRspT  *rsp, void *cb_arg)
	{
		if (rsp->status == ZB_STATUS_SUCCESS)
		{
			APP_DBG("%s : Shortrequest_cb Callback success status = 0x%02x", __func__, rsp->status);
			zigbee_app_info.thermShortAddr = rsp->nwkAddr;
			zigbee_app_info.thermEP = ICON_ENDPOINT;
		    APP_DBG("zigbee_app_info.thermextaddr extAddr = 0x%16x",  zigbee_app_info.thermExtAddr);
			APP_DBG("zigbee_app_info.thermshortaddr nwkAddr = 0x%02x",  zigbee_app_info.thermShortAddr);
			APP_DBG("zigbee_app_info.thermep EP = 0x%02x",  zigbee_app_info.thermEP);
			zigbee_app_info.ThermAddrFound = 1;
		}
		else
		{
			APP_DBG("Short request  Callback fail");
		}

	}

/**
 * @brief  Handle Zigbee get thermostat temp readings for FreeRTOS
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ProcessGetThermTemp(void *argument)
{
  UNUSED(argument);

  for(;;)
  {
    osThreadFlagsWait(1,osFlagsWaitAll,osWaitForever);
    APP_ZIGBEE_GetThermTemp();
  }
}

static void APP_ZIGBEE_GetThermTemp(void)
{
	ZbZclReadReqT readReq;
	uint64_t epid = 0U;
	if(zigbee_app_info.zb == NULL)
	{
		return;
	}
	  /* Check if the router joined the network */
	if (ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS)
	{
		return;
	}
	if (epid == 0U)
	{
		return;
	}
	if (zigbee_app_info.ThermAddrFound == 0) {
		APP_DBG("ThermAddress not assigned yet");
		return;
	}

	memset(&readReq, 0, sizeof(readReq));
	readReq.dst.mode = ZB_APSDE_ADDRMODE_SHORT;
	readReq.dst.endpoint = ICON_ENDPOINT;
	readReq.dst.nwkAddr = zigbee_app_info.thermShortAddr;//zigbee_app_info.nwkAddr;//0x0000
	readReq.count = 3U;
	readReq.attr[0]=ZCL_THERM_SVR_ATTR_LOCAL_TEMP ;
	readReq.attr[1]=ZCL_THERM_SVR_ATTR_OCCUP_COOL_SETPOINT;
	readReq.attr[2]=ZCL_THERM_SVR_ATTR_OCCUP_HEAT_SETPOINT ;
	//readReq.attr[3]=ExternalMeasuredRoomSensor ;
	//readReq.attr[4]=SetpointChangeSource ;

	APP_DBG("[DEV TEMP] Read Coordinator Temperature Attributes");
	if(ZbZclReadReq(zigbee_app_info.device_therm_client, &readReq, &GetThermTemp_cb, zigbee_app_info.device_therm_client) != ZCL_STATUS_SUCCESS)
	{
	    APP_DBG("Error, ZbZclReadReq failed (ICON_ENDPOINT)");
	}
}



static void GetThermTemp_cb(const ZbZclReadRspT *readRsp, void *arg)
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

/* USER CODE END FD_LOCAL_FUNCTIONS */
