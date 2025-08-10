#ifndef SCREEN_DISABLE_GEIGER_COUNTER_H
#define SCREEN_DISABLE_GEIGER_COUNTER_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "Graphics.h"
#include "ScreenBasicLandscape.h"
#include "TouchButton.h"

class ScreenDisableGeigerCounter: public ScreenBasicLandscape {

  //-----------------------------------------------------------------------------------------------
  // Public

  public:

    // Constructor
    ScreenDisableGeigerCounter();

    // Screen elements
    TouchButton confirm;
    TouchButton dismiss;

    // Update and draw calls
    void update(const XPT2046::Point &position) override;
    void draw(GFXcanvas16 &canvas) override;

};

#endif