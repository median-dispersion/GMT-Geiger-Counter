#ifndef RANDOM_NUMBER_GENERATOR_H
#define RANDOM_NUMBER_GENERATOR_H

#include "Arduino.h"
#include "Configuration.h"
#include "Logger.h"
#include "RandomTube.h"

class RandomNumberGenerator {

  // --------------------------------------------------------------------------------------------
  // Public

  public:

    // Get the single instance of the class
    static RandomNumberGenerator& getInstance();

    void    begin();                           // Initialize everything
    void    enable();                          // Enable the random number generator
    void    disable();                         // Disable the random number generator
    void    update();                          // Update the random number generator
    void    roll();                            // Roll a random number in the set range
    void    setState(const bool state);        // Set the random number generator state
    void    setMinimum(const uint8_t minimum); // Set the range minimum
    void    setMaximum(const uint8_t maximum); // Set the range maximum
    bool    getState();                        // Get the random number generator state
    uint8_t getMinimum();                      // Get the range minimum
    uint8_t getMaximum();                      // Get the range maximum
    bool    getRollingState();                 // Get the rolling state
    uint8_t getValue();                        // Get the final random number result or the process of the rolling process
    bool    getStaleState();                   // Retrun if the random bit is stale or not
    uint8_t getRandomBit();                    // Get the latest random bit

  // --------------------------------------------------------------------------------------------
  // Private
  
  private:

    // Prevent direct instantiation
    RandomNumberGenerator();
    RandomNumberGenerator(const RandomNumberGenerator&) = delete;
    RandomNumberGenerator& operator=(const RandomNumberGenerator&) = delete;

    bool       _initialized;  // Initialization flag
    bool       _enabled;      // Enabled flag
    RandomTube _randomTube;   // Random number generator virtual tube
    uint8_t    _minimum;      // Range minimum value
    uint8_t    _maximum;      // Range maximum value
    bool       _rolling;      // Rolling a random number flag
    uint8_t    _range;        // Range of random numbers
    uint8_t    _requiredBits; // Required bits to represent the maximum value in the range
    uint8_t    _capturedBits; // Number of captured bits
    uint8_t    _result;       // Random number result
    bool       _stale;
    uint8_t    _latestBit;

    void _calculateRange();                // Calculate the range of random numbers
    void _updateResult(const uint8_t bit); // Update the random number result with a new random bit

};

// Global reference to the RandomNumberGenerator instance for easy access
extern RandomNumberGenerator& randomNumberGenerator;

#endif