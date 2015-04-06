//Library
#include "arduino.h"
#include <Wire.h>
#include <MsTimer2.h>
#include <EEPROM.h>


//Common files
#include "Config_Joystick.h"

//Personal library
#include "Button_Matrix.h"
#include "Analog_Measure.h"
#include "HID_Report.h"

bool buttonStatus[BUTTON_GAME_NB + BUTTON_CONTROL_NB];
bool gameButtonStatus[BUTTON_GAME_NB];
bool controlButtonStatus[BUTTON_CONTROL_NB];
int rx, ry, rz, x, y;
int throttlePosition;
byte dPad1Status, dPad2Status;
int state;

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


//CButtonMatrix* buttonMatrix = new CButtonMatrix(rowMap, colMap, display);
CAnalogMeasure* axeRX = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_RX, POSITION_ANALOG_AXE_OUTPUT_MIN, POSITION_ANALOG_AXE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_AXE_RX_ADDR_OFFSET, "Axe RX");
CAnalogMeasure* axeRY = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_RY, POSITION_ANALOG_AXE_OUTPUT_MIN, POSITION_ANALOG_AXE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_AXE_RY_ADDR_OFFSET, "Axe RY");
//CAnalogMeasure* axeRZ = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_RZ, POSITION_ANALOG_AXE_OUTPUT_MIN, POSITION_ANALOG_AXE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_AXE_RZ_ADDR_OFFSET, "Axe RZ");
//CAnalogMeasure* axeX = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_X, POSITION_ANALOG_AXE_OUTPUT_MIN, POSITION_ANALOG_AXE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_AXE_X_ADDR_OFFSET, "Axe X");
//CAnalogMeasure* axeY = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_Y, POSITION_ANALOG_AXE_OUTPUT_MIN, POSITION_ANALOG_AXE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_AXE_Y_ADDR_OFFSET, "Axe Y");
//CAnalogMeasure* throttle = new CAnalogMeasure(ANALOG_INPUT_PIN_THROTTLE, POSITION_ANALOG_THROTTLE_OUTPUT_MIN, POSITION_ANALOG_THROTTLE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_THROTTLE_ADDR_OFFSET, "Throttle");
CHidReport* hidReport = new CHidReport();

///////////////////////////////////   SETUP    ////////////////////////////////////
void setup() {
  Serial.begin(9600);
  do {
    if (Serial.available() > 0) {
      Serial.print("aivailable\n");
      axeRX->initialize();
      axeRX->setCalibration();
      axeRY->initialize();
      axeRY->setCalibration();
      /*axeRZ->initialize();
      axeRZ->setCalibration();
      axeX->initialize();
      axeX->setCalibration();
      axeY->initialize();
      axeY->setCalibration();
      throttle->initialize();
      throttle->setCalibration();*/
      //while(1);
    }
    delay(1000);
  } while(millis() < 10000);
  Serial.end();
  
  //Initialize button matrix
  //buttonMatrix->initialize();
  
  //Initialize analog input
  axeRX->initialize();
  axeRY->initialize();
  //axeRZ->initialize();
  //axeX->initialize();
  //axeY->initialize();
  //throttle->initialize();
  
  //Initialize joystick report
  //hidReport = new CHidReport();
  hidReport->initialize();
  
  //Initialize variables
  memset(gameButtonStatus, false, sizeof(gameButtonStatus));
  memset(controlButtonStatus, false, sizeof(controlButtonStatus));
  rx = 0;
  ry = 0;
  rz = 0;
  x = 0;
  y = 0;
  throttlePosition = 0;
  dPad1Status = 0;
  dPad2Status = 4;
  state = 0;
  
  //Enable inter
  MsTimer2::set(5, InterruptTimer2); // période 1000ms 
  MsTimer2::start(); // active Timer 2
  delay(500);
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
  switch(state) {
    case 0:  rx = axeRX->getMeasure(); state++; break;
    case 1:  ry = axeRY->getMeasure(); state++; break;
    //case 2:  rz = axeRZ->getMeasure(); state++; break;
    //case 3:  x = axeX->getMeasure(); state++; break;
    //case 4:  y = axeY->getMeasure(); state++; break;
    //case 5:  throttlePosition = throttle->getMeasure(); state = 0; break;
    default: state = 0; break;
  }
  //Serial.print("test\n");
}


///////////////////////////////////   LOOP    ////////////////////////////////////
void loop() {
  
  static int count = 0;
  gameButtonStatus[count] = !gameButtonStatus[count];
  if (count < BUTTON_GAME_NB - 1)
    count++;
  else
    count = 0;
    
  //rx = (int)random(-32768, 32767);
  //ry = (int)random(-32768, 32767);
  rz = (int)random(-32768, 32767);
  x = (int)random(-32768, 32767);
  y = (int)random(-32768, 32767);
  throttlePosition = (int)random(-32768, 32767);
  
  if (dPad1Status < 8)
    dPad1Status++;
  else
    dPad1Status = 0;
  
  if (dPad2Status < 8)
    dPad2Status++;
  else
    dPad2Status = 0;
    
  //Set HID report
  hidReport->sendReport(gameButtonStatus, rx, ry, rz, x, y, throttlePosition, dPad1Status, dPad2Status);
  delay(50);
}
