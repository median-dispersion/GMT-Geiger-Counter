#include "ScreenSystemSettings.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenSystemSettings::ScreenSystemSettings():

  // Initialize members
  ScreenBasic(STRING_SYSTEM_SETTINGS_TITLE),
  _firmwareVersion(2, 31, 316, STRING_FIRMWARE_VERSION, FIRMWARE_VERSION)

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenSystemSettings::update(const XPT2046::Point position) {

  // Update the basic screen
  ScreenBasic::update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenSystemSettings::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasic::draw(canvas);

  // Draw screen elements
  _firmwareVersion.draw(canvas);

}