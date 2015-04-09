#include "HID_Report.h"
  
CHidReport::CHidReport() {
}

CHidReport::~CHidReport() {
}

void CHidReport::initialize() {
  Hotas.begin();
}

void CHidReport::sendReport(bool* pButton, int rx, int ry, int rz, int x, int y, int throttleValue) {
  int i;
  for (i = 0; i < BUTTON_MATRIX_NB; i++)
  {
    if (*(pButton + i) == true)
      Hotas.press(i + 1);
    else
      Hotas.release(i + 1);
  }
  
  Hotas.rxAxis(rx);
  Hotas.ryAxis(ry);
  Hotas.rzAxis(rz);
  Hotas.xAxis(x);
  Hotas.yAxis(y);
  
  Hotas.throttle(throttleValue);
  
  Hotas.dPad1(0);
  Hotas.dPad2(0);
  
  Hotas.write();
}

