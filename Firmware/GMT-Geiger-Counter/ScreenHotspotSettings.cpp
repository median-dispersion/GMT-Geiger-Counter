#include "ScreenHotspotSettings.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenHotspotSettings::ScreenHotspotSettings():

  // Initialize members
  ScreenBasicLandscape(STRING_HOTSPOT_SETTINGS),
  enable(0, STRING_ENABLE_HOTSPOT, false),
  _hotspotName(     2, 57,  316, STRING_HOTSPOT_NAME, HOTSPOT_NAME),
  _hotspotPassword( 2, 109, 316, STRING_HOTSPOT_PASSWORD, HOTSPOT_PASSWORD),
  _hotspotIPAddress(2, 161, 316, STRING_IP_ADDRESS, STRING_NOT_CONNECTED)

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
  _hotspotName.draw(canvas);
  _hotspotPassword.draw(canvas);
  _hotspotIPAddress.draw(canvas);
  
}

// ================================================================================================
// Set hotspot IP address
// ================================================================================================
void ScreenHotspotSettings::setIPAddress(const char *ipAddress) {

  _hotspotIPAddress.setValue(ipAddress);

}