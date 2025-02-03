#ifndef _SCREEN_COSMIC_RAY_DETECTOR_ROTATION_H
#define _SCREEN_COSMIC_RAY_DETECTOR_ROTATION_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "Graphics.h"
#include "ScreenBasicPortrait.h"
#include "TouchTarget.h"

class ScreenRotationConfirmation: public ScreenBasicPortrait {

  //-----------------------------------------------------------------------------------------------
  // Public

  public:

    // Screen elements
    TouchTarget confirm;

    // Constructor
    ScreenRotationConfirmation();

    // Update and draw calls
    void update(const XPT2046::Point position) override;
    void draw(GFXcanvas16 &canvas) override;

};

#endif