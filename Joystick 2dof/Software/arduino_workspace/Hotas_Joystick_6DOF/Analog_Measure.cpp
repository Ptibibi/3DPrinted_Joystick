#include "Analog_Measure.h"

CAnalogMeasure::CAnalogMeasure() {
}

CAnalogMeasure::CAnalogMeasure(int pinNumber, int outputMin, int outputMax, int storageAddr, const char* name, CU8gDisplay* pDisp) {
  display = pDisp;
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
    inputRatioMin = EEPROM.read(storageAddrMeasure + 1) << 8 + EEPROM.read(storageAddrMeasure + 2);
    inputRatioMax = EEPROM.read(storageAddrMeasure + 3) << 8 + EEPROM.read(storageAddrMeasure + 4);
  }
  else {
    setCalibration();
  }
}

void CAnalogMeasure::saveCoeff() {
  EEPROM.write(storageAddrMeasure, STORAGE_CODE_OK);
  EEPROM.write(storageAddrMeasure + 1, (inputRatioMin >> 8) & 0x00FF);
  EEPROM.write(storageAddrMeasure + 2, inputRatioMin & 0x00FF);
  EEPROM.write(storageAddrMeasure + 3, (inputRatioMax >> 8) & 0x00FF);
  EEPROM.write(storageAddrMeasure + 4, inputRatioMax & 0x00FF);
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
  int pos = (int)(coeffA * val + coeffB);
  if (outputRatioMin > pos)
    pos = outputRatioMin;
  else if (pos > outputRatioMax)
    pos = outputRatioMax;
  return pos;
}

int CAnalogMeasure::getMeasure() {
  return applicCoeff(getAnalogValue());
}

void CAnalogMeasure::setCalibration() {
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MAX!");
  delay(5000);
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MAX! --3--");
  delay(1000);
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MAX! --2--");
  delay(1000);
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MAX! --1--");
  delay(1000);
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MAX! --0--");
  inputRatioMax = getAnalogValue();
  delay(1000);
  
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MIN!");
  delay(5000);
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MIN! --3--");
  delay(1000);
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MIN! --2--");
  delay(1000);
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MIN! --1--");
  delay(1000);
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MIN! --0--");
  inputRatioMin = getAnalogValue();
  delay(1000);
  
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Finished");
  delay(1000);
  saveCoeff();
  updateCoeff();
  display->print("%d %d\n%d %d\n%f %f", inputRatioMin, inputRatioMax, outputRatioMin, outputRatioMax, coeffA, coeffB);
  delay(10000);
}
