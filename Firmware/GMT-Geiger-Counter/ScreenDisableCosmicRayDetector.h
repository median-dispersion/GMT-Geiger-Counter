#ifndef _SCREEN_DISABLE_COSMIC_RAY_DETECTOR_H
#define _SCREEN_DISABLE_COSMIC_RAY_DETECTOR_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "Graphics.h"
#include "ScreenBasicPortrait.h"
#include "TouchButton.h"

class ScreenDisableCosmicRayDetector: public ScreenBasicPortrait {

  //-----------------------------------------------------------------------------------------------
  // Public

  public:

    // Constructor
    ScreenDisableCosmicRayDetector();

    // Screen elements
    TouchButton confirm;
    TouchButton dismiss;

    // Update and draw calls
    void update(const XPT2046::Point &position) override;
    void draw(GFXcanvas16 &canvas) override;

};

#endif