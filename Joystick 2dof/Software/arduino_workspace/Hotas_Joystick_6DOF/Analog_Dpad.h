#ifndef _ANALOG_DPAD_
#define _ANALOG_DPAD_

#include "Config_Joystick_6Dof.h"
#include "Typedef_Joystick_6Dof.h"

#include "U8g_Display.h"

class CAnalogDpad {
  //Method
  public:
    CAnalogDpad();
    CAnalogDpad(int neutralZone, CU8gDisplay* pDisp);
    ~CAnalogDpad();
    void initialize();
    int getDPadStatus(int posX, int posY);
    
  //Variables
  private:
    CU8gDisplay* display;
    
    int neutral;
};

#endif /* _ANALOG_DPAD_ */
