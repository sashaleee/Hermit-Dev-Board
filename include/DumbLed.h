#ifndef dumbled_h
#define dumbled_h

#include "Arduino.h"

class DumbLed {
private:
  enum LedStates { LED_OFF, LED_ON, LED_BLINK };
  uint8_t _state;
  uint64_t _blinkTime;
  uint16_t _blinkLength = 50;
  uint8_t _ledPin;

public:
  bool inverted;
  DumbLed(uint8_t pin);
  void blink();
  void update();
  void setBlinkOnTime(uint16_t ontime);
};

#endif