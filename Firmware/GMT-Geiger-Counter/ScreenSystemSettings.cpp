#include "ScreenSystemSettings.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenSystemSettings::ScreenSystemSettings():

  // Initialize members
  ScreenBasicLandscape(STRING_SYSTEM_SETTINGS),
  sdCardMounted(0, STRING_SD_CARD_MOUNTED,       true ),
  serialLogging(1, STRING_LOG_TO_SERIAL_CONSOLE, true ),
  sdCardLogging(2, STRING_LOG_TO_SD_CARD,        true ),
  dataLogging(  3, STRING_LOG_DATA,              true ),
  eventLogging( 4, STRING_LOG_EVENT_MESSAGES,    true ),
  systemLogging(5, STRING_LOG_SYSTEM_INFO,       false)

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenSystemSettings::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasicLandscape::update(position);

  // Update screen elements
  sdCardMounted.update(position);
  serialLogging.update(position);
  sdCardLogging.update(position);
  dataLogging.update(position);
  eventLogging.update(position);
  systemLogging.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenSystemSettings::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasicLandscape::draw(canvas);

  // Draw screen elements
  sdCardMounted.draw(canvas);
  serialLogging.draw(canvas);
  sdCardLogging.draw(canvas);
  dataLogging.draw(canvas);
  eventLogging.draw(canvas);
  systemLogging.draw(canvas);

}