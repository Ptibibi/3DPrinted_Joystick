//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>
#include "diag/Trace.h"


#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

//Common files
#include "Config_Joystick_6dof.h"
#include "Typedef_Joystick_6dof.h"

//Personal library
#include "Display.h"
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
uint8_t dPadLeftStatus;
uint8_t dPadRightStatus;
uint16_t throttleLeftPosition;
uint16_t throttleRightPosition;
uint16_t yaw, pitch, roll, heaving, swaying, surging;

_gpioxConfig rowMap[] = {
	{GPIOA, GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF12_SDIO},
	{GPIOA, GPIO_PIN_1, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF12_SDIO},
	{GPIOA, GPIO_PIN_2, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF12_SDIO},
	{GPIOA, GPIO_PIN_3, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF12_SDIO},
	{GPIOA, GPIO_PIN_4, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF12_SDIO},
	{GPIOA, GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF12_SDIO},
	{GPIOA, GPIO_PIN_6, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF12_SDIO},
	{GPIOA, GPIO_PIN_7, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF12_SDIO}
};

_gpioxConfig colMap[] = {
	{GPIOA, GPIO_PIN_8, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF12_SDIO},
	{GPIOA, GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF12_SDIO},
	{GPIOA, GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF12_SDIO},
	{GPIOA, GPIO_PIN_11, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF12_SDIO},
	{GPIOA, GPIO_PIN_12, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF12_SDIO},
	{GPIOA, GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_MEDIUM, GPIO_AF12_SDIO}
};

//Create common object
CDisplay* display = new CDisplay();
//CButtonMatrix* buttonMatrix = new CButtonMatrix(rowMap, colMap, BUTTON_MATRIX_DELAY_UPDATE, display);
CAnalogMeasure* throttleLeft = new CAnalogMeasure(0, ANALOG_INPUT_PIN_THROTTLE_LEFT, -1799, 1800, STORAGE_ANALOG_MEASURE_THROTTLE_LEFT_ADDR_OFFSET, "Throttle left", display);
CAnalogMeasure* throttleRight = new CAnalogMeasure(1, ANALOG_INPUT_PIN_THROTTLE_RIGHT, -1799, 1800, STORAGE_ANALOG_MEASURE_THROTTLE_RIGHT_ADDR_OFFSET, "Throttle right", display);
CAnalogMeasure* dPadLeftX = new CAnalogMeasure(2, ANALOG_INPUT_PIN_DPAD_LEFT_X, -100, 100, STORAGE_ANALOG_MEASURE_PDAD_LEFT_X_ADDR_OFFSET, "Dpad left X", display);
CAnalogMeasure* dPadLeftY = new CAnalogMeasure(3, ANALOG_INPUT_PIN_DPAD_LEFT_Y, -100, 100, STORAGE_ANALOG_MEASURE_PDAD_LEFT_Y_ADDR_OFFSET, "Dpad left Y", display);
CAnalogMeasure* dPadRightX = new CAnalogMeasure(4, ANALOG_INPUT_PIN_DPAD_RIGHT_X, -100, 100, STORAGE_ANALOG_MEASURE_PDAD_RIGHT_X_ADDR_OFFSET, "Dpad right X", display);
CAnalogMeasure* dPadRightY = new CAnalogMeasure(5, ANALOG_INPUT_PIN_DPAD_RIGHT_Y, -100, 100, STORAGE_ANALOG_MEASURE_PDAD_RIGHT_Y_ADDR_OFFSET, "Dpad right Y", display);
CAnalogDpad* dPadLeft = new CAnalogDpad(DPAD_NEUTRAL_ZONE, display);
CAnalogDpad* dPadRight = new CAnalogDpad(DPAD_NEUTRAL_ZONE, display);
//CTracking6Dof* tracking6Dof = new CTracking6Dof(ov7670Pin, display);
//CHidReport* hidReport = new CHidReport(display);

int
main(int argc, char* argv[]) {
  HAL_Init();

  //Initialize screen
  display->initialize();
  display->print("...INIT...");
  HAL_Delay(500);

  //Initialize button matrix
  display->print("...BUTTON MATRIX...");
  HAL_Delay(500);
  //buttonMatrix->initialize();

  // Infinite loop
  while (1) {
    //Get 6DOF
    //tracking6Dof->getPosition(&yaw, &pitch, &roll, &heaving, &swaying, &surging);

    //Get button
    //buttonMatrix->getButtonStatus(buttonStatus);
    //Extract control part
    GPIO_PinState* pButton = buttonStatus;
    memcpy(gameButtonStatus, pButton, sizeof(gameButtonStatus));
    //Extract game part
    pButton = &buttonStatus[BUTTON_GAME_NB];
    memcpy(controlButtonStatus, pButton, sizeof(controlButtonStatus));

    //Get throttle position
    throttleLeftPosition = throttleLeft->getMeasure();
    throttleRightPosition = throttleRight->getMeasure();

    //Get DPAD status
    dPadLeftStatus = dPadLeft->getDPadStatus(dPadLeftX->getMeasure(), dPadLeftY->getMeasure());
    dPadRightStatus = dPadRight->getDPadStatus(dPadRightX->getMeasure(), dPadRightY->getMeasure());

    /*static int count = 0;
    if (gameButtonStatus[count] == GPIO_PIN_SET)
      gameButtonStatus[count] = GPIO_PIN_RESET;
    else
      gameButtonStatus[count] = GPIO_PIN_SET;
    if (count < BUTTON_GAME_NB - 1)
      count++;
    else
      count = 0;*/

    /*yaw = (int)random(-32768, 32767);
    pitch = (int)random(-32768, 32767);
    roll = (int)random(-32768, 32767);
    swaying = (int)random(-32768, 32767);
    surging = (int)random(-32768, 32767);
    heaving = (int)random(-32768, 32767);
    throttleLeftPosition = (int)random(-32768, 32767);
    throttleRightPosition = (int)random(-32768, 32767);*/

    /*if (dPadLeftStatus < 8)
      dPadLeftStatus++;
    else
      dPadLeftStatus = 0;

    if (dPadRightStatus < 8)
      dPadRightStatus++;
    else
      dPadRightStatus = 0;*/

    //Set HID report
    //hidReport->sendReport(gameButtonStatus, yaw, pitch, roll, swaying, surging, heaving, throttleLeftPosition, throttleRightPosition, dPadLeftStatus, dPadRightStatus);
    display->print("%d %d %d\n%d %d %d\n%d %d\n%d %d", yaw, pitch, roll, swaying, surging, heaving, throttleLeftPosition, throttleRightPosition, dPadLeftStatus, dPadRightStatus);
    HAL_Delay(50);
  }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
