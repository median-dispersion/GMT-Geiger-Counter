#include "Touchscreen.h"

// ------------------------------------------------------------------------------------------------
// Public

// Initialize global reference
Touchscreen& touchscreen = Touchscreen::getInstance();

// ================================================================================================
// Get the single instance of the class
// ================================================================================================
Touchscreen& Touchscreen::getInstance() {

  // Get the single instance
  static Touchscreen instance;

  // Return the instance
  return instance;

}

// ================================================================================================
// Initialize everything
// ================================================================================================
void Touchscreen::begin() {

  // If not initialized
  if (!_initialized) {

    // Set the initialization flag to true
    _initialized = true;

    // Setup hardware pins
    pinMode(DISPLAY_CS_PIN, OUTPUT);
    // pinMode(TOUCH_CS_PIN, OUTPUT);
    pinMode(DISPLAY_LED_PIN, OUTPUT);
    digitalWrite(DISPLAY_CS_PIN, HIGH);
    // digitalWrite(TOUCH_CS_PIN, HIGH);
    digitalWrite(DISPLAY_LED_PIN, LOW);

    // Setup display
    _display.begin();
    _display.fillScreen(ILI9341_BLACK);

    // Set the frame buffer rotation
    _canvas.setRotation(_rotation);

    // Setup touch controller
    _touch.begin();
    _touch.setRotation(_rotation);
    _touch.setCalibration(TOUCH_CALIBRATION);
    _touch.setSampleCount(TOUCH_SAMPLE_COUNT);
    _touch.setDebounceTimeout(TOUCH_DEBOUNCE_TIMEOUT_MILLISECONDS);
    _touch.setTouchPressure(TOUCH_PRESSURE_THRESHOLD);

    // Initialize logger
    logger.begin();

  }

}

// ================================================================================================
// Enable the touchscreen
// ================================================================================================
void Touchscreen::enable() {

  // If not enabled
  if (!_enabled) {

    // Turn on the display LED
    digitalWrite(DISPLAY_LED_PIN, HIGH);

    // Set the enabled flag to true
    _enabled = true;

    // Create event data
    Logger::KeyValuePair event[2] = {

      {"source", Logger::STRING_T, {.string_v = "touchscreen"}},
      {"action", Logger::STRING_T, {.string_v = "enabled"}    }

    };

    // Log event message
    logger.log(Logger::EVENT, "event", event, 2);

  }

}

// ================================================================================================
// Disable the touchscreen
// ================================================================================================
void Touchscreen::disable() {

  // If enabled
  if (_enabled) {

    // Turn off the display LED
    digitalWrite(DISPLAY_LED_PIN, LOW);

    // Set the enabled flag to false
    _enabled = false;

    // Create event data
    Logger::KeyValuePair event[2] = {

      {"source", Logger::STRING_T, {.string_v = "touchscreen"}},
      {"action", Logger::STRING_T, {.string_v = "disabled"}   }

    };

    // Log event message
    logger.log(Logger::EVENT, "event", event, 2);

  }

}

// ================================================================================================
// Update the touchscreen
// ================================================================================================
void Touchscreen::update() {

  bool refreshImmediately = false;

  // Check if touch event is occurring and if the last event has been released
  if (_touch.touched() && _touch.released()) {

    // Apply touch rotation before getting the position
    _touch.setRotation(_rotation);

    // Get the touch position
    XPT2046::Point position = _touch.getTouchPosition();

    // Check if the touch position is valid
    if (_touch.valid(position)) {

      // Update the selected screen with the touch position
      _screen->update(position);

      // Set the refresh flag to true
      refreshImmediately = true;

      // Set the last touch event time
      _lastTouchMilliseconds = millis();

    }

  }

  // If the touchscreen is currently enabled
  if (_enabled) {

    // If auto timeout is enabled
    if (_timeout) {

      // If the timeout has been reached
      if (millis() - _lastTouchMilliseconds >= DISPLAY_AUTO_TIMEOUT_SECONDS * 1000) {

        // Go to sleep
        sleep();

      }

    }
    
    // If refresh interval has been reached or refresh flag has been set
    if (millis() - _lastRefreshMilliseconds > DISPLAY_REFRESH_INTERVAL_MILLISECONDS || refreshImmediately) {

      // Refresh the display
      refresh();

    }

  }

}

// ================================================================================================
// Refresh the display by writing the pixel data to it
// ================================================================================================
void Touchscreen::refresh() {

  // Only write to the display if it is enabled
  if (_enabled) {

    // Apply display rotation before drawing
    _canvas.setRotation(_rotation);

    // Draw the selected screen to the frame buffer
    _screen->draw(_canvas);

    // Unrotate frame buffer before drawing to the screen
    _canvas.setRotation(0);

    // Draw the frame buffer to the display
    _display.drawRGBBitmap(0, 0, _canvas.getBuffer(), _canvas.width(), _canvas.height());

    // Update the refresh interval
    _lastRefreshMilliseconds = millis();

  }

}

// ================================================================================================
// Go to sleep
// ================================================================================================
void Touchscreen::sleep() {

  // If touchscreen is enabled
  if (_enabled) {

    // Disable the touchscreen
    disable();

    // Draw the sleep screen
    draw(sleepScreen);

  }

}

// ================================================================================================
// Draw a screen by reference
// ================================================================================================
void Touchscreen::draw(Screen &screen) {

  // Set the previous screen
  _previousScreen = _screen;

  // Set the screen pointer to a pointer of a reference of the selected screen 
  // (cursed...)
  _screen = &screen;

}

// ================================================================================================
// Draw a screen by pointer
// ================================================================================================
void Touchscreen::draw(Screen *screen) {

  // Set the previous screen
  _previousScreen = _screen;

  // Set the new screen
  _screen = screen;

}

// ================================================================================================
// Draw the previous screen
// ================================================================================================
void Touchscreen::drawPreviousScreen() {

  // Draw the previous screen
  draw(_previousScreen);

}

// ================================================================================================
// Set the touchscreen state
// ================================================================================================
void Touchscreen::setTouchscreenState(const bool state) {

  // Depending on the state either enable or disable the touchscreen
  if (state) {

    enable();

  } else {

    disable();

  }

}

// ================================================================================================
// Set auto timeout state
// ================================================================================================
void Touchscreen::setTimeoutState(const bool state) {

  _timeout = state;

}

// ================================================================================================
// Rotate the touchscreen into landscape orientation
// ================================================================================================
void Touchscreen::setRotationLandscape() {

  _rotation = DISPLAY_SCREEN_ROTATION_LANDSCAPE;

}

// ================================================================================================
// Rotate the touchscreen into portrait orientation
// ================================================================================================
void Touchscreen::setRotationPortrait() {

  _rotation = DISPLAY_SCREEN_ROTATION_PORTRAIT;

}

// ================================================================================================
// Get the touchscreen state
// ================================================================================================
bool Touchscreen::getTouchscreenState() {

  return _enabled;

}

// ================================================================================================
// Get the auto timeout state
// ================================================================================================
bool Touchscreen::getTimeoutState() {

  return _timeout;

}

// ------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Constructor
// ================================================================================================
Touchscreen::Touchscreen():

  // Initialize members
  _initialized(false),
  _display(DISPLAY_CS_PIN, DISPLAY_DC_PIN, DISPLAY_RST_PIN),
  _canvas(DISPLAY_WIDTH, DISPLAY_HEIGHT),
  _touch(TOUCH_CS_PIN, TOUCH_IRQ_PIN),
  _screen(&geigerCounter),
  _previousScreen(&geigerCounter),
  _enabled(false),
  _rotation(DISPLAY_SCREEN_ROTATION_LANDSCAPE),
  _timeout(true),
  _lastTouchMilliseconds(0),
  _lastRefreshMilliseconds(0)

{}