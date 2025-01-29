#include "GeigerCounter.h"

// ------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
GeigerCounter::GeigerCounter():

  // Initialize members
  _enabled(false),
  _integrationTimeSeconds(INTEGRATION_TIME_DEFAULT_SECONDS),
  _movingAverageIndex(0),
  _movingAverageTimer(NULL),
  _mainTube(MAIN_TRG_PIN, _movingAverage, _movingAverageIndex),
  _followerTube(FOLLOWER_TRG_PIN, _movingAverage, _movingAverageIndex)

{}

// ================================================================================================
// Initialize everything
// ================================================================================================
void GeigerCounter::begin() {

  // Initialize all tubes
  _mainTube.begin();
  _followerTube.begin();

}

// ================================================================================================
// Enable the Geiger counter
// ================================================================================================
void GeigerCounter::enable() {

  // If not already enabled
  if (!_enabled) {

    // Clear the moving average array
    for (uint8_t i = 0; i < 60; i++) { _movingAverage[i] = 0; }

    // Reset the position of the moving average index
    _movingAverageIndex = 0;

    // If main tube is enabled, enable pulse counting
    #if ENABLE_MAIN_TUBE == 1
      _mainTube.enable();
    #endif

    // If follower tube is enabled, enable pulse counting
    #if ENABLE_FOLLOWER_TUBE == 1
      _followerTube.enable();
    #endif

    // Set hardware timer frequency to 1Mhz
    _movingAverageTimer = timerBegin(1000000);

    // Attach the _advanceMovingAverage ISR to the hardware timer and pass the class instance pointer to it
    timerAttachInterruptArg(_movingAverageTimer, _advanceMovingAverage, this);

    // Set alarm to call the ISR function, every second, repeat, forever
    timerAlarm(_movingAverageTimer, 1000000, true, 0);

    // Set the enabled flag to true
    _enabled = true;

  }

}

// ================================================================================================
// Disable the Geiger counter
// ================================================================================================
void GeigerCounter::disable() {

  // If not already disabled
  if (_enabled) {

    // If main tube is enabled, disable pulse counting
    #if ENABLE_MAIN_TUBE == 1
      _mainTube.disable();
    #endif

    // If follower tube is enabled, disable pulse counting
    #if ENABLE_FOLLOWER_TUBE == 1
      _followerTube.disable();
    #endif

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
// Set the integration time
// ================================================================================================
void GeigerCounter::setIntegrationTime(uint16_t timeSeconds) {

  // If the provided integration time is larger than the moving average array size, set the integration time to the array size
  if (timeSeconds > 60) {

    // Set the integration time
    _integrationTimeSeconds = 60;

  // If the provided integration time is smaller than 1 set the integration time to 1
  } else if (timeSeconds < 1) {

    // Set the integration time
    _integrationTimeSeconds = 1;

  } else {

    // Set the integration time
    _integrationTimeSeconds = timeSeconds;

  }

}

// ================================================================================================
// Get the total number of counts
// ================================================================================================
uint64_t GeigerCounter::getCounts() {

  // Combine and return total number of counts for all tubes
  return _mainTube.getCounts() + _followerTube.getCounts();

}

// ================================================================================================
// Get the total number of counts for the main tube
// ================================================================================================
uint64_t GeigerCounter::getMainTubeCounts() {

  // Get and return the total number of counts for the main tube
  return _mainTube.getCounts();

}

// ================================================================================================
// Get the total number of counts for the follower tube
// ================================================================================================
uint64_t GeigerCounter::getFollowerTubeCounts() {

  // Get and return the total number of counts for the follower tube
  return _followerTube.getCounts();

}

// ================================================================================================
// Get the counts per minute
// ================================================================================================
double GeigerCounter::getCountsPerMinute() {

  // CPM variable
  double cpm = 0.0;

  // For the number of seconds in the integration time
  // This will select the number of elements from the moving average array to sum up an average over
  for (uint8_t i = 0; i < _integrationTimeSeconds; i++) {

    // Calculate a wrapped index by using i + 60 - 1, if it underflows wrapped to the end of the array
    uint8_t wrappedIndex = (_movingAverageIndex + 60 - i) % 60;

    // Add the count value of that element to the CPM variable
    cpm += _movingAverage[wrappedIndex];

  }

  // Divide the CPM value by the integration time and multiply by 60 to get the actual value for 60 seconds
  cpm = (cpm / _integrationTimeSeconds) * 60;

  // Return the counts per minute
  return cpm;

}

// ================================================================================================
// Convert counts per minute to microsieverts per hour
// ================================================================================================
double GeigerCounter::getMicrosievertsPerHour() {

  // Multiply CPM by the conversion factor and divide by the number of tubes
  return (getCountsPerMinute() * TUBE_CONVERSION_FACTOR_CPM_TO_USVH) / TOTAL_NUMBER_OF_TUBES;

}

// ================================================================================================
// Get the integration time
// ================================================================================================
uint8_t GeigerCounter::getIntegrationTime() {

  // Get and return the integration time
  return _integrationTimeSeconds;

}

// ================================================================================================
// Get the radiation rating
// ================================================================================================
GeigerCounter::RadiationRating GeigerCounter::getRadiationRating() {

  double microsievertsPerHours = getMicrosievertsPerHour();

  // Normal rating
  if (microsievertsPerHours < RADIATION_RATING_ELEVATED_USVH) {

    return RADIATION_RATING_NORMAL;

  // Elevated rating
  } else if (microsievertsPerHours >= RADIATION_RATING_ELEVATED_USVH && microsievertsPerHours < RADIATION_RATING_HIGH_USVH) {

    return RADIATION_RATING_ELEVATED;

  // High rating
  } else if (microsievertsPerHours >= RADIATION_RATING_HIGH_USVH && microsievertsPerHours < RADIATION_RATING_EXTREME_USVH) {

    return RADIATION_RATING_HIGH;
  
  // Extreme rating
  } else if (microsievertsPerHours >= RADIATION_RATING_EXTREME_USVH) {

    return RADIATION_RATING_EXTREME;

  }

}

// ------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Advance the moving average
// ================================================================================================
void IRAM_ATTR GeigerCounter::_advanceMovingAverage(void *instancePointer) {

  // Cast the generic instance pointer back to a instance pointe of type GeigerCounter 
  GeigerCounter *instance = (GeigerCounter*)instancePointer;

  // Calculate a wrapped index by using the current index + 1, if it overflows wrapped to the start of the array
  uint8_t wrappedIndex = (instance->_movingAverageIndex + 1) % 60;

  // Clear the next element in the array
  instance->_movingAverage[wrappedIndex] = 0;

  // Set the moving index to the next element in the array
  instance->_movingAverageIndex = wrappedIndex;

}