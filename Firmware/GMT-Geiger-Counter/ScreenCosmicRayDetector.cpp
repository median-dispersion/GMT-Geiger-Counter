#include "ScreenCosmicRayDetector.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenCosmicRayDetector::ScreenCosmicRayDetector():

  // Initialize members
  ScreenBasicPortrait(STRING_COSMIC_RAY_DETECTOR),
  mute( 187, 163, 51, 25, IMAGE_MUTE       ),
  sleep(187, 189, 51, 25, IMAGE_SLEEP_SMALL),
  _coincidenceEventsString("0"),
  _coincidenceEventsPerHourString("0.00 CPH"),
  _coincidenceEventsTotalString("0"),
  _mainTubeCountsString("0"),
  _followerTubeCountsString("0"),
  _coincidenceEventsScreen(2, 31, 236, 131, COLOR_PURPLE_MEDIUM, COLOR_PURPLE_DARK, _coincidenceEventsString.c_str(), STRING_COINCIDENCE_EVENTS),
  _coincidenceEventsPerHour(2, 163, 184, IMAGE_MUON_SMALL, _coincidenceEventsPerHourString.c_str()),
  _coincidenceEventsTotal(2, 189, 184, IMAGE_SUM, _coincidenceEventsTotalString.c_str()),
  _mainTubeCounts(120, 215, 118, STRING_COUNTS, _mainTubeCountsString.c_str()),
  _followerTubeCounts(120, 267, 118, STRING_COUNTS, _followerTubeCountsString.c_str())

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenCosmicRayDetector::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasicPortrait::update(position);

  // Update screen elements
  mute.update(position);
  sleep.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenCosmicRayDetector::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasicPortrait::draw(canvas);

  // Draw the diagram background and diagram
  canvas.fillRect(2, 215, 117, 51, COLOR_GRAY_DARK);
  canvas.fillRect(2, 267, 117, 51, COLOR_GRAY_DARK);
  canvas.drawRGBBitmap(12, 217, IMAGE_DIAGRAM.data, IMAGE_DIAGRAM.width, IMAGE_DIAGRAM.height);

  // Draw screen elements
  mute.draw(canvas);
  sleep.draw(canvas);
  _coincidenceEventsScreen.draw(canvas);
  _coincidenceEventsPerHour.draw(canvas);
  _coincidenceEventsTotal.draw(canvas);
  _mainTubeCounts.draw(canvas);
  _followerTubeCounts.draw(canvas);

}

// ================================================================================================
// Set coincidence events
// ================================================================================================
void ScreenCosmicRayDetector::setCoincidenceEvents(const uint64_t &coincidenceEvents) {

  _coincidenceEventsString = coincidenceEvents;

}

// ================================================================================================
// Set the coincidence events per hour value
// ================================================================================================
void ScreenCosmicRayDetector::setCoincidenceEventsPerHour(const uint32_t &coincidenceEventsPerHour) {

  _coincidenceEventsPerHourString  = coincidenceEventsPerHour;
  _coincidenceEventsPerHourString += " ";
  _coincidenceEventsPerHourString += STRING_COUNTS_PER_HOUR_ABBREVIATION;

}

// ================================================================================================
// Set the total number of coincidence events
// ================================================================================================
void ScreenCosmicRayDetector::setCoincidenceEventsTotal(const uint64_t &coincidenceEventsTotal) {

  _coincidenceEventsTotalString = coincidenceEventsTotal;

}

// ================================================================================================
// Set main tube counts
// ================================================================================================
void ScreenCosmicRayDetector::setMainTubeCounts(const uint64_t &mainTubeCounts) {

  _mainTubeCountsString = mainTubeCounts;

}

// ================================================================================================
// Set follower tube counts
// ================================================================================================
void ScreenCosmicRayDetector::setFollowerTubeCounts(const uint64_t &followerTubeCounts) {

  _followerTubeCountsString = followerTubeCounts;

}