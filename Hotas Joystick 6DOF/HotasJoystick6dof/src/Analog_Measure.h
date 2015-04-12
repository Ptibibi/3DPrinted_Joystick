#ifndef _ANALOG_MEASURE_
#define _ANALOG_MEASURE_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"
#include "stdlib.h"
#include "diag/Trace.h"

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_hal_sram.h"

#include "Timer.h"

//Common files
#include "Config_Joystick_6Dof.h"
#include "Typedef_Joystick_6Dof.h"

//Hardware library
#include "Config_Stm32F4.h"

//Functional library

class CAnalogMeasure {
  //Method
  public:
    CAnalogMeasure(_gpioxConfig gpioConfig, ADC_HandleTypeDef analogConfig, ADC_ChannelConfTypeDef ADC_ChannelConfDef, int16_t outputMin, int16_t outputMax, uint32_t storageAddr, const char* name);
    ~CAnalogMeasure();
    void setCalibration();
    void initialize();
    int16_t getMeasure();
    
  private:
    void initCoeff();
    void saveCoeff();
    int32_t getAnalogValue();
    int16_t applicCoeff(int32_t value);
    void updateCoeff();
  
  //Variables
  private:
    Timer timer;
    _gpioxConfig gpioConf;
    ADC_HandleTypeDef adcHandle;
    ADC_ChannelConfTypeDef adcChannelConf;
    uint32_t storageAddrMeasure;
    const char* nameMeasure;
    
    float coeffAPart1;
    float coeffBPart1;
    float coeffAPart2;
    float coeffBPart2;
    uint16_t inputRatioMin;
    uint16_t inputRatioMax;
    uint16_t inputRatioNeutral;
    int16_t outputRatioMin;
    int16_t outputRatioMax;
};

#ifdef __cplusplus
}
#endif

#endif /* _ANALOG_MEASURE_ */
