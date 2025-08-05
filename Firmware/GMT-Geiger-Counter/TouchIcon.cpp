#include "TouchIcon.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
TouchIcon::TouchIcon(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, const Image &icon, const uint16_t backgroundColor, const uint16_t borderColor):

  // Initialize members
  _x(x),
  _y(y),
  _width(width),
  _height(height),
  _icon(icon),
  _backgroundColor(backgroundColor),
  _borderColor(borderColor)

{}

// ================================================================================================
// Update the touch icon with the touch position
// ================================================================================================
void TouchIcon::update(const XPT2046::Point &position) {

  // Check if touch position is inside the bounding box
  if (position.x >= _x && position.x < (_x + _width) && position.y >= _y && position.y < (_y + _height)) {

    // Execute action
    action();

  }

}

// ================================================================================================
// Draw the touch icon to the frame buffer
// ================================================================================================
void TouchIcon::draw(GFXcanvas16 &canvas) {

  // Draw background
  canvas.fillRect(_x, _y, _width, _height, _backgroundColor);

  // Draw border
  canvas.drawRect(_x, _y, _width, _height, _borderColor);

  // Draw icon at the center
  canvas.drawRGBBitmap(_x + ((_width - _icon.width) / 2), _y + ((_height - _icon.height) / 2), _icon.data, _icon.width, _icon.height);

}