#include "TouchButton.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
TouchButton::TouchButton(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, const char *text, const uint16_t backgroundColor, const uint16_t borderColor):

  // Initialize members
  _x(x),
  _y(y),
  _width(width),
  _height(height),
  _text(text),
  _backgroundColor(backgroundColor),
  _borderColor(borderColor)

{}

// ================================================================================================
// Update the touch button with the touch position
// ================================================================================================
void TouchButton::update(const XPT2046::Point &position) {

  // Check if touch position is inside the bounding box
  if (position.x >= _x && position.x < (_x + _width) && position.y >= _y && position.y < (_y + _height)) {

    // Execute action
    action();

  }

}

// ================================================================================================
// Draw the touch button to the frame buffer
// ================================================================================================
void TouchButton::draw(GFXcanvas16 &canvas) {

  // Draw background
  canvas.fillRect(_x, _y, _width, _height, _backgroundColor);

  // Draw border
  canvas.drawRect(_x, _y, _width, _height, _borderColor);

  // Set the font family, size and color
  canvas.setFont(&FreeSans9pt7b);
  canvas.setTextColor(COLOR_WHITE);

  // Text position variables
  int16_t textX, textY, cursorX, cursorY;
  uint16_t textWidth, textHeight;

  // Get the bounding box of the text
  canvas.getTextBounds(_text, 0, 0, &textX, &textY, &textWidth, &textHeight);

  // Calculate the X and Y cursor position so that the text is centered
  cursorX = ((_width  - textWidth)  / 2) + _x;
  cursorY = ((_height + textHeight) / 2) + _y;

  // Set the cursor and draw text to the frame buffer
  canvas.setCursor(cursorX, cursorY);
  canvas.print(_text);

}