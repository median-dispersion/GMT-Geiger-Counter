#ifndef RANDOM_TUBE_H
#define RANDOM_TUBE_H

#include "Arduino.h"
#include "Configuration.h"

class RandomTube {

  // --------------------------------------------------------------------------------------------
  // Public

  public:

    // Constructor
    RandomTube(const uint8_t pin);

    void    begin();        // Initialize everything
    void    enable();       // Enable the RNG Tube
    void    disable();      // Disable the RNG tube
    bool    available();    // Return if random data is available
    uint8_t getRandomBit(); // Get a random bit

  // --------------------------------------------------------------------------------------------
  // Private
  
  private:

    const uint8_t     _pin;                         // Tube pin
    bool              _enabled;                     // Enabled flag
    volatile uint64_t _pulseStartTimeMicroseconds;  // Pulse start time of the current pulse
    volatile bool     _firstPulse;                  // First pulse capture flag
    volatile bool     _secondPulse;                 // Second pulse capture flag
    volatile bool     _thirdPulse;                  // Third pulse capture flag
    volatile uint64_t _firstPulseTimeMicroseconds;  // First pulse time in microseconds
    volatile uint64_t _secondPulseTimeMicroseconds; // Second pulse time in microseconds
    volatile uint64_t _thirdPulseTimeMicroseconds;  // Third pulse time in microseconds

    // Interrupt service routine for measuring pulses
    static void IRAM_ATTR _measurePulse(void *instancePointer);

};

#endif