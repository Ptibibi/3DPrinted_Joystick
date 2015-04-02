#ifndef _ANALOG_MEASURE_
#define _ANALOG_MEASURE_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_hal_sram.h"

#include "Config_Joystick_6Dof.h"
#include "Typedef_Joystick_6Dof.h"

#include "Display.h"

class CAnalogMeasure {
  //Method
  public:
    CAnalogMeasure();
    CAnalogMeasure(uint16_t analogIndex, uint16_t pinNumber, int16_t outputMin, int16_t outputMax, uint32_t storageAddr, const char* name, CDisplay* pDisp);
    ~CAnalogMeasure();
    void initialize();
    int16_t getMeasure();
    
  private:
    void initCoeff();
    void saveCoeff();
    int16_t getAnalogValue();
    int16_t applicCoeff(uint16_t value);
    void updateCoeff();
    void setCalibration();
  
  //Variables
  private:
    CDisplay* display;
    
    uint16_t index;
    uint16_t pin;
    uint32_t storageAddrMeasure;
    const char* nameMeasure;
    
    float coeffA;
    float coeffB;
    uint16_t inputRatioMin;
    uint16_t inputRatioMax;
    int16_t outputRatioMin;
    int16_t outputRatioMax;
    int16_t lastValue;
};

#ifdef __cplusplus
}
#endif

#endif /* _ANALOG_MEASURE_ */
