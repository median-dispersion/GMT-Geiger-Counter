#include "ScreenCosmicRayDetector.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenCosmicRayDetector::ScreenCosmicRayDetector():

  // Initialize members
  ScreenBasicPortrait(STRING_COSMIC_RAY_DETECTOR_TITLE),
  mute( 213, 163, 25, 25, IMAGE_MUTE       ),
  sleep(213, 189, 25, 25, IMAGE_SLEEP_SMALL),
  _coincidenceEventsOffset(0),
  _mainTubeCountsOffset(0),
  _followerTubeCountsOffset(0),
  _coincidenceEventsString("0"),
  _coincidenceEventsPerHourString("0.00 CPH"),
  _coincidenceEventsTotalString("0"),
  _mainTubeCountsString("0"),
  _followerTubeCountsString("0"),
  _coincidenceEventsScreen(2, 31, 236, 131, COLOR_MEDIUM_GREEN, COLOR_DARK_GREEN, _coincidenceEventsString.c_str(), STRING_COINCIDENCE_EVENTS),
  _coincidenceEventsPerHour(2, 163, 210, IMAGE_MUON_SMALL, _coincidenceEventsPerHourString.c_str()),
  _coincidenceEventsTotal(  2, 189, 210, IMAGE_SUM, _coincidenceEventsTotalString.c_str()),
  _mainTubeCounts( 120, 215, 118, STRING_COUNTS, _mainTubeCountsString.c_str()),
  _followerTubeCounts(120, 267, 118, STRING_COUNTS, _followerTubeCountsString.c_str())

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenCosmicRayDetector::update(const XPT2046::Point position) {

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
  canvas.fillRect(2, 215, 117, 51, COLOR_DARK_GRAY);
  canvas.fillRect(2, 267, 117, 51, COLOR_DARK_GRAY);
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
// Set the coincidence events offset
// ================================================================================================
void ScreenCosmicRayDetector::setCoincidenceEventsOffset(const uint64_t coincidenceEventsOffset) {

  _coincidenceEventsOffset = coincidenceEventsOffset;

}

// ================================================================================================
// Set the main tube offset
// ================================================================================================
void ScreenCosmicRayDetector::setMainTubeCountsOffset(const uint64_t mainTubeCountsOffset) {

  _mainTubeCountsOffset = mainTubeCountsOffset;

}

// ================================================================================================
// Set the follower tube offset
// ================================================================================================
void ScreenCosmicRayDetector::setFollowerTubeCountsOffset(const uint64_t followerTubeCountsOffset) {

  _followerTubeCountsOffset = followerTubeCountsOffset;

}

// ================================================================================================
// Calculate and set coincidence events
// ================================================================================================
void ScreenCosmicRayDetector::setCoincidenceEvents(const uint64_t coincidenceEvents) {

  _coincidenceEventsString = coincidenceEvents - _coincidenceEventsOffset;

}

// ================================================================================================
// Set the coincidence events per hour value
// ================================================================================================
void ScreenCosmicRayDetector::setCoincidenceEventsPerHour(const double coincidenceEventsPerHour) {

  _coincidenceEventsPerHourString = "";

  _coincidenceEventsPerHourString += coincidenceEventsPerHour;
  _coincidenceEventsPerHourString += " ";
  _coincidenceEventsPerHourString += STRING_COUNTS_PER_HOUR_ABBREVIATION;

}

// ================================================================================================
// Set the total number of coincidence events
// ================================================================================================
void ScreenCosmicRayDetector::setCoincidenceEventsTotal(const uint64_t coincidenceEventsTotal) {

  _coincidenceEventsTotalString = coincidenceEventsTotal;

}

// ================================================================================================
// Calculate and set main tube counts
// ================================================================================================
void ScreenCosmicRayDetector::setMainTubeCounts(const uint64_t mainTubeCounts) {

  _mainTubeCountsString = mainTubeCounts - _mainTubeCountsOffset;

}

// ================================================================================================
// Calculate and set follower tube counts
// ================================================================================================
void ScreenCosmicRayDetector::setFollowerTubeCounts(const uint64_t followerTubeCounts) {

  _followerTubeCountsString = followerTubeCounts - _followerTubeCountsOffset;

}