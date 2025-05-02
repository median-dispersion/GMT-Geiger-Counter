#ifndef _SCREEN_GEIGER_COUNTER_H
#define _SCREEN_GEIGER_COUNTER_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "GeigerCounter.h"
#include "Screen.h"
#include "Strings.h"
#include "Graphics.h"
#include "DisplayDoseScreen.h"
#include "TouchTarget.h"
#include "DisplayInfoBox.h"
#include "TouchIcon.h"

class ScreenGeigerCounter: public Screen {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:
    
    // Screen elements
    TouchIcon   audioSettings;
    TouchIcon   displaySettings;
    TouchIcon   goToSleep;
    TouchIcon   decreaseIntegrationTime;
    TouchIcon   resetIntegrationTime;
    TouchIcon   increaseIntegrationTime;
    TouchIcon   cosmicRayDetector;
    TouchIcon   radiationHistory;
    TouchIcon   trueRNG;
    TouchIcon   hotspotSettings;
    TouchIcon   wifiSettings;
    TouchIcon   systemSettings;
    TouchTarget geigerCounterSetting;

    // Constructor
    ScreenGeigerCounter();

    // Update and draw calls
    void update(const XPT2046::Point &position) override;
    void draw(GFXcanvas16 &canvas) override;

    // Set screen variables
    void setEquivalentDose(const double &equivalentDose);                          // Set the equivalent dose
    void setEquivalentDoseUnit(const GeigerCounter::DoseUnit equivalentDoseUnit);  // Set the equivalent dose unit
    void setRadiationRating(const GeigerCounter::RadiationRating radiationRating); // Set the radiation rating
    void setCountsPerMinute(const double &countsPerMinute);                        // Set the counts per minute
    void setIntegrationTime(const uint8_t integrationTime);                        // Set the integration time

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    String            _equivalentDoseString;  // Equivalent dose string
    String            _countsPerMinuteString; // Counts per minute string
    String            _integrationTimeString; // Integration time string
    DisplayDoseScreen _equivalentDoseScreen;  // Equivalent dose screen
    DisplayInfoBox    _radiationRating;       // Radiation rating info box
    DisplayInfoBox    _countsPerMinute;       // Counts per minute info box
    DisplayInfoBox    _integrationTime;       // Integration time info box

};

#endif