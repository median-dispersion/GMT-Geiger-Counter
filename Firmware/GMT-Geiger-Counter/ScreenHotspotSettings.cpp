#include "ScreenHotspotSettings.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenHotspotSettings::ScreenHotspotSettings():

  // Initialize members
  ScreenBasicLandscape(STRING_HOTSPOT_SETTINGS_TITLE),
  enable(0, STRING_ENABLE_HOTSPOT, false),
  _hostpotName(2, 57, 316, STRING_HOTSPOT_NAME, HOTSPOT_NAME),
  _hostpotPassword(2, 109, 316, STRING_HOTSPOT_PASSWORD, HOTSPOT_PASSWORD),
  _hostpotIPAddress(2, 161, 316, STRING_IP_ADDRESS, STRING_NOT_CONNECTED)

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenHotspotSettings::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasicLandscape::update(position);

  // Update screen elements
  enable.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenHotspotSettings::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasicLandscape::draw(canvas);

  // Draw screen elements
  enable.draw(canvas);
  _hostpotName.draw(canvas);
  _hostpotPassword.draw(canvas);
  _hostpotIPAddress.draw(canvas);
  
}

// ================================================================================================
// Set hotspot IP address
// ================================================================================================
void ScreenHotspotSettings::setIPAddress(const char *ipAddress) {

  _hostpotIPAddress.setValue(ipAddress);

}