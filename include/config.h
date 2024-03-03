#ifndef CONFIG_H_
#define CONFIG_H_
#include <Arduino.h>

#define ON 1
#define OFF 0

#define DEBUGGING ON
#define DEFAULT_SETTING OFF

////// CONSTANTS //////
enum ButtonNames { BUTTON_A, BUTTON_B, BUTTON_C, BUTTON_D, BUTTONS_NUM };
enum PotNames { POT_A, POT_B, POT_C, POTS_NUM };
const uint8_t USB_MIDI_CHANNEL = 1;
const uint8_t TRS_MIDI_CHANNEL = 1;
const uint8_t buttonsCC[BUTTONS_NUM] = {100, 101, 102, 103};
const uint8_t potsCC[POTS_NUM] = {40, 41, 42};

////// PINS CONFIG //////
// AUDIO OUT PINS: 2, 3
const uint8_t BUTTON_PINS[BUTTONS_NUM] = {15, 14, 11, 18};
const uint8_t POTS_PINS[POTS_NUM] = {A2, A1, A0};
const uint8_t LED_A_PIN = 8;
const uint8_t LED_B_PIN = 25;
const uint8_t MIDI_TX_PIN = 12; // MIDI out pin
const uint8_t MIDI_RX_PIN = 13; // MIDI in pin
const uint8_t SERIAL_TX_PIN = 17;
const uint8_t SERIAL_RX_PIN = 16;
const uint8_t SYNC_PIN = 19;

// const uint8_t SAMPLES_NUM = 8;
// const uint8_t NUM_TABLES = 28;
// const uint16_t SAMPLE_LENGTH = 22050; //
// const uint16_t SAMPLE_RATE = 32768;
// const float SAMPLE_PITCH = (float)SAMPLE_RATE / SAMPLE_LENGTH;

#endif