#include "GeigerMullerTube.h"

// Initialize static instance pointer
GeigerMullerTube *GeigerMullerTube::_instance = nullptr;  

// ------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
GeigerMullerTube::GeigerMullerTube(uint8_t tubePin, volatile uint16_t *movingAverage, volatile uint16_t &movingAverageIndex, volatile uint64_t &accumulativeCount):

  // Initialize members
  _tubePin(tubePin),
  _movingAverage(movingAverage),
  _movingAverageIndex(movingAverageIndex),
  _accumulativeCount(accumulativeCount)

{}

// ================================================================================================
// Initialize everything
// ================================================================================================
void GeigerMullerTube::begin() {

  // Set the static instance pointer to this object instance
  _instance = this;

  // Set the tube pin mode to INPUT
  pinMode(_tubePin, INPUT);

}

// ================================================================================================
// Enable the tube
// ================================================================================================
void GeigerMullerTube::enable() {

  // If not already enabled
  if (!_enabled) {

    // Attach a hardware interrupt to the tube pin calling the _handleInterrupt ISR
    attachInterrupt(digitalPinToInterrupt(_tubePin), this->_handleInterrupt, CHANGE);

    // Set the enabled flag to true
    _enabled = true;

  }

}

// ================================================================================================
// Disable the tube
// ================================================================================================
void GeigerMullerTube::disable() {

  // If not already disabled
  if (_enabled) {

    // Detach the hardware interrupt
    detachInterrupt(digitalPinToInterrupt(_tubePin));

    // Set the enabled flag to false
    _enabled = false;

  }

}

// ------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Interrupt service routine for handling hardware interrupts
// ================================================================================================
void IRAM_ATTR GeigerMullerTube::_handleInterrupt() {

  // Count a pulse
  _instance->_countPulse();

}

// ================================================================================================
// Count a pulse
// ================================================================================================
void IRAM_ATTR GeigerMullerTube::_countPulse() {

  // On the rising edge of the pulse
  if (digitalRead(MAIN_TRG_PIN) == HIGH) {

    // Capture the current time in microseconds
    _pulseTimerMicroseconds = micros();

  // On the falling edge
  }else {

    // Calculate the pulse length by subtracting the time from the rising edge to now in microseconds
    uint64_t pulseLengthMicroseconds = micros() - _pulseTimerMicroseconds;

    // Check if the pulse length is longer than the noise threshold
    if (pulseLengthMicroseconds > NOISE_THRESHOLD_MICROSECONDS) {

      // Add one count to the moving average
      _movingAverage[_movingAverageIndex]++;

      // Add one to the accumulative count stack
      _accumulativeCount++;

      // TODO
      // Adding just plus one to is not ideal. For just one tube, this is fine, but for multiple it is not. 
      // When daisy-chaining multiple tubes on one header, pulses from different tube might occur simultaneously or overlap.
      // To capture overlapping pulses, the total pulse length should be divided by the average singe pulse length for a tube to get the actual number of pulses.
      // This still needs to be implemented!

    }

  }

}