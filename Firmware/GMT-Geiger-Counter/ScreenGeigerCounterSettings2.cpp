#include "ScreenGeigerCounterSettings2.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenGeigerCounterSettings2::ScreenGeigerCounterSettings2():

  // Initialize members
  ScreenBasic(STRING_GEIGER_COUNTER_SETTINGS_SCREEN_TITLE),
  next(160, 213, 158, 25, IMAGE_NEXT),
  previous(2, 213, 157, 25, IMAGE_BACK)

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenGeigerCounterSettings2::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasic::update(position);

  // Update screen elements
  next.update(position);
  previous.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenGeigerCounterSettings2::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasic::draw(canvas);

  // Draw screen elements
  next.draw(canvas);
  previous.draw(canvas);

}