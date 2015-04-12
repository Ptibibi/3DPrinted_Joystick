/*******************************************************************************
Analog_Measure.h
V 0.02

Description:

********************************************************************************
********************************************************************************
V0.02 --> 12/04/15	Edition originale by BRB
*******************************************************************************/
#ifndef _ANALOG_MEASURE_
#define _ANALOG_MEASURE_

#include "arduino.h"
#include "Config_Joystick.h"

#include <EEPROM.h>

class CAnalogMeasure {
  //Method
  public:
    CAnalogMeasure(int pinNumber, int outputMin, int outputMax, int storageAddr, const char* name);
    ~CAnalogMeasure();
    void setCalibration();
    void initialize();
    int getMeasure();
    int getRationMin();
    int getRationMax();
    int getRationNeutral();
    
  private:
    void initCoeff();
    void saveCoeff();
    int getAnalogValue();
    int applicCoeff(int value);
    void updateCoeff();
  
  //Variables
  private:
    int pin;
    int storageAddrMeasure;
    const char* nameMeasure;
    
    float coeffAPart1;
    float coeffBPart1;
    float coeffAPart2;
    float coeffBPart2;
    int inputRatioMin;
    int inputRatioMax;
    int inputRatioNeutral;
    int outputRatioMin;
    int outputRatioMax;
};

#endif /* _ANALOG_MEASURE_ */
