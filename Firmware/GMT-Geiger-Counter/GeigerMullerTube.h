#include "Arduino.h"
#include "Configuration.h"

class GeigerMullerTube {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Constructor
    GeigerMullerTube(uint8_t tubePin, volatile uint16_t *movingAverage, volatile uint16_t &movingAverageIndex, volatile uint64_t &accumulativeCount);

    void begin();   // Initialize everything
    void enable();  // Enable the tube
    void disable(); // Disable the tube

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    static GeigerMullerTube *_instance; // Static instance pointer to itself

    uint8_t           _tubePin;                // The pin, the tube is connected to
    volatile uint16_t *_movingAverage;         // Pointer to the moving average array
    volatile uint16_t &_movingAverageIndex;    // Reference to the moving average array index
    volatile uint64_t &_accumulativeCount;     // Reference to the accumulative count
    bool              _enabled;                // Flag for checking if tube is enabled
    volatile uint64_t _pulseTimerMicroseconds; // Timer for measuring the pulse length
    
    static void IRAM_ATTR _handleInterrupt();  // Interrupt service routine for handling hardware interrupts
    void        IRAM_ATTR _countPulse();       // Count a pulse

};