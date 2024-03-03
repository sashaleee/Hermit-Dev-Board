#include "Potentiometer.h"

void Potentiometer::begin(uint8_t pin_) {
  pin = pin_;
  analog = ResponsiveAnalogRead(pin, true);
  analog.setAnalogResolution(4096); // for rpi pico
  analog.setActivityThreshold(32);
  reading = 6000;
  lastReading = 7000;
}

bool Potentiometer::update() {
  analog.update();
  reading = analog.getValue() >> 5;
  if (analog.hasChanged()) {
    if (lastReading != reading) {
      lastReading = reading;
      return true;
    }
  }
  return false;
}

bool Potentiometer::update16Pos() {
  const uint16_t thresholds[] = {30,   310,  550,  830,  1090, 1350,
                                 1630, 1960, 2220, 2536, 2910, 3150,
                                 3470, 3780, 4060, 4095};
  uint16_t rawReading = analog.getRawValue();
  for (uint8_t i = 0; i < sizeof(thresholds) / sizeof(thresholds[0]); i++) {
    if (rawReading <= thresholds[i]) {
      reading16pos = i + 1;
      break;
    }
  }
  if (reading16pos != last16posReading) {
    last16posReading = reading16pos;
    return true;
  }
  return false;
}
uint8_t Potentiometer::get16Pos() { return reading16pos; }

bool Potentiometer::updatePos(uint8_t div) {
  uint16_t rawReading = analog.getRawValue();
  for (uint8_t i = 0; i < (div); i++) {
    if (rawReading <= (i * (4095 / div)) + (4095 / div)) {
      readingPos = i;
      break;
    }
  }
  if (readingPos != lastPosReading) {
    lastPosReading = readingPos;
    return true;
  }
  return false;
}
uint8_t Potentiometer::getPos() { return readingPos; }

uint8_t Potentiometer::getValue() { return reading; }
