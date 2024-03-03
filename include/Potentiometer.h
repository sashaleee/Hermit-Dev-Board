#ifndef Potentiometer_h
#define Potentiometer_h

#include <Arduino.h>
#include <ResponsiveAnalogRead.h>

// Requires ResponsiveAnalogRead library
// lib_deps =  dxinteractive/ResponsiveAnalogRead@^1.2.1

class Potentiometer {
private:
  enum PotTypes { POS_4, POS_8, POS16, POS_17, POS_64, POS_128, POS_NUM };
  ResponsiveAnalogRead analog;
  uint8_t pin;
  int16_t reading;
  int16_t lastReading;
  uint8_t last16posReading;
  uint8_t reading16pos;
  uint8_t lastPosReading;
  uint8_t readingPos;

public:
  void begin(uint8_t pin_);
  bool update();
  uint8_t getValue();
  bool update16Pos();
  uint8_t get16Pos();
  bool updatePos(uint8_t div);
  uint8_t getPos();
};

#endif