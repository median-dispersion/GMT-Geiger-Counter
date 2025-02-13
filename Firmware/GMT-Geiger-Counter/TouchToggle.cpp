#include "TouchToggle.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
TouchToggle::TouchToggle(const uint8_t row, const char *text, const bool toggled):

  // Initialize members
  _y((26 * row) + 31),
  _text(text),
  _toggled(toggled)

{}

// ================================================================================================
// Update the touch toggle with the touch position
// ================================================================================================
void TouchToggle::update(const XPT2046::Point position) {

  // Check if touch position is inside the bounding box
  if (position.x >= 2 && position.x < 318 && position.y >= _y && position.y < (_y + 25)) {

    // Toggle state
    toggle();

    // Execute action and return toggle state
    action(_toggled);

  }

}

// ================================================================================================
// Draw the touch toggle to the frame buffer
// ================================================================================================
void TouchToggle::draw(GFXcanvas16 &canvas) {

  // Draw background
  canvas.fillRect(2, _y, 316, 25, COLOR_MEDIUM_GRAY);

  // If toggled on
  if (_toggled) {

    // Draw toggle in on position
    canvas.drawRect(280, _y + 2, 36, 21, COLOR_WHITE);
    canvas.fillRect(298, _y + 5, 15, 15, COLOR_WHITE);

  // If toggled off
  } else {

    // Draw toggle in off position
    canvas.drawRect(280, _y + 2, 36, 21, COLOR_LIGHT_GRAY);
    canvas.fillRect(281, _y + 3, 34, 19, COLOR_DARK_GRAY);
    canvas.fillRect(283, _y + 5, 15, 15, COLOR_LIGHT_GRAY);

  }

  // Draw text
  canvas.setFont(&FreeSans9pt7b);
  canvas.setTextColor(COLOR_WHITE);
  canvas.setCursor(5, _y + 18);
  canvas.print(_text);

}

// ================================================================================================
// Toggle state
// ================================================================================================
void TouchToggle::toggle() {

  // Invert state i.e. toggle
  _toggled = !_toggled;

}

// ================================================================================================
// Toggle a specific state
// ================================================================================================
void TouchToggle::toggle(bool toggled) {

  _toggled = toggled;

}

// ================================================================================================
// Toggle on
// ================================================================================================
void TouchToggle::toggleOn() {

  _toggled = true;

}

// ================================================================================================
// Toggle off
// ================================================================================================
void TouchToggle::toggleOff() {

  _toggled = false;

}

// ================================================================================================
// Return the toggle state
// ================================================================================================
bool TouchToggle::toggled() {

  return _toggled;

}