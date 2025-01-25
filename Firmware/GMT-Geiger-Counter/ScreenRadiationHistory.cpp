#include "ScreenRadiationHistory.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenRadiationHistory::ScreenRadiationHistory():

  // Initialize members
  ScreenBasic(STRING_RADIATION_HISTORY_TITLE),
  placeholder(2, 31, 316, "Info", "Not yet available!")

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenRadiationHistory::update(XPT2046::Point position) {

  // Update the basic screen
  ScreenBasic::update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenRadiationHistory::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasic::draw(canvas);

  // Draw screen elements
  placeholder.draw(canvas);

}