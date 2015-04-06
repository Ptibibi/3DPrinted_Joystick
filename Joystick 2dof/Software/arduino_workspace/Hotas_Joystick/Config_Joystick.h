#ifndef _CONFIG_JOYSTICK_H_
#define _CONFIG_JOYSTICK_H_

#include "arduino.h"

///////////////////////////////////      STORAGE      /////////////////////////////
#define STORAGE_CODE_NOK 0
#define STORAGE_CODE_OK 1

#define STORAGE_PART_SYSTEM_ADDR_START 100

#define STORAGE_ANALOG_MEASURE_ADDR_START 200

#define STORAGE_ANALOG_MEASURE_AXE_RX_ADDR_OFFSET 0
#define STORAGE_ANALOG_MEASURE_AXE_RY_ADDR_OFFSET 5
#define STORAGE_ANALOG_MEASURE_AXE_RZ_ADDR_OFFSET 10
#define STORAGE_ANALOG_MEASURE_AXE_X_ADDR_OFFSET 15
#define STORAGE_ANALOG_MEASURE_AXE_Y_ADDR_OFFSET 20
#define STORAGE_ANALOG_MEASURE_THROTTLE_ADDR_OFFSET 25


///////////////////////////////////   BUTTON MATRIX   /////////////////////////////
#define BUTTON_MATRIX_NB_ROWS 8
#define BUTTON_MATRIX_NB_COLS 5
#define BUTTON_MATRIX_NB BUTTON_MATRIX_NB_ROWS * BUTTON_MATRIX_NB_COLS

#define BUTTON_MATRIX_ROWS_PIN_1 31
#define BUTTON_MATRIX_ROWS_PIN_2 27
#define BUTTON_MATRIX_ROWS_PIN_3 1
#define BUTTON_MATRIX_ROWS_PIN_4 28
#define BUTTON_MATRIX_ROWS_PIN_5 29
#define BUTTON_MATRIX_ROWS_PIN_6 30
#define BUTTON_MATRIX_ROWS_PIN_7 12
#define BUTTON_MATRIX_ROWS_PIN_8 32

#define BUTTON_MATRIX_COLS_PIN_1 21
#define BUTTON_MATRIX_COLS_PIN_2 20
#define BUTTON_MATRIX_COLS_PIN_3 32
#define BUTTON_MATRIX_COLS_PIN_4 19
#define BUTTON_MATRIX_COLS_PIN_5 18

#define BUTTON_GAME_NB 32
#define BUTTON_CONTROL_NB 8

///////////////////////////////////        SPI        /////////////////////////////
#define SPI_PIN_CLK 9
#define SPI_PIN_MOSI 10
#define SPI_PIN_CS 8
#define SPI_PIN_DC 11


///////////////////////////////////   ANALOG INPUT    /////////////////////////////
#define ANALOG_INPUT_PIN_AXE_RX A0
#define ANALOG_INPUT_PIN_AXE_RY A1
#define ANALOG_INPUT_PIN_AXE_RZ A2
#define ANALOG_INPUT_PIN_AXE_X A3
#define ANALOG_INPUT_PIN_AXE_Y A4
#define ANALOG_INPUT_PIN_THROTTLE A5


///////////////////////////////////    ANALOG DPAD    /////////////////////////////
#define DPAD_NEUTRAL_ZONE 30


///////////////////////////////////      RATION       /////////////////////////////
#define POSITION_ANALOG_THROTTLE_OUTPUT_MIN -32768
#define POSITION_ANALOG_THROTTLE_OUTPUT_MAX 32767
#define POSITION_ANALOG_AXE_OUTPUT_MIN -32768
#define POSITION_ANALOG_AXE_OUTPUT_MAX 32767
#define POSITION_ANALOG_DPAD_OUTPUT_MIN -100
#define POSITION_ANALOG_DPAD_OUTPUT_MAX 100

#endif /* _CONFIG_JOYSTICK_H_ */