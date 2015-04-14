#include "Core.h"

CCore::CCore() {
	trace_printf("CCore constructor\n");
}

CCore::~CCore() {
	trace_printf("CCore destructor\n");
}

CCore* CCore::GetInstance() {
	if (NULL == _singleton) {
		trace_printf("creating singleton\n");
		_singleton =  new CCore;
	} else {
		trace_printf("singleton already created!\n");
	}
	return _singleton;
}

void CCore::kill() {
  if (NULL != _singleton) {
      delete _singleton;
      _singleton = NULL;
    }
}


CPin::CPin(_gpioxConfig gpio) {
	conf = gpio;

	// Enable GPIO Peripheral clock
	uint32_t portNumber = (uint32_t)conf.gpiox - (uint32_t)GPIOA_BASE;
	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN << (portNumber));

	GPIO_InitTypeDef GPIO_InitDef;
	GPIO_InitDef.Pin = conf.initDef.Pin;
	GPIO_InitDef.Mode = conf.initDef.Mode;
	GPIO_InitDef.Pull = conf.initDef.Pull;
	GPIO_InitDef.Speed = conf.initDef.Speed;
	GPIO_InitDef.Alternate = conf.initDef.Alternate;
	HAL_GPIO_Init(conf.gpiox, &GPIO_InitDef);
}

void CPin::Write(GPIO_PinState value) {
	HAL_GPIO_WritePin(conf.gpiox, conf.initDef.Pin, value);
}

GPIO_PinState CPin::Read() {
	return HAL_GPIO_ReadPin(conf.gpiox, conf.initDef.Pin);
}
