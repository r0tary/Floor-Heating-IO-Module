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

extern void CalculateTemp_Thread(void *argument);
void ADC_Temp_Thread_Start(void);
