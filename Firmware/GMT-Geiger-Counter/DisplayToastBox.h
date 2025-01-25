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
    DisplayToastBox(uint16_t x, uint16_t y, uint16_t width, String title, String value);

    void draw(GFXcanvas16 &canvas); // Draw the toast box
    void setValue(String value);    // Set the toast box value

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    uint16_t _x;     // X position
    uint16_t _y;     // Y position
    uint16_t _width; // Toast box width
    String   _title; // Toast box title
    String   _value; // Toast box text value

};

#endif