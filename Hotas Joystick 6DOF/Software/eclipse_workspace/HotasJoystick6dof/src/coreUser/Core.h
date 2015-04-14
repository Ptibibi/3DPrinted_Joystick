#ifndef _CORE_
#define _CORE_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"
#include "diag/Trace.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"

#include "../src/Config_Joystick_6Dof.h"
#include "../src/Typedef_Joystick_6Dof.h"

class CCore {

	private:
		// Constructeur/destructeur
		CCore();
		~CCore();

	public:
		// Interface publique
		static CCore *GetInstance();
		static void kill();

	private:
		// Variables membres
		static CCore *_singleton;

};

class CPin {
	public:
		// Interface publique
		CPin(_gpioxConfig gpio);
		~CPin() {};
		void Write(GPIO_PinState value);
		GPIO_PinState Read();

	private:
		// Variables membres
		_gpioxConfig conf;
};

#ifdef __cplusplus
}
#endif

#endif /* _CORE_ */
