#include "RandomTube.h"

// ------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
RandomTube::RandomTube(const uint8_t pin):

  _pin(pin),
  _enabled(false),
  _pulseStartTimeMicroseconds(0),
  _firstPulse(false),
  _secondPulse(false),
  _thirdPulse(false),
  _firstPulseTimeMicroseconds(0),
  _secondPulseTimeMicroseconds(0),
  _thirdPulseTimeMicroseconds(0)

{}

// ================================================================================================
// Initialize everything
// ================================================================================================
void RandomTube::begin() {

  // Set the pin mode to INPUT
  pinMode(_pin, INPUT);

}

// ================================================================================================
// Enable the RNG Tube
// ================================================================================================
void RandomTube::enable() {

  // If not already enabled
  if (!_enabled) {

    // Reset all pulse capture flags
    _firstPulse  = false;
    _secondPulse = false;
    _thirdPulse  = false;

    // Attach a hardware interrupt to the tube pin calling the _measurePulse ISR on state change and passing the class instance pointer to it
    attachInterruptArg(digitalPinToInterrupt(_pin), _measurePulse, this, CHANGE);

    // Set the enabled flag to true
    _enabled = true;

  }

}

// ================================================================================================
// Disable the RNG tube
// ================================================================================================
void RandomTube::disable() {

  // If not already disabled
  if (_enabled) {

    // Detach the hardware interrupt
    detachInterrupt(digitalPinToInterrupt(_pin));

    // Set the enabled flag to false
    _enabled = false;

  }

}

// ================================================================================================
// Return if random data is available
// ================================================================================================
bool RandomTube::available() {

  // Default the available flag to false
  bool available = false;

  // If all three pulses have been captured
  if (_firstPulse && _secondPulse && _thirdPulse) { 
    
    // Set the available flag to true
    available = true; 
  
  }

  // Return bit availability
  return available;

}

// ================================================================================================
// Get a random bit
// ================================================================================================
uint8_t RandomTube::getRandomBit() {

  // Set the random bit to an invalid value
  uint8_t bit = UINT8_MAX;

  // If random data is available
  if (available()) {

    // Calculate the time delta between the first and second and second and third pulse
    uint64_t firstTimeDeltaMicroseconds  = _secondPulseTimeMicroseconds - _firstPulseTimeMicroseconds;
    uint64_t secondTimeDeltaMicroseconds = _thirdPulseTimeMicroseconds  - _secondPulseTimeMicroseconds;

    // Reset all pulse capture flags
    _firstPulse  = false;
    _secondPulse = false;
    _thirdPulse  = false;

    // If the first time delta is smaller then second one
    if (firstTimeDeltaMicroseconds < secondTimeDeltaMicroseconds) {

      // Set the random bit to 0
      bit = 0;

    // If the first time delta is larger then second one
    } else {

      // Set the random bit to 1
      bit = 1;

    }

  }

  // Return the random bit
  return bit;

}

// ------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Interrupt service routine for measuring pulses
// ================================================================================================
void IRAM_ATTR RandomTube::_measurePulse(void *instancePointer) {

  // Cast the generic instance pointer back to the correct type
  RandomTube *instance = (RandomTube*)instancePointer;

  // On the rising edge of the pulse
  if (digitalRead(instance->_pin) == HIGH) {

    // Capture the current time in microseconds
    instance->_pulseStartTimeMicroseconds = micros();
  
  // On the falling edge
  }else {

    // Calculate the pulse length by subtracting the time from the rising edge to now in microseconds
    uint64_t pulseLengthMicroseconds = micros() - instance->_pulseStartTimeMicroseconds;

    // Check if the pulse length is longer than the noise threshold
    if (pulseLengthMicroseconds > TUBE_NOISE_THRESHOLD_MICROSECONDS) {

      // If the first and second pulse have been captured but not the third one
      if (instance->_firstPulse && instance->_secondPulse && !instance->_thirdPulse) {

        // Set the third pulse time to the start time of this pulse
        instance->_thirdPulseTimeMicroseconds = instance->_pulseStartTimeMicroseconds;

        // Set the third pulse capture flag to true
        instance->_thirdPulse = true;

      }

      // If the first pulse has been captured but not the second one
      if (instance->_firstPulse && !instance->_secondPulse) {

        // Set the second pulse time to the start time of this pulse
        instance->_secondPulseTimeMicroseconds = instance->_pulseStartTimeMicroseconds;

        // Set the second pulse capture flag to true
        instance->_secondPulse = true;

      }

      // If the first pulse has not been captured
      if (!instance->_firstPulse) {

        // Set the first pulse time to the start time of this pulse
        instance->_firstPulseTimeMicroseconds = instance->_pulseStartTimeMicroseconds;

        // Set the first pulse capture flag to true
        instance->_firstPulse = true;

      }

    }

  }

}