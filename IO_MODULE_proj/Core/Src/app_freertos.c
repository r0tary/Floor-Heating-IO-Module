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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */



/*------------Thread attributes/handles------------*/

// Temperature thread handle
osThreadId_t TempCalcHandle;
const osThreadAttr_t TempCalc_attributes = {
  .name = "TempCalc",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

// Control thread handle
osThreadId_t ControlHandle;
const osThreadAttr_t Control_attributes = {
	  .name = "Control",
	  .priority = (osPriority_t) osPriorityNormal,
	  .stack_size = 128 * 4
};

// Timer handle
osTimerId_t controlTimerHandle;
const osTimerAttr_t controlTimer_attributes = {
  .name = "controlTimer"
};

// Screen thread handle
osThreadId_t ScreenHandle;
const osThreadAttr_t Screen_attributes = {
	  .name = "Screen",
	  .priority = (osPriority_t) osPriorityLow,
	  .stack_size = 128 * 4
};


/*------------Event Flags/attributes------------*/
// Thread flags handle
osEventFlagsId_t tempFlagsHandle;
const osEventFlagsAttr_t tempFlags_attributes = {
  .name = "tempFlags"
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

//Control timer frequency
#define CONTROLFREQ 1000

// PID variables
float pid_X = 0;
float pid_Px = 0;
float pid_Ix = 0;
float pid_Dx = 0;
int x_setpoint = 0;
float pid_error = 0;
float last_x_error = 0;
float dt = 0;
unsigned long currentTime = 0;
unsigned long last_pid_timer = 0;

// PID tuning parameters
static const float KPx = 2.4;
static const float KIx = 0.015;
static const float KDx = 0.28;

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */



//IO Module Configuration function
void IO_Module_Init(modbusHandler_t * modH)
{

	bitWrite(modH,TWA1_STATUS,TWA_1);
	bitWrite(modH,TWA2_STATUS,TWA_2);
	bitWrite(modH,TWA3_STATUS,TWA_3);
	bitWrite(modH,TWA4_STATUS,TWA_4);


}



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

void Control_Thread_Init(modbusHandler_t *modH)
{
	ControlHandle = osThreadNew(ControlTask, modH, &Control_attributes);
	controlTimerHandle = osTimerNew(ControlExecTim, osTimerPeriodic, NULL, &controlTimer_attributes);
}


// System Threads


void ControlTask(void *argument){

	modbusHandler_t *modH = (modbusHandler_t *)argument;
	// Add the control algorithm and schedule the task properly to execute every period of time
	// TODO

	uint8_t TWA_Status = 0;

	for(;;)
	{

		TWA_Status = bitRead(modH,1);
		HAL_GPIO_WritePin(TWA2_GPIO_Port, TWA2_Pin,TWA_Status);
		osDelay(5000);

		/*pid_error = //feedback(room T) - x_setpoint;
		pid_Px = KPx * pid_error;
		pid_Ix = pid_Ix + (KIx * pid_error);
		pid_Dx = KDx * ((pid_error - last_x_error) / dt);
		pid_X = pid_Px + pid_Ix + pid_Dx;
		last_x_error = pid_error;*/

		osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	}
}


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
		osDelay(1);
	}

}
void bitWrite(modbusHandler_t * modH, uint8_t pos, uint8_t val)
{
	uint16_t *temp;
	temp = &modH->u16regsCoilsRO[pos/16];

	if (val == 1) {
		*temp |= (1UL << (pos%16));
	}
	else {
		*temp &= ~(1UL << (pos%16));
	}
}

uint8_t bitRead(modbusHandler_t *modH, uint8_t pos)
{
	uint16_t *temp;
	uint8_t res;
	temp = &modH->u16regsCoils[0];

	res = (*temp >> pos) & 0x01;
	return res;
}

/* ControlExecTim function */
void ControlExecTim(void *argument)
{
  /* USER CODE BEGIN ControlExecTim */
	//HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	osThreadFlagsSet(ControlHandle, 0x01);
  /* USER CODE END ControlExecTim */
}

void Screen_Thread(void *argument){

}


/* USER CODE END Application */

