#ifndef button_h
#define button_h
#define MASK 0b11000111

#include "Arduino.h"

class Button {
private:
  uint8_t _pin;
  uint8_t _history;
  uint8_t _state;
  uint8_t _clickCounter;
  uint32_t _pressTime;
  uint16_t _holdTime;
  const uint16_t DOUBLE_PRESS_TIME = 600;

public:
  enum ButtonStates { RELEASE, PRESS, HOLD, HOLD_RELEASE, DOUBLE_PRESS };
  void begin(uint8_t pin_);
  void setHoldTime(uint16_t time_);
  bool update();
  uint8_t getState();
};

#endif