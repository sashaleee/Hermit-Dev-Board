#ifndef CONFIG_H_
#define CONFIG_H_
#include <Arduino.h>

////// CONSTANTS //////
enum ButtonNames { BUTTON_A, BUTTON_B, BUTTON_C, BUTTON_D, BUTTONS_NUM };
enum PotNames { POT_A, POT_B, POT_C, POTS_NUM };
const uint8_t USB_MIDI_CHANNEL = 1;
const uint8_t TRS_MIDI_CHANNEL = 1;
const uint8_t buttonsCC[BUTTONS_NUM] = {100, 101, 102, 103};
const uint8_t potsCC[POTS_NUM] = {40, 41, 42};

////// PINS CONFIG //////
const uint8_t BUTTON_PINS[BUTTONS_NUM] = {15, 14, 11, 18};
const uint8_t POTS_PINS[POTS_NUM] = {A2, A1, A0};
const uint8_t LED_A_PIN = 8;      // LED pin
const uint8_t LED_B_PIN = 25;     // Builtin LED pin
const uint8_t MIDI_TX_PIN = 12;   // MIDI out pin
const uint8_t MIDI_RX_PIN = 13;   // MIDI in pin
const uint8_t SERIAL_TX_PIN = 17; // UART TX pin
const uint8_t SERIAL_RX_PIN = 16; // UART RX pin
#endif