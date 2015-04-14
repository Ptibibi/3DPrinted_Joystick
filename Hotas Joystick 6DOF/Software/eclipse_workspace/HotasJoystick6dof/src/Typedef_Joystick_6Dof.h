#ifndef _TYPEDEF_JOYSTICK_6DOF_
#define _TYPEDEF_JOYSTICK_6DOF_

#include "stdint.h"
#include "stdbool.h"
#include "string.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"

struct _gpioxConfig {
	/*_gpioxConfig()
    {
		gpiox = GPIOE;
		initDef = {GPIO_PIN_8, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FAST, GPIO_AF0_MCO};
    }*/
	GPIO_TypeDef* gpiox;
	GPIO_InitTypeDef initDef;
};

union rgb555_ {
	uint8_t port;
	struct {
		uint8_t na1 : 1;
		uint8_t R : 5;
		uint8_t na2 : 2;
	};
};

struct ledTracking_{
	uint16_t led[3][4];
	bool newData;
};

struct coord_ {
	uint16_t X;
	uint16_t Y;
};

struct ov7670Pin_ {
	uint16_t siocPin;
	uint16_t siodPin;
	uint16_t xclkPin;
	uint16_t pclkPin;
	uint16_t hrefPin;
	uint16_t vSynchPin;
	uint16_t pwdmPin;
	uint16_t resetPin;
	uint16_t d0Pin;
	uint16_t d1Pin;
	uint16_t d2Pin;
	uint16_t d3Pin;
	uint16_t d4Pin;
	uint16_t d5Pin;
	uint16_t d6Pin;
	uint16_t d7Pin;
	uint16_t d8Pin;
};

#endif /* _TYPEDEF_JOYSTICK_6DOF_ */
