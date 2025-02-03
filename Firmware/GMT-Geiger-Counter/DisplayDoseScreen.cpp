#include "DisplayDoseScreen.h"

// ------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
DisplayDoseScreen::DisplayDoseScreen(

  const uint16_t x,
  const uint16_t y,
  const uint16_t width,
  const uint16_t height,
  const uint16_t borderColor,
  const uint16_t backgroundColor,
  const char     *value,
  const char     *unit

):

  // Initialize members
  _x(x),
  _y(y),
  _width(width),
  _height(height),
  _borderColor(borderColor),
  _backgroundColor(backgroundColor),
  _value(value),
  _unit(unit)

{}

// ================================================================================================
// Draw the dose screen
// ================================================================================================
void DisplayDoseScreen::draw(GFXcanvas16 &canvas) {

  // Draw the dose screen border and background
  canvas.drawRect(_x, _y, _width, _height, _borderColor);
  canvas.fillRect(_x + 1, _y + 1, _width - 2, _height - 2, _backgroundColor);

  // Set the font family, size and color
  canvas.setFont(&FreeSansBold24pt7b);
  canvas.setTextColor(COLOR_WHITE);

  // Text position variables
  int16_t textX, textY, cursorX, cursorY;
  uint16_t textWidth, textHeight;

  // Get the bounding box of the value text
  canvas.getTextBounds(_value, 0, 0, &textX, &textY, &textWidth, &textHeight);

  // Calculate the X and Y cursor position so that the value text is centered
  cursorX = ((_width -  textWidth)  / 2) + _x;
  cursorY = (_height * 0.55) + _y;

  // Set the cursor and draw value text to the frame buffer
  canvas.setCursor(cursorX, cursorY);
  canvas.print(_value);

  // Set the font family, size and color
  canvas.setFont(&FreeSans9pt7b);
  canvas.setTextColor(COLOR_WHITE);

  // Get the bounding box of the unit text
  canvas.getTextBounds(_unit, 0, 0, &textX, &textY, &textWidth, &textHeight);

  // Calculate the X cursor position so that the unit text is centered
  // Use the value text Y position and add a 25px offset
  // A little janky but works as long as the font size of the value text doesn't change
  cursorX = ((_width - textWidth) / 2) + 2;
  cursorY += 25;

  // Set the cursor and print the unit text
  canvas.setCursor(cursorX, cursorY);
  canvas.print(_unit);

}

// ================================================================================================
// Set the screen border color
// ================================================================================================
void DisplayDoseScreen::setBorderColor(const uint16_t color) {

  _borderColor = color;

}

// ================================================================================================
// Set the screen background color
// ================================================================================================
void DisplayDoseScreen::setBackgroundColor(const uint16_t color) {

  _backgroundColor = color;

}

// ================================================================================================
// Set the screen background color
// ================================================================================================
void DisplayDoseScreen::setValue(const char *value) {

  _value = value;

}

// ================================================================================================
// Set the screen background color
// ================================================================================================
void DisplayDoseScreen::setUnit(const char *unit) {

  _unit = unit;

}