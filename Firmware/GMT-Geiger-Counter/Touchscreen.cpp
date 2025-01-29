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
  _on(true),
  _lastRefreshMilliseconds(0),
  _rotation(DISPLAY_SCREEN_ROTATION_LANDSCAPE)

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
// Update touchscreen
// ================================================================================================
void Touchscreen::update() {

  bool refreshImmediately = false;

  // Check if touch event is occurring and if the last event has been released
  if (_touch.touched() && _touch.released()) {

    // Apply touch rotation before checking
    _touch.setRotation(_rotation);

    // Get the touch position
    XPT2046::Point position = _touch.getTouchPosition();

    // Check if the touch position is valid
    if (_touch.valid(position)) {

      // Update the selected screen with the touch position
      _screen->update(position);

      // Set the refresh flag to true
      refreshImmediately = true;

    }

  }

  // Only draw the screen if the display is set to on
  if (_on) {

    // If refresh interval has been reached or refresh flag has been set
    if (millis() - _lastRefreshMilliseconds > DISPLAY_REFRESH_INTERVAL_MILLISECONDS || refreshImmediately) {

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

}

// ================================================================================================
// Draw a screen by reference
// ================================================================================================
void Touchscreen::draw(Screen &screen) {

  // Set the screen pointer to a pointer of a reference of the selected screen 
  // (cursed...)
  _screen = &screen;

}

// ================================================================================================
// Draw a screen by pointer
// ================================================================================================
void Touchscreen::draw(Screen *screen) {

  _screen = screen;

}

// ================================================================================================
// Turn display on
// ================================================================================================
void Touchscreen::on() {

  // Turn on display LED
  digitalWrite(DISPLAY_LED_PIN, HIGH);

  // Set on flag to true
  _on = true;

}

// ================================================================================================
// Turn display off
// ================================================================================================
void Touchscreen::off() {

  // Turn off display LED
  digitalWrite(DISPLAY_LED_PIN, LOW);

  // Set on flag to false
  _on = false;

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