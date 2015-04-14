#ifndef _ANALOG_DPAD_
#define _ANALOG_DPAD_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"
#include "math.h"
#include "diag/Trace.h"

#include "Timer.h"

#include "../src/Config_Joystick_6Dof.h"
#include "../src/Typedef_Joystick_6Dof.h"

class CAnalogDpad {
  //Method
  public:
    CAnalogDpad(int16_t neutralZone);
    ~CAnalogDpad();
    void initialize();
    int16_t getDPadStatus(int16_t posX, int16_t posY);
    
  //Variables
  private:
    Timer timer;
    int16_t neutral;
};

#ifdef __cplusplus
}
#endif

#endif /* _ANALOG_DPAD_ */
