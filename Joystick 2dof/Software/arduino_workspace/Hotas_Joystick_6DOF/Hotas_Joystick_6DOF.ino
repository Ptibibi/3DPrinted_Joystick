//Library
#include <Wire.h>
#include <U8glib.h>
#include <EEPROM.h>

//Common files
#include "Config_Joystick_6Dof.h"
#include "Typedef_Joystick_6Dof.h"

//Personal library
#include "U8g_Display.h"
#include "Analog_Measure.h"
#include "Analog_Dpad.h"
#include "Button_Matrix.h"
#include "HID_Report.h"

int x, y, z, tX, tY;
int throttlePosition;
bool buttonStatus[BUTTON_GAME_NB + BUTTON_CONTROL_NB];
bool gameButtonStatus[BUTTON_GAME_NB];
bool controlButtonStatus[BUTTON_CONTROL_NB];
byte dPadStatus;

byte rowMap[BUTTON_MATRIX_NB_ROWS] = {
  BUTTON_MATRIX_ROWS_PIN_1, 
  BUTTON_MATRIX_ROWS_PIN_2, 
  BUTTON_MATRIX_ROWS_PIN_3, 
  BUTTON_MATRIX_ROWS_PIN_4, 
  BUTTON_MATRIX_ROWS_PIN_5, 
  BUTTON_MATRIX_ROWS_PIN_6, 
  BUTTON_MATRIX_ROWS_PIN_7, 
  BUTTON_MATRIX_ROWS_PIN_8,
};
byte colMap[BUTTON_MATRIX_NB_COLS] = {
  BUTTON_MATRIX_COLS_PIN_1, 
  BUTTON_MATRIX_COLS_PIN_2, 
  BUTTON_MATRIX_COLS_PIN_3, 
  BUTTON_MATRIX_COLS_PIN_4, 
  BUTTON_MATRIX_COLS_PIN_5, 
};


CU8gDisplay* display = new CU8gDisplay(SPI_PIN_CLK, SPI_PIN_MOSI, SPI_PIN_CS, SPI_PIN_DC);
CButtonMatrix* buttonMatrix = new CButtonMatrix(rowMap, colMap, BUTTON_MATRIX_DELAY_UPDATE, display);
CAnalogMeasure* throttle = new CAnalogMeasure(ANALOG_INPUT_PIN_THROTTLE, POSITION_ANALOG_THROTTLE_OUTPUT_MIN, POSITION_ANALOG_THROTTLE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_THROTTLE_ADDR_OFFSET, "Throttle", display);
CAnalogMeasure* dPadX = new CAnalogMeasure(ANALOG_INPUT_PIN_DPAD_X, POSITION_ANALOG_DPAD_OUTPUT_MIN, POSITION_ANALOG_DPAD_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_PDAD_X_ADDR_OFFSET, "Dpad X", display);
CAnalogMeasure* dPadY = new CAnalogMeasure(ANALOG_INPUT_PIN_DPAD_Y, POSITION_ANALOG_DPAD_OUTPUT_MIN, POSITION_ANALOG_DPAD_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_PDAD_Y_ADDR_OFFSET, "Dpad Y", display);
CAnalogMeasure* axeX = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_X, POSITION_ANALOG_AXE_OUTPUT_MIN, POSITION_ANALOG_AXE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_AXE_X_ADDR_OFFSET, "Axe X", display);
CAnalogMeasure* axeY = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_Y, POSITION_ANALOG_AXE_OUTPUT_MIN, POSITION_ANALOG_AXE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_AXE_Y_ADDR_OFFSET, "Axe Y", display);
CAnalogMeasure* axeZ = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_Z, POSITION_ANALOG_AXE_OUTPUT_MIN, POSITION_ANALOG_AXE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_AXE_Z_ADDR_OFFSET, "Axe Z", display);
CAnalogMeasure* axeRX = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_RX, POSITION_ANALOG_AXE_OUTPUT_MIN, POSITION_ANALOG_AXE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_AXE_RX_ADDR_OFFSET, "Axe RX", display);
CAnalogMeasure* axeRY = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_RY, POSITION_ANALOG_AXE_OUTPUT_MIN, POSITION_ANALOG_AXE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_AXE_RY_ADDR_OFFSET, "Axe RY", display);


CAnalogDpad* dPad = new CAnalogDpad(DPAD_NEUTRAL_ZONE, display);
CHidReport* hidReport = new CHidReport(display);

///////////////////////////////////   SETUP    ////////////////////////////////////
void setup() {
  
  //Initialize screen
  display->initialize();
  display->print("...INIT...");
  delay(500);
  
  //Initialize button matrix
  display->print("...BUTTON MATRIX...");
  delay(500);
  buttonMatrix->initialize();
  
  //Initialize analog input
  display->print("...ANALOG INPUT...");
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
}

///////////////////////////////////   LOOP    ////////////////////////////////////
void loop() {
  
  //Get button
  buttonMatrix->getButtonStatus(buttonStatus);
  //Extract control part
  bool* pButton = buttonStatus;
  memcpy(gameButtonStatus, pButton, sizeof(gameButtonStatus));
  //Extract game part
  pButton = &buttonStatus[BUTTON_GAME_NB];
  memcpy(controlButtonStatus, pButton, sizeof(controlButtonStatus));
  
  //Get throttle position
  throttlePosition = throttle->getMeasure();
  x = axeX->getMeasure();
  y = axeY->getMeasure();
  z = axeZ->getMeasure();
  rX = axeRX->getMeasure();
  rY = axeRY->getMeasure();
  
  //Get DPAD status
  dPadStatus = dPad->getDPadStatus(dPadX->getMeasure(), dPadY->getMeasure());
  
  //Set HID report
  hidReport->sendReport(gameButtonStatus, x, y, z, rX, rY, throttlePosition, dPadStatus);
  display->print("%d %d %d\n%d %d %d\n%d\n%d", x, y, z, rX, rY, throttlePosition, dPadStatus);
  delay(50);
}
