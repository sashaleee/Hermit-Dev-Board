#include "Button.h"

void Button::begin(uint8_t pin_) {
  _holdTime = 300;
  _pin = pin_;
  _history = 0b11111111; // 0
  pinMode(_pin, INPUT_PULLUP);
}

bool Button::update() {
  _history = (_history << 1);
  _history |= digitalRead(_pin);
  if ((_history & MASK) == 0b11000000) { // PRESS
    _pressTime = millis();
    _history = 0b00000000;
    _state = PRESS;
    return true;
  } else if (_history == 0b00000000 && _state == PRESS &&
             millis() > _pressTime + _holdTime) { // HOLD
    _state = HOLD;
    return true;
  } else if ((_history & MASK) == 0b00000111) { // RELEASE
    _history = 0b11111111;
    if (_state == HOLD) {
      _state = HOLD_RELEASE;
    } else {
      _state = RELEASE;
    }
    return true;
  }
  return false;
}
uint8_t Button::getState() { return _state; }
void Button::setHoldTime(uint16_t time_) { _holdTime = time_; }