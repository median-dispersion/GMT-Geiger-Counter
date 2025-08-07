#ifndef _SCREEN_SYSTEM_SETTINGS_3_H
#define _SCREEN_SYSTEM_SETTINGS_3_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "Graphics.h"
#include "ScreenBasicLandscape.h"
#include "TouchIcon.h"

class ScreenSystemSettings3: public ScreenBasicLandscape {

  //-----------------------------------------------------------------------------------------------
  // Public

  public:

    // Screen elements
    TouchIcon next;
    TouchIcon previous;
    TouchIcon restart;
    TouchIcon reset;

    // Constructor
    ScreenSystemSettings3();

    // Update and draw calls
    void update(const XPT2046::Point &position) override;
    void draw(GFXcanvas16 &canvas) override;

};

#endif