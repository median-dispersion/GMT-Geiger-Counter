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
void TouchToggle::update(const XPT2046::Point &position) {

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
  canvas.fillRect(2, _y, 316, 25, COLOR_GRAY_MEDIUM);

  // If toggled on
  if (_toggled) {

    // Draw toggle in on position
    canvas.drawRect(284, _y + 4, 28, 17, COLOR_WHITE);
    canvas.fillRect(298, _y + 7, 11, 11, COLOR_WHITE);

  // If toggled off
  } else {

    // Draw toggle in off position
    canvas.drawRect(284, _y + 4, 28, 17, COLOR_GRAY_LIGHT);
    canvas.fillRect(287, _y + 7, 11, 11, COLOR_GRAY_LIGHT);

  }

  // Draw text
  canvas.setFont(&FreeSans9pt7b);
  canvas.setTextColor(COLOR_WHITE);
  canvas.setCursor(5, _y + 18);
  canvas.print(_text);

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
// Toggle state
// ================================================================================================
void TouchToggle::toggle() {

  // Invert state i.e. toggle
  _toggled = !_toggled;

}

// ================================================================================================
// Toggle a specific state
// ================================================================================================
void TouchToggle::setToggleState(const bool toggled) {

  _toggled = toggled;

}

// ================================================================================================
// Return the toggle state
// ================================================================================================
bool TouchToggle::getToggleState() {

  return _toggled;

}