#ifndef _SCREEN_RADIATION_HISTORY_H
#define _SCREEN_RADIATION_HISTORY_H

#include "Arduino.h"
#include "Configuration.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "Graphics.h"
#include "ScreenBasic.h"
#include "DisplayInfoBox.h"

class ScreenRadiationHistory: public ScreenBasic {

  //-----------------------------------------------------------------------------------------------
  // Public

  public:

    // Constructor
    ScreenRadiationHistory();

    // Update and draw calls
    void update(XPT2046::Point position) override;
    void draw(GFXcanvas16 &canvas) override;

    void setRadiationHistory(double countsPerMinute); // Update the radiation history

  //-----------------------------------------------------------------------------------------------
  // Private
  
  public:

    uint32_t _history[RADIATION_HISTORY_LENGTH_MINUTES]; // History array
    uint8_t  _historyIndex;                              // Index of the history array
    uint32_t _maximumCountsPerMinute;                    // Maximum counts per minute value in the array
    uint64_t _historyTimerMilliseconds;                  // History update timer
    DisplayInfoBox _average;                             // Average counts per minute screen element
    DisplayInfoBox _maximum;                             // Maximum counts per minute screen element
    DisplayInfoBox _minimum;                             // Minimum counts per minute screen element

};

#endif