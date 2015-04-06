#ifndef _GPIO_CONFIG__
#define _GPIO_CONFIG__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include "Config_Joystick_6Dof.h"
#include "Typedef_Joystick_6Dof.h"

void setConfigGPIO(_gpioxConfig config);

#ifdef __cplusplus
}
#endif

#endif // _GPIO_CONFIG__
