#include "ScreenGeigerCounterSettings1.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenGeigerCounterSettings1::ScreenGeigerCounterSettings1():

  // Initialize members
  ScreenBasicLandscape(STRING_GEIGER_COUNTER_SETTINGS),
  next(    160, 213, 158, 25, IMAGE_NEXT),
  previous(2,   213, 157, 25, IMAGE_BACK),
  _countsString("0"),
  _mainTubeCountsString("0"),
  _followerTubeCountsString("0"),
  _countsScreen(2, 31, 316, 77, COLOR_BLUE_MEDIUM, COLOR_BLUE_DARK, _countsString.c_str(), STRING_COUNTS),
  _mainTubeCounts(    160, 109, 158, STRING_COUNTS, _mainTubeCountsString.c_str()),
  _followerTubeCounts(160, 161, 158, STRING_COUNTS, _followerTubeCountsString.c_str())

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenGeigerCounterSettings1::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasicLandscape::update(position);

  // Update screen elements
  next.update(position);
  previous.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenGeigerCounterSettings1::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasicLandscape::draw(canvas);

  // Draw the diagram background and diagram
  canvas.fillRect(2, 109, 157, 51, COLOR_GRAY_DARK);
  canvas.fillRect(2, 161, 157, 51, COLOR_GRAY_DARK);
  canvas.drawRGBBitmap(32, 111, IMAGE_DIAGRAM.data, IMAGE_DIAGRAM.width, IMAGE_DIAGRAM.height);

  // Draw screen elements
  next.draw(canvas);
  previous.draw(canvas);
  _countsScreen.draw(canvas);
  _mainTubeCounts.draw(canvas);
  _followerTubeCounts.draw(canvas);

}

// ================================================================================================
// Set the total counts
// ================================================================================================
void ScreenGeigerCounterSettings1::setCounts(const uint64_t &counts) {

  _countsString = counts;

}

// ================================================================================================
// Set the main tube counts
// ================================================================================================
void ScreenGeigerCounterSettings1::setMainTubeCounts(const uint64_t &counts) {

  _mainTubeCountsString = counts;

}

// ================================================================================================
// Set the follower tube counts
// ================================================================================================
void ScreenGeigerCounterSettings1::setFollowerTubeCounts(const uint64_t &counts) {

  _followerTubeCountsString = counts;

}