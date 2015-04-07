#include "Analog_Measure.h"

CAnalogMeasure::CAnalogMeasure(int pinNumber, int outputMin, int outputMax, int storageAddr, const char* name) {
  pin = pinNumber;
  outputRatioMin = outputMin;
  outputRatioMax = outputMax;
  storageAddrMeasure = storageAddr;
  nameMeasure = name;
  
  pinMode(pin, INPUT);
}

CAnalogMeasure::~CAnalogMeasure() {
}

void CAnalogMeasure::initialize() {
  initCoeff();
  updateCoeff();
}

void CAnalogMeasure::initCoeff() {
  if (EEPROM.read(storageAddrMeasure) == STORAGE_CODE_OK) {
    inputRatioMin = EEPROM.read(storageAddrMeasure + 1);
    inputRatioMin += EEPROM.read(storageAddrMeasure + 2) << 8;
    
    inputRatioMax = EEPROM.read(storageAddrMeasure + 3);
    inputRatioMax += EEPROM.read(storageAddrMeasure + 4) << 8;
    
    inputRatioNeutral = EEPROM.read(storageAddrMeasure + 5);
    inputRatioNeutral += EEPROM.read(storageAddrMeasure + 6) << 8;
  }
  else {
    inputRatioMin = 0;
    inputRatioMax = 1024;
    inputRatioNeutral = 0;
  }
}

void CAnalogMeasure::saveCoeff() {
  EEPROM.write(storageAddrMeasure, STORAGE_CODE_OK);
  EEPROM.write(storageAddrMeasure + 1, (inputRatioMin) & 0x00FF);
  EEPROM.write(storageAddrMeasure + 2, (inputRatioMin >> 8) & 0x00FF);
  
  EEPROM.write(storageAddrMeasure + 3, (inputRatioMax) & 0x00FF);
  EEPROM.write(storageAddrMeasure + 4, (inputRatioMax >> 8) & 0x00FF);
  
  EEPROM.write(storageAddrMeasure + 5, (inputRatioNeutral) & 0x00FF);
  EEPROM.write(storageAddrMeasure + 6, (inputRatioNeutral >> 8) & 0x00FF);
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
  float middle = (outputRatioMax - outputRatioMin) / 2 + outputRatioMin;
  //(32767 - -32768) / 2 + -32768
  if (((middle - inputRatioNeutral) < pos) && (pos < (middle + inputRatioNeutral)))
    pos = middle;
  else if (outputRatioMin > pos)
  //if (outputRatioMin > pos)
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
  int maxValue = getAnalogValue();
  
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Release!\n");
  delay(5000);
  int middleValue1 = getAnalogValue();
  
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Set MIN!\n");
  delay(5000);
  int minValue = getAnalogValue();
  
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Release!\n");
  delay(5000);
  int middleValue2 = getAnalogValue();
  
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Finished\n");
  inputRatioNeutral = (int)(abs(middleValue1 - middleValue2) / 2);
  inputRatioMax = maxValue;
  inputRatioMin = minValue;
  delay(2000);
  saveCoeff();
  updateCoeff();
  Serial.println(inputRatioMin);
  Serial.println(inputRatioNeutral);
  Serial.println(inputRatioMax);
  Serial.println(outputRatioMin);
  Serial.println(outputRatioMax);
  delay(5000);
}
