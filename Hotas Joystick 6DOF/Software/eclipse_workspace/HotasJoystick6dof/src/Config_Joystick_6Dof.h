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


///////////////////////////////////   ANALOG INPUT    /////////////////////////////
/*#define ANALOG_INPUT_NB_PIN 6
#define ANALOG_INPUT_PIN_DPAD_LEFT_X	1//A0
#define ANALOG_INPUT_PIN_DPAD_LEFT_Y	2//A1
#define ANALOG_INPUT_PIN_DPAD_RIGHT_X	3//A2
#define ANALOG_INPUT_PIN_DPAD_RIGHT_Y	4//A3
#define ANALOG_INPUT_PIN_THROTTLE_LEFT	5//A4
#define ANALOG_INPUT_PIN_THROTTLE_RIGHT	6//A5*/

#define POSITION_ANALOG_THROTTLE_OUTPUT_MIN -32768
#define POSITION_ANALOG_THROTTLE_OUTPUT_MAX 32767
#define POSITION_ANALOG_AXE_OUTPUT_MIN -32768
#define POSITION_ANALOG_AXE_OUTPUT_MAX 32767
#define POSITION_ANALOG_DPAD_OUTPUT_MIN -100
#define POSITION_ANALOG_DPAD_OUTPUT_MAX 100
#define ANALOG_INPUT_NEUTRAL_OFFSET 0


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


///////////////////////////////////      OV7670       /////////////////////////////
#define WireOV7670 Wire1

/*#define OV7670_SCCB_SIO_C_PIN 6
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
#define LED_SIZE_MAX 90*/

#endif /* _CONFIG_JOYSTICK_6DOF_H_ */
