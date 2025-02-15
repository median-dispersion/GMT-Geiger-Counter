#ifndef _SCREEN_DISPLAY_SETTING_H
#define _SCREEN_DISPLAY_SETTING_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "ScreenBasic.h"
#include "TouchToggle.h"

class ScreenDisplaySettings: public ScreenBasic {

  //-----------------------------------------------------------------------------------------------
  // Public
  
  public:

    // Screen elements
    TouchToggle display;
    TouchToggle timeout;

    // Constructor
    ScreenDisplaySettings();

    // Update and draw calls
    void update(const XPT2046::Point &position) override;
    void draw(GFXcanvas16 &canvas) override;

};

#endif