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
#include "PID0.h"
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

// Screen thread handle
osThreadId_t ScreenHandle;
const osThreadAttr_t Screen_attributes = {
	  .name = "Screen",
	  .priority = (osPriority_t) osPriorityLow,
	  .stack_size = 128 * 4
};

/*------------Thread attributes/handles------------*/
// Timer handle
osTimerId_t controlTimerHandle;
const osTimerAttr_t controlTimer_attributes = {
  .name = "controlTimer"
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
volatile uint16_t ADCrawReading[2];
volatile double ADCvoltage[2];
volatile double Temperature[2];

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

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */



//IO Module Configuration function
void IO_Module_Init(io_module_t * IO)
{
	bitWrite(IO,TWA1_STATUS,TWA_1);
	bitWrite(IO,TWA2_STATUS,TWA_2);
	bitWrite(IO,TWA3_STATUS,TWA_3);
	bitWrite(IO,TWA4_STATUS,TWA_4);

	bitWrite(IO,TEMP1_STATUS,PT1k_1);
	bitWrite(IO,TEMP2_STATUS,PT1k_2);
	bitWrite(IO,TEMP3_STATUS,PT1k_3);
	bitWrite(IO,TEMP4_STATUS,PT1k_4);
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

void Control_Thread_Init(io_module_t *IO)
{
	ControlHandle = osThreadNew(ControlTask, IO, &Control_attributes);
	controlTimerHandle = osTimerNew(ControlExecTim, osTimerPeriodic, NULL, &controlTimer_attributes);
}

#if MODE == 0
void ControlTask(void *argument){
	osTimerStart(controlTimerHandle, CONTROLFREQ);
	io_module_t *IO = (io_module_t *)argument;
	// Add the control algorithm and schedule the task properly to execute every period of time
	// TODO

	for(;;)
	{
		/*pid_error = //feedback(room T) - x_setpoint;
		pid_Px = KPx * pid_error;
		pid_Ix = pid_Ix + (KIx * pid_error);
		pid_Dx = KDx * ((pid_error - last_x_error) / dt);
		pid_X = pid_Px + pid_Ix + pid_Dx;
		last_x_error = pid_error;*/

		// Request room temperature ¿Function?, run control algorithm and get an output
		// Check output and change state of the TWA based on it.
		// Run this loop every 1 second

		/*TWA_Status = bitRead(modH,1);
		HAL_GPIO_WritePin(TWA2_GPIO_Port, TWA2_Pin,TWA_Status);
		osDelay(5000);*/

		osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		PID0_step(Temperature[0]);

		if (PID0_Y.y > 0){
			HAL_GPIO_WritePin(TWA1_GPIO_Port, TWA1_Pin, 1);
			bitWrite(IO, TWA1_EN, 1);
		}
		else if (PID0_Y.y == 0){
			HAL_GPIO_WritePin(TWA1_GPIO_Port, TWA1_Pin, 0);
			bitWrite(IO, TWA1_EN, 0);
		}
	}
}
#endif
#if MODE == 1

void ControlTask(void *argument){
	io_module_t *IO = (io_module_t *)argument;
	uint8_t TWA_Status = 0;

	for(;;)
	{

		if (bitRead(IO->u16regsCoilsRO,TWA1_STATUS) == true){
			TWA_Status = bitRead(IO->u16regsCoils,TWA1_EN);
			HAL_GPIO_WritePin(TWA1_GPIO_Port, TWA1_Pin,TWA_Status);
			bitWrite(IO, TWA1_EN, TWA_Status);
		}

		if (bitRead(IO->u16regsCoilsRO,TWA2_STATUS) == true){
			TWA_Status = bitRead(IO->u16regsCoils,TWA2_EN);
			HAL_GPIO_WritePin(TWA2_GPIO_Port, TWA2_Pin,TWA_Status);
			bitWrite(IO, TWA2_EN, TWA_Status);
		}

		if (bitRead(IO->u16regsCoilsRO,TWA3_STATUS) == true){
			TWA_Status = bitRead(IO->u16regsCoils,TWA3_EN);
			HAL_GPIO_WritePin(TWA3_GPIO_Port, TWA3_Pin,TWA_Status);
			bitWrite(IO, TWA3_EN, TWA_Status);
		}

		if (bitRead(IO->u16regsCoilsRO,TWA4_STATUS) == true){
			TWA_Status = bitRead(IO->u16regsCoils,TWA4_EN);
			HAL_GPIO_WritePin(TWA4_GPIO_Port, TWA4_Pin,TWA_Status);
			bitWrite(IO, TWA4_EN, TWA_Status);
		}

		osDelay(5000);
	}
}
#endif
#if MODE == 2


#endif

void CalculateTemp_Thread(void *argument){

	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

	for(;;)
	{
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADCrawReading,2);
		//osEventFlagsWait(tempFlagsHandle, 0x01, osFlagsWaitAll, osWaitForever);
		osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);
		ADCvoltage[0] = ADCrawReading[0] * 0.00073242;
		Temperature[0] = ((ADCvoltage[0] - 0.408)*100) / 2.04;
		ADCvoltage[1] = ADCrawReading[1] * 0.00073242;
		Temperature[1] = ((ADCvoltage[1] - 0.408)*100) / 2.04;
		HAL_ADC_Stop_DMA(&hadc1);
		osDelay(1);
	}

}

void bitWrite(io_module_t * IO, uint8_t pos, uint8_t val)
// Temperature = (((ADCrawReading * 0.00073242) - 0.408)*100) / 2.04;
{
	uint16_t *temp;
	temp = &IO->u16regsCoilsRO[pos/16];

	if (val == 1) {
		*temp |= (1UL << (pos%16));
	}
	else {
		*temp &= ~(1UL << (pos%16));
	}
}

uint8_t bitRead(uint16_t *Coils, uint8_t coilNr)
{
	uint16_t temp;
	uint8_t res, pos;

	pos = coilNr - ((coilNr/16)*16);
	temp = Coils[coilNr/16];
	res = (temp >> pos ) & 0x01;

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

