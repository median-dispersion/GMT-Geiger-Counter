#ifndef _SCREEN_GEIGER_COUNTER_H
#define _SCREEN_GEIGER_COUNTER_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "GeigerCounter.h"
#include "Screen.h"
#include "Strings.h"
#include "Graphics.h"
#include "DisplayInfoBox.h"
#include "TouchIcon.h"

class ScreenGeigerCounter: public Screen {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:
    
    // Screen elements
    TouchIcon audioSettings;
    TouchIcon displaySettings;
    TouchIcon goToSleep;
    TouchIcon decreaseIntegrationTime;
    TouchIcon resetIntegrationTime;
    TouchIcon increaseIntegrationTime;
    TouchIcon radiationHistory;
    TouchIcon cosmicRayDetector;
    TouchIcon trueRNG;
    TouchIcon hotspotSettings;
    TouchIcon wifiSettings;
    TouchIcon systemSettings;

    // Constructor
    ScreenGeigerCounter();

    // Update and draw calls
    void update(XPT2046::Point position) override;
    void draw(GFXcanvas16 &canvas) override;

    // Set screen variables
    void setEquivalentDose(String equivalentDose);                           // Set the equivalent dose
    void setEquivalentDoseUnit(String equivalentDoseUnit);                   // Set the equivalent dose unit
    void setRadiationRating(GeigerCounter::RadiationRating radiationRating); // Set the radiation rating
    void setCountsPerMinute(String countsPerMinute);                         // Set the counts per minute
    void setIntegrationTime(String integrationTime);                         // Set the integration time

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    String         _equivalentDose;      // Equivalent dose string
    String         _equivalentDoseUnit;  // Equivalent dose unit string
    uint16_t       _doseBorderColor;     // Dose screen border color
    uint16_t       _doseBackgroundColor; // Dose screen background color
    DisplayInfoBox _radiationRating;     // Radiation rating info box
    DisplayInfoBox _countsPerMinute;     // Counts per minute info box
    DisplayInfoBox _integrationTime;     // Integration time info box

};

#endif