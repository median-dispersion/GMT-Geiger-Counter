#include "DisplayToastBox.h"

// ------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
DisplayToastBox::DisplayToastBox(uint16_t x, uint16_t y, uint16_t width, String title, String value):

  // Initialize members
  _x(x),
  _y(y),
  _width(width),
  _title(title),
  _value(value)

{}

// ================================================================================================
// Draw the toast box
// ================================================================================================
void DisplayToastBox::draw(GFXcanvas16 &canvas) {

  // Draw background
  canvas.fillRect(_x, _y, _width, 51, COLOR_DARK_GRAY);

  // Set the font family, size and color
  canvas.setFont(&FreeSans9pt7b);
  canvas.setTextColor(COLOR_LIGHT_GRAY);

  // Draw title
  canvas.setCursor(5, _y + 20);
  canvas.print(_title);

  // Set the font family, size and color
  canvas.setFont(&FreeSansBold9pt7b);
  canvas.setTextColor(COLOR_WHITE);

  // Draw title
  canvas.setCursor(5, _y + 42);
  canvas.print(_value);

}

// ================================================================================================
// Set the toast box value
// ================================================================================================
void DisplayToastBox::setValue(String value) {

  _value = value;

}