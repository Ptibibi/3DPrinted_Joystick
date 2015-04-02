/*
Copyright (c) 2014 NicoHood
See the readme for credit to other people.

Hotas example

Press a button and demonstrate Gamepad actions
*/

const int pinLed = LED_BUILTIN;
const int pinButton = 2;
int pos = 0;

void setup() {
  pinMode(pinLed, OUTPUT);
  pinMode(pinButton, INPUT_PULLUP);

  // Sends a clean report to the host. This is important on any Arduino type.
  // Make sure all desired USB functions are activated in USBAPI.h!
  Hotas.begin();
}

void loop() {
  //if (!digitalRead(pinButton)) {
  if (true) {
    digitalWrite(pinLed, HIGH);

    // press button 1-32 and reset (34 becaue its written later)
    static uint8_t count = 1;
    Hotas.press(count++);
    if (count == 34) {
      Hotas.releaseAll();
      count = 1;
    }

    // move x/y Axis to a new position (16bit)
    if (pos < 200)
      pos++;
    else
      pos = 0;
    Hotas.xAxis(random(0xFFFF));
    Hotas.yAxis(random(0xFFFF));
    Hotas.zAxis(random(0xFFFF));
    Hotas.rxAxis(random(0xFFFF));
    Hotas.ryAxis(random(0xFFFF));
    Hotas.rzAxis(random(0xFFFF));
    Hotas.throttleLeft(random(0xFFFF));
    Hotas.throttleRight(random(0xFFFF));

    // go through all dPad positions
    // values: 0-8 (0==centred)
    static uint8_t dpad1 = HOTAS_DPAD_CENTERED;
    Hotas.dPad1(dpad1++);
    if(dpad1>HOTAS_DPAD_UP_LEFT) dpad1 = HOTAS_DPAD_CENTERED;
    static int8_t dpad2 = HOTAS_DPAD_CENTERED;
    Hotas.dPad2(dpad2--);
    if(dpad2<HOTAS_DPAD_CENTERED) dpad2 = HOTAS_DPAD_UP_LEFT;


    // functions before only set the values
    // this writes the report to the host
    Hotas.write();

    // simple debounce
    delay(50);
    digitalWrite(pinLed, LOW);
  }
}

/*
Prototypes:

void begin(void);
void end(void);
void write(void);
void press(uint8_t b);
void release(uint8_t b);
void releaseAll(void);
void buttons(uint32_t b);
void xAxis(int16_t a);
void yAxis(int16_t a);
void rxAxis(int16_t a);
void ryAxis(int16_t a);
void zAxis(int8_t a);
void rzAxis(int8_t a);
void dPad1(int8_t d);
void dPad2(int8_t d);

Definitions:
GAMEPAD_DPAD_CENTERED 0
GAMEPAD_DPAD_UP 1
GAMEPAD_DPAD_UP_RIGHT 2
GAMEPAD_DPAD_RIGHT 3
GAMEPAD_DPAD_DOWN_RIGHT 4
GAMEPAD_DPAD_DOWN 5
GAMEPAD_DPAD_DOWN_LEFT 6
GAMEPAD_DPAD_LEFT 7
GAMEPAD_DPAD_UP_LEFT 8
*/
