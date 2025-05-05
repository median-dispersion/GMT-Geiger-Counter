#ifndef _SCREEN_RADIATION_HISTORY_H
#define _SCREEN_RADIATION_HISTORY_H

#include "Arduino.h"
#include "Configuration.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "Graphics.h"
#include "ScreenBasicLandscape.h"
#include "DisplayInfoBox.h"

class ScreenRadiationHistory: public ScreenBasicLandscape {

  //-----------------------------------------------------------------------------------------------
  // Public

  public:

    // Constructor
    ScreenRadiationHistory();

    // Update and draw calls
    void update(const XPT2046::Point &position) override;
    void draw(GFXcanvas16 &canvas) override;

    void setRadiationHistory(uint32_t *history, const uint8_t index); // Update the radiation history

  //-----------------------------------------------------------------------------------------------
  // Private
  
  public:

    uint32_t       *_history;                     // History array
    uint8_t        _historyIndex;                 // Index of the history array
    uint8_t        _timeSteps;                    // History graph time steps
    uint32_t       _countSteps;                   // History graph count steps
    String         _averageCountsPerMinuteString; // Average counts per minute string
    String         _maximumCountsPerMinuteString; // Maximum counts per minute string
    String         _minimumCountsPerMinuteString; // Minimum counts per minute string
    DisplayInfoBox _averageCountsPerMinute;       // Average counts per minute screen element
    DisplayInfoBox _maximumCountsPerMinute;       // Maximum counts per minute screen element
    DisplayInfoBox _minimumCountsPerMinute;       // Minimum counts per minute screen element

};

#endif