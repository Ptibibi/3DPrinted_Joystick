#ifndef _BUTTON_MATRIX_
#define _BUTTON_MATRIX_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "diag/Trace.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"

#include "Timer.h"

#include "../src/Config_Joystick_6Dof.h"
#include "../src/Typedef_Joystick_6Dof.h"
#include "../src/Config_Stm32F4.h"
#include "../src/coreUser/Core.h"

class CButtonMatrix {
  //Method
  public:
    CButtonMatrix(_gpioxConfig* pRowMap, _gpioxConfig* pColMap, GPIO_PinState* pButtonStatus);
    ~CButtonMatrix();
    void initialize();
    void getButtonStatus();

  private:
    void updateButtonStatus();
    void updateMatrixStatus();
    void setMatrix(uint16_t stateMatrix);

  //Variables
  private:
	CCore* core = CCore::GetInstance();
    Timer timer;
    GPIO_PinState* buttonStatus;
    uint16_t stateMatrix;

    CPin* rowPin[BUTTON_MATRIX_NB_ROWS];
    CPin* colPin[BUTTON_MATRIX_NB_COLS];

};

#ifdef __cplusplus
}
#endif

#endif /* _BUTTON_MATRIX_ */
