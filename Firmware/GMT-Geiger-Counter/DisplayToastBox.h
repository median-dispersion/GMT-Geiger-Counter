#ifndef _DISPLAY_TOAST_BOX_H
#define _DISPLAY_TOAST_BOX_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "Graphics.h"

class DisplayToastBox {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Constructor
    DisplayToastBox(const uint16_t x, const uint16_t y, const uint16_t width, const char *title, const char *value);

    void draw(GFXcanvas16 &canvas);   // Draw the toast box
    void setValue(const char *value); // Set the toast box value

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    const uint16_t _x;      // X position
    const uint16_t _y;      // Y position
    const uint16_t _width;  // Toast box width
    const char     *_title; // Toast box title
    const char     *_value; // Toast box text value

};

#endif