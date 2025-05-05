#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "Arduino.h"

// Fonts
#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSansBold9pt7b.h"
#include "Fonts/FreeSansBold24pt7b.h"

// Color definitions
#define COLOR_BLACK         0x0000
#define COLOR_WHITE         0xFFFF
#define COLOR_GRAY_DARKEST  0x1083
#define COLOR_GRAY_DARK     0x2946
#define COLOR_GRAY_MEDIUM   0x5acc
#define COLOR_GRAY_LIGHT    0x8c52
#define COLOR_RED_DARK      0x3000
#define COLOR_RED_MEDIUM    0xb800
#define COLOR_GREEN_DARK    0x0101
#define COLOR_GREEN_MEDIUM  0x0508
#define COLOR_LIME_DARK     0x1901
#define COLOR_LIME_MEDIUM   0x6502
#define COLOR_BLUE_DARK     0x00c7
#define COLOR_BLUE_MEDIUM   0x041f
#define COLOR_YELLOW_DARK   0x3940
#define COLOR_YELLOW_MEDIUM 0xdd00
#define COLOR_PURPLE_DARK   0x1806
#define COLOR_PURPLE_MEDIUM 0x801f
#define COLOR_NEON          0x0ff0

// Image structure
struct Image {

  const uint16_t width;
  const uint16_t height;
  const uint16_t *data;

};

// Image data
extern Image IMAGE_AVERAGE;
extern Image IMAGE_BACK;
extern Image IMAGE_CLOCK;
extern Image IMAGE_DECREASE;
extern Image IMAGE_DIAGRAM;
extern Image IMAGE_DICE;
extern Image IMAGE_DISPLAY;
extern Image IMAGE_HISTORY;
extern Image IMAGE_HOTSPOT;
extern Image IMAGE_INCREASE;
extern Image IMAGE_MAXIMUM;
extern Image IMAGE_MINIMUM;
extern Image IMAGE_MUON;
extern Image IMAGE_MUON_SMALL;
extern Image IMAGE_MUTE;
extern Image IMAGE_NEXT;
extern Image IMAGE_PARTICLE;
extern Image IMAGE_QUESTION;
extern Image IMAGE_RADIATION;
extern Image IMAGE_RESET;
extern Image IMAGE_ROTATE;
extern Image IMAGE_SETTINGS;
extern Image IMAGE_SLEEP;
extern Image IMAGE_SLEEP_SMALL;
extern Image IMAGE_SUM;
extern Image IMAGE_VOLUME;
extern Image IMAGE_WIFI;

#endif