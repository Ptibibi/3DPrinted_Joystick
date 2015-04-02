#include "Analog_Dpad.h"

CAnalogDpad::CAnalogDpad() {
}

CAnalogDpad::CAnalogDpad(int neutralZone, CU8gDisplay* pDisp) {
  display = pDisp;
  neutral = neutralZone;
}

CAnalogDpad::~CAnalogDpad() {
}

void CAnalogDpad::initialize() {
}

int CAnalogDpad::getDPadStatus(int posX, int posY) {
  int dist = (int)sqrt(pow(posX, 2) + pow(posY, 2));
  int angle = (int)(180 / PI * atan2(posX, posY));
  
  if (dist < neutral)
    return HOTAS_DPAD_CENTERED;
    
  else if ((-15 <= angle) && (angle <= 15))
    return HOTAS_DPAD_UP;
  else if ((-165 >= angle) || (angle >= 165))
    return HOTAS_DPAD_DOWN;
    
  else if ((75 <= angle) && (angle <= 105))
    return HOTAS_DPAD_RIGHT;
  else if ((-105 <= angle) && (angle <= -75))
    return HOTAS_DPAD_LEFT;
    
  else if ((0 < angle) && (angle < 90))
    return HOTAS_DPAD_UP_RIGHT;
  else if ((90 < angle) && (angle < 180))
    return HOTAS_DPAD_DOWN_RIGHT;
    
  else if ((-180 < angle) && (angle < -90))
    return HOTAS_DPAD_DOWN_LEFT;
  else if ((-90 < angle) && (angle < 0))
    return HOTAS_DPAD_UP_LEFT;
}
