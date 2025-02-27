#include "ScreenGeigerCounterSettings2.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenGeigerCounterSettings2::ScreenGeigerCounterSettings2():

  // Initialize members
  ScreenBasic(STRING_GEIGER_COUNTER_SETTINGS_SCREEN_TITLE),
  autoRange(0, STRING_AUTO_RANGE, true),
  sievert(  1, STRING_SIEVERTS,   true),
  rem(      2, STRING_REM,        false),
  rontgen(  3, STRING_RONTGEN,    false),
  gray(     4, STRING_GRAY,       false),
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
  autoRange.update(position);
  sievert.update(position);
  rem.update(position);
  rontgen.update(position);
  gray.update(position);
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
  autoRange.draw(canvas);
  sievert.draw(canvas);
  rem.draw(canvas);
  rontgen.draw(canvas);
  gray.draw(canvas);
  next.draw(canvas);
  previous.draw(canvas);

}