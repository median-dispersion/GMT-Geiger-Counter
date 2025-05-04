#include "ScreenWiFiSettings.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenWiFiSettings::ScreenWiFiSettings():

  // Initialize members
  ScreenBasicLandscape(STRING_WIFI_SETTINGS),
  enable(0, STRING_ENABLE_WIFI, false),
  _wifiName(     2, 57,  316, STRING_WIFI_NAME, WIFI_NAME),
  _wifiPassword( 2, 109, 316, STRING_WIFI_PASSWORD, STRING_ASTERISKS),
  _wifiIPAddress(2, 161, 316, STRING_IP_ADDRESS, STRING_NOT_CONNECTED)

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenWiFiSettings::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasicLandscape::update(position);

  // Update screen elements
  enable.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenWiFiSettings::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasicLandscape::draw(canvas);

  // Draw screen elements
  enable.draw(canvas);
  _wifiName.draw(canvas);
  _wifiPassword.draw(canvas);
  _wifiIPAddress.draw(canvas);

}

// ================================================================================================
// Set the WiFi name / SSID
// ================================================================================================
void ScreenWiFiSettings::setWiFiName(const char *wifiName) {

  _wifiName.setValue(wifiName);

}

// ================================================================================================
// Set WiFi IP address
// ================================================================================================
void ScreenWiFiSettings::setIPAddress(const char *ipAddress) {

  _wifiIPAddress.setValue(ipAddress);

}