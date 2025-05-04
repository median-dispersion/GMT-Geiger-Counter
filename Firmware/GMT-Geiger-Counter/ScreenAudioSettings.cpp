#include "ScreenAudioSettings.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenAudioSettings::ScreenAudioSettings():

  // Initialize members
  ScreenBasicLandscape(STRING_AUDIO_SETTINGS),
  detections(    0, STRING_DETECTIONS,      true ),
  notifications( 1, STRING_NOTIFICATIONS,   true ),
  alerts(        2, STRING_ALERTS,          true ),
  interface(     3, STRING_INTERFACE,       true ),
  muteEverything(4, STRING_MUTE_EVERYTHING, false)

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenAudioSettings::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasicLandscape::update(position);

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
  ScreenBasicLandscape::draw(canvas);

  // Draw screen elements
  detections.draw(canvas);
  notifications.draw(canvas);
  alerts.draw(canvas);
  interface.draw(canvas);
  muteEverything.draw(canvas);

}