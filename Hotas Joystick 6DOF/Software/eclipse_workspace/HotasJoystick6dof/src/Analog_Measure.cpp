#include "Analog_Measure.h"

CAnalogMeasure::CAnalogMeasure(_gpioxConfig conf, int16_t outputMin, int16_t outputMax, uint32_t storageAddr, const char* name) {
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

  timer.start();
}

CAnalogMeasure::~CAnalogMeasure() {
}

void CAnalogMeasure::initialize() {
  setConfigGPIO(config);
  //and analogconfig
  initCoeff();
  updateCoeff();
}

void CAnalogMeasure::initCoeff() {
  //if (EEPROM.read(storageAddrMeasure) == STORAGE_CODE_OK) {
  //  inputRatioMin = EEPROM.read(storageAddrMeasure + 1) << 8 + EEPROM.read(storageAddrMeasure + 2);
  //  inputRatioMax = EEPROM.read(storageAddrMeasure + 3) << 8 + EEPROM.read(storageAddrMeasure + 4);
  //}
  //else {
    inputRatioMin = 0;
    inputRatioMin = 1024;
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
  float pos = coeffA * (float)value + coeffB;
  if (outputRatioMin > pos)
    pos = outputRatioMin;
  else if (pos > outputRatioMax)
    pos = outputRatioMax;
  return (int16_t)pos;
}

int16_t CAnalogMeasure::getMeasure() {
  return applicCoeff(getAnalogValue());
}

void CAnalogMeasure::setCalibration() {
  trace_printf("CALIBRATION\n%s\n%s", nameMeasure, "Set MAX!\n");
  timer.sleep(5000);
  trace_printf("CALIBRATION\n%s\n%s", nameMeasure, "Set MAX! --3--\n");
  timer.sleep(1000);
  trace_printf("CALIBRATION\n%s\n%s", nameMeasure, "Set MAX! --2--\n");
  timer.sleep(1000);
  trace_printf("CALIBRATION\n%s\n%s", nameMeasure, "Set MAX! --1--\n");
  timer.sleep(1000);
  trace_printf("CALIBRATION\n%s\n%s", nameMeasure, "Set MAX! --0--\n");
  //inputRatioMax = getAnalogValue();
  timer.sleep(1000);
  
  trace_printf("CALIBRATION\n%s\n%s", nameMeasure, "Set MIN!\n");
  timer.sleep(5000);
  trace_printf("CALIBRATION\n%s\n%s", nameMeasure, "Set MIN! --3--\n");
  timer.sleep(1000);
  trace_printf("CALIBRATION\n%s\n%s", nameMeasure, "Set MIN! --2--\n");
  timer.sleep(1000);
  trace_printf("CALIBRATION\n%s\n%s", nameMeasure, "Set MIN! --1--\n");
  timer.sleep(1000);
  trace_printf("CALIBRATION\n%s\n%s", nameMeasure, "Set MIN! --0--\n");
  //inputRatioMin = getAnalogValue();
  timer.sleep(1000);
  
  trace_printf("CALIBRATION\n%s\n%s", nameMeasure, "Finished\n");
  timer.sleep(1000);
  saveCoeff();
  updateCoeff();
  trace_printf("%d %d\n%d %d\n%f %f\n", inputRatioMin, inputRatioMax, outputRatioMin, outputRatioMax, coeffA, coeffB);
  timer.sleep(5000);
}
