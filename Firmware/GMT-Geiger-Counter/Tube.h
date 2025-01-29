#ifndef _TUBE_H
#define _TUBE_H

#include "Arduino.h"
#include "Configuration.h"

class Tube {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Constructor
    Tube(const uint8_t pin, volatile uint16_t *movingAverage, volatile uint8_t &movingAverageIndex);

    void begin();            // Initialize everything
    void enable();           // Enable the tube
    void disable();          // Disable the tube
    uint64_t getCounts();    // Get the total number of counts

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    const uint8_t     _pin;                    // The pin, the tube is connected to
    volatile uint16_t *_movingAverage;         // Pointer to the moving average array
    volatile uint8_t  &_movingAverageIndex;    // Reference to the moving average array index
    bool              _enabled;                // Flag for checking if tube is enabled
    volatile uint64_t _pulseTimerMicroseconds; // Timer for measuring the pulse length
    volatile uint64_t _counts;                 // Total number of counts

    // Interrupt service routine for counting pulses
    static void IRAM_ATTR _countPulse(void *instancePointer);

};

#endif