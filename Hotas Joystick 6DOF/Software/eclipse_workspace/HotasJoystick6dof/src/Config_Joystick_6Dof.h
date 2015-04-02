#ifndef _CONFIG_JOYSTICK_6DOF_H_
#define _CONFIG_JOYSTICK_6DOF_H_

# define M_PI           3.14159265358979323846  /* pi */

///////////////////////////////////      STORAGE      /////////////////////////////
#define STORAGE_CODE_NOK 0
#define STORAGE_CODE_OK 1

#define STORAGE_PART_SYSTEM_ADDR_START 100

#define STORAGE_ANALOG_MEASURE_ADDR_START 200
#define STORAGE_ANALOG_MEASURE_THROTTLE_LEFT_ADDR_OFFSET 0
#define STORAGE_ANALOG_MEASURE_THROTTLE_RIGHT_ADDR_OFFSET 5
#define STORAGE_ANALOG_MEASURE_PDAD_LEFT_X_ADDR_OFFSET 10
#define STORAGE_ANALOG_MEASURE_PDAD_LEFT_Y_ADDR_OFFSET 15
#define STORAGE_ANALOG_MEASURE_PDAD_RIGHT_X_ADDR_OFFSET 20
#define STORAGE_ANALOG_MEASURE_PDAD_RIGHT_Y_ADDR_OFFSET 25

#define STORAGE_LED_TRACKING_ADDR_START 300

///////////////////////////////////   BUTTON MATRIX   /////////////////////////////
#define BUTTON_MATRIX_NB_ROWS 8
#define BUTTON_MATRIX_NB_COLS 6
#define BUTTON_MATRIX_NB BUTTON_MATRIX_NB_ROWS * BUTTON_MATRIX_NB_COLS
#define BUTTON_MATRIX_ROWS_PIN_1 40
#define BUTTON_MATRIX_ROWS_PIN_2 39
#define BUTTON_MATRIX_ROWS_PIN_3 38
#define BUTTON_MATRIX_ROWS_PIN_4 37
#define BUTTON_MATRIX_ROWS_PIN_5 36
#define BUTTON_MATRIX_ROWS_PIN_6 35
#define BUTTON_MATRIX_ROWS_PIN_7 34
#define BUTTON_MATRIX_ROWS_PIN_8 33

#define BUTTON_MATRIX_OLS_PIN_1 32
#define BUTTON_MATRIX_OLS_PIN_2 31
#define BUTTON_MATRIX_OLS_PIN_3 30
#define BUTTON_MATRIX_OLS_PIN_4 29
#define BUTTON_MATRIX_OLS_PIN_5 28
#define BUTTON_MATRIX_OLS_PIN_6 27

#define BUTTON_GAME_NB 32
#define BUTTON_CONTROL_NB 8

#define BUTTON_MATRIX_DELAY_UPDATE 5

///////////////////////////////////        SPI        /////////////////////////////
#define SPI_PIN_CLK 12
#define SPI_PIN_MOSI 11
#define SPI_PIN_CS 10
#define SPI_PIN_DC 9


///////////////////////////////////   ANALOG INPUT    /////////////////////////////
#define ANALOG_INPUT_NB_PIN 6
#define ANALOG_INPUT_PIN_DPAD_LEFT_X	1//A0
#define ANALOG_INPUT_PIN_DPAD_LEFT_Y	2//A1
#define ANALOG_INPUT_PIN_DPAD_RIGHT_X	3//A2
#define ANALOG_INPUT_PIN_DPAD_RIGHT_Y	4//A3
#define ANALOG_INPUT_PIN_THROTTLE_LEFT	5//A4
#define ANALOG_INPUT_PIN_THROTTLE_RIGHT	6//A5


///////////////////////////////////    ANALOG DPAD    /////////////////////////////
#define DPAD_NEUTRAL_ZONE 30

#define DPAD_CENTERED	0
#define DPAD_UP		1
#define DPAD_UP_RIGHT	2
#define DPAD_RIGHT	3
#define DPAD_DOWN_RIGHT	4
#define DPAD_DOWN	5
#define DPAD_DOWN_LEFT	6
#define DPAD_LEFT	7
#define DPAD_UP_LEFT	8


///////////////////////////////////      RATION       /////////////////////////////
#define POSITION_THROTTLE_MIN -32768
#define POSITION_THROTTLE_MAX 32767
#define POSITION_DPAD_MIN -32768
#define POSITION_DPAD_MAX 32767


///////////////////////////////////      OV7670       /////////////////////////////
#define WireOV7670 Wire1

#define OV7670_SCCB_SIO_C_PIN 6
#define OV7670_SCCB_SIO_D_PIN 5
#define OV7670_XCLK_PIN 7
#define OV7670_PCLK_PIN 41
#define OV7670_HREF_PIN 43
#define OV7670_VSYNCH_PIN 42
#define OV7670_PWDN_PIN 53
#define OV7670_RESET_PIN 52
#define OV7670_D0_PIN 50
#define OV7670_D1_PIN 51
#define OV7670_D2_PIN 48
#define OV7670_D3_PIN 49
#define OV7670_D4_PIN 46
#define OV7670_D5_PIN 47
#define OV7670_D6_PIN 44
#define OV7670_D7_PIN 45

#define NB_LINE_PER_FRAME 480
#define NB_PIXEL_PER_LINE 640
#define NB_PIXEL NB_PIXEL_PER_LINE * NB_LINE_PER_FRAME

#define PIXEL_RED_UNDETECTED 0
#define PIXEL_RED_DETECTED 1
#define PIXEL_RED_INTENSITY_TARGET 10

#define LED_SIZE_MIN 10
#define LED_SIZE_MAX 90


///////////////////////////////////   INTERRUPTION    /////////////////////////////

#endif /* _CONFIG_JOYSTICK_6DOF_H_ */
