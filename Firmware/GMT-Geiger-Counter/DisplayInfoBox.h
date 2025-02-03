#ifndef _DISPLAY_INFO_BOX_H
#define _DISPLAY_INFO_BOX_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "Graphics.h"

class DisplayInfoBox {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Constructor
    DisplayInfoBox(const uint16_t x, const uint16_t y, const uint16_t width, const Image icon, const char *value);

    void draw(GFXcanvas16 &canvas);   // Draw the info box
    void setValue(const char *value); // Set the info box value

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    const uint16_t _x;      // X position
    const uint16_t _y;      // Y position
    const uint16_t _width;  // Info box width
    const Image    _icon;   // Info box icon
    const char     *_value; // Info box text value

};

#endif