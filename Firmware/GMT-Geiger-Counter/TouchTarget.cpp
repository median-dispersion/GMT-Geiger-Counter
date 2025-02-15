#include "TouchTarget.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
TouchTarget::TouchTarget(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height):

  // Initialize members
  _x(x),
  _y(y),
  _width(width),
  _height(height)

{}

// ================================================================================================
// Update the touch target with the touch position
// ================================================================================================
void TouchTarget::update(const XPT2046::Point &position) {

  // Check if touch position is inside the bounding box
  if (position.x >= _x && position.x < (_x + _width) && position.y >= _y && position.y < (_y + _height)) {

    // Execute action
    action();

  }

}