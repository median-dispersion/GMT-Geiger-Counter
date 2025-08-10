#ifndef _SCREEN_TRUE_RNG_H
#define _SCREEN_TRUE_RNG_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "ScreenBasicLandscape.h"
#include "TouchIcon.h"
#include "DisplayDoseScreen.h"

class ScreenTrueRNG: public ScreenBasicLandscape {

  //-----------------------------------------------------------------------------------------------
  // Public

  public:

    // Screen elements
    TouchIcon increaseMinimum;
    TouchIcon decreaseMinimum;
    TouchIcon increaseMaximum;
    TouchIcon decreaseMaximum;
    TouchIcon roll;
    TouchIcon mute;
    TouchIcon sleep;

    // Constructor
    ScreenTrueRNG();

    // Update and draw calls
    void update(const XPT2046::Point &position) override;
    void draw(GFXcanvas16 &canvas) override;

    void setValue(const uint8_t value);     // Set the screen value
    void setState(const bool rolling);      // Set the rolling state
    void setMinimum(const uint8_t minimum); // Set the minimum value
    void setMaximum(const uint8_t maximum); // Set the maximum value

  //-----------------------------------------------------------------------------------------------
  // Private

  private:

    String            _screenString;      // Screen string
    String            _descriptionString; // Screen description string
    String            _minimumString;     // Range minimum string
    String            _maximumString;     // Range maximum string
    DisplayDoseScreen _resultScreen;      // Result screen element

};

#endif