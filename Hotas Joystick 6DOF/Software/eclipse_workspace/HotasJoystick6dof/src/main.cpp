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
uint16_t analogState;

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

_gpioxConfig ssd1306IO[] = {
};

//Create common object
/*CDisplay* display = new CDisplay();
//CButtonMatrix* buttonMatrix = new CButtonMatrix(rowMap, colMap, BUTTON_MATRIX_DELAY_UPDATE, display);
CAnalogMeasure* throttleLeft = new CAnalogMeasure(analogInput[0], POSITION_ANALOG_THROTTLE_OUTPUT_MIN, POSITION_ANALOG_THROTTLE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_THROTTLE_LEFT_ADDR_OFFSET, "Throttle", display);
CAnalogMeasure* dPadLeftX = new CAnalogMeasure(analogInput[1], POSITION_ANALOG_DPAD_OUTPUT_MIN, POSITION_ANALOG_DPAD_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_PDAD_LEFT_X_ADDR_OFFSET, "Dpad left X", display);
CAnalogMeasure* dPadLeftY = new CAnalogMeasure(analogInput[2], POSITION_ANALOG_DPAD_OUTPUT_MIN, POSITION_ANALOG_DPAD_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_PDAD_LEFT_Y_ADDR_OFFSET, "Dpad left Y", display);
CAnalogMeasure* dPadRightX = new CAnalogMeasure(analogInput[3], POSITION_ANALOG_DPAD_OUTPUT_MIN, POSITION_ANALOG_DPAD_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_PDAD_RIGHT_X_ADDR_OFFSET, "Dpad right X", display);
CAnalogMeasure* dPadRightY = new CAnalogMeasure(analogInput[4], POSITION_ANALOG_DPAD_OUTPUT_MIN, POSITION_ANALOG_DPAD_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_PDAD_RIGHT_Y_ADDR_OFFSET, "Dpad right Y", display);
CAnalogDpad* dPadLeft = new CAnalogDpad(DPAD_NEUTRAL_ZONE, display);
CAnalogDpad* dPadRight = new CAnalogDpad(DPAD_NEUTRAL_ZONE, display);
//CTracking6Dof* tracking6Dof = new CTracking6Dof(ov7670Pin, display);
//CHidReport* hidReport = new CHidReport(display);
*/
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

  //Initialize screen
  //display->initialize();
  //display->print("...INIT...");
  timer.sleep(500);

  //Initialize button matrix
  //display->print("...BUTTON MATRIX...");
  timer.sleep(500);
  //buttonMatrix->initialize();


  BlinkLed blinkLed;
  // Perform all necessary initialisations for the LED.
  blinkLed.powerUp();
  setConfigGPIO(colMap[5]);
  _gpioxConfig other[] = {
      {GPIOA, GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO},
  };
  setConfigGPIO(other[0]);

  while(1) {
    blinkLed.turnOn();
    timer.sleep(500);

    blinkLed.turnOff();
    HAL_GPIO_WritePin(colMap[5].gpiox, colMap[5].Pin, HAL_GPIO_ReadPin(other[0].gpiox, other[0].Pin));
    timer.sleep(500);
  }

  //Initialize analog input
  /*display->print("...ANALOG INPUT...");
  delay(500);
  throttle->initialize();
  dPadX->initialize();
  dPadY->initialize();
  axeX->initialize();
  axeY->initialize();
  axeZ->initialize();
  axeRX->initialize();
  axeRY->initialize();

  //Initialize analog Dpad
  display->print("...ANALOG DPAD...");
  delay(500);
  dPad->initialize();

  //Initialize joystick report
  display->print("...REPORT...");
  delay(500);
  hidReport->initialize();

  //Initialize variables
  display->print("...VARIABLES...");
  delay(500);
  memset(gameButtonStatus, false, sizeof(gameButtonStatus));
  memset(controlButtonStatus, false, sizeof(controlButtonStatus));
  x = 0;
  y = 0;
  z = 0;
  rX = 0;
  rY = 0;
  throttlePosition = 0;
  dPadStatus = HOTAS_DPAD_CENTERED;
  padX = 0;
  padY = 0;*/
  analogState = 0;

  //Enable inter
  //display->print("...INTER...");
  //MsTimer2::set(5, InterruptTimer2); // période 1000ms
  //MsTimer2::start(); // active Timer 2
  timer.sleep(500);

  // Infinite loop
  while (1) {
    //Get 6DOF
    //tracking6Dof->getPosition(&yaw, &pitch, &roll, &heaving, &swaying, &surging);

    //Get throttle position
   // throttleLeftPosition = throttleLeft->getMeasure();

    //Get DPAD status
    //dPadLeftStatus = dPadLeft->getDPadStatus(dPadLeftX->getMeasure(), dPadLeftY->getMeasure());
    //dPadRightStatus = dPadRight->getDPadStatus(dPadRightX->getMeasure(), dPadRightY->getMeasure());

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
    //display->print("%d %d %d\n%d %d %d\n%d %d\n%d %d", yaw, pitch, roll, swaying, surging, heaving, throttleLeftPosition, throttleRightPosition, dPadLeftStatus, dPadRightStatus);

    timer.sleep(50);
  }
}


void InterruptTimer2() { // debut de la fonction d'interruption Timer2
  //Get button
  //buttonMatrix->getButtonStatus(buttonStatus);
  //Extract control part
  //bool* pButton = buttonStatus;
  //memcpy(gameButtonStatus, pButton, sizeof(gameButtonStatus));
  //Extract game part
  //pButton = &buttonStatus[BUTTON_GAME_NB];
  //memcpy(controlButtonStatus, pButton, sizeof(controlButtonStatus));

  //get analog
  /*switch(analogState) {
    case 0:  throttleLeftPositionPosition = throttleLeft->getMeasure(); analogState++; break;
    case 1:  padX = dPadX->getMeasure(); analogState++; break;
    case 2:  padY = dPadY->getMeasure(); analogState++; break;//dPadStatus = dPad->getDPadStatus(padX, padY); break;
    case 3:  x = axeX->getMeasure(); analogState++; break;
    case 4:  y = axeY->getMeasure(); analogState++; break;
    case 5:  z = axeZ->getMeasure(); analogState++; break;
    case 6:  rX = axeRX->getMeasure(); analogState++; break;
    case 7:  rY = axeRY->getMeasure(); analogState = 0; break;
    default: analogState = 0; break;
  }*/
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
