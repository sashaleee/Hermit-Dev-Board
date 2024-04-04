### Hermit Board ###
 This example showcases a minimal Raspberry Pi Pico project that is capable of outputting stereo audio using the Mozzi library and has MIDI I/O connectivity via a USB port. It is also possible to utilize hardware MIDI I/O and UART (refer to config.h for pin setup) and to read and write to the EEPROM (see the Raspberry Pi Pico Arduino core documentation https://arduino-pico.readthedocs.io/en/latest/eeprom.html).

Setup instructions:
1. Install VS Code and PlatformIO

2. Get Mozzi-1.1.1 library from
https://github.com/sensorium/Mozzi/releases/tag/1.1.1 and put it in lib folder.

3. In AudioConfigRP2040.h (/lib/Mozzi-1.1.1):
  ```
  #define RP2040_AUDIO_OUT_MODE PWM_VIA_BARE_CHIP
  #define AUDIO_CHANNEL_1_PIN 2
  #define AUDIO_CHANNEL_2_PIN 3
  ```

4. In mozzi_config.h (/lib/Mozzi-1.1.1):
```
// #define AUDIO_CHANNELS MONO
#define AUDIO_CHANNELS STEREO
 ```

 Note:
 This project uses this Raspberry Pi Pico Arduino core https://github.com/earlephilhower/arduino-pico

### Schematic (without MIDI I/O and UART) ###
![schematic](https://github.com/sashaleee/Hermit-Dev-Board/blob/main/Breadboard%20Hermit_2024-04-03.png)
