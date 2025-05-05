#ifndef _SCREE_GEIGER_COUNTER_SETTINGS_2_H
#define _SCREE_GEIGER_COUNTER_SETTINGS_2_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "Graphics.h"
#include "ScreenBasicLandscape.h"
#include "TouchToggle.h"
#include "TouchRadio.h"
#include "TouchIcon.h"

class ScreenGeigerCounterSettings2: public ScreenBasicLandscape {

  //-----------------------------------------------------------------------------------------------
  // Public
  
  public:

    // Screen elements
    TouchToggle autoIntegrate;
    TouchToggle autoRange;
    TouchRadio  sieverts;
    TouchRadio  rem;
    TouchRadio  rontgen;
    TouchRadio  gray;
    TouchIcon   next;
    TouchIcon   previous;

    // Constructor
    ScreenGeigerCounterSettings2();

    // Update and draw calls
    void update(const XPT2046::Point &position) override;
    void draw(GFXcanvas16 &canvas) override;

};

#endif