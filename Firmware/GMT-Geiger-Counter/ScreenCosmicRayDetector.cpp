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
  _coincidenceEvents("0"),
  _coincidenceEventsPerHour(2, 163, 210, IMAGE_MUON_SMALL, "0.00 " + String(STRING_COSMIC_RAY_DETECTOR_COUNTS_PER_HOUR_ABBREVIATION)),
  _coincidenceEventsTotal(  2, 189, 210, IMAGE_SUM,        "0"                                                                      ),
  _mainTubeCountsOffset(0),
  _followerTubeCountsOffset(0),
  _mainTubeCounts(    120, 215, 118, STRING_COSMIC_RAY_DETECTOR_COUNTS, "0"),
  _followerTubeCounts(120, 267, 118, STRING_COSMIC_RAY_DETECTOR_COUNTS, "0")

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenCosmicRayDetector::update(XPT2046::Point position) {

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

  // Draw the coincidence events screen border and background
  canvas.drawRect(2, 31, 236, 131, COLOR_MEDIUM_GREEN);
  canvas.fillRect(3, 32, 234, 129, COLOR_DARK_GREEN);

  // Set the font family, size and color
  canvas.setFont(&FreeSansBold24pt7b);
  canvas.setTextColor(COLOR_WHITE);

  // Text position variables
  int16_t textX, textY, cursorX, cursorY;
  uint16_t textWidth, textHeight;

  // Get the bounding box of the coincidence events text
  canvas.getTextBounds(_coincidenceEvents, 0, 0, &textX, &textY, &textWidth, &textHeight);

  // Calculate the X and Y cursor position so that the coincidence events text is centered
  cursorX = ((236 - textWidth)  / 2) + 2;
  cursorY = 100;

  // Set the cursor and draw coincidence events text to the frame buffer
  canvas.setCursor(cursorX, cursorY);
  canvas.print(_coincidenceEvents);

  // Set the font family, size and color
  canvas.setFont(&FreeSans9pt7b);
  canvas.setTextColor(COLOR_WHITE);

  // Get the bounding box of the the unit text
  canvas.getTextBounds(STRING_COSMIC_RAY_DETECTOR_COINCIDENCE_EVENTS, 0, 0, &textX, &textY, &textWidth, &textHeight);

  // Calculate cursor position for the unit text
  cursorX = ((236 - textWidth)  / 2) + 2;
  cursorY = 130;

  // Set the cursor and print the unit text
  canvas.setCursor(cursorX, cursorY);
  canvas.print(STRING_COSMIC_RAY_DETECTOR_COINCIDENCE_EVENTS);

  // Draw the diagram background and diagram
  canvas.fillRect(2, 215, 117, 51, COLOR_DARK_GRAY);
  canvas.fillRect(2, 267, 117, 51, COLOR_DARK_GRAY);
  canvas.drawRGBBitmap(12, 217, IMAGE_DIAGRAM.data, IMAGE_DIAGRAM.width, IMAGE_DIAGRAM.height);

  // Draw screen elements
  mute.draw(canvas);
  sleep.draw(canvas);
  _coincidenceEventsPerHour.draw(canvas);
  _coincidenceEventsTotal.draw(canvas);
  _mainTubeCounts.draw(canvas);
  _followerTubeCounts.draw(canvas);

}

// ================================================================================================
// Set the coincidence events offset
// ================================================================================================
void ScreenCosmicRayDetector::setCoincidenceEventsOffset(uint64_t coincidenceEventsOffset) {

  _coincidenceEventsOffset = coincidenceEventsOffset;

}

// ================================================================================================
// Calculate and set coincidence events
// ================================================================================================
void ScreenCosmicRayDetector::setCoincidenceEvents(uint64_t coincidenceEvents) {

  // Calculate new coincidence events by subtracting the coincidence events offset
  String counts = String(coincidenceEvents - _coincidenceEventsOffset);

  // Set the main tube counts value
  _coincidenceEvents = counts;

}

// ================================================================================================
// Set the coincidence events per hour value
// ================================================================================================
void ScreenCosmicRayDetector::setCoincidenceEventsPerHour(String coincidenceEventsPerHour) {

  _coincidenceEventsPerHour.setValue(coincidenceEventsPerHour);

}

// ================================================================================================
// Set the total number of coincidence events
// ================================================================================================
void ScreenCosmicRayDetector::setCoincidenceEventsTotal(String coincidenceEventsTotal) {

  _coincidenceEventsTotal.setValue(coincidenceEventsTotal);

}

// ================================================================================================
// Set the main tube offset
// ================================================================================================
void ScreenCosmicRayDetector::setMainTubeCountsOffset(uint64_t mainTubeCountsOffset) {

  _mainTubeCountsOffset = mainTubeCountsOffset;

}

// ================================================================================================
// Set the follower tube offset
// ================================================================================================
void ScreenCosmicRayDetector::setFollowerTubeCountsOffset(uint64_t followerTubeCountsOffset) {

  _followerTubeCountsOffset = followerTubeCountsOffset;

}

// ================================================================================================
// Calculate and set main tube counts
// ================================================================================================
void ScreenCosmicRayDetector::setMainTubeCounts(uint64_t mainTubeCounts) {

  // Calculate new main tube counts by subtracting the main tube offset
  String counts = String(mainTubeCounts - _mainTubeCountsOffset);

  // Set the main tube counts value
  _mainTubeCounts.setValue(counts);

}

// ================================================================================================
// Calculate and set follower tube counts
// ================================================================================================
void ScreenCosmicRayDetector::setFollowerTubeCounts(uint64_t followerTubeCounts) {

  // Calculate new follower tube counts by subtracting the main follower offset
  String counts = String(followerTubeCounts - _followerTubeCountsOffset);

  // Set the follower tube counts value
  _followerTubeCounts.setValue(String(counts));

}