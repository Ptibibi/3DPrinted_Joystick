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
#include "Config_Gpio.h"

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

GPIO_PinState buttonStatus[BUTTON_GAME_NB + BUTTON_CONTROL_NB];
GPIO_PinState gameButtonStatus[BUTTON_GAME_NB];
GPIO_PinState controlButtonStatus[BUTTON_CONTROL_NB];
uint16_t padLeftX, padLeftY;
uint16_t padRightX, padRightY;
uint8_t dPadLeftStatus, dPadRightStatus;
uint16_t throttleLeftPosition, throttleRightPosition;
uint16_t yaw, pitch, roll, heaving, swaying, surging;

_gpioxConfig rowMap[] = {
	{GPIOE, GPIO_PIN_8, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO},
	{GPIOE, GPIO_PIN_9, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO},
	{GPIOE, GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO},
	{GPIOE, GPIO_PIN_11, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO},
	{GPIOE, GPIO_PIN_12, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO},
	{GPIOE, GPIO_PIN_13, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO},
	{GPIOE, GPIO_PIN_14, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO},
	{GPIOE, GPIO_PIN_15, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}
};

_gpioxConfig colMap[] = {
	{GPIOD, GPIO_PIN_8, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO},
	{GPIOD, GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO},
	{GPIOD, GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO},
	{GPIOD, GPIO_PIN_11, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO},
	{GPIOD, GPIO_PIN_12, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO},
	{GPIOD, GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO}
};

_gpioxConfig analogInput[] = {
	{GPIOA, GPIO_PIN_1, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF9_CAN1}, //dpad1x
	{GPIOA, GPIO_PIN_2, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF9_CAN1}, //dpad1y
	{GPIOA, GPIO_PIN_3, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF9_CAN1}, //dpad2x
	{GPIOB, GPIO_PIN_0, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF9_CAN1}, //dpad2y
	{GPIOB, GPIO_PIN_1, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF9_CAN1}, //throttle
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
CAnalogMeasure* throttleLeft = new CAnalogMeasure(analogInput[0], POSITION_ANALOG_THROTTLE_OUTPUT_MIN, POSITION_ANALOG_THROTTLE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_THROTTLE_LEFT_ADDR_OFFSET, "Throttle Left");
CAnalogMeasure* dPadLeftX = new CAnalogMeasure(analogInput[1], POSITION_ANALOG_DPAD_OUTPUT_MIN, POSITION_ANALOG_DPAD_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_PDAD_LEFT_X_ADDR_OFFSET, "Dpad left X");
CAnalogMeasure* dPadLeftY = new CAnalogMeasure(analogInput[2], POSITION_ANALOG_DPAD_OUTPUT_MIN, POSITION_ANALOG_DPAD_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_PDAD_LEFT_Y_ADDR_OFFSET, "Dpad left Y");
CAnalogMeasure* dPadRightX = new CAnalogMeasure(analogInput[3], POSITION_ANALOG_DPAD_OUTPUT_MIN, POSITION_ANALOG_DPAD_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_PDAD_RIGHT_X_ADDR_OFFSET, "Dpad right X");
CAnalogMeasure* dPadRightY = new CAnalogMeasure(analogInput[4], POSITION_ANALOG_DPAD_OUTPUT_MIN, POSITION_ANALOG_DPAD_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_PDAD_RIGHT_Y_ADDR_OFFSET, "Dpad right Y");
CAnalogDpad* dPadLeft = new CAnalogDpad(DPAD_NEUTRAL_ZONE);
CAnalogDpad* dPadRight = new CAnalogDpad(DPAD_NEUTRAL_ZONE);
//CTracking6Dof* tracking6Dof = new CTracking6Dof(ov7670Pin);
//CHidReport* hidReport = new CHidReport();

int
main(int argc, char* argv[]) {
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
	memset(buttonStatus, GPIO_PIN_RESET, sizeof(buttonStatus));
	memset(gameButtonStatus, GPIO_PIN_RESET, sizeof(gameButtonStatus));
	memset(controlButtonStatus, GPIO_PIN_RESET, sizeof(controlButtonStatus));
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
	  //hidReport->sendReport(gameButtonStatus, yaw, pitch, roll, swaying, surging, heaving, throttleLeftPosition, throttleRightPosition, dPadLeftStatus, dPadRightStatus);
	  trace_printf("%d %d %d\t%d %d %d\t%d %d\t%d %d\n", yaw, pitch, roll, swaying, surging, heaving, throttleLeftPosition, throttleRightPosition, dPadLeftStatus, dPadRightStatus);

	  timer.sleep(50);
	}
}


void InterruptTimer2() { // debut de la fonction d'interruption Timer2
	//Get button
	buttonMatrix->getButtonStatus(buttonStatus);
	//Extract control part
	GPIO_PinState* pButton = buttonStatus;
	memcpy(gameButtonStatus, pButton, sizeof(gameButtonStatus));
	//Extract game part
	pButton = &buttonStatus[BUTTON_GAME_NB];
	memcpy(controlButtonStatus, pButton, sizeof(controlButtonStatus));

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
