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

// Temperature thread handle
osThreadId_t TempCalcHandle;
const osThreadAttr_t TempCalc_attributes = {
  .name = "TempCalc",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

// Thread flags handle
osEventFlagsId_t tempFlagsHandle;
const osEventFlagsAttr_t tempFlags_attributes = {
  .name = "tempFlags"
};

// Control thread handle
osThreadId_t ControlHandle;
const osThreadAttr_t Control_attributes = {
	  .name = "Control",
	  .priority = (osPriority_t) osPriorityLow,
	  .stack_size = 128 * 4
};
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
volatile uint16_t ADCrawReading;
volatile double ADCvoltage;
volatile double Temperature;
/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/* USER CODE END FunctionPrototypes */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

// ADC complete conversion callback
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

	//osEventFlagsSet(tempFlagsHandle,0x01);
	osThreadFlagsSet(TempCalcHandle, 0x01);

}

// Initializes the thread and event flags in charge of calculating the temperature values form PT1000
void ADC_Temp_Thread_Start(void)
{
	TempCalcHandle = osThreadNew(CalculateTemp_Thread, NULL, &TempCalc_attributes);
	tempFlagsHandle = osEventFlagsNew(&tempFlags_attributes);
}

// Initializes required components for Control algorithm thread
void Control_Thread_Init(void)
{
	ControlHandle = osThreadNew(ControlTask, NULL, &Control_attributes);
}



// System Threads

void CalculateTemp_Thread(void *argument){

	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

	for(;;)
	{
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADCrawReading,1);
		//osEventFlagsWait(tempFlagsHandle, 0x01, osFlagsWaitAll, osWaitForever);
		osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);
		ADCvoltage = ADCrawReading * 0.00073242;
		Temperature = ((ADCvoltage - 0.408)*100) / 2.04;
		HAL_ADC_Stop_DMA(&hadc1);
		osDelay(2);
	}

}

void ControlTask(void *argument){
	// Add the control algorithm and schedule the task properly to execute every period of time
	// TODO

	for(;;)
	{
		osDelay(1);
	}

}
/* USER CODE END Application */

