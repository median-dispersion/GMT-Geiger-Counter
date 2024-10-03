#include <Arduino.h>
#include "Configuration.h"

class GeigerCounter {

  public:

    void begin();   // Initialize everything
    void enable();  // Enable pulse counting
    void disable(); // Disable pulse counting

    void setIntegrationTime(uint8_t integrationTimeSeconds); // Set the integration time

    double   getCountsPerMinute();     // Get the counts per minute
    uint64_t getAccumulativeCount();   // Get the total number of counts that were recorded since power on
    double   getMicroSievertPerHour(); // Get micro Sievert per hour by converting CPM to µSv/h

  private:

    static GeigerCounter* _instance; // Static pointer to the class instance

    volatile uint64_t _mainTubeTimerMicroseconds          = 0;     // Timer for measuring the main pulse length
    volatile uint64_t _followerTubeTimerMicroseconds      = 0;     // Timer for measuring the follower pulse length
    volatile uint16_t _movingAverage[MOVING_AVERAGE_SIZE] = {0};   // Array for storing counts per second for a duration of *SIZE* seconds
    volatile uint16_t _movingAverageIndex                 = 0;     // Index for the moving average array
    hw_timer_t        *_movingAverageTimer                = NULL;  // Hardware timer for advancing the moving average array
    volatile uint64_t _accumulativeCount                  = 0;     // The total number of counts that were recorded since power on
    bool              _countingEnabled                    = false; // Flag for checking if pulse counting is enabled
    uint8_t           _integrationTimeSeconds             = 0;     // Number of seconds to use from the moving average array to average over

    void        IRAM_ATTR _countMainPulse();          // ISR for counting the main pulse
    void        IRAM_ATTR _countFollowerPulse();      // ISR for counting the follower pulse
    void        IRAM_ATTR _advanceMovingAverage();    // Advance the moving average, every second add +1 to the index
    static void IRAM_ATTR _handleMainInterrupt();     // Handle the main tube interrupt event
    static void IRAM_ATTR _handleFollowerInterrupt(); // Handle the follower tube interrupt event
    static void IRAM_ATTR _handleTimerInterrupt();    // Handle the timer interrupt event

};