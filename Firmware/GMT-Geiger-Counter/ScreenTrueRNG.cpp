#include "ScreenTrueRNG.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenTrueRNG::ScreenTrueRNG():

  // Initialize members
  ScreenBasicLandscape(STRING_TRUE_RNG_TITLE),
  placeholder(2, 31, 316, STRING_INFO, STRING_NOT_YET_AVAILABLE)

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenTrueRNG::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasicLandscape::update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenTrueRNG::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasicLandscape::draw(canvas);

  // Draw screen elements
  placeholder.draw(canvas);

}