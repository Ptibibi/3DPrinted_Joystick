#include "Analog_Measure.h"

CAnalogMeasure::CAnalogMeasure() {
}

CAnalogMeasure::CAnalogMeasure(int pinNumber, int outputMin, int outputMax, int storageAddr, const char* name) {
  pin = pinNumber;
  outputRatioMin = outputMin;
  outputRatioMax = outputMax;
  storageAddrMeasure = storageAddr;
  nameMeasure = name;
}

CAnalogMeasure::~CAnalogMeasure() {
}

void CAnalogMeasure::initialize() {
  pinMode(pin, INPUT);
  initCoeff();
  updateCoeff();
}

void CAnalogMeasure::initCoeff() {
  if (EEPROM.read(storageAddrMeasure) == STORAGE_CODE_OK) {
    inputRatioMin = EEPROM.read(storageAddrMeasure + 1);
    inputRatioMin += EEPROM.read(storageAddrMeasure + 2) << 8;
    
    inputRatioMax = EEPROM.read(storageAddrMeasure + 3);
    inputRatioMax += EEPROM.read(storageAddrMeasure + 4) << 8;
  }
  else {
    inputRatioMin = 0;
    inputRatioMax = 1024;
  }
}

void CAnalogMeasure::saveCoeff() {
  EEPROM.write(storageAddrMeasure, STORAGE_CODE_OK);
  EEPROM.write(storageAddrMeasure + 1, (inputRatioMin) & 0x00FF);
  EEPROM.write(storageAddrMeasure + 2, (inputRatioMin >> 8) & 0x00FF);
  
  EEPROM.write(storageAddrMeasure + 3, (inputRatioMax) & 0x00FF);
  EEPROM.write(storageAddrMeasure + 4, (inputRatioMax >> 8) & 0x00FF);
}

void CAnalogMeasure::updateCoeff() {
  float inMin = (float)inputRatioMin;
  float inMax = (float)inputRatioMax;
  float outMin = (float)outputRatioMin;
  float outMax = (float)outputRatioMax;
  coeffA = (outMax - outMin) / (inMax - inMin);
  coeffB = ((inMax * outMin) - (inMin * outMax)) / (inMax - inMin);
}

int CAnalogMeasure::getAnalogValue() {
  return analogRead(pin);
}

int CAnalogMeasure::applicCoeff(int value) {
  float val = value;
  float pos = coeffA * val + coeffB;
  if (outputRatioMin > pos)
    pos = outputRatioMin;
  else if (pos > outputRatioMax)
    pos = outputRatioMax;
  return (int)pos;
}

int CAnalogMeasure::getMeasure() {
  return applicCoeff(getAnalogValue());
}

void CAnalogMeasure::setCalibration() {
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Set MAX!\n");
  delay(5000);
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Set MAX! --3--\n");
  delay(1000);
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Set MAX! --2--\n");
  delay(1000);
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Set MAX! --1--\n");
  delay(1000);
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Set MAX! --0--\n");
  inputRatioMax = getAnalogValue();
  delay(1000);
  
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Set MIN!\n");
  delay(5000);
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Set MIN! --3--\n");
  delay(1000);
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Set MIN! --2--\n");
  delay(1000);
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Set MIN! --1--\n");
  delay(1000);
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Set MIN! --0--\n");
  inputRatioMin = getAnalogValue();
  delay(1000);
  
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Finished\n");
  delay(1000);
  saveCoeff();
  updateCoeff();
  Serial.println(inputRatioMin);
  Serial.println(inputRatioMax);
  Serial.println(outputRatioMin);
  Serial.println(outputRatioMax);
  Serial.println(coeffA);
  Serial.println(coeffB);
  delay(5000);
}
