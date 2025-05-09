#include "GeigerCounter.h"

// ------------------------------------------------------------------------------------------------
// Public

// Initialize global reference
GeigerCounter& geigerCounter = GeigerCounter::getInstance();

// ================================================================================================
// Get the single instance of the class
// ================================================================================================
GeigerCounter& GeigerCounter::getInstance() {

  // Get the single instance
  static GeigerCounter instance;

  // Return the instance
  return instance;

}

// ================================================================================================
// Initialize everything
// ================================================================================================
void GeigerCounter::begin() {

  // If not initalized
  if (!_initialized) {

    // Set initialization flag to true
    _initialized = true;

    // Initialize logger
    logger.begin();

    // Initialize all tubes
    _mainTube.begin();
    _followerTube.begin();

  }

}

// ================================================================================================
// Enable the Geiger counter
// ================================================================================================
void GeigerCounter::enable() {

  // If not enabled
  if (!_enabled) {

    // Clear the moving average array
    for (uint8_t i = 0; i < 60; i++) { _movingAverage[i] = 0; }

    // Reset the position of the moving average index
    _movingAverageIndex = 0;

    // Set all elements in the radiation history array to an impossibly high value marking them as cleared
    for (uint8_t sample = 0; sample < RADIATION_HISTORY_LENGTH_MINUTES; sample++) { _history[sample] = UINT32_MAX; }

    // If main tube is enabled in the main configuration file, enable pulse counting
    #if ENABLE_MAIN_TUBE == 1
      _mainTube.enable();
    #endif

    // If follower tube is enabled in the main configuration file, enable pulse counting
    #if ENABLE_FOLLOWER_TUBE == 1
      _followerTube.enable();
    #endif

    // Set hardware timer frequency to 1Mhz
    _movingAverageTimer = timerBegin(1000000);

    // Attach the update ISR to the hardware timer and pass the class instance pointer to it
    timerAttachInterruptArg(_movingAverageTimer, _update, this);

    // Set alarm to call the ISR function, every second, repeat, forever
    timerAlarm(_movingAverageTimer, 1000000, true, 0);

    // Set the enabled flag to true
    _enabled = true;

    // Create event data
    Logger::KeyValuePair event[2] = {

      {"source", Logger::STRING_T, {.string_v = "geigerCounter"}},
      {"action", Logger::STRING_T, {.string_v = "enabled"}      }

    };

    // Log event message
    logger.log(Logger::EVENT, "event", event, 2);

  }

}

// ================================================================================================
// Disable the Geiger counter
// ================================================================================================
void GeigerCounter::disable() {

  // If not disabled
  if (_enabled) {

    // If main tube is enabled in the main configuration file, disable pulse counting
    #if ENABLE_MAIN_TUBE == 1
      _mainTube.disable();
    #endif

    // If follower tube is enabled in the main configuration file, disable pulse counting
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

    // Create event data
    Logger::KeyValuePair event[2] = {

      {"source", Logger::STRING_T, {.string_v = "geigerCounter"}},
      {"action", Logger::STRING_T, {.string_v = "disabled"}     }

    };

    // Log event message
    logger.log(Logger::EVENT, "event", event, 2);

  }

}

// ================================================================================================
// Set the Geiger counter state
// ================================================================================================
void GeigerCounter::setGeigerCounterState(const bool state) {

  // Depending on the state either enable or disable the Geiger counter
  if (state) {

    enable();

  } else {

    disable();

  }

}

// ================================================================================================
// Set the integration time
// ================================================================================================
void GeigerCounter::setIntegrationTime(const uint8_t timeSeconds) {

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
// Set the state of the automatic integration time adjustment
// ================================================================================================
void GeigerCounter::setAutoIntegrateState(const bool state) {

  _autoIntegrate = state;

}

// ================================================================================================
// Set if the equivalent dose should auto range
// ================================================================================================
void GeigerCounter::setAutoRangeState(const bool state) {

  _autoRange = state;

}

// ================================================================================================
// Set the measurement unit of the equivalent dose
// ================================================================================================
void GeigerCounter::setMeasurementUnit(const MeasurementUnit unit) {

  _measurementUnit = unit;

}

// ================================================================================================
// Get the Geiger counter state
// ================================================================================================
bool GeigerCounter::getGeigerCounterState() {

  return _enabled;

}

// ================================================================================================
// Get the set integration time
// ================================================================================================
uint8_t GeigerCounter::getIntegrationTime() {

  return _integrationTimeSeconds;

}

// ================================================================================================
// Get the state of the automatic integration time adjustment
// ================================================================================================
bool GeigerCounter::getAutoIntegrateState() {

  return _autoIntegrate;

}

// ================================================================================================
// Get the auto ranging state
// ================================================================================================
bool GeigerCounter::getAutoRangeState() {

  return _autoRange;

}

// ================================================================================================
// Get the measurement unit
// ================================================================================================
GeigerCounter::MeasurementUnit GeigerCounter::getMeasurementUnit() {

  return _measurementUnit;

}

// ================================================================================================
// Get the total number of counts
// ================================================================================================
uint64_t GeigerCounter::getCounts() {

  // Combine and return total number of counts for all tubes
  return _mainTube.getCounts() + _followerTube.getCounts(); 

}

// ================================================================================================
// Get the number of counts the main tube has recorded
// ================================================================================================
uint64_t GeigerCounter::getMainTubeCounts() {

  // Get and return the total number of counts for the main tube
  return _mainTube.getCounts();

}

// ================================================================================================
// Get the number of counts the follower tube has recorded
// ================================================================================================
uint64_t GeigerCounter::getFollowerTubeCounts() {

  // Get and return the total number of counts for the follower tube
  return _followerTube.getCounts();

}

// ================================================================================================
// Get the counts per minute for a fixed integration time
// ================================================================================================
double GeigerCounter::getCountsPerMinute(const uint8_t timeSeconds) {

  // CPM variable
  double cpm = 0.0;

  // For the number of seconds in the integration time
  // This will select the number of elements from the moving average array to sum up an average over
  for (uint8_t i = 0; i < timeSeconds; i++) {

    // Calculate a wrapped index by using i + 60 - 1, if it underflows wrapped to the end of the array
    uint8_t wrappedIndex = (_movingAverageIndex + 60 - i) % 60;

    // Add the count value of that element to the CPM variable
    cpm += _movingAverage[wrappedIndex];

  }

  // Divide the CPM value by the integration time and multiply by 60 to get the actual value for 60 seconds
  cpm = (cpm / timeSeconds) * 60.0;

  // Return the counts per minute
  return cpm;

}

// ================================================================================================
// Get the counts per minute
// ================================================================================================
double GeigerCounter::getCountsPerMinute() {

  // If automatically adjusting the integration time is enabled
  if (_autoIntegrate) {

    // If at least one second has passed since the last adjustment
    if (millis() - _autoIntegrationTimer >= 1000) {

      // Get the ratio between a shortterm and longterm counts per minute readings
      float ratio = getCountsPerMinute(10) / getCountsPerMinute(60);

      // Set the default integration time as a target
      uint8_t target = INTEGRATION_TIME_AUTO_AVERAGE_SECONDS;

      // Depending on the ration between the short and longterm counts per minute value
      // Set the target integration time to its minimum or its maximum
      if      (ratio > INTEGRATION_TIME_AUTO_UPPER_BOUND) { target = INTEGRATION_TIME_AUTO_MINIMUM_SECONDS; }
      else if (ratio < INTEGRATION_TIME_AUTO_LOWER_BOUND) { target = INTEGRATION_TIME_AUTO_MAXIMUM_SECONDS; }
      else                                                { target = INTEGRATION_TIME_AUTO_AVERAGE_SECONDS; }

      // Get the current integration time
      uint8_t integrationTime = getIntegrationTime();

      // Increase or decrease the current integration time by the step size to reach the target
      if      (integrationTime < target) { integrationTime += INTEGRATION_TIME_STEP_SIZE_SECONDS; }
      else if (integrationTime > target) { integrationTime -= INTEGRATION_TIME_STEP_SIZE_SECONDS; }

      // Clip the integration time to the minimum or maximum if necessary
      if      (integrationTime < INTEGRATION_TIME_AUTO_MINIMUM_SECONDS) { integrationTime = INTEGRATION_TIME_AUTO_MINIMUM_SECONDS; }
      else if (integrationTime > INTEGRATION_TIME_AUTO_MAXIMUM_SECONDS) { integrationTime = INTEGRATION_TIME_AUTO_MAXIMUM_SECONDS; }

      // Set the new integration time
      setIntegrationTime(integrationTime);

      // Update the timer
      _autoIntegrationTimer = millis();

    }

  }

  // Get the counts per minute value with the currently set integration time
  return getCountsPerMinute(getIntegrationTime());

}

// ================================================================================================
// Get microsieverts per hour for a fixed integration time
// ================================================================================================
double GeigerCounter::getMicrosievertsPerHour(const uint8_t timeSeconds) {

  // Multiply CPM by the conversion factor and divide by the number of tubes
  return (getCountsPerMinute(timeSeconds) * TUBE_CONVERSION_FACTOR_CPM_TO_USVH) / TOTAL_NUMBER_OF_TUBES;

}

// ================================================================================================
// Get microsieverts per hour
// ================================================================================================
double GeigerCounter::getMicrosievertsPerHour() {

  // Multiply CPM by the conversion factor and divide by the number of tubes
  return (getCountsPerMinute() * TUBE_CONVERSION_FACTOR_CPM_TO_USVH) / TOTAL_NUMBER_OF_TUBES;

}

// ================================================================================================
// Get the equivalent dose in the selected measurement unit
// ================================================================================================
double GeigerCounter::getEquivalentDose() {

  // Equivalent dose variable
  double equivalentDose = 0.0;

  // Depending on the selected measurement unit
  // Convert the microsieverts per hour value to that unit
  // Store the resulting value in the equivalent dose variable
  switch (_measurementUnit) {

    case SIEVERTS: equivalentDose = getMicrosievertsPerHour();         break;
    case REM:      equivalentDose = getMicrosievertsPerHour() * 100.0; break;
    case RONTGEN:  equivalentDose = getMicrosievertsPerHour() * 100.0; break;
    case GRAY:     equivalentDose = getMicrosievertsPerHour();         break;

  }

  // If auto range is enabled
  if (_autoRange) {

    // If equivalent dose is >= 500 milli
    // Divide value to convert to base unit and set metric prefix
    if (equivalentDose >= 500000) {

      equivalentDose /= 1000000.0;
      _metricPrefix   = METRIC_BASE;
    
    // If equivalent dose is >= 500 micro
    // Divide value to convert to milli unit and set metric prefix
    } else if (equivalentDose >= 500) {

      equivalentDose /= 1000.0;
      _metricPrefix   = METRIC_MILLI;

    }

  }

  // Return the equivalent dose
  return equivalentDose;

}

// ================================================================================================
// Get the equivalent dose unit
// ================================================================================================
GeigerCounter::EquivalentDoseUnit GeigerCounter::getEquivalentDoseUnit() {

  // Selected unit
  EquivalentDoseUnit unit = MICRO_SIEVERTS_PER_HOUR;

  // Depending on the measurement unit and the metric prefix
  // Select a dose unit
  // (Still a little messy but works)
  switch (_measurementUnit) {

    case SIEVERTS:

      switch (_metricPrefix) {

        case METRIC_MICRO: unit = MICRO_SIEVERTS_PER_HOUR; break;
        case METRIC_MILLI: unit = MILLI_SIEVERTS_PER_HOUR; break;
        case METRIC_BASE:  unit = SIEVERTS_PER_HOUR;       break;

      }

    break;

    case REM:

      switch (_metricPrefix) {

        case METRIC_MICRO: unit = MICRO_REM_PER_HOUR; break;
        case METRIC_MILLI: unit = MILLI_REM_PER_HOUR; break;
        case METRIC_BASE:  unit = REM_PER_HOUR;       break;

      }

    break;

    case RONTGEN:

      switch (_metricPrefix) {

        case METRIC_MICRO: unit = MICRO_RONTGEN_PER_HOUR; break;
        case METRIC_MILLI: unit = MILLI_RONTGEN_PER_HOUR; break;
        case METRIC_BASE:  unit = RONTGEN_PER_HOUR;       break;

      }

    break;

    case GRAY:

      switch (_metricPrefix) {

        case METRIC_MICRO: unit = MICRO_GRAY_PER_HOUR; break;
        case METRIC_MILLI: unit = MILLI_GRAY_PER_HOUR; break;
        case METRIC_BASE:  unit = GRAY_PER_HOUR;       break;

      }

    break;

  }

  // Return selected unit
  return unit;

}

// ================================================================================================
// Get the radiation rating
// ================================================================================================
GeigerCounter::RadiationRating GeigerCounter::getRadiationRating() {

  // Get microsieverts per hour measurement
  double microsievertsPerHours = getMicrosievertsPerHour();

  RadiationRating rating = RATING_NORMAL;

  // Normal rating
  if (microsievertsPerHours < RADIATION_RATING_ELEVATED_USVH) {

    rating = RATING_NORMAL;

  // Elevated rating
  } else if (microsievertsPerHours >= RADIATION_RATING_ELEVATED_USVH && microsievertsPerHours < RADIATION_RATING_MEDIUM_USVH) {

    rating = RATING_ELEVATED;
  
  // Medium rating
  } else if (microsievertsPerHours >= RADIATION_RATING_MEDIUM_USVH && microsievertsPerHours < RADIATION_RATING_HIGH_USVH) {

    rating = RATING_MEDIUM;

  // High rating
  } else if (microsievertsPerHours >= RADIATION_RATING_HIGH_USVH && microsievertsPerHours < RADIATION_RATING_EXTREME_USVH) {

    rating = RATING_HIGH;
  
  // Extreme rating
  } else if (microsievertsPerHours >= RADIATION_RATING_EXTREME_USVH) {

    rating = RATING_EXTREME;

  }

  return rating;

}

// ================================================================================================
// Get a pointer to the radiation history array
// ================================================================================================
uint32_t* GeigerCounter::getHistory() {

  return _history;

}

// ================================================================================================
// Get the index of the radiation history array
// ================================================================================================
uint8_t GeigerCounter::getHistoryIndex() {

  return _historyIndex;

}

// ------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Constructor
// ================================================================================================
GeigerCounter::GeigerCounter():

  // Initialize members
  _initialized(false),
  _movingAverageIndex(0),
  _movingAverageTimer(NULL),
  _mainTube(MAIN_TRG_PIN, _movingAverage, _movingAverageIndex),
  _followerTube(FOLLOWER_TRG_PIN, _movingAverage, _movingAverageIndex),
  _enabled(false),
  _integrationTimeSeconds(INTEGRATION_TIME_AUTO_AVERAGE_SECONDS),
  _autoIntegrate(true),
  _autoIntegrationTimer(0),
  _autoRange(true),
  _measurementUnit(SIEVERTS),
  _metricPrefix(METRIC_MICRO),
  _historyIndex(0),
  _historyTimerSeconds(0)

{}

// ================================================================================================
// Interrupt service routine for updating the Geiger counter
// ================================================================================================
void IRAM_ATTR GeigerCounter::_update(void *instancePointer) {

  // Cast the generic instance pointer back to a instance pointer of type GeigerCounter 
  GeigerCounter *instance = (GeigerCounter*)instancePointer;

  // Calculate a wrapped index by using the current index + 1, if it overflows wrapped to the start of the array
  uint8_t wrappedIndex = (instance->_movingAverageIndex + 1) % 60;

  // Clear the next element in the array
  instance->_movingAverage[wrappedIndex] = 0;

  // Set the moving index to the next element in the array
  instance->_movingAverageIndex = wrappedIndex;

  // Increase the history timer by 1 second
  instance->_historyTimerSeconds++;

  // If one minute on the history timer has passed
  if (instance->_historyTimerSeconds >= 60) {

    // Set the element to the current counts per minute value
    instance->_history[instance->_historyIndex] = instance->getCountsPerMinute(60);

    // Set the history index to the next element in the array
    instance->_historyIndex = (instance->_historyIndex + 1) % RADIATION_HISTORY_LENGTH_MINUTES;

    // Reset the history timer
    instance->_historyTimerSeconds = 0;

  }

}