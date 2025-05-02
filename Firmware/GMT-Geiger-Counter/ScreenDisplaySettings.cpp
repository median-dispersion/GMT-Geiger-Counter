#include "ScreenDisplaySettings.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenDisplaySettings::ScreenDisplaySettings():

  // Initialize members
  ScreenBasicLandscape(STRING_DISPLAY_SETTINGS_TITLE),
  display(0, STRING_TOGGLE_DISPLAY, true),
  timeout(1, STRING_AUTO_TIMEOUT,   true)

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenDisplaySettings::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasicLandscape::update(position);

  // Update screen elements
  display.update(position);
  timeout.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenDisplaySettings::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasicLandscape::draw(canvas);

  // Draw screen elements
  display.draw(canvas);
  timeout.draw(canvas);

}