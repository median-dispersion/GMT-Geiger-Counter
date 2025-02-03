#ifndef _GEIGER_COUNTER_H
#define _GEIGER_COUNTER_H

#include "Arduino.h"
#include "Configuration.h"
#include "Tube.h"

class GeigerCounter {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Radiation rating enumerator
    enum RadiationRating {

      RADIATION_RATING_UNKNOWN,
      RADIATION_RATING_NORMAL,
      RADIATION_RATING_ELEVATED,
      RADIATION_RATING_HIGH,
      RADIATION_RATING_EXTREME

    };

    // Constructor
    GeigerCounter();

    void            begin();                                       // Initialize everything
    void            enable();                                      // Enable the Geiger counter
    void            disable();                                     // Disable the Geiger counter
    bool            enabled();                                     // Returns if the Geiger counter is enabled
    void            setIntegrationTime(const uint8_t timeSeconds); // Set the integration time
    uint64_t        getCounts();                                   // Get the total number of counts
    uint64_t        getMainTubeCounts();                           // Get the total number of counts for the main tube
    uint64_t        getFollowerTubeCounts();                       // Get the total number of counts for the follower tube
    double          getCountsPerMinute();                          // Get the counts per minute
    double          getMicrosievertsPerHour();                     // Convert counts per minute to Microsieverts per hour
    uint8_t         getIntegrationTime();                          // Get the integration time
    RadiationRating getRadiationRating();                          // Get the radiation rating

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    bool              _enabled;                // Flag for checking if the Geiger counter is enabled
    uint8_t           _integrationTimeSeconds; // Number of seconds to use from the moving average array to average over
    volatile uint16_t _movingAverage[60];      // Array for storing counts per second for a duration of 60 seconds
    volatile uint8_t  _movingAverageIndex;     // Index of the moving average array
    hw_timer_t        *_movingAverageTimer;    // Hardware timer for advancing the moving average array
    Tube              _mainTube;               // Main Tube
    Tube              _followerTube;           // Follower tube

    // Interrupt service routine for advancing the moving average
    static void IRAM_ATTR _advanceMovingAverage(void *instancePointer);

};

#endif