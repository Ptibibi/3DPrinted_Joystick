#include "Analog_Measure.h"

CAnalogMeasure::CAnalogMeasure(_gpioxConfig conf, int16_t outputMin, int16_t outputMax, uint32_t storageAddr, const char* name, CDisplay* pDisp) {
  display = pDisp;
  //DueFlashStorage EEPROM();
  config = conf;
  inputRatioMin = 0;
  inputRatioMax = 0;
  outputRatioMin = outputMin;
  outputRatioMax = outputMax;
  lastValue = 0;
  storageAddrMeasure = storageAddr;
  nameMeasure = name;
  coeffA = 0;
  coeffB = 0;
}

CAnalogMeasure::~CAnalogMeasure() {
}

void CAnalogMeasure::initialize() {
  //pinMode(pin, INPUT);
  initCoeff();
  updateCoeff();
}

void CAnalogMeasure::initCoeff() {
  //if (EEPROM.read(storageAddrMeasure) == STORAGE_CODE_OK) {
  //  inputRatioMin = EEPROM.read(storageAddrMeasure + 1) << 8 + EEPROM.read(storageAddrMeasure + 2);
  //  inputRatioMax = EEPROM.read(storageAddrMeasure + 3) << 8 + EEPROM.read(storageAddrMeasure + 4);
  //}
  //else {
    setCalibration();
  //}
}

void CAnalogMeasure::saveCoeff() {
  //EEPROM.write(storageAddrMeasure, STORAGE_CODE_OK);
  //EEPROM.write(storageAddrMeasure + 1, (inputRatioMin >> 8) & 0x00FF);
  //EEPROM.write(storageAddrMeasure + 2, inputRatioMin & 0x00FF);
  //EEPROM.write(storageAddrMeasure + 3, (inputRatioMax >> 8) & 0x00FF);
  //EEPROM.write(storageAddrMeasure + 4, inputRatioMax & 0x00FF);
}

void CAnalogMeasure::updateCoeff() {
  float inMin = (float)inputRatioMin;
  float inMax = (float)inputRatioMax;
  float outMin = (float)outputRatioMin;
  float outMax = (float)outputRatioMax;
  coeffA = (outMax - outMin) / (inMax - inMin);
  coeffB = ((inMax * outMin) - (inMin * outMax)) / (inMax - inMin);
}

int16_t CAnalogMeasure::getAnalogValue() {
  //return analogRead(pin);
  return 0;
}

int16_t CAnalogMeasure::applicCoeff(uint16_t value) {
  float val = value;
  int pos = (int)(coeffA * val + coeffB);
  if (outputRatioMin > pos)
    pos = outputRatioMin;
  else if (pos > outputRatioMax)
    pos = outputRatioMax;
  return pos;
}

int16_t CAnalogMeasure::getMeasure() {
  return applicCoeff(getAnalogValue());
}

void CAnalogMeasure::setCalibration() {
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MAX!");
  HAL_Delay(5000);
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MAX! --3--");
  HAL_Delay(1000);
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MAX! --2--");
  HAL_Delay(1000);
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MAX! --1--");
  HAL_Delay(1000);
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MAX! --0--");
  //inputRatioMax = getAnalogValue();
  HAL_Delay(1000);
  
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MIN!");
  HAL_Delay(5000);
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MIN! --3--");
  HAL_Delay(1000);
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MIN! --2--");
  HAL_Delay(1000);
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MIN! --1--");
  HAL_Delay(1000);
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Set MIN! --0--");
  //inputRatioMin = getAnalogValue();
  HAL_Delay(1000);
  
  display->print("CALIBRATION\n%s\n%s", nameMeasure, "Finished");
  HAL_Delay(1000);
  saveCoeff();
  updateCoeff();
  display->print("%d %d\n%d %d\n%f %f", inputRatioMin, inputRatioMax, outputRatioMin, outputRatioMax, coeffA, coeffB);
  HAL_Delay(10000);
}
