#include "HID_Report.h"
  
CHidReport::CHidReport() {
}

CHidReport::CHidReport(CU8gDisplay* pDisp) {
  display = pDisp;
}

CHidReport::~CHidReport() {
}

void CHidReport::initialize() {
  Hotas.begin();
}

void CHidReport::sendReport(bool* pButton, int x, int y, int z, int rX, int rY, int throttleValue, byte dPadStatus) {
  int i;
  for (i = 0; i < BUTTON_GAME_NB; i++)
  {
    if (*(pButton + i) == true)
      Hotas.press(i + 1);
    else
      Hotas.release(i + 1);
  }
  
  Hotas.xAxis(x);
  Hotas.yAxis(y);
  Hotas.zAxis(z);
  Hotas.rxAxis(rX);
  Hotas.ryAxis(rY);
  Hotas.xAxis2(0);
  Hotas.yAxis2(0);
  
  Hotas.throttle(throttleValue);
  
  Hotas.dPad1(dPad1);
  Hotas.dPad2(dPad2);
  
  Hotas.write();
}

