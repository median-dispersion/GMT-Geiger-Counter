#ifndef _SCREEN_SYSTEM_SETTINGS_H
#define _SCREEN_SYSTEM_SETTINGS_H

#include "Arduino.h"
#include "Configuration.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "ScreenBasic.h"
#include "TouchToggle.h"
#include "DisplayToastBox.h"

class ScreenSystemSettings: public ScreenBasic {

  //-----------------------------------------------------------------------------------------------
  // Public

  public:

    // Screen elements
    TouchToggle serialLogging;
    TouchToggle sdCardLogging;
    TouchToggle eventLogging;
    TouchToggle systemLogging;

    // Constructor
    ScreenSystemSettings();

    // Update and draw calls
    void update(const XPT2046::Point &position) override;
    void draw(GFXcanvas16 &canvas) override;

  //-----------------------------------------------------------------------------------------------
  // Private

  private:

    DisplayToastBox _firmwareVersion; // Firmware version toast text box

};

#endif