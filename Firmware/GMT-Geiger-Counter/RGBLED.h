#ifndef _RGB_LED_H
#define _RGB_LED_H

#include "Arduino.h"
#include "Configuration.h"
#include "Logger.h"
#include "GeigerCounter.h"

class RGBLED {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Get the single instance of the class
    static RGBLED& getInstance();

    void begin();
    void enable();
    void disable();
    void update();
    void setLEDState(const bool state);
    void setRadiationRating(const GeigerCounter::RadiationRating rating);
    bool getLEDState();

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    // Prevent direct instantiation
    RGBLED();
    RGBLED(const RGBLED&) = delete;
    RGBLED& operator=(const RGBLED&) = delete;

    bool                           _initialized; // FLag for checking if the RGB LED has been initalized
    bool                           _enabled;     // Flag for checking if RGB LED is enabled
    GeigerCounter::RadiationRating _rating;      // Radiation rating
    uint64_t                       _ledTimer;

    uint8_t _adjustColorBrightness(const uint8_t color); // Adjust the brightness of a color

};

// Global reference to the RGB LED instance for easy access
extern RGBLED& rgbLED;

#endif