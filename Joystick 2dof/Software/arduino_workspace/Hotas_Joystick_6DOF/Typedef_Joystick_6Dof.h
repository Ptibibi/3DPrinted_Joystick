#ifndef _TYPEDEF_JOYSTICK_6DOF_
#define _TYPEDEF_JOYSTICK_6DOF_

#include "arduino.h"

typedef union {
  byte port;
  struct {
    byte na1 : 1;
    byte R : 5;
    byte na2 : 2;
  };
} rgb555_;

typedef struct {
  int led[3][4];
  bool newData;
} ledTracking_;

typedef struct {
  int X;
  int Y;
} coord_;

typedef struct {
  byte siocPin;
  byte siodPin;
  byte xclkPin;
  byte pclkPin;
  byte hrefPin;
  byte vSynchPin;
  byte pwdmPin;
  byte resetPin;
  byte d0Pin;
  byte d1Pin;
  byte d2Pin;
  byte d3Pin;
  byte d4Pin;
  byte d5Pin;
  byte d6Pin;
  byte d7Pin;
  byte d8Pin;
} ov7670Pin_;

#endif /* _TYPEDEF_JOYSTICK_6DOF_ */

