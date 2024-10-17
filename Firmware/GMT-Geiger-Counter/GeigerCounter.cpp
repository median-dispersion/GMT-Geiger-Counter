#include "GeigerCounter.h"

// Initialize static instance pointer
GeigerCounter *GeigerCounter::_instance = nullptr;  

// ------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
GeigerCounter::GeigerCounter():

  // Initialize members
  _movingAverageIndex(0),
  _movingAverageTimer(NULL),
  _accumulativeCount(0),
  _enabled(false),
  _integrationTimeSeconds(DEFAULT_INTEGRATION_TIME),
  _mainTube(MAIN_TRG_PIN, _movingAverage, _movingAverageIndex, _accumulativeCount),
  _followerTube(FOLLOWER_TRG_PIN, _movingAverage, _movingAverageIndex, _accumulativeCount)

{}

// ================================================================================================
// Initialize everything
// ================================================================================================
void GeigerCounter::begin() {

  // Set the static instance pointer to this object instance
  this->_instance = this;

  // If main tube is enabled, initialize main tube
  #if ENABLE_MAIN_TUBE == 1
    _mainTube.begin();
  #endif

  // If follower tube is enabled, initialize follower tube
  #if ENABLE_FOLLOWER_TUBE == 1
    _followerTube.begin();
  #endif

}

// ================================================================================================
// Enable the Geiger counter
// ================================================================================================
void GeigerCounter::enable() {

  // If not already enabled
  if (!_enabled) {

    // Clear the moving average array
    for (uint16_t i = 0; i < MOVING_AVERAGE_SIZE; i++) {
      _movingAverage[i] = 0;
    }

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

    // Attach the _handleInterrupt ISR to the hardware timer
    timerAttachInterrupt(_movingAverageTimer, _handleInterrupt);

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
    timerDetachInterrupt(this->_movingAverageTimer);

    // Stop and free the hardware timer
    timerEnd(this->_movingAverageTimer);

    // Clear the hardware timer
    this->_movingAverageTimer = NULL;

    // Set the enabled flag to false
    _enabled = false;

  }

}

// ================================================================================================
// Set the integration time
// ================================================================================================
void GeigerCounter::setIntegrationTime(uint16_t timeSeconds) {

  // If the provided integration time is larger than the moving average array size, set the integration time to the array size
  if (timeSeconds > MOVING_AVERAGE_SIZE) {

    // Set the integration time
    _integrationTimeSeconds = MOVING_AVERAGE_SIZE;

  }else {

    // Set the integration time
    _integrationTimeSeconds = timeSeconds;

  }

}

// ================================================================================================
// Get the counts per minute
// ================================================================================================
double GeigerCounter::getCountsPerMinute() {

  // CPM variable
  double cpm = 0.0;

  // For the number of seconds in the integration time
  // This will select the number of elements from the moving average array to sum up an average over
  for (uint16_t i = 0; i < _integrationTimeSeconds - 1; i++) {

    // Calculate a wrapped index by using i + 1, if it overflows wrapped to the start of the array
    uint16_t wrappedIndex = (_movingAverageIndex - i + MOVING_AVERAGE_SIZE) % MOVING_AVERAGE_SIZE;

    // Add the count value of that element to the CPM variable
    cpm += _movingAverage[wrappedIndex];

  }

  // Divide the CPM value by the integration time and multiply by 60 to get the actual value for 60 seconds
  cpm = (cpm / _integrationTimeSeconds) * 60;

  return cpm;

}

// ================================================================================================
// Get the total number of recorded counts since power on
// ================================================================================================
uint64_t GeigerCounter::getAccumulativeCount(){

  return _accumulativeCount;

}

// ================================================================================================
// Convert counts per minute to micro Sievert per hour
// ================================================================================================
double GeigerCounter::getMicroSievertPerHour() {

  // Multiply CPM by the conversion factor and divide by the number of tubes
  return (getCountsPerMinute() * CONVERSION_FACTOR_CPM_TO_USVH) / NUMBER_OF_TUBES;

}

// ================================================================================================
// Get the set integration time
// ================================================================================================
uint16_t GeigerCounter::getIntegrationTime() {

  return _integrationTimeSeconds;

}

// ------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Interrupt service routine for handling hardware interrupts
// ================================================================================================
void IRAM_ATTR GeigerCounter::_handleInterrupt() {

  _instance->_advanceMovingAverage();

}

// ================================================================================================
// Advance the moving average
// ================================================================================================
void IRAM_ATTR GeigerCounter::_advanceMovingAverage() {

  // Calculate a wrapped index by using the current index + 1, if it overflows wrapped to the start of the array
  uint16_t wrappedIndex = (_movingAverageIndex + 1) % MOVING_AVERAGE_SIZE;

  // Clear the next element in the array
  _movingAverage[wrappedIndex] = 0;

  // Set the moving index to the next element in the array
  _movingAverageIndex = wrappedIndex;

}