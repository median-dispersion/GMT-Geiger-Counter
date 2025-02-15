#ifndef _SCREEN_TRUE_RNG_H
#define _SCREEN_TRUE_RNG_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "ScreenBasic.h"
#include "DisplayToastBox.h"

class ScreenTrueRNG: public ScreenBasic {

  //-----------------------------------------------------------------------------------------------
  // Public

  public:

    // Temporary placeholder
    DisplayToastBox placeholder;

    // Constructor
    ScreenTrueRNG();

    // Update and draw calls
    void update(const XPT2046::Point &position) override;
    void draw(GFXcanvas16 &canvas) override;

};

#endif