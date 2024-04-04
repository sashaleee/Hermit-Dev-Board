### HERMIT DEV BOARD V1.0 ###
 Example project

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
