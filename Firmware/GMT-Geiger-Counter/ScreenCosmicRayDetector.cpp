#include "ScreenCosmicRayDetector.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenCosmicRayDetector::ScreenCosmicRayDetector():

  // Initialize members
  ScreenBasic(STRING_COSMIC_RAY_DETECTOR_TITLE),
  placeholder(2, 31, 316, "Info", "Not yet available!")

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenCosmicRayDetector::update(XPT2046::Point position) {

  // Update the basic screen
  ScreenBasic::update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenCosmicRayDetector::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasic::draw(canvas);

  // Draw screen elements
  placeholder.draw(canvas);

}