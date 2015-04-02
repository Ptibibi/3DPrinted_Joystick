#ifndef _U8G_DISPLAY_
#define _U8G_DISPLAY_

#include "Config_Joystick_6Dof.h"
#include "Typedef_Joystick_6Dof.h"

#include <U8glib.h>

class CU8gDisplay {
  //Method
  public:
    CU8gDisplay();
    CU8gDisplay(byte clkPin, byte mosiPin, byte csPin, byte dcPin);
    ~CU8gDisplay();
    void initialize();
    void print(const char* txt, ...);
  
  //Variables
  private:
    U8GLIB_SSD1306_128X64 *pu8g;
    
    int lineOffset;
};

#endif /* _U8G_DISPLAY_ */
