/*

### HERMIT BOARD ###
 This example showcases a minimal Raspberry Pi Pico project that is capable of
outputting stereo audio using the Mozzi library and has MIDI I/O connectivity
via a USB port. It is also possible to utilize hardware MIDI I/O and UART (refer
to config.h for pin setup) and to read and write to the EEPROM (see the
Raspberry Pi Pico Arduino core documentation
https://arduino-pico.readthedocs.io/en/latest/eeprom.html).

Setup instructions:
1. Install VS Code and PlatformIO

2. Get Mozzi-1.1.1 library from
https://github.com/sensorium/Mozzi/releases/tag/1.1.1 and put it in lib folder.

3. In AudioConfigRP2040.h (/lib/Mozzi-1.1.1):
  #define RP2040_AUDIO_OUT_MODE PWM_VIA_BARE_CHIP
  #define AUDIO_CHANNEL_1_PIN 2
  #define AUDIO_CHANNEL_2_PIN 3

4. In mozzi_config.h (/lib/Mozzi-1.1.1):
// #define AUDIO_CHANNELS MONO
#define AUDIO_CHANNELS STEREO

 Note:
 This project uses this Raspberry Pi Pico Arduino core
https://github.com/earlephilhower/arduino-pico

*/

#define CONTROL_RATE 256
#include "includes.h"

DumbLed ledA(LED_B_PIN);
DumbLed ledB(LED_A_PIN);
Button buttons[BUTTONS_NUM];
Potentiometer pots[POTS_NUM];
Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, USB_MIDI);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, TRS_MIDI);
#if (DEBUGGING == ON)
SerialPIO UART(SERIAL_TX_PIN, SERIAL_RX_PIN, 128);
#endif

int16_t rateA;
int16_t rateB;
int16_t depthB;
int16_t rateC;
int16_t depthC;
int16_t balanceA;
int16_t balanceB;

uint8_t page;

Oscil<SIN8192_NUM_CELLS, AUDIO_RATE> operatorA(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, AUDIO_RATE> operatorB(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, AUDIO_RATE> operatorC(SIN8192_DATA);

////// MIDI CLOCK, TRANSPORT, CC messages //////
void handleStart() {}
void handleStop() {}
void handleClock() {}
void midiCCchange(uint8_t channel, uint8_t CCnumber, uint8_t CCvalue) {
  switch (CCnumber) {
  case 1:
    break;
  }
}

void setup() {
  ////// HARDWARE SETUP //////
  pinMode(23, OUTPUT);      // FOR CLEANER POWER
  digitalWrite(23, HIGH);   // LOW - (PFM), HIGH - improved ripple (PWM)
  analogReadResolution(12); // 12
  Serial1.setRX(MIDI_RX_PIN);
  Serial1.setTX(MIDI_TX_PIN);
  ////// MIDI SETUP //////
  USB_MIDI.setHandleControlChange(midiCCchange);
  USB_MIDI.setHandleClock(handleClock);
  USB_MIDI.setHandleStart(handleStart);
  USB_MIDI.setHandleStop(handleStop);
  USB_MIDI.begin(MIDI_CHANNEL_OMNI);
  TRS_MIDI.begin(MIDI_CHANNEL_OMNI);
  USB_MIDI.turnThruOff();
  TRS_MIDI.turnThruOff();
  ////// BUTTONS & POTS //////
  for (uint8_t i = 0; i < BUTTONS_NUM; i++) {
    buttons[i].begin(BUTTON_PINS[i]);
    buttons[i].update();
  }
  for (int8_t i = 0; i < POTS_NUM; i++) {
    pots[i].begin(POTS_PINS[i]);
  }
  ////// LED SETUP //////
  ledA.inverted = true;
  ////// EEPROM //////
  EEPROM.begin(1024);
  ////// MOZZI //////
  startMozzi(CONTROL_RATE);
}

void updateControl() {
  ////// BUTTONS //////
  for (uint8_t i = 0; i < BUTTONS_NUM; i++) {
    if (buttons[i].update()) {
      uint8_t state = buttons[i].getState();
      ////// SEND CC MIDI MESSAGES //////
      if (state == Button::PRESS) {
        USB_MIDI.sendControlChange(buttonsCC[i], 127, USB_MIDI_CHANNEL);
      } else if (state == Button::RELEASE || state == Button::HOLD_RELEASE) {
        USB_MIDI.sendControlChange(buttonsCC[i], 0, USB_MIDI_CHANNEL);
      }
      ////// DO STUFF //////
      switch (i) {
      case BUTTON_A:
        if (state == Button::PRESS) {
          ////// TOGGLE PAGE //////
          page = !page;
          ledB.inverted = !ledB.inverted;
        } else if (state == Button::HOLD_RELEASE) {
        }
        break;
      case BUTTON_B:
        if (state == Button::PRESS) {
          ////// RANDOMIZE PAGE A //////
          rateA = mtof((int)map(random(128), 0, 127, 26, 120)); // 26..90
          rateB = map(random(128), 0, 127, 0, 255);             // 0..255
          depthB = random(256);                                 // 0..127
          ledA.blink();
        }
        break;
      case BUTTON_C:
        if (state == Button::PRESS) {
          ////// RANDOMIZE PAGE B //////
          rateC = map(random(128), 0, 127, 0, 16);   // 0..63
          depthC = map(random(128), 0, 127, 0, 31);  // 0..31
          balanceA = map(random(128), 0, 127, 0, 8); // 0..3
          balanceB = 8 - balanceA;
          ledA.blink();
        }
        break;
      case BUTTON_D:
        break;
      }
    }
  }
  ////// POTS //////
  for (int8_t i = 0; i < POTS_NUM; i++) {
    if (pots[i].update()) {
      uint8_t CCvalue = pots[i].getValue();
      USB_MIDI.sendControlChange(potsCC[i], CCvalue, USB_MIDI_CHANNEL);
      switch (i) {
      case 0:
        if (page == 0) {
          rateA = mtof((int)map(CCvalue, 0, 127, 26, 120)); // 26..90
        } else {
          rateC = map(CCvalue, 0, 127, 0, 16); // 0..63
        }
        break;
      case 1:
        if (page == 0) {
          rateB = map(CCvalue, 0, 127, 0, 255); // 0..255
        } else {
          depthC = map(CCvalue, 0, 127, 0, 31); // 0..31
        }
        break;
      case 2:
        if (page == 0) {
          depthB = map(CCvalue, 0, 127, 0, 255); // 0..127
        } else {
          balanceA = map(CCvalue, 0, 127, 0, 8); // 0..3
          balanceB = 8 - balanceA;               // 3..0
        }
        break;
      }
    }
  }
  ////// LED //////
  ledA.update();
  ledB.update();
  ////// UPDATE FREQUENCIES //////
  operatorA.setFreq(rateA);
  operatorB.setFreq(rateB);
  operatorC.setFreq(rateC);
}
////// AUDIO OUTPUT //////
AudioOutput_t updateAudio() {
  int16_t l;
  int16_t r;
  operatorB.setFreq(rateB + (operatorC.next() * depthC * balanceB));
  operatorA.setFreq(rateA + (operatorB.next() * depthB) +
                        (operatorC.next() * depthC * balanceA) >>
                    2);
  l = operatorA.next();
  r = l;
  // return StereoOutput::fromAlmostNBit(AUDIO_BITS, l, r);
  return StereoOutput::from8Bit(l, r);
}

void loop() { audioHook(); }

////// SECOND CORE //////
void setup1() {
  ////// UART //////
  UART.begin(115200);
  UART.println("OK");
}
void loop1() { USB_MIDI.read(); }