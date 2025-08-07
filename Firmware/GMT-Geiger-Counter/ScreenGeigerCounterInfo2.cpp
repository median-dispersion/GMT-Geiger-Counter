#include "ScreenGeigerCounterInfo2.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenGeigerCounterInfo2::ScreenGeigerCounterInfo2():

  // Initialize members
  ScreenBasicLandscape(STRING_GEIGER_COUNTER_INFO),
  next(    160, 213, 158, 25, IMAGE_NEXT),
  previous(2,   213, 157, 25, IMAGE_BACK),
  _absorbedDoseString("0.00"),
  _mainAbsorbedDoseString("0.00"),
  _followerAbsorbedDoseString("0.00"),
  _absorbedDoseScreen(2, 31, 316, 77, COLOR_BLUE_MEDIUM, COLOR_BLUE_DARK, _absorbedDoseString.c_str(), STRING_ABSORBED_DOSE_MICRO_SIEVERTS),
  _mainAbsorbedDose(    160, 109, 158, STRING_MICRO_SIEVERTS_ABBREVIATION, _mainAbsorbedDoseString.c_str()),
  _followerAbsorbedDose(160, 161, 158, STRING_MICRO_SIEVERTS_ABBREVIATION, _followerAbsorbedDoseString.c_str())

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenGeigerCounterInfo2::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasicLandscape::update(position);

  // Update screen elements
  next.update(position);
  previous.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenGeigerCounterInfo2::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasicLandscape::draw(canvas);

  // Draw the diagram background and diagram
  canvas.fillRect(2, 109, 157, 51, COLOR_GRAY_DARK);
  canvas.fillRect(2, 161, 157, 51, COLOR_GRAY_DARK);
  canvas.drawRGBBitmap(32, 111, IMAGE_DIAGRAM.data, IMAGE_DIAGRAM.width, IMAGE_DIAGRAM.height);

  // Draw screen elements
  next.draw(canvas);
  previous.draw(canvas);
  _absorbedDoseScreen.draw(canvas);
  _mainAbsorbedDose.draw(canvas);
  _followerAbsorbedDose.draw(canvas);

}

// ================================================================================================
// Set the total absorbed dose
// ================================================================================================
void ScreenGeigerCounterInfo2::setTotalAbsorbedDose(const double &dose) {

  _absorbedDoseString = dose;

}

// ================================================================================================
// Set the main tube absorbed dose
// ================================================================================================
void ScreenGeigerCounterInfo2::setMainAbsorbedDose(const double &dose) {

  _mainAbsorbedDoseString = dose;

}

// ================================================================================================
// Set the follower tube absorbed dose
// ================================================================================================
void ScreenGeigerCounterInfo2::setFollowerAbsorbedDose(const double &dose) {

  _followerAbsorbedDoseString = dose;

}