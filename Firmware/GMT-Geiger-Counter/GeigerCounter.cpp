#include "GeigerCounter.h"

// ------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Initialize everything
// ================================================================================================
void GeigerCounter::begin() {

  // Store the instance pointer
  this->_instance = this;

  // If main tube is enabled, set its pin mode to input
  if (ENABLE_MAIN_TUBE) { pinMode(MAIN_TRG_PIN, INPUT); }

  // If follower tube is enabled, set its pin mode to input
  if (ENABLE_FOLLOWER_TUBE) { pinMode(FOLLOWER_TRG_PIN, INPUT); }

}

// ================================================================================================
// Enable pulse counting
// ================================================================================================
void GeigerCounter::enable() {

  // If not already enabled
  if (!this->_countingEnabled) {
    
    // Clear the moving average array
    for (uint16_t i = 0; i < MOVING_AVERAGE_SIZE; i++) {
      this->_movingAverage[i] = 0;
    }

    // Reset the position of the moving average index
    this->_movingAverageIndex = 0;

    // TODO
    // Instead of using attachInterrupt, maybe using the EPS32 S3's RMT would be more efficient?

    // If the main tube is enabled, attach a hardware interrupt to the main tube pin to call the _handleMainInterrupt ISR
    if (ENABLE_MAIN_TUBE) {
      attachInterrupt(digitalPinToInterrupt(MAIN_TRG_PIN), this->_handleMainInterrupt, CHANGE);
    }

    // If the follower tube is enabled, attach a hardware interrupt to the follower tube pin to call the _handleFollowerInterrupt ISR
    if (ENABLE_FOLLOWER_TUBE) {
      attachInterrupt(digitalPinToInterrupt(FOLLOWER_TRG_PIN), this->_handleFollowerInterrupt, CHANGE);
    }

    // Set hardware timer frequency to 1Mhz
    this->_movingAverageTimer = timerBegin(1000000);

    // Attach the _handleTimerInterrupt ISR to the hardware timer
    timerAttachInterrupt(this->_movingAverageTimer, &this->_handleTimerInterrupt);

    // Set alarm to call the ISR function, every second, repeat, forever
    timerAlarm(this->_movingAverageTimer, 1000000, true, 0);

    // Set the enabled flag to true
    this->_countingEnabled = true;
  
  }

}

// ================================================================================================
// Disable pulse counting
// ================================================================================================
void GeigerCounter::disable() {

  // If enabled
  if (this->_countingEnabled) {

    // If the main tube is enabled, detach the hardware interrupt
    if (ENABLE_MAIN_TUBE) {
      detachInterrupt(digitalPinToInterrupt(MAIN_TRG_PIN));
    }

    // If the follower tube is enabled, detach the hardware interrupt
    if (ENABLE_FOLLOWER_TUBE) {
      detachInterrupt(digitalPinToInterrupt(FOLLOWER_TRG_PIN));
    }

    // Detach the ISR from the hardware timer
    timerDetachInterrupt(this->_movingAverageTimer);

    // Stop and free the hardware timer
    timerEnd(this->_movingAverageTimer);

    // Clear the hardware timer
    this->_movingAverageTimer = NULL;

    // Set the enabled flag to false
    this->_countingEnabled = false;

  }

}

// ================================================================================================
// Set the integration time
// ================================================================================================
void GeigerCounter::setIntegrationTime(uint8_t integrationTimeSeconds) {

  // If the provided integration time is larger than the moving average array size, set the integration time to the array size
  if (integrationTimeSeconds > MOVING_AVERAGE_SIZE) {

    // Set the integration time
    this->_integrationTimeSeconds = MOVING_AVERAGE_SIZE;

  }else {

    // Set the integration time
    this->_integrationTimeSeconds = integrationTimeSeconds;

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
  for (uint16_t i = 0; i < this->_integrationTimeSeconds - 1; i++) {

    // Calculate a wrapped index by using i + 1, if it overflows wrapped to the start of the array
    uint16_t wrappedIndex = (this->_movingAverageIndex - i + MOVING_AVERAGE_SIZE) % MOVING_AVERAGE_SIZE;

    // Add the count value of that element to the CPM variable
    cpm += this->_movingAverage[wrappedIndex];

  }

  // Divide the CPM value by the integration time and multiply by 60 to get the actual value for 60 seconds
  cpm = (cpm / this->_integrationTimeSeconds) * 60;

  return cpm;

}

// ================================================================================================
// Get the total number of counts since power on
// ================================================================================================
uint64_t GeigerCounter::getAccumulativeCount(){

  return this->_accumulativeCount;

}

// ================================================================================================
// Convert counts per minute to micro Sievert per hour
// ================================================================================================
double GeigerCounter::getMicroSievertPerHour() {

  // Multiply CPM by the conversion factor and divide by the number of tubes
  return (this->getCountsPerMinute() * CONVERSION_FACTOR_CPM_TO_USVH) / NUMBER_OF_TUBES;

}

// ------------------------------------------------------------------------------------------------
// Private

// Initialize static instance pointer
GeigerCounter* GeigerCounter::_instance = nullptr;  

// ================================================================================================
// ISR for counting the main pulse
// ================================================================================================
void IRAM_ATTR GeigerCounter::_countMainPulse() {

  // On the rising edge of the pulse
  if (digitalRead(MAIN_TRG_PIN) == HIGH) {

    // Capture the current time in microseconds
    this->_mainTubeTimerMicroseconds = micros();

  // On the falling edge
  }else {

    // Calculate the pulse length by subtracting the time from the rising edge to now in microseconds
    uint64_t pulseLengthMicroseconds = micros() - this->_mainTubeTimerMicroseconds;

    // Check if the pulse length is longer than the noise threshold
    if (pulseLengthMicroseconds > NOISE_THRESHOLD_MICROSECONDS) {

      // Add one count to the moving average
      this->_movingAverage[this->_movingAverageIndex]++;

      // Add one to the accumulative count stack
      this->_accumulativeCount++;

      // TODO
      // Adding just plus one to is not ideal. For just one tube, this is fine, but for multiple it is not. 
      // When daisy-chaining multiple tubes on one header, pulses from different tube might occur simultaneously or overlap.
      // To capture overlapping pulses, the total pulse length should be divided by the average singe pulse length for a tube to get the actual number of pulses.
      // This still needs to be implemented!

    }

  }

}

// ================================================================================================
// ISR for counting the follower pulse
// ================================================================================================
void IRAM_ATTR GeigerCounter::_countFollowerPulse() {

  // On the rising edge of the pulse
  if (digitalRead(FOLLOWER_TRG_PIN) == HIGH) {

    // Capture the current time in microseconds
    this->_followerTubeTimerMicroseconds = micros();

  // On the falling edge
  }else {

    // Calculate the pulse length by subtracting the time from the rising edge to now in microseconds
    uint64_t pulseLengthMicroseconds = micros() - this->_followerTubeTimerMicroseconds;

    // Check if the pulse length is longer than the noise threshold
    if (pulseLengthMicroseconds > NOISE_THRESHOLD_MICROSECONDS) {
      
      // Add one count to the moving average
      this->_movingAverage[this->_movingAverageIndex]++;

      // Add one to the accumulative count stack
      this->_accumulativeCount++;

      // TODO
      // Adding just plus one to is not ideal. For just one tube, this is fine, but for multiple it is not. 
      // When daisy-chaining multiple tubes on one header, pulses from different tube might occur simultaneously or overlap.
      // To capture overlapping pulses, the total pulse length should be divided by the average singe pulse length for a tube to get the actual number of pulses.
      // This still needs to be implemented!

    }

  }

}

// ================================================================================================
// Advance the moving average
// ================================================================================================
void IRAM_ATTR GeigerCounter::_advanceMovingAverage() {

  // Calculate a wrapped index by using the current index + 1, if it overflows wrapped to the start of the array
  uint16_t wrappedIndex = (this->_movingAverageIndex + 1) % MOVING_AVERAGE_SIZE;

  // Clear the next element in the array
  this->_movingAverage[wrappedIndex] = 0;

  // Set the moving index to the next element in the array
  this->_movingAverageIndex = wrappedIndex;

}

// ================================================================================================
// Static interrupt handler calls
// ================================================================================================
void IRAM_ATTR GeigerCounter::_handleMainInterrupt()     { _instance->_countMainPulse();       }
void IRAM_ATTR GeigerCounter::_handleFollowerInterrupt() { _instance->_countFollowerPulse();   }
void IRAM_ATTR GeigerCounter::_handleTimerInterrupt()    { _instance->_advanceMovingAverage(); }