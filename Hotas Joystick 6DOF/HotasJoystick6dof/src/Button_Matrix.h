#ifndef _BUTTON_MATRIX_H_
#define _BUTTON_MATRIX_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "diag/Trace.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"

#include "Timer.h"

#include "Config_Joystick_6Dof.h"
#include "Typedef_Joystick_6Dof.h"

#include "Timer.h"
#include "Config_Stm32F4.h"

class CButtonMatrix {
  //Method
  public:
    CButtonMatrix(_gpioxConfig* pRowMap, _gpioxConfig* pColMap);
    ~CButtonMatrix();
    void initialize();
    void getButtonStatus(GPIO_PinState* pButtonStatus);

  private:
    void updateButtonStatus();
    void updateMatrixStatus();
    void setMatrix(uint16_t stateMatrix);

  //Variables
  private:
    Timer timer;
    _gpioxConfig rowMap[BUTTON_MATRIX_NB_ROWS];
    _gpioxConfig colMap[BUTTON_MATRIX_NB_COLS];
    GPIO_PinState buttonStatus[BUTTON_MATRIX_NB];
    uint16_t stateMatrix;

};

#ifdef __cplusplus
}
#endif

#endif /* _BUTTON_MATRIX_H_ */
