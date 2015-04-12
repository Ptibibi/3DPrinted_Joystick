/*******************************************************************************
Hotas_Joystick.ino
V 0.02

Description:

********************************************************************************
********************************************************************************
V0.02 --> 12/04/15	Edition originale by BRB
*******************************************************************************/
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
CAnalogMeasure* axeRX = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_RX, HID_MIN_16BITS, HID_MAX_16BITS, STORAGE_ANALOG_MEASURE_AXE_RX_ADDR_OFFSET, "Axe RX");
CAnalogMeasure* axeRY = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_RY, HID_MIN_16BITS, HID_MAX_16BITS, STORAGE_ANALOG_MEASURE_AXE_RY_ADDR_OFFSET, "Axe RY");
CAnalogMeasure* axeRZ = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_RZ, HID_MIN_8BITS, HID_MAX_8BITS, STORAGE_ANALOG_MEASURE_AXE_RZ_ADDR_OFFSET, "Axe RZ");
CAnalogMeasure* axeX = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_X, HID_MIN_16BITS, HID_MAX_16BITS, STORAGE_ANALOG_MEASURE_AXE_X_ADDR_OFFSET, "Axe X");
CAnalogMeasure* axeY = new CAnalogMeasure(ANALOG_INPUT_PIN_AXE_Y, HID_MIN_16BITS, HID_MAX_16BITS, STORAGE_ANALOG_MEASURE_AXE_Y_ADDR_OFFSET, "Axe Y");
CAnalogMeasure* throttle = new CAnalogMeasure(ANALOG_INPUT_PIN_THROTTLE, HID_MIN_8BITS, HID_MAX_8BITS, STORAGE_ANALOG_MEASURE_THROTTLE_ADDR_OFFSET, "Throttle");
CHidReport* hidReport = new CHidReport();

///////////////////////////////////   SETUP    ////////////////////////////////////
void setup() {
  Serial.begin(9600);
  
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
  
  calibrationManager();
  
  //Enable inter
  MsTimer2::set(5, InterruptTimer2);
  MsTimer2::start();
  delay(500);
}

void InterruptTimer2() {
  //Get button
  buttonMatrix->getButtonStatus(buttonStatus);
  
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
  //Set HID report
  hidReport->sendReport(buttonStatus, rx, ry, rz, x, y, throttlePosition);
  delay(50);
}



void calibrationManager() {
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
        Serial.print("[1] Calibration axeRX (");
        Serial.print(axeRX->getRationMin());
        Serial.print(", ");
        Serial.print(axeRX->getRationNeutral());
        Serial.print(", ");
        Serial.print(axeRX->getRationMax());
        Serial.print(")\n");
        
        Serial.print("[2] Calibration axeRY (");
        Serial.print(axeRY->getRationMin());
        Serial.print(", ");
        Serial.print(axeRY->getRationNeutral());
        Serial.print(", ");
        Serial.print(axeRY->getRationMax());
        Serial.print(")\n");
        
        Serial.print("[3] Calibration axeRZ (");
        Serial.print(axeRZ->getRationMin());
        Serial.print(", ");
        Serial.print(axeRZ->getRationNeutral());
        Serial.print(", ");
        Serial.print(axeRZ->getRationMax());
        Serial.print(")\n");
        
        Serial.print("[4] Calibration axeX (");
        Serial.print(axeX->getRationMin());
        Serial.print(", ");
        Serial.print(axeX->getRationNeutral());
        Serial.print(", ");
        Serial.print(axeX->getRationMax());
        Serial.print(")\n");
        
        Serial.print("[5] Calibration axeZ (");
        Serial.print(axeY->getRationMin());
        Serial.print(", ");
        Serial.print(axeY->getRationNeutral());
        Serial.print(", ");
        Serial.print(axeY->getRationMax());
        Serial.print(")\n");
        
        Serial.print("[6] Calibration Throttle (");
        Serial.print(throttle->getRationMin());
        Serial.print(", ");
        Serial.print(throttle->getRationNeutral());
        Serial.print(", ");
        Serial.print(throttle->getRationMax());
        Serial.print(")\n");
        
        Serial.print("\n");
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
}
