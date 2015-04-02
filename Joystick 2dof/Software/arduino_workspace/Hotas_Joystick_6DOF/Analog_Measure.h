#ifndef _ANALOG_MEASURE_
#define _ANALOG_MEASURE_

#include "Config_Joystick_6Dof.h"
#include "Typedef_Joystick_6Dof.h"

#include "U8g_Display.h"
#include <EEPROM.h>

class CAnalogMeasure {
  //Method
  public:
    CAnalogMeasure();
    CAnalogMeasure(int pinNumber, int outputMin, int outputMax, int storageAddr, const char* name, CU8gDisplay* pDisp);
    ~CAnalogMeasure();
    void initialize();
    int getMeasure();
    
  private:
    void initCoeff();
    void saveCoeff();
    int getAnalogValue();
    int applicCoeff(int value);
    void updateCoeff();
    void setCalibration();
  
  //Variables
  private:
    CU8gDisplay* display;
    //DueFlashStorage EEPROM;
    
    int pin;
    int storageAddrMeasure;
    const char* nameMeasure;
    
    float coeffA;
    float coeffB;
    int inputRatioMin;
    int inputRatioMax;
    int outputRatioMin;
    int outputRatioMax;
    int lastValue;
};

#endif /* _ANALOG_MEASURE_ */
