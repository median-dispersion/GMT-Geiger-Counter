#ifndef _TOUCH_TARGET_H
#define _TOUCH_TARGET_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"

class TouchTarget {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Constructor
    TouchTarget(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height);

    void update(const XPT2046::Point position); // Update the touch target with the touch position
    void (*action)();                           // Function pointer to execute a function on touchdown

  //-----------------------------------------------------------------------------------------------
  // Private

  private:

    const uint16_t _x;      // X position
    const uint16_t _y;      // Y position
    const uint16_t _width;  // Touch target width
    const uint16_t _height; // Touch target height

};

#endif