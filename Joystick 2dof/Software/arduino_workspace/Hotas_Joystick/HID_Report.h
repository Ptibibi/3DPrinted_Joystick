/*******************************************************************************
HID_Report.h
V 0.02

Description:

********************************************************************************
********************************************************************************
V0.02 --> 12/04/15	Edition originale by BRB
*******************************************************************************/
#ifndef _HID_REPORT_
#define _HID_REPORT_

#include "arduino.h"
#include "Config_Joystick.h"

class CHidReport {
  //Method
  public:
    CHidReport();
    ~CHidReport();
    void initialize();
    void sendReport(bool* pButton, int rx, int ry, int rz, int x, int y, int throttleValue);
    
  //Variables
  private:
    
};

#endif /* _HID_REPORT_ */

