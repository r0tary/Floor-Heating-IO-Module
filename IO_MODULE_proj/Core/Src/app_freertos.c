/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
#include "app_freertos.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* Definitions for TempCalc */






/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
//extern ADC_HandleTypeDef hadc1;

//extern osThreadId_t TempCalcHandle;
//extern const osThreadAttr_t TempCalc_attributes;

/* Definitions for tempFlags */
//extern osEventFlagsId_t tempFlagsHandle;
//extern const osEventFlagsAttr_t tempFlags_attributes;

extern void CalculateTemp_Thread(void *argument);
/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

//void ADC_Temp_Thread_Start(void);


/* USER CODE END FunctionPrototypes */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */


void ADC_Temp_Thread_Start(void)
{
	TempCalcHandle = osThreadNew(CalculateTemp_Thread, NULL, &TempCalc_attributes);
	tempFlagsHandle = osEventFlagsNew(&tempFlags_attributes);

}




/* USER CODE END Application */

