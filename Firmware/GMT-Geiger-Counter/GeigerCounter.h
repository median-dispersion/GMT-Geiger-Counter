#include "Arduino.h"
#include "GeigerMullerTube.h"
#include "Configuration.h"

class GeigerCounter {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Constructor
    GeigerCounter();

    void     begin();                                  // Initialize everything
    void     enable();                                 // Enable the Geiger counter
    void     disable();                                // Disable the Geiger counter
    void     setIntegrationTime(uint16_t timeSeconds); // Set the integration time
    double   getCountsPerMinute();                     // Get the counts per minute
    uint64_t getAccumulativeCount();                   // Get the total number of recorded counts since power on
    double   getMicroSievertPerHour();                 // Convert counts per minute to micro Sievert per hour
    uint16_t getIntegrationTime();                     // Get the set integration time

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    static GeigerCounter *_instance; // Static instance pointer to itself

    GeigerMullerTube  _mainTube;                                // Main GM tube
    GeigerMullerTube  _followerTube;                            // Follower GM tube
    volatile uint16_t _movingAverage[MOVING_AVERAGE_SIZE] = {}; // Array for storing counts per second for a duration of *SIZE* seconds
    volatile uint16_t _movingAverageIndex;                      // Index of the moving average array
    hw_timer_t        *_movingAverageTimer;                     // Hardware timer for advancing the moving average array
    volatile uint64_t _accumulativeCount;                       // The total number of recorded counts since power on
    bool              _enabled;                                 // Flag for checking if the Geiger counter is enabled
    uint16_t          _integrationTimeSeconds;                  // Number of seconds to use from the moving average array to average over

    static void IRAM_ATTR _handleInterrupt();      // Interrupt service routine for handling hardware interrupts
    void        IRAM_ATTR _advanceMovingAverage(); // Advance the moving average

};