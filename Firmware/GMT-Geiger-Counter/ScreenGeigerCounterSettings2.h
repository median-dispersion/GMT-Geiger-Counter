#ifndef _SCREE_GEIGER_COUNTER_SETTINGS_2_H
#define _SCREE_GEIGER_COUNTER_SETTINGS_2_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "Graphics.h"
#include "ScreenBasic.h"
#include "TouchToggle.h"
#include "TouchRadio.h"
#include "TouchIcon.h"

class ScreenGeigerCounterSettings2: public ScreenBasic {

  //-----------------------------------------------------------------------------------------------
  // Public
  
  public:

    // Screen elements
    TouchToggle autoRange;
    TouchRadio  sievert;
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