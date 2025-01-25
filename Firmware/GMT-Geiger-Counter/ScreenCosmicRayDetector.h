#ifndef _SCREEN_COSMIC_RAY_DETECTOR_H
#define _SCREEN_COSMIC_RAY_DETECTOR_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "ScreenBasic.h"
#include "DisplayToastBox.h"

class ScreenCosmicRayDetector: public ScreenBasic {

  //-----------------------------------------------------------------------------------------------
  // Public
  public:

    // Temporary placeholder
    DisplayToastBox placeholder;

    // Constructor
    ScreenCosmicRayDetector();

    // Update and draw calls
    void update(XPT2046::Point position) override;
    void draw(GFXcanvas16 &canvas) override;

};

#endif