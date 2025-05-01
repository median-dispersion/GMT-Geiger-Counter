#ifndef _TOUCH_ICON_H
#define _TOUCH_ICON_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Graphics.h"

class TouchIcon {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Constructor
    TouchIcon(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, const Image &icon);

    void update(const XPT2046::Point &position); // Update the touch icon with the touch position
    void draw(GFXcanvas16 &canvas);              // Draw the touch icon to the frame buffer
    void (*action)();                            // Function pointer to execute a function on touchdown

  //-----------------------------------------------------------------------------------------------
  // Private

  private:

    const uint16_t _x;      // X position
    const uint16_t _y;      // Y position
    const uint16_t _width;  // Touch icon width
    const uint16_t _height; // Touch icon height
    const Image    &_icon;  // Touch icon graphic

};

#endif