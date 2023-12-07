/*
 * app_freertos.h
 *
 *  Created on: Dec 1, 2023
 *      Author: ivand
 */

#ifndef INC_APP_FREERTOS_H_
#define INC_APP_FREERTOS_H_



#endif /* INC_APP_FREERTOS_H_ */

#define bitRead(value, bit_pos) (((value) >> (bit_pos)) & 0x01)


extern ADC_HandleTypeDef hadc1;

//Thread Handles and attributes
extern osThreadId_t TempCalcHandle;
	extern const osThreadAttr_t TempCalc_attributes;
extern osThreadId_t ControlHandle;
	extern const osThreadAttr_t Control_attributes;
extern osThreadId_t ScreenHandle;
	extern const osThreadAttr_t Screen_attributes;


//Thread prototypes
extern void CalculateTemp_Thread(void *argument);
extern void ControlTask(void *argument);
extern void Screen_Thread(void *argument);


//Event handles and attributes
extern osEventFlagsId_t tempFlagsHandle;
	extern const osEventFlagsAttr_t tempFlags_attributes;


//Function prototypes
void ADC_Temp_Thread_Start(void);
void Control_Thread_Init(void);


//Coil map, number indicates the coil number
typedef enum COIL_MAP
{
	TWA1_STATUS 		= 0, //TWA connection, 0 = not connected, 1 = connected
	TWA1_EN 			= 1, //0 = TWA off, 1 = TWA on
	TWA2_STATUS 		= 2,
	TWA2_EN 			= 3,
	TWA3_STATUS 		= 4,
	TWA3_EN 			= 5,
	TWA4_STATUS 		= 6,
	TWA4_EN 			= 7,

	TEMP1_EN 			= 16, //Physical temperature sensor
	TEMP2_EN			= 17,
	TEMP3_EN			= 18,
	TEMP4_EN			= 19,
	TEMP1_W_EN			= 20, //Wireless temperature thermostat
	TEMP2_W_EN			= 21,
	TEMP3_W_EN			= 22,
	TEMP4_W_EN			= 23

}coil_t;

//Holding registers - can be written and read from
static uint16_t Holding_Registers_Database[10]={
		0000,  1111,  2222,  3333,  4444,  5555,  6666,  7777,  8888,  9999
};

//Input Registers - can only be read
static uint16_t Input_Register_Database[20] = {
		0000,  1111,  2222,  3333,  4444,  5555,  6666,  7777,  8888,  9999
};

//Coil database - 1bit registers, can be written and read from
static uint16_t Holding_Coils_Database[5]={

};

//Input coil database - 1bit registers, can only be read
static  uint16_t Input_Coils_Database[5]={
		0b0000000000000000, 0b0000000000000000
};

