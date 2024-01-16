/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Modbus.h"
#include "app_freertos.h"
#include "app_zigbee.h"
#include "ssd1306.h"
#include "PID0.h"
#include "app_conf.h"
#include "app_entry.h"
#include "app_common.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern uint16_t Holding_Registers_Database[10];
extern uint16_t Input_Registers_Database[10];
extern uint16_t Holding_Coils_Database[3];
extern uint16_t Input_Coils_Database[3];

extern uint16_t ZigbeeCurrentTemp[4];
extern uint16_t ZigbeeSetpointTemp[4];

extern uint8_t RxCB_type;
extern uint8_t Zigbee_ready;
extern osThreadId_t ModbusStartHandle;
extern osMutexId_t    MtxUSART;

extern osEventFlagsId_t nwk_event_id;
extern osEventFlagsId_t nwk_ready_event_id;
extern osEventFlagsId_t asoc_event_id;
extern int Network_created;

enum
{
	FLAG_NWK = 0x01,
	FLAG_ASOC = 0x01
};

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_4
#define B1_GPIO_Port GPIOC
#define LD2_Pin GPIO_PIN_0
#define LD2_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_1
#define LD3_GPIO_Port GPIOB
#define TWA4_Pin GPIO_PIN_12
#define TWA4_GPIO_Port GPIOB
#define TWA1_Pin GPIO_PIN_13
#define TWA1_GPIO_Port GPIOB
#define TWA2_Pin GPIO_PIN_14
#define TWA2_GPIO_Port GPIOB
#define TWA3_Pin GPIO_PIN_15
#define TWA3_GPIO_Port GPIOB
#define DE_EN_Pin GPIO_PIN_6
#define DE_EN_GPIO_Port GPIOC
#define JTMS_Pin GPIO_PIN_13
#define JTMS_GPIO_Port GPIOA
#define JTCK_Pin GPIO_PIN_14
#define JTCK_GPIO_Port GPIOA
#define B2_Pin GPIO_PIN_0
#define B2_GPIO_Port GPIOD
#define B3_Pin GPIO_PIN_1
#define B3_GPIO_Port GPIOD
#define JTDO_Pin GPIO_PIN_3
#define JTDO_GPIO_Port GPIOB
#define STLINK_RX_Pin GPIO_PIN_6
#define STLINK_RX_GPIO_Port GPIOB
#define STLINK_TX_Pin GPIO_PIN_7
#define STLINK_TX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
