#include "ScreenSystemSettings.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenSystemSettings::ScreenSystemSettings():

  // Initialize members
  ScreenBasic(STRING_SYSTEM_SETTINGS_TITLE),
  serialLogging(0, STRING_LOG_TO_SERIAL_CONSOLE,     true ),
  sdCardLogging(1, STRING_LOG_TO_SD_CARD,            true ),
  eventLogging( 2, STRING_LOG_SYSTEM_EVENT_MESSAGES, true ),
  systemLogging(3, STRING_LOG_SYSTEM_DATA,           false),
  _firmwareVersion(2, 135, 316, STRING_FIRMWARE_VERSION, FIRMWARE_VERSION)

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenSystemSettings::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasic::update(position);

  // Update screen elements
  serialLogging.update(position);
  sdCardLogging.update(position);
  eventLogging.update(position);
  systemLogging.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenSystemSettings::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasic::draw(canvas);

  // Draw screen elements
  serialLogging.draw(canvas);
  sdCardLogging.draw(canvas);
  eventLogging.draw(canvas);
  systemLogging.draw(canvas);
  _firmwareVersion.draw(canvas);

}