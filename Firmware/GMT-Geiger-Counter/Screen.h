#ifndef _SCREEN_H
#define _SCREEN_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"

// A purely virtual base class for use in polymorphism
class Screen {

  //-----------------------------------------------------------------------------------------------
  // Public

  public:

    // Virtual update function
    virtual void update(const XPT2046::Point &position) = 0;

    // Virtual draw function
    virtual void draw(GFXcanvas16 &canvas) = 0;

    // Virtual destructor
    virtual ~Screen() = default;

};

#endif