#include "ScreenSystemSettings2.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenSystemSettings2::ScreenSystemSettings2():

  // Initialize members
  ScreenBasicLandscape(STRING_SYSTEM_SETTINGS),
  next(    160, 213, 158, 25, IMAGE_NEXT),
  previous(2,   213, 157, 25, IMAGE_BACK),
  firmware(2, 31,  316, STRING_FIRMWARE_VERSION,   FIRMWARE_VERSION),
  upTime(  2, 83,  316, STRING_SYSTEM_UPTIME,      _upTimeString.c_str()),
  memory(  2, 135, 316, STRING_MEMORY_USAGE_BYTES, _memoryString.c_str()),
  _upTimeString("0 min"),
  _memoryString("0 / 0")

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenSystemSettings2::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasicLandscape::update(position);

  // Update screen elements
  next.update(position);
  previous.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenSystemSettings2::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasicLandscape::draw(canvas);

  // Update up time string
  _upTimeString  = millis() / 60000;
  _upTimeString += " ";
  _upTimeString += STRING_MINUTES_ABBREVIATION;
  
  // Update memory sting
  _memoryString =  ESP.getFreeHeap();
  _memoryString += " / ";
  _memoryString += ESP.getHeapSize();

  upTime.setValue(_upTimeString.c_str());
  memory.setValue(_memoryString.c_str());

  // Draw screen elements
  next.draw(canvas);
  previous.draw(canvas);
  firmware.draw(canvas);
  upTime.draw(canvas);
  memory.draw(canvas);

}