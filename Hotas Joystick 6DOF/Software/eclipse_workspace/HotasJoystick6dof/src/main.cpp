 /*******************************************************************************
 main.cpp
 V 0.01

 Description:

 ********************************************************************************
 ********************************************************************************
 V0.01 --> 13/04/15	Edition originale by BRB
 *******************************************************************************/
#include <stdio.h>
#include <math.h>
#include "diag/Trace.h"

#include <map>

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

#include "Timer.h"
#include "BlinkLed.h"

//Common files
#include "Config_Joystick_6dof.h"
#include "Typedef_Joystick_6dof.h"

//Hardware library
#include "Config_Stm32F4.h"
#include "hardware/Hardware_Init.h"

#include "coreUser/Core.h"

//Functional library
#include "libUser/Button_Matrix/Button_Matrix.h"
#include "libUser/Analog_Measure/Analog_Measure.h"
#include "libUser/Dpad/Analog_Dpad.h"
//#include "Tracking_6DOF.h"

#include "libUser/HID_Report/HID_Report.h"

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

// Initialisation du singleton CCore à NULL
CCore *CCore::_singleton = NULL;

GPIO_PinState buttonStatus[BUTTON_MATRIX_NB];
int16_t padLeftX, padLeftY;
int16_t padRightX, padRightY;
int16_t dPadLeftStatus, dPadRightStatus;
int16_t throttleLeftPosition, throttleRightPosition;
int16_t yaw, pitch, roll, heaving, swaying, surging;
uint16_t i;

_gpioxConfig colMap[] = {
	{GPIOE, {GPIO_PIN_8, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}},
	{GPIOE, {GPIO_PIN_9, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}},
	{GPIOE, {GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}},
	{GPIOE, {GPIO_PIN_11, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}},
	{GPIOE, {GPIO_PIN_12, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}},
	{GPIOE, {GPIO_PIN_13, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}},
	{GPIOE, {GPIO_PIN_14, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}},
	{GPIOE, {GPIO_PIN_15, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}},
};

_gpioxConfig rowMap[] = {
	{GPIOD, {GPIO_PIN_8, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}},
	{GPIOD, {GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}},
	{GPIOD, {GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}},
	{GPIOD, {GPIO_PIN_11, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}},
	{GPIOD, {GPIO_PIN_12, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}},
	{GPIOD, {GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}},
};

_gpioxConfig analogMap[] = {
	{GPIOA, {GPIO_PIN_1, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF9_CAN1}},
	{GPIOA, {GPIO_PIN_2, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF9_CAN1}},
	{GPIOA, {GPIO_PIN_3, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF9_CAN1}},
	{GPIOB, {GPIO_PIN_4, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF9_CAN1}},
	{GPIOB, {GPIO_PIN_5, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF9_CAN1}},
};
ADC_HandleTypeDef ADC_HandleDef[] = {
//	ADC1, {ADC_CLOCKPRESCALER_PCLK_DIV8, ADC_RESOLUTION12b, ADC_DATAALIGN_RIGHT, ENABLE, EOC_SEQ_CONV, ENABLE, ENABLE, 1, DISABLE, 8, ADC_EXTERNALTRIGCONVEDGE_NONE, ADC_EXTERNALTRIGCONV_T1_CC1}
};
ADC_ChannelConfTypeDef ADC_ChannelConfDef[] = {
	ADC_CHANNEL_0, 1, ADC_SAMPLETIME_3CYCLES, 0
};

_gpioxConfig ov7670IO[] = {
	//{??, ??, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, ??}, //Pclk
	{GPIOA, GPIO_PIN_4, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF13_DCMI}, //hsync ??
	{GPIOA, GPIO_PIN_6, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF13_DCMI}, //clk ??
	{GPIOB, GPIO_PIN_7, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF13_DCMI}, //vsync ??
	{GPIOC, GPIO_PIN_6, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF13_DCMI}, //d0
	//{GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF13_DCMI}, //d1
	{GPIOC, GPIO_PIN_7, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF13_DCMI}, //d1 ??
	{GPIOC, GPIO_PIN_8, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF13_DCMI}, //d2
	{GPIOC, GPIO_PIN_9, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF13_DCMI}, //d3
	{GPIOC, GPIO_PIN_11, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF13_DCMI}, //d4
	{GPIOB, GPIO_PIN_6, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF13_DCMI}, //d5 ??
	{GPIOB, GPIO_PIN_8, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF13_DCMI}, //d6
	{GPIOE, GPIO_PIN_6, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF13_DCMI}, //d7
};

//Create core manager
CCore* core = CCore::GetInstance();

//Create common object
CButtonMatrix* buttonMatrix = new CButtonMatrix(rowMap, colMap, buttonStatus);
CAnalogMeasure* throttleLeft = new CAnalogMeasure(analogMap[0], ADC_HandleDef[0], ADC_ChannelConfDef[0], POSITION_ANALOG_THROTTLE_OUTPUT_MIN, POSITION_ANALOG_THROTTLE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_THROTTLE_LEFT_ADDR_OFFSET, "Throttle Left");
CAnalogMeasure* throttleRight = new CAnalogMeasure(analogMap[0], ADC_HandleDef[0], ADC_ChannelConfDef[0], POSITION_ANALOG_THROTTLE_OUTPUT_MIN, POSITION_ANALOG_THROTTLE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_THROTTLE_RIGHT_ADDR_OFFSET, "Throttle Right");
CAnalogMeasure* dPadLeftX = new CAnalogMeasure(analogMap[1], ADC_HandleDef[0], ADC_ChannelConfDef[0], POSITION_ANALOG_DPAD_OUTPUT_MIN, POSITION_ANALOG_DPAD_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_PDAD_LEFT_X_ADDR_OFFSET, "Dpad left X");
CAnalogMeasure* dPadLeftY = new CAnalogMeasure(analogMap[2], ADC_HandleDef[0], ADC_ChannelConfDef[0], POSITION_ANALOG_DPAD_OUTPUT_MIN, POSITION_ANALOG_DPAD_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_PDAD_LEFT_Y_ADDR_OFFSET, "Dpad left Y");
CAnalogMeasure* dPadRightX = new CAnalogMeasure(analogMap[3], ADC_HandleDef[0], ADC_ChannelConfDef[0], POSITION_ANALOG_DPAD_OUTPUT_MIN, POSITION_ANALOG_DPAD_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_PDAD_RIGHT_X_ADDR_OFFSET, "Dpad right X");
CAnalogMeasure* dPadRightY = new CAnalogMeasure(analogMap[4], ADC_HandleDef[0], ADC_ChannelConfDef[0], POSITION_ANALOG_DPAD_OUTPUT_MIN, POSITION_ANALOG_DPAD_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_PDAD_RIGHT_Y_ADDR_OFFSET, "Dpad right Y");
CAnalogDpad* dPadLeft = new CAnalogDpad(DPAD_NEUTRAL_ZONE);
CAnalogDpad* dPadRight = new CAnalogDpad(DPAD_NEUTRAL_ZONE);
//CTracking6Dof* tracking6Dof = new CTracking6Dof(ov7670Pin);
CHidReport* hidReport = new CHidReport();

int
main(int argc, char* argv[]) {
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	SystemInit();
	SystemClock_Config();
	SystemCoreClockUpdate();

	/* Configure the system clock */

	/* System interrupt init*/
	/* Sets the priority grouping field */
	/*HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
	HAL_NVIC_SetPriority((IRQn_Type)SysTick_IRQn, (unsigned long)0, (unsigned long)0);*/

	/* Initialize all configured peripherals */
	GPIO_Init();

	// Send a greeting to the trace device (skipped on Release).
	trace_puts("Hello ARM World!");
	// At this stage the system clock should have already been configured
	// at high speed.
	trace_printf("System clock: %uHz\n", SystemCoreClock);


	Timer timer;
	timer.start();

	//Initialize button matrix
	trace_printf("...BUTTON MATRIX...\n");
	timer.sleep(500);
	/*uint16_t index = 0;
	std::map<uint16_t, _gpioxConfig> rowMap;
	rowMap[index++] = {GPIOE, {GPIO_PIN_8, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}};
	rowMap[index++] = {GPIOE, {GPIO_PIN_9, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}};
	rowMap[index++] = {GPIOE, {GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}};
	rowMap[index++] = {GPIOE, {GPIO_PIN_11, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}};
	rowMap[index++] = {GPIOE, {GPIO_PIN_12, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}};
	rowMap[index++] = {GPIOE, {GPIO_PIN_13, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}};
	rowMap[index++] = {GPIOE, {GPIO_PIN_14, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}};
	rowMap[index++] = {GPIOE, {GPIO_PIN_15, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}};

	index = 0;
	std::map<std::uint16_t, _gpioxConfig> colMap;
	colMap[index++] = {GPIOD, {GPIO_PIN_8, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}};
	colMap[index++] = {GPIOD, {GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}};
	colMap[index++] = {GPIOD, {GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}};
	colMap[index++] = {GPIOD, {GPIO_PIN_11, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}};
	colMap[index++] = {GPIOD, {GPIO_PIN_12, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}};
	colMap[index++] = {GPIOD, {GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}};
	std::map<std::string, std::map<uint16_t, _gpioxConfig>> buttonMatrixConfig;
	buttonMatrixConfig["row"] = rowMap;
	buttonMatrixConfig["col"] = colMap;*/

	buttonMatrix->initialize();

	//Initialize analog input
	trace_printf("...ANALOG INPUT...\n");
	timer.sleep(500);
	throttleLeft->initialize();
	throttleRight->initialize();
	dPadLeftX->initialize();
	dPadLeftY->initialize();
	dPadRightX->initialize();
	dPadRightY->initialize();

	//Initialize analog Dpad
	trace_printf("...ANALOG DPAD...\n");
	timer.sleep(500);
	dPadLeft->initialize();
	dPadRight->initialize();

	//Initialize Tracking 6Dof
	trace_printf("...TRACKING 6DOF...\n");
	timer.sleep(500);
	//tracking6Dof->initialize();

	//Initialize joystick report
	trace_printf("...REPORT...\n");
	timer.sleep(500);
	hidReport->initialize();

	//Initialize variables
	trace_printf("...VARIABLES...\n");
	timer.sleep(500);
	for (i = 0; i < BUTTON_MATRIX_NB; i++)
		buttonStatus[i] = GPIO_PIN_RESET;
	padLeftX = 0;
	padLeftY = 0;
	padRightX = 0;
	padRightY = 0;
	dPadLeftStatus = 0;
	dPadRightStatus = 0;
	throttleLeftPosition = 0;
	throttleRightPosition = 0;
	yaw = 0;
	pitch = 0;
	roll = 0;
	heaving = 0;
	swaying = 0;
	surging = 0;

	//Enable inter
	trace_printf("...INTER...\n");
	//MsTimer2::set(5, InterruptTimer2); // pÃ©riode 1000ms
	//MsTimer2::start(); // active Timer 2
	timer.sleep(500);

	// Infinite loop
	while (1) {
		//Get 6DOF
		//tracking6Dof->getPosition(&yaw, &pitch, &roll, &heaving, &swaying, &surging);

		//Set HID report
		hidReport->sendReport(buttonStatus, yaw, pitch, roll, swaying, surging, heaving, throttleLeftPosition, throttleRightPosition, dPadLeftStatus, dPadRightStatus);
		trace_printf("%d %d %d\t%d %d %d\t%d %d\t%d %d\n", yaw, pitch, roll, swaying, surging, heaving, throttleLeftPosition, throttleRightPosition, dPadLeftStatus, dPadRightStatus);

		timer.sleep(50);
	}
}


void InterruptTimer2() { // debut de la fonction d'interruption Timer2
	//Get button
	buttonMatrix->getButtonStatus();

	//get analog
	static uint16_t interState = 0;
	switch(interState) {
		case 0:  throttleLeftPosition = throttleLeft->getMeasure(); interState++; break;
		case 1:  throttleRightPosition = throttleRight->getMeasure(); interState++; break;
		case 2:  padLeftX = dPadLeftX->getMeasure(); interState++; break;
		case 3:  padLeftY = dPadLeftY->getMeasure(); interState++; dPadLeftStatus = dPadLeft->getDPadStatus(padLeftX, padLeftY); break;
		case 4:  padRightX = dPadRightX->getMeasure(); interState++; break;
		case 5:  padRightY = dPadRightY->getMeasure(); interState = 0; dPadRightStatus = dPadRight->getDPadStatus(padRightX, padRightY); break;
		default: interState = 0; break;
	}
}

#pragma GCC diagnostic pop
