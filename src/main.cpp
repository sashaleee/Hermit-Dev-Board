/*
### HERMIT DEV BOARD V1.0 ###
 Example project

Setup instructions:
0. Install VS Code and PlatformIO

1. Put Mozzi-1.1.1 in lib folder

2. In AudioConfigRP2040.h:
  #define RP2040_AUDIO_OUT_MODE PWM_VIA_BARE_CHIP
  #define AUDIO_CHANNEL_1_PIN 2
  #define AUDIO_CHANNEL_2_PIN 3

3. In mozzi_config.h:
 #define AUDIO_CHANNELS STEREO

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

int aRate;
int bRate;
int bDepth;
int cRate;
int cDepth;
int balanceA;
int balanceB;

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
  for (int i = 0; i < POTS_NUM; i++) {
    pots[i].begin(POTS_PINS[i]);
  }
  ////// LED SETUP //////
  ledA.inverted = true;
  ////// EEPROM //////
  EEPROM.begin(1024);
  ////// MOZZI //////
  startMozzi(CONTROL_RATE);
  ////// UART //////
#if (DEBUGGING == ON)
  UART.begin(115200);
  UART.println("OK");
#endif
}

void updateControl() {
  ////// BUTTONS //////
  for (uint8_t i = 0; i < BUTTONS_NUM; i++) {
    if (buttons[i].update()) {
      uint8_t state = buttons[i].getState();
      ////// SEND CC MIDI MESSAGES //////
      if (state == Button::PRESS) {
        USB_MIDI.sendControlChange(buttonsCC[i], 127, USB_MIDI_CHANNEL);
        TRS_MIDI.sendControlChange(buttonsCC[i], 127, USB_MIDI_CHANNEL);
      } else if (state == Button::RELEASE || state == Button::HOLD_RELEASE) {
        USB_MIDI.sendControlChange(buttonsCC[i], 0, USB_MIDI_CHANNEL);
        TRS_MIDI.sendControlChange(buttonsCC[i], 0, USB_MIDI_CHANNEL);
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
          aRate = mtof(26 + ((int)random(128) >> 1)); // aRate
          bRate = random(128) << 1;                   // bRate
          bDepth = random(64);                        // bDepth
          ledA.blink();
        }
        break;
      case BUTTON_C:
        if (state == Button::PRESS) {
          ////// RANDOMIZE PAGE B //////
          cRate = random(128) >> 1;    // cRate
          cDepth = random(128) >> 2;   // cDepth // 1
          balanceA = random(128) >> 5; // balanceA/B
          balanceB = 3 - balanceA;
          ledA.blink();
        }
        break;
      case BUTTON_D:
        break;
      }
    }
  }
  ////// POTS //////
  for (int i = 0; i < POTS_NUM; i++) {
    if (pots[i].update()) {
      uint8_t CCvalue = pots[i].getValue();
      USB_MIDI.sendControlChange(potsCC[i], CCvalue, USB_MIDI_CHANNEL);
      TRS_MIDI.sendControlChange(potsCC[i], CCvalue, USB_MIDI_CHANNEL);
      switch (i) {
      case 0:
        if (page == 0) {
          aRate = mtof(26 + (CCvalue >> 1)); // aRate
        } else {
          cRate = CCvalue >> 1; // cRate
        }
        break;
      case 1:
        if (page == 0) {
          bRate = CCvalue << 1; // bRate
        } else {
          cDepth = CCvalue >> 2; // cDepth // 1
        }
        break;
      case 2:
        if (page == 0) {
          bDepth = CCvalue; // bDepth
        } else {
          balanceA = CCvalue >> 5; // balanceA/B
          balanceB = 3 - balanceA;
        }
        break;
      }
    }
  }
  ////// LED //////
  ledA.update();
  ledB.update();
  ////// UPDATE FREQUENCIES //////
  operatorA.setFreq(aRate);
  operatorB.setFreq(bRate);
  operatorC.setFreq(cRate);
}
AudioOutput_t updateAudio() {
  int16_t l;
  int16_t r;
  operatorB.setFreq(bRate + (operatorC.next() * cDepth * balanceB));
  operatorA.setFreq(aRate + (operatorB.next() * bDepth) +
                        (operatorC.next() * cDepth * balanceA) >>
                    1);
  l = operatorA.next();
  r = l;
  // return StereoOutput::fromAlmostNBit(AUDIO_BITS, l, r);
  return StereoOutput::from8Bit(l, r);
}

void loop() { audioHook(); }

void setup1() {}
void loop1() { USB_MIDI.read(); }