#ifndef _SCREEN_SLEEP_H
#define _SCREEN_SLEEP_H

#include "Arduino.h"
#include "Configuration.h"
#include "Screen.h"
#include "TouchTarget.h"

class ScreenSleep: public Screen {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    TouchTarget wakeup;

    // Constructor
    ScreenSleep();

    // Update and draw calls
    void update(XPT2046::Point position) override;
    void draw(GFXcanvas16 &canvas) override;

};

#endif