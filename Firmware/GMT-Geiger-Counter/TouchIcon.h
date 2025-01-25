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
    TouchIcon(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Image icon);

    void update(XPT2046::Point position); // Update the touch icon with the touch position
    void draw(GFXcanvas16 &canvas);       // Draw the touch icon to the frame buffer
    void (*action)();                     // Function pointer to execute a function on touchdown

  //-----------------------------------------------------------------------------------------------
  // Private

  private:

    uint16_t _x;      // X position
    uint16_t _y;      // Y position
    uint16_t _width;  // Touch icon width
    uint16_t _height; // Touch icon height
    Image    _icon;   // Touch icon graphic

};

#endif