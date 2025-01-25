#include "TouchIcon.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
TouchIcon::TouchIcon(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Image icon):

  // Initialize members
  _x(x),
  _y(y),
  _width(width),
  _height(height),
  _icon(icon)

{}

// ================================================================================================
// Update the touch icon with the touch position
// ================================================================================================
void TouchIcon::update(XPT2046::Point position) {

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
  canvas.fillRect(_x, _y, _width, _height, COLOR_MEDIUM_GRAY);

  // Draw icon at the center
  canvas.drawRGBBitmap(_x + ((_width - _icon.width) / 2), _y + ((_height - _icon.height) / 2), _icon.data, _icon.width, _icon.height);

}