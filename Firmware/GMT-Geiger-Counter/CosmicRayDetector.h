#ifndef _COSMIC_RAY_DETECTOR_H
#define _COSMIC_RAY_DETECTOR_H

#include "Arduino.h"
#include "Configuration.h"
#include "Logger.h"
#include "Tube.h"

class CosmicRayDetector {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Constructor
    CosmicRayDetector();

    void     begin();                       // Initialize everything
    void     enable();                      // Enable the cosmic ray detector
    void     disable();                     // Disable the cosmic ray detector
    bool     enabled();                     // Returns if the cosmic ray detector is enabled
    uint64_t getCoincidenceEvents();        // Get the total number of coincidence events
    uint32_t getCoincidenceEventsPerHour(); // Get the number of coincidence events per hour

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    bool              _enabled;             // Flag for checking if the cosmic ray detector is enabled
    volatile uint16_t _movingAverage[60];   // Array for storing coincidence events per hour
    volatile uint8_t  _movingAverageIndex;  // Index of the moving average array
    hw_timer_t        *_movingAverageTimer; // Hardware timer for advancing the moving average array
    Tube              _coincidenceTube;     // Virtual coincidence tube

    // Interrupt service routine for advancing the moving average
    static void IRAM_ATTR _advanceMovingAverage(void *instancePointer);

};

#endif