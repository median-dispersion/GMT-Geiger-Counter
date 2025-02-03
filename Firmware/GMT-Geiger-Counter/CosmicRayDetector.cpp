#include "CosmicRayDetector.h"

// ------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
CosmicRayDetector::CosmicRayDetector():

  // Initialize members
  _enabled(false),
  _movingAverageIndex(0),
  _movingAverageTimer(NULL),
  _coincidenceTube(COINCIDENCE_TRG_PIN, _movingAverage, _movingAverageIndex)

{}

// ================================================================================================
// Initialize everything
// ================================================================================================
void CosmicRayDetector::begin() {

  // Initialize the coincidence tube
  _coincidenceTube.begin();

}

// ================================================================================================
// Enable the cosmic ray detector
// ================================================================================================
void CosmicRayDetector::enable() {

  // If not already enabled
  if (!_enabled) {

    // Clear the moving average array
    for (uint8_t i = 0; i < 60; i++) { _movingAverage[i] = 0; }

    // Reset the position of the moving average index
    _movingAverageIndex = 0;

    // Enable the coincidence tube
    _coincidenceTube.enable();

    // Set hardware timer frequency to 1Mhz
    _movingAverageTimer = timerBegin(1000000);

    // Attach the _advanceMovingAverage ISR to the hardware timer and pass the class instance pointer to it
    timerAttachInterruptArg(_movingAverageTimer, _advanceMovingAverage, this);

    // Set alarm to call the ISR function, every minute, repeat, forever
    timerAlarm(_movingAverageTimer, 60000000, true, 0);

    // Set the enabled flag to true
    _enabled = true;

  }

}

// ================================================================================================
// Disable the cosmic ray detector
// ================================================================================================
void CosmicRayDetector::disable() {

  // If not already disabled
  if (_enabled) {

    // Disable the coincidence tube
    _coincidenceTube.disable();

    // Detach the ISR from the hardware timer
    timerDetachInterrupt(_movingAverageTimer);

    // Stop and free the hardware timer
    timerEnd(_movingAverageTimer);

    // Clear the hardware timer
    _movingAverageTimer = NULL;

    // Set the enabled flag to false
    _enabled = false;

  }

}

// ================================================================================================
// Returns if the cosmic ray detector is enabled
// ================================================================================================
bool CosmicRayDetector::enabled() {

  return _enabled;

}

// ================================================================================================
// Get the total number of coincidence events
// ================================================================================================
uint64_t CosmicRayDetector::getCoincidenceEvents() {

  // Get and return the total number of coincidence events
  return _coincidenceTube.getCounts();

}

// ================================================================================================
// Get the number of coincidence events per hour
// ================================================================================================
double CosmicRayDetector::getCoincidenceEventsPerHour() {

  // Coincidence events per hour variable
  double cph = 0.0;

  // For all element in the moving average array
  for (uint8_t i = 0; i < 60; i++) {

    // Add to the sum of all elements
    cph += _movingAverage[i];

  }

  // Average all values
  cph = cph / 60.0;

  // Return the number of coincidence events per hour
  return cph;

}

// ------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Advance the moving average
// ================================================================================================
void IRAM_ATTR CosmicRayDetector::_advanceMovingAverage(void *instancePointer) {

  // Cast the generic instancePointer back to a instance pointe of type CosmicRayDetector 
  CosmicRayDetector *instance = (CosmicRayDetector*)instancePointer;

  // Calculate a wrapped index by using the current index + 1, if it overflows wrapped to the start of the array
  uint16_t wrappedIndex = (instance->_movingAverageIndex + 1) % 60;

  // Clear the next element in the array
  instance->_movingAverage[wrappedIndex] = 0;

  // Set the moving index to the next element in the array
  instance->_movingAverageIndex = wrappedIndex;

}