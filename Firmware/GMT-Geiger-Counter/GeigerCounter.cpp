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
  _measurementUnit(MEASUREMENT_UNIT_SIEVERT),
  _metricPrefix(METRIC_PREFIX_MICRO),
  _autoRange(true),
  _movingAverageIndex(0),
  _movingAverageTimer(NULL),
  _mainTube(MAIN_TRG_PIN, _movingAverage, _movingAverageIndex),
  _followerTube(FOLLOWER_TRG_PIN, _movingAverage, _movingAverageIndex)

{}

// ================================================================================================
// Initialize everything
// ================================================================================================
void GeigerCounter::begin() {

  // Initialize logger if not already
  logger.begin();

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

    // Get logger event data
    Logger::KeyValuePair event[2] = {

      {"event",   Logger::STRING, {.string_value = "geigerCounter"}},
      {"enabled", Logger::BOOL,   {.bool_value   = true }          }

    };

    // Log geiger counter enable event
    logger.event(event, 2);

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

    // Get logger event data
    Logger::KeyValuePair event[2] = {

      {"event",   Logger::STRING, {.string_value = "geigerCounter"}},
      {"enabled", Logger::BOOL,   {.bool_value   = false }         }

    };

    // Log geiger counter disable event
    logger.event(event, 2);

    // Set the enabled flag to false
    _enabled = false;

  }

}

// ================================================================================================
// Returns if the Geiger counter is enabled
// ================================================================================================
bool GeigerCounter::enabled() {

  return _enabled;

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
// Set the measurement unit of the equivalent dose
// ================================================================================================
void GeigerCounter::setMeasurementUnit(const MeasurementUnit unit) {

  _measurementUnit = unit;

}

// ================================================================================================
// Set if the equivalent dose should auto range
// ================================================================================================
void GeigerCounter::setAutoRangeState(const bool state) {

  _autoRange = state;

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
  cpm = (cpm / _integrationTimeSeconds) * 60.0;

  // Return the counts per minute
  return cpm;

}

// ================================================================================================
// Convert counts per minute to Microsieverts per hour
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

    case MEASUREMENT_UNIT_SIEVERT: equivalentDose = getMicrosievertsPerHour();         break;
    case MEASUREMENT_UNIT_REM:     equivalentDose = getMicrosievertsPerHour() * 100.0; break;
    case MEASUREMENT_UNIT_RONTGEN: equivalentDose = getMicrosievertsPerHour() * 100.0; break;
    case MEASUREMENT_UNIT_GRAY:    equivalentDose = getMicrosievertsPerHour();         break;

  }
  
  // Set the micro prefix
  _metricPrefix = METRIC_PREFIX_MICRO;

  // If auto range is enabled
  if (_autoRange) {

    // If equivalent dose is >= 500 milli
    // Divide value to convert to base unit and set metric prefix
    if (equivalentDose >= 500000) {

      equivalentDose /= 1000000.0;
      _metricPrefix   = METRIC_PREFIX_BASE;
    
    // If equivalent dose is >= 500 micro
    // Divide value to convert to milli unit and set metric prefix
    } else if (equivalentDose >= 500) {

      equivalentDose /= 1000.0;
      _metricPrefix   = METRIC_PREFIX_MILLI;

    }

  }

  // Return the equivalent dose
  return equivalentDose;

}

// ================================================================================================
// 
// ================================================================================================
const char* GeigerCounter::getEquivalentDoseUnit() {

  // Unit string
  const char *unit = STRING_MICRO_SIEVERTS_PER_HOUR_ABBREVIATION;

  // Depending on the measurement unit and the metric prefix
  // Select a unit string
  // (A little messy but works)
  switch (_measurementUnit) {

    case MEASUREMENT_UNIT_SIEVERT:

      switch (_metricPrefix) {

        case METRIC_PREFIX_MICRO: unit = STRING_MICRO_SIEVERTS_PER_HOUR_ABBREVIATION; break;
        case METRIC_PREFIX_MILLI: unit = STRING_MILLI_SIEVERTS_PER_HOUR_ABBREVIATION; break;
        case METRIC_PREFIX_BASE:  unit = STRING_SIEVERTS_PER_HOUR_ABBREVIATION;       break;

      }

    break;

    case MEASUREMENT_UNIT_REM:

      switch (_metricPrefix) {

        case METRIC_PREFIX_MICRO: unit = STRING_MICRO_REM_PER_HOUR_ABBREVIATION; break;
        case METRIC_PREFIX_MILLI: unit = STRING_MILLI_REM_PER_HOUR_ABBREVIATION; break;
        case METRIC_PREFIX_BASE:  unit = STRING_REM_PER_HOUR_ABBREVIATION;       break;

      }

    break;

    case MEASUREMENT_UNIT_RONTGEN:

      switch (_metricPrefix) {

        case METRIC_PREFIX_MICRO: unit = STRING_MICRO_RONTGEN_PER_HOUR_ABBREVIATION; break;
        case METRIC_PREFIX_MILLI: unit = STRING_MILLI_RONTGEN_PER_HOUR_ABBREVIATION; break;
        case METRIC_PREFIX_BASE:  unit = STRING_RONTGEN_PER_HOUR_ABBREVIATION;       break;

      }

    break;

    case MEASUREMENT_UNIT_GRAY:

      switch (_metricPrefix) {

        case METRIC_PREFIX_MICRO: unit = STRING_MICRO_GRAY_PER_HOUR_ABBREVIATION; break;
        case METRIC_PREFIX_MILLI: unit = STRING_MILLI_GRAY_PER_HOUR_ABBREVIATION; break;
        case METRIC_PREFIX_BASE:  unit = STRING_GRAY_PER_HOUR_ABBREVIATION;       break;

      }

    break;
    
  }

  // Return the unit string
  return unit;

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

  RadiationRating rating = RADIATION_RATING_NORMAL;

  // Normal rating
  if (microsievertsPerHours < RADIATION_RATING_ELEVATED_USVH) {

    rating = RADIATION_RATING_NORMAL;

  // Elevated rating
  } else if (microsievertsPerHours >= RADIATION_RATING_ELEVATED_USVH && microsievertsPerHours < RADIATION_RATING_MEDIUM_USVH) {

    rating = RADIATION_RATING_ELEVATED;
  
  // Medium rating
  } else if (microsievertsPerHours >= RADIATION_RATING_MEDIUM_USVH && microsievertsPerHours < RADIATION_RATING_HIGH_USVH) {

    rating = RADIATION_RATING_MEDIUM;

  // High rating
  } else if (microsievertsPerHours >= RADIATION_RATING_HIGH_USVH && microsievertsPerHours < RADIATION_RATING_EXTREME_USVH) {

    rating = RADIATION_RATING_HIGH;
  
  // Extreme rating
  } else if (microsievertsPerHours >= RADIATION_RATING_EXTREME_USVH) {

    rating = RADIATION_RATING_EXTREME;

  }

  return rating;

}

// ================================================================================================
// Get the auto range state
// ================================================================================================
bool GeigerCounter::getAutoRangeState() {

  return _autoRange;

}

// ------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Advance the moving average
// ================================================================================================
void IRAM_ATTR GeigerCounter::_advanceMovingAverage(void *instancePointer) {

  // Cast the generic instance pointer back to a instance pointer of type GeigerCounter 
  GeigerCounter *instance = (GeigerCounter*)instancePointer;

  // Calculate a wrapped index by using the current index + 1, if it overflows wrapped to the start of the array
  uint8_t wrappedIndex = (instance->_movingAverageIndex + 1) % 60;

  // Clear the next element in the array
  instance->_movingAverage[wrappedIndex] = 0;

  // Set the moving index to the next element in the array
  instance->_movingAverageIndex = wrappedIndex;

}