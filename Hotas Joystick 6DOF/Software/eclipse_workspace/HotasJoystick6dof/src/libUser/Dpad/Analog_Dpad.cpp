#include "Analog_Dpad.h"

CAnalogDpad::CAnalogDpad(int16_t neutralZone) {
  neutral = neutralZone;

  timer.start();
}

CAnalogDpad::~CAnalogDpad() {
}

void CAnalogDpad::initialize() {
}

int16_t CAnalogDpad::getDPadStatus(int16_t posX, int16_t posY) {
  int16_t dist = (int16_t)sqrt(pow(posX, 2) + pow(posY, 2));
  int16_t angle = (int16_t)(180 / M_PI * atan2(posX, posY));
  
  if (dist < neutral)
    return DPAD_CENTERED;
    
  else if ((-15 <= angle) && (angle <= 15))
    return DPAD_UP;
  else if ((-165 >= angle) || (angle >= 165))
    return DPAD_DOWN;
    
  else if ((75 <= angle) && (angle <= 105))
    return DPAD_RIGHT;
  else if ((-105 <= angle) && (angle <= -75))
    return DPAD_LEFT;
    
  else if ((0 < angle) && (angle < 90))
    return DPAD_UP_RIGHT;
  else if ((90 < angle) && (angle < 180))
    return DPAD_DOWN_RIGHT;
    
  else if ((-180 < angle) && (angle < -90))
    return DPAD_DOWN_LEFT;
  else if ((-90 < angle) && (angle < 0))
    return DPAD_UP_LEFT;

  return DPAD_CENTERED;
}
