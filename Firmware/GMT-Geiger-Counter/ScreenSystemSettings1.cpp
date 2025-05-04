#include "ScreenSystemSettings1.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenSystemSettings1::ScreenSystemSettings1():

  // Initialize members
  ScreenBasicLandscape(STRING_SYSTEM_SETTINGS),
  next(    160, 213, 158, 25, IMAGE_NEXT),
  previous(2,   213, 157, 25, IMAGE_BACK),
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
void ScreenSystemSettings1::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasicLandscape::update(position);

  // Update screen elements
  next.update(position);
  previous.update(position);
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
void ScreenSystemSettings1::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasicLandscape::draw(canvas);

  // Draw screen elements
  next.draw(canvas);
  previous.draw(canvas);
  sdCardMounted.draw(canvas);
  serialLogging.draw(canvas);
  sdCardLogging.draw(canvas);
  dataLogging.draw(canvas);
  eventLogging.draw(canvas);
  systemLogging.draw(canvas);

}