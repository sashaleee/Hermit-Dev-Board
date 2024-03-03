#include "DumbLed.h"
#include <Arduino.h>
DumbLed::DumbLed(uint8_t pin) {
  _ledPin = pin;
  pinMode(_ledPin, OUTPUT);
  digitalWrite(_ledPin, LOW);
}

void DumbLed::update() {
  if (millis() > _blinkTime + _blinkLength) {
    digitalWrite(_ledPin, inverted);
  }
}

void DumbLed::blink() {
  _blinkTime = millis();
  digitalWrite(_ledPin, !inverted);
  _state = 0;
}

void DumbLed::setBlinkOnTime(uint16_t ontime) { _blinkLength = ontime; }