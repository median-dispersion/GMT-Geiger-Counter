#include "RandomNumberGenerator.h"

// ------------------------------------------------------------------------------------------------
// Public

// Initialize global reference
RandomNumberGenerator& randomNumberGenerator = RandomNumberGenerator::getInstance();

// ================================================================================================
// Get the single instance of the class
// ================================================================================================
RandomNumberGenerator& RandomNumberGenerator::getInstance() {

  // Get the single instance
  static RandomNumberGenerator instance;

  // Return the instance
  return instance;

}

// ================================================================================================
// Initialize everything
// ================================================================================================
void RandomNumberGenerator::begin() {

  // If not initalized
  if (!_initialized) {

    // Set initialization flag to true
    _initialized = true;

    // Initialize logger
    logger.begin();

    // Initialize the random tube
    _randomTube.begin();

  }

}

// ================================================================================================
// Enable the random number generator
// ================================================================================================
void RandomNumberGenerator::enable() {

  // If not enabled
  if (!_enabled) {

    // Enable the random tube
    _randomTube.enable();

    // Reset parameters
    _rolling      = false;
    _stale        = true;
    _result       = 0;
    _capturedBits = 0;
    _latestBit    = 0;

    // Set the enabled flag to true
    _enabled = true;

    // Create event data
    Logger::KeyValuePair event[2] = {

      {"source", Logger::STRING_T, {.string_v = "randomNumberGenerator"}},
      {"action", Logger::STRING_T, {.string_v = "enabled"}              }

    };

    // Log event message
    logger.log(Logger::EVENT, "event", event, 2);

  }

}

// ================================================================================================
// Disable the random number generator
// ================================================================================================
void RandomNumberGenerator::disable() {

  // If not disabled
  if (_enabled) {

    // Disable the random tube
    _randomTube.disable();

    // Reset parameters
    _rolling      = false;
    _stale        = true;
    _result       = 0;
    _capturedBits = 0;
    _latestBit    = 0;

    // Set the enabled flag to false
    _enabled = false;

    // Create event data
    Logger::KeyValuePair event[2] = {

      {"source", Logger::STRING_T, {.string_v = "randomNumberGenerator"}},
      {"action", Logger::STRING_T, {.string_v = "disabled"}             }

    };

    // Log event message
    logger.log(Logger::EVENT, "event", event, 2);

  }

}

// ================================================================================================
// Update the random number generator
// ================================================================================================
void RandomNumberGenerator::update() {

  // If enabled
  if (getState()) {

    // If random data is available
    if (_randomTube.available()) {

      // Set the latest bit the the current random bit
      _latestBit = _randomTube.getRandomBit();

      // Set the stale state to false
      _stale = false;

      // If currently rolling, update the result with the new random bit
      if (_rolling) { _updateResult(_latestBit); }

      // Create random bit data
      Logger::KeyValuePair data[1] = {{"bit", Logger::UINT8_T, {.uint8_v = _latestBit}}};

      // Log random bit data
      logger.log(Logger::DATA, "randomNumberGenerator", data, 1, false);

    }

  }

}

// ================================================================================================
// Roll a random number in the set range
// ================================================================================================
void RandomNumberGenerator::roll() {

  // If not already rolling a random number
  if (!_rolling) {

    // Reset the result value and number of captured bits
    _result       = 0;
    _capturedBits = 0;

    // Calculate the range of random numbers
    _calculateRange();

    // Set the rolling flag to true
    _rolling = true;

  }

}

// ================================================================================================
// Set the random number generator state
// ================================================================================================
void RandomNumberGenerator::setState(const bool state) {

  // Depending on the state either enable or disable the random number generator
  if (state) {

    enable();

  } else {

    disable();

  }

}

// ================================================================================================
// Set the range minimum
// ================================================================================================
void RandomNumberGenerator::setMinimum(const uint8_t minimum) {

  _minimum = minimum;

}

// ================================================================================================
// Set the range maximum
// ================================================================================================
void RandomNumberGenerator::setMaximum(const uint8_t maximum) {

  _maximum = maximum;

}

// ================================================================================================
// Get the random number generator state
// ================================================================================================
bool RandomNumberGenerator::getState() {

  return _enabled;

}

// ================================================================================================
// Get the range minimum
// ================================================================================================
uint8_t RandomNumberGenerator::getMinimum() {

  return _minimum;

}

// ================================================================================================
// Get the range maximum
// ================================================================================================
uint8_t RandomNumberGenerator::getMaximum() {

  return _maximum;

}

// ================================================================================================
// Get the rolling state
// ================================================================================================
bool RandomNumberGenerator::getRollingState() {

  return _rolling;

}

// ================================================================================================
// Get the final random number result or the process of the rolling process
// ================================================================================================
uint8_t RandomNumberGenerator::getValue() {

  // Set the result to percentage of how many bits have been captured
  uint8_t result = (_capturedBits * 100 + _requiredBits / 2) / _requiredBits;

  // Never allow a result percentage of 100%
  if (result == 100) { result = 99; }

  // Check if no longer rolling
  if (!_rolling) {

    // Set the result
    result = _result;

  }

  // Return the result
  return result;

}

// ================================================================================================
// Retrun if the random bit is stale or not
// ================================================================================================
bool RandomNumberGenerator::getStaleState() {

  return _stale;

}

// ================================================================================================
// Get the latest random bit
// ================================================================================================
uint8_t RandomNumberGenerator::getRandomBit() {

  // Set the stale flag to true
  _stale = true;

  // Retrun the latest random bit
  return _latestBit;

}

// ------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Constructor
// ================================================================================================
RandomNumberGenerator::RandomNumberGenerator():

  // Initialize members
  _initialized(false),
  _enabled(false),
  _randomTube(RANDOM_TRG_PIN),
  _minimum(1),
  _maximum(6),
  _rolling(false),
  _range(6),
  _requiredBits(3),
  _capturedBits(0),
  _result(0),
  _stale(true),
  _latestBit(0)

{}

// ================================================================================================
// Calculate the range of random numbers
// ================================================================================================
void RandomNumberGenerator::_calculateRange() {

  // Calculate the range
  _range = _maximum - _minimum + 1;

  // Reset the number of required bits
  _requiredBits = 0;

  // Upper bound of range
  uint8_t upperBound = _range - 1;

  // Calculate how many bits are required to represent the maximum value in the range
  while (upperBound > 0) {

    // Increase the number of required bits
    _requiredBits++;

    // Shift right by 1 bit (divide by 2)
    upperBound >>= 1;

  }

}

// ================================================================================================
// Update the random number result with a new random bit
// ================================================================================================
void RandomNumberGenerator::_updateResult(const uint8_t bit) {

  // Check if the captured bits are still lesss then the required bits
  if (_capturedBits < _requiredBits) {

    // Shift left and add the random bit at LSB
    _result = (_result << 1) | bit;

    // Increase the number of captured bits
    _capturedBits++;
  
  }
  
  // If enough bits are captured
  if (_capturedBits >= _requiredBits) {

    // If the result is inside the desired range
    if (_result < _range) {

      // Add the minimum offset to the result
      _result += _minimum;

      // Set the rolling flag to false
      _rolling = false;

    // If the result is outside the desired range
    } else {

      // Reject the result and start a new round of capturing bits
      _result       = 0;
      _capturedBits = 0;

    }

  }

}