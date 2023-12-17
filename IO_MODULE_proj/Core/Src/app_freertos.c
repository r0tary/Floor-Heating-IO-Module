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
#include "IO_Config.h"
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

/*------------Timer attributes/handles------------*/
// Timer handle for control thread
osTimerId_t controlTimerHandle;
const osTimerAttr_t controlTimer_attributes = {
  .name = "controlTimer"
};

// Timer handle for TWA control
osTimerId_t TwaTimerHandle[N_ROOMS];
const osTimerAttr_t TwaTimer_attributes = {
  .name = "TwaTimer"
};

/*------------Event Flags/attributes------------*/

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
volatile uint16_t ADCrawReading[N_ROOMS];
volatile double ADCvoltage[N_ROOMS];

io_module_t rooms[N_ROOMS];

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
	/*
	bitWrite(IO,TWA1_STATUS,TWA_1);
	bitWrite(IO,TWA2_STATUS,TWA_2);
	bitWrite(IO,TWA3_STATUS,TWA_3);
	bitWrite(IO,TWA4_STATUS,TWA_4);

	bitWrite(IO,TEMP1_STATUS,PT1k_1);
	bitWrite(IO,TEMP2_STATUS,PT1k_2);
	bitWrite(IO,TEMP3_STATUS,PT1k_3);
	bitWrite(IO,TEMP4_STATUS,PT1k_4);
	*/

	// Initialize rooms
	for(int i = 0; i < N_ROOMS; i++)
	{
		rooms[i].u16regsCoils = Holding_Coils_Database;
		rooms[i].u16regsCoilsRO = Input_Coils_Database;
		rooms[i].u16regsHR = Holding_Registers_Database;
		rooms[i].u16regsRO = Input_Registers_Database;
		rooms[i].Pt = i;
		rooms[i].PID_Param.Error = 0;

		switch (rooms[i].Pt) {
			case 0:
				rooms[i].Twa = TWA2_Pin;
				rooms[i].CoilNR = TWA1_EN;
				break;
			case 1:
				rooms[i].Twa = TWA1_Pin;
				rooms[i].CoilNR = TWA2_EN;
				break;
			case 2:
				rooms[i].Twa = TWA3_Pin;
				rooms[i].CoilNR = TWA3_EN;
				break;
			case 3:
				rooms[i].Twa = TWA4_Pin;
				rooms[i].CoilNR = TWA4_EN;
				break;
		}

	}
}


void ADC_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Common config
   * Default ADC if only 1 channel configured
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;

  if(N_ROOMS > 1){
	  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
	  hadc1.Init.NbrOfConversion = N_ROOMS;
  }

  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure initial ADC channel
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure depending on N_ROOMS the rest of the ADC Channels
  */
  if(N_ROOMS == 2)
  {
	  sConfig.Channel = ADC_CHANNEL_9;
	  sConfig.Rank = ADC_REGULAR_RANK_2;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
		Error_Handler();
	  }
  }

  if(N_ROOMS == 3)
  {
	  sConfig.Channel = ADC_CHANNEL_11;
	  sConfig.Rank = ADC_REGULAR_RANK_3;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
		Error_Handler();
	  }
  }

  if(N_ROOMS == 4)
  {
	  sConfig.Channel = ADC_CHANNEL_12;
	  sConfig.Rank = ADC_REGULAR_RANK_4;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
		Error_Handler();
	  }
  }
}

// Initializes the thread and event flags in charge of calculating the temperature values form PT1000
void ADC_Temp_Thread_Start(io_module_t *IO)
{
	TempCalcHandle = osThreadNew(CalculateTemp_Thread, NULL, &TempCalc_attributes);
}


// Initializes required components for Control algorithm thread
void Control_Thread_Init(void)
{
	ControlHandle = osThreadNew(ControlTask, &rooms, &Control_attributes);
	controlTimerHandle = osTimerNew(ControlExecTim, osTimerPeriodic, NULL, &controlTimer_attributes);
	for(int i = 0; i < N_ROOMS; i++)
	{
		TwaTimerHandle[i] = osTimerNew(TwaControlTim, osTimerOnce,&rooms[i], &TwaTimer_attributes);
	}
}


#if MODE == 0

real_T Output;
void ControlTask(void *argument){
	osTimerStart(controlTimerHandle, CONTROLFREQ);
	io_module_t *IO = (io_module_t *)argument;
	// Add the control algorithm and schedule the task properly to execute every period of time
	// TODO

	for(;;)
	{

		// Request room temperature ¿Function?, run control algorithm and get an output
		// Check output and change state of the TWA based on it.
		// Run this loop CONTROLFREQ

		osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);

		for(int i = 0; i < N_ROOMS; i++)
		{
			Output = PID0_step(&rooms[i]);
			// Timer based on PID output
			if(Output!= 0)
			{
				HAL_GPIO_WritePin(TWA2_GPIO_Port, IO->Twa, 1);
				HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
				bitWrite(IO, 1);
				osTimerStart(TwaTimerHandle[i], Output*CONTROLFREQ);
			}

		}
		/*
		if (PID0_Y.y > 0){
			HAL_GPIO_WritePin(TWA1_GPIO_Port, TWA1_Pin, 1);

		}
		else if (PID0_Y.y == 0){
			HAL_GPIO_WritePin(TWA1_GPIO_Port, TWA1_Pin, 0);
			bitWrite(IO, TWA1_EN, 0);
		}
		*/
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
	uint16_t *temp;
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

	for(;;)
	{
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADCrawReading,N_ROOMS);
		osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);

		for(int i = 0; i < N_ROOMS; i++)
		{
			ADCvoltage[i] = ADCrawReading[i] * 0.00080566;
			rooms[i].TempRoom = (ADCvoltage[i]*49.02) - 24.20;

			temp = &rooms->u16regsRO[i];
			*temp = (uint16_t)rooms[i].TempRoom;
		}

		HAL_ADC_Stop_DMA(&hadc1);
		osDelay(1);
	}

}


void bitWrite(io_module_t * IO, uint8_t val)
{
	uint16_t *temp;
	uint8_t pos = IO->CoilNR;
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
	osThreadFlagsSet(ControlHandle, 0x01);
  /* USER CODE END ControlExecTim */
}


// TWA control callback
void TwaControlTim(void *argument)
{
	io_module_t *IO = (io_module_t *)argument;
	HAL_GPIO_WritePin(TWA2_GPIO_Port, IO->Twa, 0);
	bitWrite(IO, 0);
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
}


// ADC complete conversion callback
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	osThreadFlagsSet(TempCalcHandle, 0x01);
}


void Screen_Thread(void *argument){

}


/* USER CODE END Application */

