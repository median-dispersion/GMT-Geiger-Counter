#ifndef _SCREEN_HOTSPOT_SETTINGS_H
#define _SCREEN_HOTSPOT_SETTINGS_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "ScreenBasic.h"
#include "DisplayToastBox.h"

class ScreenHotspotSettings: public ScreenBasic {

  //-----------------------------------------------------------------------------------------------
  // Public

  public:

    // Temporary placeholder
    DisplayToastBox placeholder;

    // Constructor
    ScreenHotspotSettings();

    // Update and draw calls
    void update(const XPT2046::Point position) override;
    void draw(GFXcanvas16 &canvas) override;

};

#endif