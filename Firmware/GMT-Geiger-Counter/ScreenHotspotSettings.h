#ifndef _SCREEN_HOTSPOT_SETTINGS_H
#define _SCREEN_HOTSPOT_SETTINGS_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "ScreenBasicLandscape.h"
#include "TouchToggle.h"
#include "DisplayToastBox.h"

class ScreenHotspotSettings: public ScreenBasicLandscape {

  //-----------------------------------------------------------------------------------------------
  // Public

  public:

    // Screen elements
    TouchToggle enable;

    // Constructor
    ScreenHotspotSettings();

    // Update and draw calls
    void update(const XPT2046::Point &position) override;
    void draw(GFXcanvas16 &canvas) override;

    void setIPAddress(const char *ipAddress); // Set hotspot IP address

  //-----------------------------------------------------------------------------------------------
  // Private

  private:

    // Screen elements
    DisplayToastBox _hostpotName;
    DisplayToastBox _hostpotPassword;
    DisplayToastBox _hostpotIPAddress;

};

#endif