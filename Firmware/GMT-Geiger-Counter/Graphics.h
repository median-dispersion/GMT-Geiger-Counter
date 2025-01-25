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
#define COLOR_DARK_GRAY     0x2946
#define COLOR_MEDIUM_GRAY   0x52ac
#define COLOR_LIGHT_GRAY    0x83f3
#define COLOR_DARK_GREEN    0x00c0
#define COLOR_MEDIUM_GREEN  0x07e0
#define COLOR_DARK_ORANGE   0x30c0
#define COLOR_MEDIUM_ORANGE 0xfc00
#define COLOR_DARK_RED      0x2800
#define COLOR_MEDIUM_RED    0xf800

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
extern Image IMAGE_NEXT;
extern Image IMAGE_PARTICLE;
extern Image IMAGE_RADIATION;
extern Image IMAGE_RESET;
extern Image IMAGE_SETTINGS;
extern Image IMAGE_SLEEP;
extern Image IMAGE_SLEEP_SMALL;
extern Image IMAGE_VOLUME;
extern Image IMAGE_WIFI;

#endif