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
// 
// ================================================================================================
void Touchscreen::begin() {

  // If not initialized
  if (!_initialized) {

    // Set initialization flag to true
    _initialized = true;

    // Setup pins for the touchscreen and display
    pinMode(DISPLAY_CS_PIN, OUTPUT);
    /* pinMode(TOUCH_CS_PIN, OUTPUT); */
    pinMode(DISPLAY_LED_PIN, OUTPUT);
    digitalWrite(DISPLAY_CS_PIN, HIGH);
    /* digitalWrite(TOUCH_CS_PIN, HIGH); */
    digitalWrite(DISPLAY_LED_PIN, LOW);

    // Initialize logger
    logger.begin();

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

    // Turn on the touchscreen
    enable();

  }

}

// ================================================================================================
// 
// ================================================================================================
void Touchscreen::enable() {

  // If not enabled
  if (!_enabled) {

    // Turn on the display LED
    digitalWrite(DISPLAY_LED_PIN, HIGH);

    // Set the enabled flag to true
    _enabled = true;

    // Create hardware event data
    Logger::KeyValuePair event[2] = {

      {"target", Logger::STRING_T, {.string_v = "touchscreen"}},
      {"action", Logger::STRING_T, {.string_v = "enabled"}    }

    };

    // Event data log message
    String message = "";

    // Log hardware event data
    logger.log(Logger::EVENT, "event", event, 2, message);

  }

}

// ================================================================================================
// 
// ================================================================================================
void Touchscreen::disable() {

  // If not disabled
  if (_enabled) {

    // Turn off the display LED
    digitalWrite(DISPLAY_LED_PIN, LOW);

    // Set the enabled flag to false
    _enabled = false;

    // Create hardware event data
    Logger::KeyValuePair event[2] = {

      {"target", Logger::STRING_T, {.string_v = "touchscreen"}},
      {"action", Logger::STRING_T, {.string_v = "disabled"}   }

    };

    // Event data log message
    String message = "";

    // Log hardware event data
    logger.log(Logger::EVENT, "event", event, 2, message);

  }

}

// ================================================================================================
// 
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

  // If auto timeout is enabled
  if (_timeout) {

    // If the screen is currently enabled
    if (_enabled) {

      // If the timeout has been reached
      if (millis() - _lastTouchMilliseconds >= DISPLAY_AUTO_TIMEOUT_SECONDS * 1000) {

        // Go to sleep
        sleep();

      }

    }

  }
  
  // If refresh interval has been reached or refresh flag has been set
  if (millis() - _lastRefreshMilliseconds > DISPLAY_REFRESH_INTERVAL_MILLISECONDS || refreshImmediately) {

    // Refresh the display
    refresh();

  }

}

// ================================================================================================
// 
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
// 
// ================================================================================================
void Touchscreen::sleep() {

  // Only go to sleep if the touchscreen is enabled
  if (_enabled) {

    // Disable the touchscreen
    disable();

    // Draw the sleep screen
    draw(_sleep);

  }

}

// ================================================================================================
// 
// ================================================================================================
void Touchscreen::wakeup() {

  // Draw previous screen
  draw(_previousScreen);

  // Enable the touchscreen
  enable();

}

// ================================================================================================
// 
// ================================================================================================
void Touchscreen::draw(Screen &screen) {

  // Set the previous screen
  _previousScreen = _screen;

  // Set the screen pointer to a pointer of a reference of the selected screen 
  // (cursed...)
  _screen = &screen;

}

// ================================================================================================
// 
// ================================================================================================
void Touchscreen::draw(Screen *screen) {

  // Set the previous screen
  _previousScreen = _screen;

  // Set the new screen
  _screen = screen;

}

// ================================================================================================
// 
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
// 
// ================================================================================================
void Touchscreen::setTimeoutState(const bool state) {

  _timeout = state;

}

// ================================================================================================
// 
// ================================================================================================
void Touchscreen::setRotationLandscape() {

  _rotation = DISPLAY_SCREEN_ROTATION_LANDSCAPE;

}

// ================================================================================================
// 
// ================================================================================================
void Touchscreen::setRotationPortrait() {

  _rotation = DISPLAY_SCREEN_ROTATION_PORTRAIT;

}


// ================================================================================================
// 
// ================================================================================================
bool Touchscreen::getTouchscreenState() {

  return _enabled;

}

// ================================================================================================
// 
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
  _enabled(false),
  _rotation(DISPLAY_SCREEN_ROTATION_LANDSCAPE),
  _timeout(true),
  _lastTouchMilliseconds(0),
  _lastRefreshMilliseconds(0),
  _screen(&geigerCounter),
  _previousScreen(&geigerCounter)

{}