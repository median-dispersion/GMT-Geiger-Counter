#include "TouchRadio.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
TouchRadio::TouchRadio(const uint8_t row, const char *text, const bool selected):

  // Initialize members
  _y((26 * row) + 31),
  _text(text),
  _selected(selected)

{}

// ================================================================================================
// Update the touch radio with the touch position
// ================================================================================================
void TouchRadio::update(const XPT2046::Point &position) {

  // Check if touch position is inside the bounding box
  if (position.x >= 2 && position.x < 318 && position.y >= _y && position.y < (_y + 25)) {

    // Select the touch radio
    select();

    // Execute action
    action();

  }

}

// ================================================================================================
// Draw the touch radio to the frame buffer
// ================================================================================================
void TouchRadio::draw(GFXcanvas16 &canvas) {

  // Draw background
  canvas.fillRect(2, _y, 316, 25, COLOR_GRAY_MEDIUM);

  // If selected
  if (_selected) {

    // Draw radio in selected state
    canvas.drawRect(291, _y + 5, 15, 15, COLOR_WHITE);
    canvas.fillRect(294, _y + 8, 9, 9, COLOR_WHITE);

  // If deselected
  } else {

    // Draw radio in deselected state
    canvas.drawRect(291, _y + 5, 15, 15, COLOR_GRAY_LIGHT);

  }

  // Draw text
  canvas.setFont(&FreeSans9pt7b);
  canvas.setTextColor(COLOR_WHITE);
  canvas.setCursor(5, _y + 18);
  canvas.print(_text);

}

// ================================================================================================
// Select radio
// ================================================================================================
void TouchRadio::select() {

  _selected = true;

}

// ================================================================================================
// Deselect radio
// ================================================================================================
void TouchRadio::deselect() {

  _selected = false;

}

// ================================================================================================
// Set select state
// ================================================================================================
void TouchRadio::setSelectState(const bool state) {

  // Depending on the state either select radio or not
  if (state) {

    select();

  } else {

    deselect();

  }

}

// ================================================================================================
// Return the radio state
// ================================================================================================
bool TouchRadio::getSelectState() {

  return _selected;

}