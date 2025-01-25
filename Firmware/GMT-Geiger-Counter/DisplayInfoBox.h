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
    DisplayInfoBox(uint16_t x, uint16_t y, uint16_t width, Image icon, String value);

    void draw(GFXcanvas16 &canvas); // Draw the info box
    void setValue(String value);    // Set the info box value

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    uint16_t _x;     // X position
    uint16_t _y;     // Y position
    uint16_t _width; // Info box width
    Image    _icon;  // Info box icon
    String   _value; // Info box text value

};

#endif