#ifndef _ANALOG_DPAD_
#define _ANALOG_DPAD_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"
#include "math.h"

#include "Config_Joystick_6Dof.h"
#include "Typedef_Joystick_6Dof.h"

#include "Display.h"

class CAnalogDpad {
  //Method
  public:
    CAnalogDpad();
    CAnalogDpad(int16_t neutralZone, CDisplay* pDisp);
    ~CAnalogDpad();
    void initialize();
    int16_t getDPadStatus(int16_t posX, int16_t posY);
    
  //Variables
  private:
    CDisplay* display;
    
    int16_t neutral;
};

#ifdef __cplusplus
}
#endif

#endif /* _ANALOG_DPAD_ */
