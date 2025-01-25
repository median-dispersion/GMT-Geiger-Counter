#include "ScreenTrueRNG.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenTrueRNG::ScreenTrueRNG():

  // Initialize members
  ScreenBasic(STRING_TRUE_RNG_TITLE),
  placeholder(2, 31, 316, "Info", "Not yet available!")

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenTrueRNG::update(XPT2046::Point position) {

  // Update the basic screen
  ScreenBasic::update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenTrueRNG::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasic::draw(canvas);

  // Draw screen elements
  placeholder.draw(canvas);

}