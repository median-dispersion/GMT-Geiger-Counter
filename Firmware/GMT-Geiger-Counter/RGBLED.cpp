#include "RGBLED.h"

// ------------------------------------------------------------------------------------------------
// Public

// Initialize global reference
RGBLED& rgbLED = RGBLED::getInstance();

// ================================================================================================
// Get the single instance of the class
// ================================================================================================
RGBLED& RGBLED::getInstance() {

  // Get the single instance
  static RGBLED instance;

  // Return the instance
  return instance;

}

// ================================================================================================
// 
// ================================================================================================
void RGBLED::begin() {

  // If not initialized
  if (!_initialized) {

    // Set initialization flag to true
    _initialized = true;

    // Setup RGB LED pin
    pinMode(RGB_LED_PIN, OUTPUT);
    digitalWrite(RGB_LED_PIN, LOW);
    rgbLedWrite(RGB_LED_PIN, 0, 0, 0);

  }

}

// ================================================================================================
// 
// ================================================================================================
void RGBLED::enable() {

  // If not enabled
  if (!_enabled) {

    // Set enabled flag
    _enabled = true;

  }

}

// ================================================================================================
// 
// ================================================================================================
void RGBLED::disable() {

  // If enabled
  if (_enabled) {

    // Disable the RGB LED
    rgbLedWrite(RGB_LED_PIN, 0, 0, 0);

    // Set enabled flag
    _enabled = false;

  }

}

// ================================================================================================
// 
// ================================================================================================
void RGBLED::update() {

  // If RGB LED is enabled
  if (_enabled) {

    switch (_rating) {

      // Depending on the rating set the RGB LED color
      case GeigerCounter::RATING_NORMAL:   rgbLedWrite(RGB_LED_PIN, _adjustColorBrightness(0),   _adjustColorBrightness(255), _adjustColorBrightness(0)); break;
      case GeigerCounter::RATING_ELEVATED: rgbLedWrite(RGB_LED_PIN, _adjustColorBrightness(64),  _adjustColorBrightness(255), _adjustColorBrightness(0)); break;
      case GeigerCounter::RATING_MEDIUM:   rgbLedWrite(RGB_LED_PIN, _adjustColorBrightness(255), _adjustColorBrightness(128), _adjustColorBrightness(0)); break;
      case GeigerCounter::RATING_HIGH:     rgbLedWrite(RGB_LED_PIN, _adjustColorBrightness(255), _adjustColorBrightness(0),   _adjustColorBrightness(0)); break;
      
      // If rating is extreme
      case GeigerCounter::RATING_EXTREME:

        // Set the color to red and blink the LED
        if      (millis() - _ledTimer < 500 ) { rgbLedWrite(RGB_LED_PIN, _adjustColorBrightness(255), _adjustColorBrightness(0), _adjustColorBrightness(0)); } 
        else if (millis() - _ledTimer < 1000) { rgbLedWrite(RGB_LED_PIN, 0,                           0,                         0                        ); } 
        else                                  { _ledTimer = millis();                                                                                        }

      break;

    }

  }

}

// ================================================================================================
// 
// ================================================================================================
void RGBLED::setLEDState(const bool state) {

  // Depending on the state either enable or disable the RGB LED
  if (state) {

    enable();

  } else {

    disable();

  }

}

// ================================================================================================
// 
// ================================================================================================
void RGBLED::setRadiationRating(const GeigerCounter::RadiationRating rating) {

  _rating = rating;

}

// ================================================================================================
// 
// ================================================================================================
bool RGBLED::getLEDState() {

  return _enabled;

}

// ------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Constructor
// ================================================================================================
RGBLED::RGBLED():

  // Initialize members
  _initialized(false),
  _enabled(false),
  _rating(GeigerCounter::RATING_NORMAL),
  _ledTimer(0)

{}

// ================================================================================================
// 
// ================================================================================================
uint8_t RGBLED::_adjustColorBrightness(const uint8_t color) {

  return (RGB_LED_MAXIMUM_BRIGHTNESS / 255.0) * color;

}