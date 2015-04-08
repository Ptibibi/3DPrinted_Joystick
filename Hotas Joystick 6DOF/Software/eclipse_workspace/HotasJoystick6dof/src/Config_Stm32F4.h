#ifndef _CONFIG_STM32F4_
#define _CONFIG_STM32F4_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_adc.h"
//#include "stm32f4xx_hal_adc_ex.h"

#include "Config_Joystick_6Dof.h"
#include "Typedef_Joystick_6Dof.h"

void setConfigGPIO(GPIO_TypeDef* GPIO_Def, GPIO_InitTypeDef GPIO_InitDefInput);
ADC_HandleTypeDef setConfigADC(GPIO_TypeDef* GPIO_Def, GPIO_InitTypeDef GPIO_InitDefInput, ADC_HandleTypeDef ADC_HandleDefInput, ADC_ChannelConfTypeDef ADC_ChannelConfDefInput);

#ifdef __cplusplus
}
#endif

#endif // _CONFIG_STM32F4_
