#ifndef _COSMIC_RAY_DETECTOR_H
#define _COSMIC_RAY_DETECTOR_H

#include "Arduino.h"
#include "Configuration.h"
#include "Logger.h"
#include "GeigerCounter.h"
#include "CoincidenceTube.h"

class CosmicRayDetector {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Get the single instance of the class
    static CosmicRayDetector& getInstance();

    void     begin();                                     // Initialize everything
    void     enable();                                    // Enable the cosmic ray detector
    void     disable();                                   // Disable the cosmic ray detector
    void     setCosmicRayDetectorState(const bool state); // Set the state of the cosmic ray detector
    bool     getCosmicRayDetectorState();                 // Returns if the cosmic ray detector is enabled
    uint64_t getCoincidenceEvents();                      // Get the number of coincidence events since the last time the cosmic ray detector was enabled
    uint64_t getCoincidenceEventsTotal();                 // Get the total number of coincidence events
    uint32_t getCoincidenceEventsPerHour();               // Get the number of coincidence events per hour
    uint64_t getMainTubeCounts();                         // Get the number of counts from the main tube since enabling the cosmic ray detector
    uint64_t getFollowerTubeCounts();                     // Get the number of counts from the follower tube since enabling the cosmic ray detector 

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    // Prevent direct instantiation
    CosmicRayDetector();
    CosmicRayDetector(const CosmicRayDetector&) = delete;
    CosmicRayDetector& operator=(const CosmicRayDetector&) = delete;

    bool              _initialized;           // Flag for checking of the cosmic ray detector is enabled
    volatile uint16_t _movingAverage[60];     // Array for storing coincidence events per hour
    volatile uint8_t  _movingAverageIndex;    // Index of the moving average array
    hw_timer_t        *_movingAverageTimer;   // Hardware timer for advancing the moving average array
    CoincidenceTube   _coincidenceTube;       // Virtual coincidence tube
    bool              _enabled;               // Flag for checking if the cosmic ray detector is enabled
    uint64_t          _coincidenceTubeOffset; // Coincidence tube counts offset
    uint64_t          _mainTubeOffset;        // Main tube counts offset
    uint64_t          _followerTubeOffset;    // Follower tube counts offset

    // Interrupt service routine for advancing the moving average
    static void IRAM_ATTR _advanceMovingAverage(void *instancePointer);

};

// Global reference to the cosmic ray detector instance for easy access
extern CosmicRayDetector& cosmicRayDetector;

#endif