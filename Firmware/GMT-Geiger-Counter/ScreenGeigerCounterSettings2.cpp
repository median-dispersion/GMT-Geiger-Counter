#include "ScreenGeigerCounterSettings2.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenGeigerCounterSettings2::ScreenGeigerCounterSettings2():

  // Initialize members
  ScreenBasicLandscape(STRING_GEIGER_COUNTER_SETTINGS),
  autoIntegrate(0, STRING_AUTO_INTEGRATE, true),
  autoRange(    1, STRING_AUTO_RANGE,     true),
  sieverts(     2, STRING_SIEVERTS,       true),
  rem(          3, STRING_REM,            false),
  rontgen(      4, STRING_RONTGEN,        false),
  gray(         5, STRING_GRAY,           false),
  next(    160, 213, 158, 25, IMAGE_NEXT),
  previous(2,   213, 157, 25, IMAGE_BACK)

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenGeigerCounterSettings2::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasicLandscape::update(position);

  // Update screen elements
  autoIntegrate.update(position);
  autoRange.update(position);
  sieverts.update(position);
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
  ScreenBasicLandscape::draw(canvas);

  // Draw screen elements
  autoIntegrate.draw(canvas);
  autoRange.draw(canvas);
  sieverts.draw(canvas);
  rem.draw(canvas);
  rontgen.draw(canvas);
  gray.draw(canvas);
  next.draw(canvas);
  previous.draw(canvas);

}