#include "ScreenAudioSettings.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenAudioSettings::ScreenAudioSettings():

  // Initialize members
  ScreenBasic(STRING_AUDIO_SETTINGS_TITLE),
  detections(    0, STRING_TOGGLE_DETECTIONS,      true ),
  notifications( 1, STRING_TOGGLE_NOTIFICATIONS,   true ),
  alerts(        2, STRING_TOGGLE_ALERTS,          true ),
  interface(     3, STRING_TOGGLE_INTERFACE,       true ),
  muteEverything(4, STRING_TOGGLE_MUTE_EVERYTHING, false)

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenAudioSettings::update(const XPT2046::Point position) {

  // Update the basic screen
  ScreenBasic::update(position);

  // Update screen elements
  detections.update(position);
  notifications.update(position);
  alerts.update(position);
  interface.update(position);
  muteEverything.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenAudioSettings::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasic::draw(canvas);

  // Draw screen elements
  detections.draw(canvas);
  notifications.draw(canvas);
  alerts.draw(canvas);
  interface.draw(canvas);
  muteEverything.draw(canvas);

}