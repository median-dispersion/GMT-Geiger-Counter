#ifndef _SCREEN_AUDIO_SETTINGS_H
#define _SCREEN_AUDIO_SETTINGS_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "ScreenBasicLandscape.h"
#include "TouchToggle.h"

class ScreenAudioSettings: public ScreenBasicLandscape {

  //-----------------------------------------------------------------------------------------------
  // Public
  
  public:

    // Screen elements
    TouchToggle detections;
    TouchToggle notifications;
    TouchToggle alerts;
    TouchToggle interface;
    TouchToggle muteEverything;

    // Constructor
    ScreenAudioSettings();

    // Update and draw calls
    void update(const XPT2046::Point &position) override;
    void draw(GFXcanvas16 &canvas) override;

};

#endif