#include "Button.h"
#include "DumbLed.h"
#include "Potentiometer.h"
#include "config.h"
#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <MIDI.h>
#include <MozziGuts.h>
#include <Oscil.h>
#include <Sample.h>
#include <mozzi_midi.h>

#include <tables/saw4096_int8.h>
#include <tables/sin4096_int8.h>
#include <tables/sin8192_int8.h>
