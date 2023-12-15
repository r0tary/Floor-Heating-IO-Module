/*
 * app_freertos.h
 *
 *  Created on: Dec 1, 2023
 *      Author: ivand
 */

#ifndef INC_APP_FREERTOS_H_
#define INC_APP_FREERTOS_H_
#endif /* INC_APP_FREERTOS_H_ */


#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))


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
void ControlExecTim(void *argument);


typedef struct
	{
		uint16_t *u16regsHR;
		uint16_t *u16regsRO;
		uint16_t *u16regsCoils;
		uint16_t *u16regsCoilsRO;
		const uint8_t Pt;
		const uint16_t Twa;
	}io_module_t;


//Function prototypes
void ADC_Temp_Thread_Start(void);
void Control_Thread_Init(io_module_t *IO);
void IO_Module_Init(io_module_t *IO);
void bitWrite(io_module_t *IO, uint8_t pos, uint8_t val);
uint8_t bitRead(uint16_t *IO, uint8_t coilNR);
void TwaControlTim(void *argument);



/*------------IO Module Configuration------------*/

//TWA configuration, 0 = NOT connected, 1 = connected
#define TWA_1			0
#define TWA_2			1
#define TWA_3			0
#define TWA_4			0

//PT1000 temp sensor configuration, 0 = NOT connected, 1 = connected
#define PT1k_1		0
#define PT1k_2		0
#define PT1k_3		0
#define PT1k_4		0



typedef enum MODES
{
	STANDART			= 0,
	TEST_TWA			= 1,
	TEST_PID			= 2
}mode_t;

#define MODE 0





//Coil map, number indicates the coil number
typedef enum COIL_MAP
{
	TWA1_STATUS 		= 0, //TWA connection
	TWA1_EN 			= 1,
	TWA2_STATUS 		= 2,
	TWA2_EN 			= 3,
	TWA3_STATUS 		= 4,
	TWA3_EN 			= 5,
	TWA4_STATUS 		= 6,
	TWA4_EN 			= 7,

	TEMP1_STATUS 			= 16, //Physical temperature sensor
	TEMP2_STATUS			= 17,
	TEMP3_STATUS			= 18,
	TEMP4_STATUS			= 19,
	TEMP1_W_STATUS			= 20, //Wireless temperature thermostat
	TEMP2_W_STATUS			= 21,
	TEMP3_W_STATUS			= 22,
	TEMP4_W_STATUS			= 23
}coil_t;

/*typedef enum INPUT_REG_MAP
{

}input_reg_t;
*/





