#include "Analog_Measure.h"

CAnalogMeasure::CAnalogMeasure(int pinNumber, int outputMin, int outputMax, int storageAddr, const char* name) {
  pin = pinNumber;
  outputRatioMin = outputMin;
  outputRatioMax = outputMax;
  storageAddrMeasure = storageAddr;
  nameMeasure = name;
  coeffAPart1 = 0;
  coeffBPart1 = 0;
  coeffAPart2 = 0;
  coeffBPart2 = 0;
  
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
  float inMiddle = (inMax - inMin) / 2 + inMin;
  float inMiddlePart1 = inMiddle + inputRatioNeutral;
  float inMiddlePart2 = inMiddle - inputRatioNeutral;
  float outMin = (float)outputRatioMin;
  float outMax = (float)outputRatioMax;
  float outMiddle = (outMax - outMin) / 2 + outMin;

  //coeffA = (outMax - outMin) / (inMax - inMin);
  //coeffB = ((inMax * outMin) - (inMin * outMax)) / (inMax - inMin);
  coeffAPart1 = (outMax - outMiddle) / (inMax - inMiddlePart1);
  coeffBPart1 = ((inMax * outMiddle) - (inMiddlePart1 * outMax)) / (inMax - inMiddlePart1);

  coeffAPart2 = (outMiddle - outMin) / (inMiddlePart2 - inMin);
  coeffBPart2 = ((inMiddlePart2 * outMin) - (inMin * outMiddle)) / (inMiddlePart2 - inMin);
}

int CAnalogMeasure::getAnalogValue() {
  return analogRead(pin);
}

int CAnalogMeasure::applicCoeff(int value) {
  float inMin = (float)inputRatioMin;
  float inMax = (float)inputRatioMax;
  float inMiddle = (inMax - inMin) / 2 + inMin;
  float inMiddlePart1 = inMiddle + inputRatioNeutral;
  float inMiddlePart2 = inMiddle - inputRatioNeutral;
  float outMin = (float)outputRatioMin;
  float outMax = (float)outputRatioMax;
  float outMiddle = (outMax - outMin) / 2 + outMin;

  float val = value;
  float pos = 0;
  
  if (val > inMiddlePart1) {
    pos = coeffAPart1 * val + coeffBPart1;
    if (pos > outMax)
      pos = outMax;
  }
  else if (inMiddlePart2 < val) {
    pos = coeffAPart2 * val + coeffBPart2;
    if (outMin > pos)
      pos = outMin;
  }
  else
    pos = outMiddle;
  
  return (int)pos;
}

int CAnalogMeasure::getMeasure() {
  return applicCoeff(getAnalogValue());
}

void CAnalogMeasure::setCalibration() {
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Start!\n");
  delay(5000);
  
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Set MAX!\n");
  delay(5000);
  int maxValue = getAnalogValue();
  
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Release!\n");
  delay(5000);
  int maxReleaseValue = getAnalogValue();
  
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Set MIN!\n");
  delay(5000);
  int minValue = getAnalogValue();
  
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Release!\n");
  delay(5000);
  int minReleaseValue = getAnalogValue();
  
  Serial.print("CALIBRATION ");
  Serial.print(nameMeasure);
  Serial.print(" Finished\n");
  int inMiddle = ((maxValue - minValue) / 2) + minValue;
  if (abs(maxReleaseValue - inMiddle) > abs(minReleaseValue - inMiddle))
    inputRatioNeutral = (int)abs(maxReleaseValue - inMiddle) + ANALOG_INPUT_NEUTRAL_OFFSET;
  else
    inputRatioNeutral = (int)abs(minReleaseValue - inMiddle) + ANALOG_INPUT_NEUTRAL_OFFSET;
  inputRatioMax = (uint16_t)maxValue;
  inputRatioMin = (uint16_t)minValue;
  
  saveCoeff();
  updateCoeff();
  Serial.println(inputRatioMin);
  Serial.println(inputRatioNeutral);
  Serial.println(inputRatioMax);
  Serial.println(outputRatioMin);
  Serial.println(outputRatioMax);
  delay(5000);
}
