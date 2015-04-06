#include "Config_Gpio.h"

void setConfigGPIO(_gpioxConfig config) {
  // Enable GPIO Peripheral clock
  uint32_t portNumber = (uint32_t)config.gpiox - (uint32_t)GPIOA_BASE;
  RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN << (portNumber));

  GPIO_InitTypeDef GPIO_InitDef;
  GPIO_InitDef.Pin = config.Pin;
  GPIO_InitDef.Mode = config.Mode;
  GPIO_InitDef.Pull = config.Pull;
  GPIO_InitDef.Speed = config.Speed;
  GPIO_InitDef.Alternate = config.Alternate;
  HAL_GPIO_Init(config.gpiox, &GPIO_InitDef);
}
