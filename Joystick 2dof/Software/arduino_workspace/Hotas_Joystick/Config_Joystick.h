/*******************************************************************************
Config_Joystick.h
V 0.02

Description:

********************************************************************************
********************************************************************************
V0.02 --> 12/04/15	Edition originale by BRB
*******************************************************************************/
#ifndef _CONFIG_JOYSTICK_H_
#define _CONFIG_JOYSTICK_H_

#include "arduino.h"

///////////////////////////////////      STORAGE      /////////////////////////////
#define STORAGE_CODE_NOK 0
#define STORAGE_CODE_OK 1
#define STORAGE_ANALOG_MEASURE_ADDR_START 200

#define STORAGE_ANALOG_MEASURE_AXE_RX_ADDR_OFFSET 0
#define STORAGE_ANALOG_MEASURE_AXE_RY_ADDR_OFFSET 7
#define STORAGE_ANALOG_MEASURE_AXE_RZ_ADDR_OFFSET 14
#define STORAGE_ANALOG_MEASURE_AXE_X_ADDR_OFFSET 21
#define STORAGE_ANALOG_MEASURE_AXE_Y_ADDR_OFFSET 28
#define STORAGE_ANALOG_MEASURE_THROTTLE_ADDR_OFFSET 35


///////////////////////////////////   BUTTON MATRIX   /////////////////////////////
#define BUTTON_MATRIX_NB_ROWS 6
#define BUTTON_MATRIX_NB_COLS 6
#define BUTTON_MATRIX_NB BUTTON_MATRIX_NB_ROWS * BUTTON_MATRIX_NB_COLS

#define BUTTON_MATRIX_COLS_PIN_1 2
#define BUTTON_MATRIX_COLS_PIN_2 3
#define BUTTON_MATRIX_COLS_PIN_3 4
#define BUTTON_MATRIX_COLS_PIN_4 5
#define BUTTON_MATRIX_COLS_PIN_5 6
#define BUTTON_MATRIX_COLS_PIN_6 7

#define BUTTON_MATRIX_ROWS_PIN_1 8
#define BUTTON_MATRIX_ROWS_PIN_2 9
#define BUTTON_MATRIX_ROWS_PIN_3 10
#define BUTTON_MATRIX_ROWS_PIN_4 11
#define BUTTON_MATRIX_ROWS_PIN_5 12
#define BUTTON_MATRIX_ROWS_PIN_6 13

///////////////////////////////////   ANALOG INPUT    /////////////////////////////
#define ANALOG_INPUT_PIN_AXE_RX A0
#define ANALOG_INPUT_PIN_AXE_RY A1
#define ANALOG_INPUT_PIN_AXE_RZ A2
#define ANALOG_INPUT_PIN_AXE_X A3
#define ANALOG_INPUT_PIN_AXE_Y A4
#define ANALOG_INPUT_PIN_THROTTLE A5

#define HID_MIN_8BITS -128
#define HID_MAX_8BITS 127
#define HID_MIN_16BITS -32768
#define HID_MAX_16BITS 32767

#define ANALOG_INPUT_NEUTRAL_OFFSET 10


///////////////////////////////////    ANALOG DPAD    /////////////////////////////
#define DPAD_NEUTRAL_ZONE 30

#endif /* _CONFIG_JOYSTICK_H_ */
