#include "DisplayInfoBox.h"

// ------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
DisplayInfoBox::DisplayInfoBox(uint16_t x, uint16_t y, uint16_t width, Image icon, String value):

  // Initialize members
  _x(x),
  _y(y),
  _width(width - 26),
  _icon(icon),
  _value(value)

{}

// ================================================================================================
// Draw the info box
// ================================================================================================
void DisplayInfoBox::draw(GFXcanvas16 &canvas) {

  // Draw icon & icon background
  canvas.fillRect(_x, _y, 25, 25, COLOR_DARK_GRAY);
  canvas.drawRGBBitmap(_x + ((25 - _icon.width) / 2), _y + ((25 - _icon.height) / 2), _icon.data, _icon.width, _icon.height);

  // Draw value background
  canvas.fillRect(_x + 26, _y, _width, 25, COLOR_DARK_GRAY);

  // Draw value string
  canvas.setFont(&FreeSans9pt7b);
  canvas.setTextColor(COLOR_WHITE);
  canvas.setCursor(_x + 31, _y + 18);
  canvas.print(_value);

}

// ================================================================================================
// Set the info box value
// ================================================================================================
void DisplayInfoBox::setValue(String value) {

  _value = value;

}