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

bool buttonStatus[BUTTON_MATRIX_NB];
int rx, ry, rz, x, y;
int throttlePosition;
int state;
int i;

byte rowMap[BUTTON_MATRIX_NB_ROWS] = {
  BUTTON_MATRIX_ROWS_PIN_1,
  BUTTON_MATRIX_ROWS_PIN_2,
  BUTTON_MATRIX_ROWS_PIN_3,
  BUTTON_MATRIX_ROWS_PIN_4,
  BUTTON_MATRIX_ROWS_PIN_5,
  BUTTON_MATRIX_ROWS_PIN_6,
};
byte colMap[BUTTON_MATRIX_NB_COLS] = {
  BUTTON_MATRIX_COLS_PIN_1, 
  BUTTON_MATRIX_COLS_PIN_2, 
  BUTTON_MATRIX_COLS_PIN_3, 
  BUTTON_MATRIX_COLS_PIN_4, 
  BUTTON_MATRIX_COLS_PIN_5, 
  BUTTON_MATRIX_COLS_PIN_6, 
};


CButtonMatrix* buttonMatrix = new CButtonMatrix(rowMap, colMap);
CAnalogMeasure* axeRX = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_RX, POSITION_ANALOG_AXE_OUTPUT_MIN, POSITION_ANALOG_AXE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_AXE_RX_ADDR_OFFSET, "Axe RX");
CAnalogMeasure* axeRY = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_RY, POSITION_ANALOG_AXE_OUTPUT_MIN, POSITION_ANALOG_AXE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_AXE_RY_ADDR_OFFSET, "Axe RY");
CAnalogMeasure* axeRZ = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_RZ, POSITION_ANALOG_AXE_OUTPUT_MIN, POSITION_ANALOG_AXE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_AXE_RZ_ADDR_OFFSET, "Axe RZ");
CAnalogMeasure* axeX = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_X, POSITION_ANALOG_AXE_OUTPUT_MIN, POSITION_ANALOG_AXE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_AXE_X_ADDR_OFFSET, "Axe X");
CAnalogMeasure* axeY = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_Y, POSITION_ANALOG_AXE_OUTPUT_MIN, POSITION_ANALOG_AXE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_AXE_Y_ADDR_OFFSET, "Axe Y");
CAnalogMeasure* throttle = new CAnalogMeasure(ANALOG_INPUT_PIN_THROTTLE, POSITION_ANALOG_THROTTLE_OUTPUT_MIN, POSITION_ANALOG_THROTTLE_OUTPUT_MAX, STORAGE_ANALOG_MEASURE_THROTTLE_ADDR_OFFSET, "Throttle");
CHidReport* hidReport = new CHidReport();

///////////////////////////////////   SETUP    ////////////////////////////////////
void setup() {
  Serial.begin(9600);
  bool exitCalib = false;
  do {
    Serial.print("Send any character to start calibration manager:\n");
    delay(1000);
    if (Serial.available() > 0) {
      Serial.print("Open calibration manager:\n");
      Serial.read();
      do {
        Serial.print("************************************************************************\n");
        Serial.print("************************************************************************\n");
        Serial.print("Select option:\n");
        Serial.print("[1] Calibration axeRX\n");
        Serial.print("[2] Calibration axeRX\n");
        Serial.print("[3] Calibration axeRX\n");
        Serial.print("[4] Calibration axeRX\n");
        Serial.print("[5] Calibration axeRX\n");
        Serial.print("[6] Calibration axeRX\n");
        Serial.print("\n");
        //Serial.print("[C] Clear\n");
        Serial.print("[0] Exit\n");
        
        do {
          if (Serial.available() > 0) {
            char c = Serial.read();
            switch (c) {
              case '1': axeRX->setCalibration(); break;
              case '2': axeRY->setCalibration(); break;
              case '3': axeRZ->setCalibration(); break;
              case '4': axeX->setCalibration(); break;
              case '5': axeY->setCalibration(); break;
              case '6': throttle->setCalibration(); break;
              
              case '0': exitCalib = true; break;
              default: 
                Serial.print("[");
                Serial.print(c);
                Serial.print("] is a wrong value...\nRetry...\n");
            }
            break;
          }
        } while(1);
      } while(!exitCalib);
    }
  } while((millis() < 10000) && !exitCalib);
    Serial.print("Close calibration manager:\n");
  Serial.print("System start...\n");
  Serial.print("You can close the window\n");
  Serial.end();
  
  //Initialize button matrix
  buttonMatrix->initialize();
  
  //Initialize analog input
  axeRX->initialize();
  axeRY->initialize();
  axeRZ->initialize();
  axeX->initialize();
  axeY->initialize();
  throttle->initialize();
  
  //Initialize joystick report
  hidReport->initialize();
  
  //Initialize variables
  for (i = 0; i < BUTTON_MATRIX_NB; i++)
    buttonStatus[i] = false;
  rx = 0;
  ry = 0;
  rz = 0;
  x = 0;
  y = 0;
  throttlePosition = 0;
  state = 0;
  
  //Enable inter
  MsTimer2::set(5, InterruptTimer2); // période 1000ms 
  MsTimer2::start(); // active Timer 2
  delay(500);
}

void InterruptTimer2() { // debut de la fonction d'interruption Timer2
  //Get button
  //buttonMatrix->getButtonStatus(buttonStatus);
  
  //get analog
  switch(state) {
    case 0:  rx = axeRX->getMeasure(); state++; break;
    case 1:  ry = axeRY->getMeasure(); state++; break;
    case 2:  rz = axeRZ->getMeasure(); state++; break;
    case 3:  x = axeX->getMeasure(); state++; break;
    case 4:  y = axeY->getMeasure(); state++; break;
    case 5:  throttlePosition = throttle->getMeasure(); state = 0; break;
    default: state = 0; break;
  }
}


///////////////////////////////////   LOOP    ////////////////////////////////////
void loop() {
  static int count = 0;
  buttonStatus[count] = !buttonStatus[count];
  if (count < BUTTON_MATRIX_NB - 1)
    count++;
  else
    count = 0;
  //Set HID report
  hidReport->sendReport(buttonStatus, rx, ry, rz, x, y, throttlePosition);
  delay(50);
}
