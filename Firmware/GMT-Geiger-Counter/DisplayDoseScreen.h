#ifndef _DISPLAY_DOSE_SCREEN_H
#define _DISPLAY_DOSE_SCREEN_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "Graphics.h"

class DisplayDoseScreen {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Constructor
    DisplayDoseScreen(
      const uint16_t x,
      const uint16_t y,
      const uint16_t width,
      const uint16_t height,
      const uint16_t borderColor,
      const uint16_t backgroundColor,
      const char     *value,
      const char     *unit
    );

    void draw(GFXcanvas16 &canvas);                // Draw the dose screen
    void setBorderColor(const uint16_t color);     // Set the screen border color
    void setBackgroundColor(const uint16_t color); // Set the screen background color
    void setValue(const char *value);              // Set the screen value
    void setUnit(const char *unit);                // Set the screen unit

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    const uint16_t _x;               // X position
    const uint16_t _y;               // Y position
    const uint16_t _width;           // Screen width
    const uint16_t _height;          // Screen height
    uint16_t       _borderColor;     // Screen border color
    uint16_t       _backgroundColor; // Screen background color
    const char     *_value;          // Value string
    const char     *_unit;           // Unit string

};

#endif