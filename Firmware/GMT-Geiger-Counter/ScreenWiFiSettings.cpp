#include "ScreenWiFiSettings.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenWiFiSettings::ScreenWiFiSettings():

  // Initialize members
  ScreenBasic(STRING_WIFI_SETTINGS_TITLE),
  placeholder(2, 31, 316, "Info", "Not yet available!")

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenWiFiSettings::update(XPT2046::Point position) {

  // Update the basic screen
  ScreenBasic::update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenWiFiSettings::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasic::draw(canvas);

  // Draw screen elements
  placeholder.draw(canvas);

}