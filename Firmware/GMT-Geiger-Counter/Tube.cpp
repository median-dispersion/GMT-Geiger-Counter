#include "Tube.h"

// ------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
Tube::Tube(const uint8_t pin, volatile uint16_t *movingAverage, volatile uint8_t &movingAverageIndex):

  // Initialize members
  _pin(pin),
  _movingAverage(movingAverage),
  _movingAverageIndex(movingAverageIndex),
  _enabled(false),
  _pulseStartTimeMicroseconds(0),
  _counts(0)

{}

// ================================================================================================
// Initialize everything
// ================================================================================================
void Tube::begin() {

  // Set the pin mode to INPUT
  pinMode(_pin, INPUT);

}

// ================================================================================================
// Enable the tube
// ================================================================================================
void Tube::enable() {

  // If not already enabled
  if (!_enabled) {

    // Attach a hardware interrupt to the tube pin calling the _countPulse ISR on state change and passing the class instance pointer to it
    attachInterruptArg(digitalPinToInterrupt(_pin), _countPulse, this, CHANGE);

    // Set the enabled flag to true
    _enabled = true;

  }

}

// ================================================================================================
// Disable the tube
// ================================================================================================
void Tube::disable() {

  // If not already disabled
  if (_enabled) {

    // Detach the hardware interrupt
    detachInterrupt(digitalPinToInterrupt(_pin));

    // Set the enabled flag to false
    _enabled = false;

  }

}

// ================================================================================================
// Set the tube state
// ================================================================================================
void Tube::setTubeState(const bool state) {

  // Depending on the state either enable or disable the tube
  if (state) {

    enable();

  } else {

    disable();

  }

}

// ================================================================================================
// Returns if the tube is enabled
// ================================================================================================
bool Tube::getTubeState() {

  return _enabled;

}

// ================================================================================================
// Get the total number of counts
// ================================================================================================
uint64_t Tube::getCounts() {
  
  // Get and return the total number of counts
  return _counts;

}

// ------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Count a pulse
// ================================================================================================
void IRAM_ATTR Tube::_countPulse(void *instancePointer) {

  // Cast the generic instance pointer back to a instance pointe of type Tube 
  Tube *instance = (Tube*)instancePointer;

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
      
      // Add one count to the moving average
      instance->_movingAverage[instance->_movingAverageIndex]++;

      // Add one count to the total number of counts
      instance->_counts++;

      // TODO
      // Adding just plus one to is not ideal. For just one tube, this is fine, but for multiple it is not. 
      // When daisy-chaining multiple tubes on one header, pulses from different tube might occur simultaneously or overlap.
      // To capture overlapping pulses, the total pulse length should be divided by the average singe pulse length for a tube to get the actual number of pulses.
      // This still needs to be implemented!

    }

  }

}