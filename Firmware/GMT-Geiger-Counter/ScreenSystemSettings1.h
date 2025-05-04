#ifndef _SCREEN_SYSTEM_SETTINGS_1_H
#define _SCREEN_SYSTEM_SETTINGS_1_H

#include "Arduino.h"
#include "Configuration.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "ScreenBasicLandscape.h"
#include "TouchToggle.h"
#include "DisplayToastBox.h"

class ScreenSystemSettings1: public ScreenBasicLandscape {

  //-----------------------------------------------------------------------------------------------
  // Public

  public:

    // Screen elements
    TouchIcon next;
    TouchIcon previous;
    TouchToggle sdCardMounted;
    TouchToggle serialLogging;
    TouchToggle sdCardLogging;
    TouchToggle dataLogging;
    TouchToggle eventLogging;
    TouchToggle systemLogging;

    // Constructor
    ScreenSystemSettings1();

    // Update and draw calls
    void update(const XPT2046::Point &position) override;
    void draw(GFXcanvas16 &canvas) override;

};

#endif