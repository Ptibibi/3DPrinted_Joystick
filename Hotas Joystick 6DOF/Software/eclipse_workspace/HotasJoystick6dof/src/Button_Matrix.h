#ifndef _BUTTON_MATRIX_H_
#define _BUTTON_MATRIX_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "string.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"

#include "Config_Joystick_6Dof.h"
#include "Typedef_Joystick_6Dof.h"

#include "Timer.h"
#include "Config_Gpio.h"
#include "Display.h"

class CButtonMatrix {
  //Method
  public:
    CButtonMatrix(_gpioxConfig* pRowMap, _gpioxConfig* pColMap, uint64_t updateDelay, CDisplay* pDisp);
    ~CButtonMatrix();
    void initialize();
    void getButtonStatus(GPIO_PinState* pButtonStatus);

  private:
    void setMatrix(uint16_t stateCol);
    void nextButtonMatrixState();
    void updateButtonStatus();

  //Variables
  private:
    CDisplay* display;
    uint64_t nextDelay;
    uint64_t timeNextUpdate;
    _gpioxConfig rowMap[BUTTON_MATRIX_NB_ROWS];
    _gpioxConfig colMap[BUTTON_MATRIX_NB_COLS];
    GPIO_PinState buttonStatus[BUTTON_MATRIX_NB];
    uint16_t stateCol;
    Timer timer;

};

#ifdef __cplusplus
}
#endif

#endif /* _BUTTON_MATRIX_H_ */
