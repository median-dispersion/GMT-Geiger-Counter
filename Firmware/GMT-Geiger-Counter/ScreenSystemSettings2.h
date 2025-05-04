#ifndef _SCREEN_SYSTEM_SETTINGS_2_H
#define _SCREEN_SYSTEM_SETTINGS_2_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "ScreenBasicLandscape.h"
#include "TouchIcon.h"
#include "DisplayToastBox.h"

class ScreenSystemSettings2: public ScreenBasicLandscape {

  //-----------------------------------------------------------------------------------------------
  // Public

  public:

    // Screen elements
    TouchIcon next;
    TouchIcon previous;
    DisplayToastBox firmware;
    DisplayToastBox upTime;
    DisplayToastBox memory;

    // Constructor
    ScreenSystemSettings2();

    // Update and draw calls
    void update(const XPT2046::Point &position) override;
    void draw(GFXcanvas16 &canvas) override;

  //-----------------------------------------------------------------------------------------------
  // Private

  private:

    String _upTimeString;
    String _memoryString;

};

#endif