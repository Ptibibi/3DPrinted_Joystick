#ifndef _HID_REPORT_
#define _HID_REPORT_

#include "Config_Joystick_6Dof.h"
#include "Typedef_Joystick_6Dof.h"

#include "U8g_Display.h"

class CHidReport {
  //Method
  public:
    CHidReport();
    CHidReport(CU8gDisplay* pDisp);
    ~CHidReport();
    void initialize();
    void sendReport(bool* pButton, int x, int y, int z, int rX, int rY, int throttleValue, byte dPadStatus);
    
  //Variables
  private:
    CU8gDisplay* display;
    
};

#endif /* _HID_REPORT_ */

