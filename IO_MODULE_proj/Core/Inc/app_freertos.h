/*
 * app_freertos.h
 *
 *  Created on: Dec 1, 2023
 *      Author: ivand
 */

#ifndef INC_APP_FREERTOS_H_
#define INC_APP_FREERTOS_H_



#endif /* INC_APP_FREERTOS_H_ */

extern ADC_HandleTypeDef hadc1;
extern osThreadId_t TempCalcHandle;
extern const osThreadAttr_t TempCalc_attributes;
extern osEventFlagsId_t tempFlagsHandle;
extern const osEventFlagsAttr_t tempFlags_attributes;
extern osThreadId_t ControlHandle;
extern const osThreadAttr_t Control_attributes;
extern void CalculateTemp_Thread(void *argument);
extern void ControlTask(void *argument);
void ADC_Temp_Thread_Start(void);
void Control_Thread_Init(void);

//Holding registers - can be written and read from
static uint16_t Holding_Registers_Database[10]={
		0000,  1111,  2222,  3333,  4444,  5555,  6666,  7777,  8888,  9999
};

//Input Registers - can only be read
static uint16_t Input_Register_Database[20] = {
		0000,  1111,  2222,  3333,  4444,  5555,  6666,  7777,  8888,  9999
};

//Coil database - 1bit registers, can be written and read from
static uint16_t Holding_Coils_Database[10]={
		0b1010101001010101, 0b0100100110011100,
		0b1100110010100011, 0b0110011010101111,
		0b0110000010111100
};

//Input coil database - 1bit registers, can only be read
static  uint16_t Input_Coils_Database[10]={
		0b0100100110011100, 0b1010101001010101,
		0b1100110010100011, 0b0110011010101111,
		0b0110000010111100
};

