#include "CosmicRayDetector.h"

// ------------------------------------------------------------------------------------------------
// Public

// Initialize global reference
CosmicRayDetector& cosmicRayDetector = CosmicRayDetector::getInstance();

// ================================================================================================
// Get the single instance of the class
// ================================================================================================
CosmicRayDetector& CosmicRayDetector::getInstance() {

  // Get the single instance
  static CosmicRayDetector instance;

  // Return the instance
  return instance;

}

// ================================================================================================
// Initialize everything
// ================================================================================================
void CosmicRayDetector::begin() {

  // If not initialized
  if (!_initialized) {

    // Set initialized flag to true
    _initialized = true;

    // Initialize logger if not already
    logger.begin();

    // Initialize the Geiger counter
    geigerCounter.begin();

    // Initialize the coincidence tube
    _coincidenceTube.begin();

  }

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

    // Set the tube offsets
    _coincidenceTubeOffset = _coincidenceTube.getCounts();
    _mainTubeOffset        = geigerCounter.getMainTubeCounts();
    _followerTubeOffset    = geigerCounter.getFollowerTubeCounts();

    // Enable the Geiger counter
    geigerCounter.enable();

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

    // Create event data
    Logger::KeyValuePair event[2] = {

      {"source", Logger::STRING_T, {.string_v = "cosmicRayDetector"}},
      {"action", Logger::STRING_T, {.string_v = "enabled"}          }

    };

    // Log event message
    logger.log(Logger::EVENT, "event", event, 2);

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

    // Create event data
    Logger::KeyValuePair event[2] = {

      {"source", Logger::STRING_T, {.string_v = "cosmicRayDetector"}},
      {"action", Logger::STRING_T, {.string_v = "disabled"}         }

    };

    // Log event message
    logger.log(Logger::EVENT, "event", event, 2);

  }

}

// ================================================================================================
// Set the state of the cosmic ray detector
// ================================================================================================
void CosmicRayDetector::setCosmicRayDetectorState(const bool state) {

  // Depending on the state either enable or disable the cosmic ray detector
  if (state) {

    enable();

  } else {

    disable();

  }

}

// ================================================================================================
// Returns if the cosmic ray detector is enabled
// ================================================================================================
bool CosmicRayDetector::getCosmicRayDetectorState() {

  return _enabled;

}

// ================================================================================================
// Get the number of coincidence events since the last time the cosmic ray detector was enabled
// ================================================================================================
uint64_t CosmicRayDetector::getCoincidenceEvents() {

  // Subtract the coincidence tube offset from the total number of coincidence events and return that
  return _coincidenceTube.getCounts() - _coincidenceTubeOffset;

}


// ================================================================================================
// Get the total number of coincidence events
// ================================================================================================
uint64_t CosmicRayDetector::getCoincidenceEventsTotal() {

  // Get and return the total number of coincidence events
  return _coincidenceTube.getCounts();

}

// ================================================================================================
// Get the number of coincidence events per hour
// ================================================================================================
uint32_t CosmicRayDetector::getCoincidenceEventsPerHour() {

  // Coincidence events per hour variable
  uint32_t cph = 0;

  // For all element in the moving average array
  for (uint8_t i = 0; i < 60; i++) {

    // Add to the sum of all elements
    cph += _movingAverage[i];

  }

  // Return the number of coincidence events per hour
  return cph;

}

// ================================================================================================
// Get the number of counts from the main tube since enabling the cosmic ray detector
// ================================================================================================
uint64_t CosmicRayDetector::getMainTubeCounts() {

  // Return the number of counts the main tube has recorded minus the set offset
  return geigerCounter.getMainTubeCounts() - _mainTubeOffset;

}

// ================================================================================================
// Get the number of counts from the follower tube since enabling the cosmic ray detector 
// ================================================================================================
uint64_t CosmicRayDetector::getFollowerTubeCounts() {

  // Return the number of counts the follower tube has recorded minus the set offset
  return geigerCounter.getFollowerTubeCounts() - _followerTubeOffset;

}

// ------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Constructor
// ================================================================================================
CosmicRayDetector::CosmicRayDetector():

  // Initialize members
  _initialized(false),
  _movingAverageIndex(0),
  _movingAverageTimer(NULL),
  _coincidenceTube(COINCIDENCE_TRG_PIN, _movingAverage, _movingAverageIndex),
  _enabled(false),
  _coincidenceTubeOffset(0),
  _mainTubeOffset(0),
  _followerTubeOffset(0)

{}

// ================================================================================================
// Interrupt service routine for advancing the moving average
// ================================================================================================
void IRAM_ATTR CosmicRayDetector::_advanceMovingAverage(void *instancePointer) {

  // Cast the generic instancePointer back to a instance pointer of type CosmicRayDetector 
  CosmicRayDetector *instance = (CosmicRayDetector*)instancePointer;

  // Calculate a wrapped index by using the current index + 1, if it overflows wrapped to the start of the array
  uint8_t wrappedIndex = (instance->_movingAverageIndex + 1) % 60;

  // Clear the next element in the array
  instance->_movingAverage[wrappedIndex] = 0;

  // Set the moving index to the next element in the array
  instance->_movingAverageIndex = wrappedIndex;

}