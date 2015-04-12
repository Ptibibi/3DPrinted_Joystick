//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>
#include "diag/Trace.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_rcc_ex.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_cortex.h"

#include "Timer.h"
#include "BlinkLed.h"

//Common files
#include "Config_Joystick_6dof.h"
#include "Typedef_Joystick_6dof.h"

//Hardware library
#include "Config_Stm32F4.h"

//Functional library
#include "Button_Matrix.h"
#include "Analog_Measure.h"
#include "Analog_Dpad.h"
//#include "Tracking_6DOF.h"
//#include "HID_Report.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

GPIO_PinState buttonStatus[BUTTON_MATRIX_NB];
uint16_t padLeftX, padLeftY;
uint16_t padRightX, padRightY;
uint8_t dPadLeftStatus, dPadRightStatus;
uint16_t throttleLeftPosition, throttleRightPosition;
uint16_t yaw, pitch, roll, heaving, swaying, surging;
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

//Create common object
CButtonMatrix* buttonMatrix = new CButtonMatrix(rowMap, colMap);
CAnalogMeasure* throttleLeft = new CAnalogMeasure(analogMap[0], ADC_HandleDef[0], ADC_ChannelConfDef[0], POSITION_ANALOG_THROTTLE_OUTPUT_MIN, POSITION_ANALOG_THROTTLE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_THROTTLE_LEFT_ADDR_OFFSET, "Throttle Left");
CAnalogMeasure* dPadLeftX = new CAnalogMeasure(analogMap[1], ADC_HandleDef[0], ADC_ChannelConfDef[0], POSITION_ANALOG_DPAD_OUTPUT_MIN, POSITION_ANALOG_DPAD_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_PDAD_LEFT_X_ADDR_OFFSET, "Dpad left X");
CAnalogMeasure* dPadLeftY = new CAnalogMeasure(analogMap[2], ADC_HandleDef[0], ADC_ChannelConfDef[0], POSITION_ANALOG_DPAD_OUTPUT_MIN, POSITION_ANALOG_DPAD_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_PDAD_LEFT_Y_ADDR_OFFSET, "Dpad left Y");
CAnalogMeasure* dPadRightX = new CAnalogMeasure(analogMap[3], ADC_HandleDef[0], ADC_ChannelConfDef[0], POSITION_ANALOG_DPAD_OUTPUT_MIN, POSITION_ANALOG_DPAD_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_PDAD_RIGHT_X_ADDR_OFFSET, "Dpad right X");
CAnalogMeasure* dPadRightY = new CAnalogMeasure(analogMap[4], ADC_HandleDef[0], ADC_ChannelConfDef[0], POSITION_ANALOG_DPAD_OUTPUT_MIN, POSITION_ANALOG_DPAD_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_PDAD_RIGHT_Y_ADDR_OFFSET, "Dpad right Y");
CAnalogDpad* dPadLeft = new CAnalogDpad(DPAD_NEUTRAL_ZONE);
CAnalogDpad* dPadRight = new CAnalogDpad(DPAD_NEUTRAL_ZONE);
//CTracking6Dof* tracking6Dof = new CTracking6Dof(ov7670Pin);
//CHidReport* hidReport = new CHidReport();

int
main(int argc, char* argv[]) {
	Date test;
	test.day = 10;
	// Send a greeting to the trace device (skipped on Release).
	trace_puts("Hello ARM World!");
	// At this stage the system clock should have already been configured
	// at high speed.
	trace_printf("System clock: %uHz\n", SystemCoreClock);

	SystemInit();
	SystemCoreClockUpdate();
	HAL_Init();

	Timer timer;
	timer.start();

	//Initialize button matrix
	trace_printf("...BUTTON MATRIX...\n");
	timer.sleep(500);
	buttonMatrix->initialize();

	//Initialize analog input
	trace_printf("...ANALOG INPUT...\n");
	timer.sleep(500);
	throttleLeft->initialize();
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
	//hidReport->initialize();

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
	//MsTimer2::set(5, InterruptTimer2); // période 1000ms
	//MsTimer2::start(); // active Timer 2
	timer.sleep(500);

	// Infinite loop
	while (1) {
	  //Get 6DOF
	  //tracking6Dof->getPosition(&yaw, &pitch, &roll, &heaving, &swaying, &surging);

	  //Set HID report
	  //hidReport->sendReport(buttonStatus, yaw, pitch, roll, swaying, surging, heaving, throttleLeftPosition, throttleRightPosition, dPadLeftStatus, dPadRightStatus);
	  trace_printf("%d %d %d\t%d %d %d\t%d %d\t%d %d\n", yaw, pitch, roll, swaying, surging, heaving, throttleLeftPosition, throttleRightPosition, dPadLeftStatus, dPadRightStatus);

	  timer.sleep(50);
	}
}


void InterruptTimer2() { // debut de la fonction d'interruption Timer2
	//Get button
	buttonMatrix->getButtonStatus(buttonStatus);

	//get analog
	static uint16_t interState = 0;
	switch(interState) {
		case 0:  throttleLeftPosition = throttleLeft->getMeasure(); interState++; break;
		case 1:  padLeftX = dPadLeftX->getMeasure(); interState++; break;
		case 2:  padLeftY = dPadLeftY->getMeasure(); interState++; dPadLeftStatus = dPadLeft->getDPadStatus(padLeftX, padLeftY); break;
		case 3:  padRightX = dPadRightX->getMeasure(); interState++; break;
		case 4:  padRightY = dPadRightY->getMeasure(); interState = 0; dPadRightStatus = dPadRight->getDPadStatus(padRightX, padRightY); break;
		default: interState = 0; break;
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
