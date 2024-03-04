### HERMIT DEV BOARD V1.0 ###
 Example project

Setup instructions:
0. Install VS Code and PlatformIO

1. Get Mozzi-1.1.1 library from
https://github.com/sensorium/Mozzi/releases/tag/1.1.1 and put it in lib folder.

2. In AudioConfigRP2040.h (/lib/Mozzi-1.1.1):
  #define RP2040_AUDIO_OUT_MODE PWM_VIA_BARE_CHIP
  #define AUDIO_CHANNEL_1_PIN 2
  #define AUDIO_CHANNEL_2_PIN 3

3. In mozzi_config.h (/lib/Mozzi-1.1.1):
 #define AUDIO_CHANNELS STEREO