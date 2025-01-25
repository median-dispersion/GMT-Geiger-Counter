#ifndef _SCREEN_RADIATION_HISTORY_H
#define _SCREEN_RADIATION_HISTORY_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "ScreenBasic.h"
#include "DisplayToastBox.h"

class ScreenRadiationHistory: public ScreenBasic {

  //-----------------------------------------------------------------------------------------------
  // Public
  public:

    // Temporary placeholder
    DisplayToastBox placeholder;

    // Constructor
    ScreenRadiationHistory();

    // Update and draw calls
    void update(XPT2046::Point position) override;
    void draw(GFXcanvas16 &canvas) override;

};

#endif