#ifndef _DISPLAY_
#define _DISPLAY_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdarg.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"

#include "Config_Joystick_6Dof.h"
#include "Typedef_Joystick_6Dof.h"

#include "SSD1306_OLED.h"

class CDisplay {
  //Method
  public:
	CDisplay();
	CDisplay(uint16_t clkPin, uint16_t mosiPin, uint16_t csPin, uint16_t dcPin);
    ~CDisplay();
    void initialize();
    void print(const char* txt, ...);

  //Variables
  private:
    SSD1306_OLED ssd1306;

    uint16_t lineOffset;
};

#ifdef __cplusplus
}
#endif

#endif /* _DISPLAY_ */
