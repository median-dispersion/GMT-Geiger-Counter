#include "Touchscreen.h"

// ------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
Touchscreen::Touchscreen():

  // Initialize members
  _display(DISPLAY_CS_PIN, DISPLAY_DC_PIN, DISPLAY_RST_PIN),
  _canvas(DISPLAY_WIDTH, DISPLAY_HEIGHT),
  _touch(TOUCH_CS_PIN, TOUCH_IRQ_PIN),
  _screen(&geigerCounter),
  _previousScreen(&geigerCounter),
  _enabled(true),
  _lastRefreshMilliseconds(0),
  _rotation(DISPLAY_SCREEN_ROTATION_LANDSCAPE),
  _timeout(true),
  _lastTouchMilliseconds(0)

{}

// ================================================================================================
// Initialize everything
// ================================================================================================
void Touchscreen::begin() {

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

  // Turn on display LED
  pinMode(DISPLAY_LED_PIN, OUTPUT);
  digitalWrite(DISPLAY_LED_PIN, HIGH);

}

// ================================================================================================
// Turn display on
// ================================================================================================
void Touchscreen::enable() {

  // Turn on display LED
  digitalWrite(DISPLAY_LED_PIN, HIGH);

  // Set on flag to true
  _enabled = true;

}

// ================================================================================================
// Turn display off
// ================================================================================================
void Touchscreen::disable() {

  // Turn off display LED
  digitalWrite(DISPLAY_LED_PIN, LOW);

  // Set on flag to false
  _enabled = false;

}

// ================================================================================================
// Returns if the touchscreen is enabled
// ================================================================================================
bool Touchscreen::enabled() {

  return _enabled;

}

// ================================================================================================
// Enable display auto timeout
// ================================================================================================
void Touchscreen::enableTimeout() {

  _timeout = true;

}

// ================================================================================================
// Disable display auto timeout
// ================================================================================================
void Touchscreen::disableTimeout() {

  _timeout = false;

}

// ================================================================================================
// Retruns if the auto timeout has been enabled
// ================================================================================================
bool Touchscreen::timeout() {

  return _timeout;

}

// ================================================================================================
// Update touchscreen
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

        // Draw the sleep screen
        draw(sleep);

        // Disable the touchscreen
        disable();

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
// Rotate the display to the landscape orientation
// ================================================================================================
void Touchscreen::rotateLandscape() {

  _rotation = DISPLAY_SCREEN_ROTATION_LANDSCAPE;

}

// ================================================================================================
// Rotate the display to the portrait orientation
// ================================================================================================
void Touchscreen::rotatePortrait() {

  _rotation = DISPLAY_SCREEN_ROTATION_PORTRAIT;

}

// ================================================================================================
// Retruns the current screen
// ================================================================================================
Screen* Touchscreen::getScreen() {

  // Return a pointer to the curent screen
  return _screen;

};

// ================================================================================================
// Retruns the previous screen
// ================================================================================================
Screen* Touchscreen::getPreviousScreen() {

  // Return a pointer to the previous screen
  return _previousScreen;

};