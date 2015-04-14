 /*******************************************************************************
 HID_Report.h
 V 0.01

 Description:

 ********************************************************************************
 ********************************************************************************
 V0.01 --> 13/04/15	Edition originale by BRB
 *******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _HARDWARE_INIT_
#define _HARDWARE_INIT_
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_cortex.h"
#include "stm32f4xx_hal_pwr.h"
#include "stm32f4xx_hal_flash.h"

void GPIO_Init(void);

void SystemClock_Config(void);

#ifdef __cplusplus
}
#endif
#endif /*_HARDWARE_INIT_ */
