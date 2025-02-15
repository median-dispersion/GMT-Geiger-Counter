#include "ScreenGeigerCounter.h"

// ------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenGeigerCounter::ScreenGeigerCounter():

  // Initialize members
  audioSettings(          163, 31,  51, 51, IMAGE_VOLUME  ),
  displaySettings(        215, 31,  51, 51, IMAGE_DISPLAY ),
  goToSleep(              267, 31,  51, 51, IMAGE_SLEEP   ),
  decreaseIntegrationTime(163, 83,  51, 51, IMAGE_DECREASE),
  resetIntegrationTime(   215, 83,  51, 51, IMAGE_RESET   ),
  increaseIntegrationTime(267, 83,  51, 51, IMAGE_INCREASE),
  cosmicRayDetector(      163, 135, 51, 51, IMAGE_MUON    ),
  radiationHistory(       215, 135, 51, 51, IMAGE_HISTORY ),
  trueRNG(                267, 135, 51, 51, IMAGE_DICE    ),
  hotspotSettings(        163, 187, 51, 51, IMAGE_HOTSPOT ),
  wifiSettings(           215, 187, 51, 51, IMAGE_WIFI    ),
  systemSettings(         267, 187, 51, 51, IMAGE_SETTINGS),

  geigerCounterSetting(2, 31, 160, 129),
  _equivalentDoseString("0.00"),
  _equivalentDoseUnitString(STRING_EQUIVALENT_DOSE_UNIT_USVH),
  _countsPerMinuteString("0 CPM"),
  _integrationTimeString("30 s"),

  _equivalentDoseScreen(2, 31, 160, 129, COLOR_MEDIUM_GREEN, COLOR_DARK_GREEN, _equivalentDoseString.c_str(), _equivalentDoseUnitString.c_str()),
  _radiationRating(2, 161, 160, IMAGE_RADIATION, STRING_RADIATION_RATING_NORMAL),
  _countsPerMinute(2, 187, 160, IMAGE_PARTICLE,  _countsPerMinuteString.c_str()),
  _integrationTime(2, 213, 160, IMAGE_CLOCK,     _integrationTimeString.c_str())

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenGeigerCounter::update(const XPT2046::Point &position) {

  // Update screen elements
  audioSettings.update(position);
  displaySettings.update(position);
  goToSleep.update(position);
  decreaseIntegrationTime.update(position);
  resetIntegrationTime.update(position);
  increaseIntegrationTime.update(position);
  cosmicRayDetector.update(position);
  radiationHistory.update(position);
  trueRNG.update(position);
  hotspotSettings.update(position);
  wifiSettings.update(position);
  systemSettings.update(position);
  geigerCounterSetting.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenGeigerCounter::draw(GFXcanvas16 &canvas) {

  // Clear framebuffer
  canvas.fillScreen(COLOR_BLACK);

  // Draw screen border
  canvas.drawRect(0, 29, 320, 211, COLOR_WHITE);

  // Draw title bar
  canvas.drawRect(0, 0, 320, 30, COLOR_WHITE);
  canvas.fillRect(1, 1, 318, 28, COLOR_DARK_GRAY);

  // Draw title text
  canvas.setFont(&FreeSans9pt7b);
  canvas.setTextColor(COLOR_WHITE);
  canvas.setCursor(5, 20);
  canvas.print(STRING_GEIGER_COUNTER_SCREEN_TITLE);

  // Draw screen elements
  audioSettings.draw(canvas);
  displaySettings.draw(canvas);
  goToSleep.draw(canvas);
  decreaseIntegrationTime.draw(canvas);
  resetIntegrationTime.draw(canvas);
  increaseIntegrationTime.draw(canvas);
  cosmicRayDetector.draw(canvas);
  radiationHistory.draw(canvas);
  trueRNG.draw(canvas);
  hotspotSettings.draw(canvas);
  wifiSettings.draw(canvas);
  systemSettings.draw(canvas);
  _equivalentDoseScreen.draw(canvas);
  _radiationRating.draw(canvas);
  _countsPerMinute.draw(canvas);
  _integrationTime.draw(canvas);

}

// ================================================================================================
// Set the equivalent dose
// ================================================================================================
void ScreenGeigerCounter::setEquivalentDose(const double &equivalentDose) {

  // Convert equivalent dose to a string
  _equivalentDoseString = equivalentDose;

}

// ================================================================================================
// Set the equivalent dose unit
// ================================================================================================
void ScreenGeigerCounter::setEquivalentDoseUnit(const String &equivalentDoseUnit) {

  _equivalentDoseUnitString = equivalentDoseUnit;

}

// ================================================================================================
// Set the radiation rating
// ================================================================================================
void ScreenGeigerCounter::setRadiationRating(const GeigerCounter::RadiationRating radiationRating) {

  // Depending on the radiation rating
  // Update the dose screen border and background color
  // Update the radiation rating info box value
  
  switch (radiationRating) {

    case GeigerCounter::RADIATION_RATING_UNKNOWN:
      _equivalentDoseScreen.setBorderColor(COLOR_MEDIUM_GREEN);
      _equivalentDoseScreen.setBackgroundColor(COLOR_DARK_GREEN);
      _radiationRating.setValue(STRING_RADIATION_RATING_UNKNOWN);
    break;

    case GeigerCounter::RADIATION_RATING_NORMAL:
      _equivalentDoseScreen.setBorderColor(COLOR_MEDIUM_GREEN);
      _equivalentDoseScreen.setBackgroundColor(COLOR_DARK_GREEN);
      _radiationRating.setValue(STRING_RADIATION_RATING_NORMAL);
    break;

    case GeigerCounter::RADIATION_RATING_ELEVATED:
      _equivalentDoseScreen.setBorderColor(COLOR_MEDIUM_ORANGE);
      _equivalentDoseScreen.setBackgroundColor(COLOR_DARK_ORANGE);
      _radiationRating.setValue(STRING_RADIATION_RATING_ELEVATED);
    break;

    case GeigerCounter::RADIATION_RATING_HIGH:
      _equivalentDoseScreen.setBorderColor(COLOR_MEDIUM_RED);
      _equivalentDoseScreen.setBackgroundColor(COLOR_DARK_RED);
      _radiationRating.setValue(STRING_RADIATION_RATING_HIGH);
    break;

    case GeigerCounter::RADIATION_RATING_EXTREME:
      _equivalentDoseScreen.setBorderColor(COLOR_MEDIUM_RED);
      _equivalentDoseScreen.setBackgroundColor(COLOR_DARK_RED);
      _radiationRating.setValue(STRING_RADIATION_RATING_EXTREME);
    break;

  }

}


// ================================================================================================
// Set the counts per minute
// ================================================================================================
void ScreenGeigerCounter::setCountsPerMinute(const double &countsPerMinute) {

  // Convert counts per minute to a string with unit appended to it
  _countsPerMinuteString  = (uint32_t)(round(countsPerMinute));
  _countsPerMinuteString += " ";
  _countsPerMinuteString += STRING_COUNTS_PER_MINUTE_ABBREVIATION;

}

// ================================================================================================
// Set the integration time
// ================================================================================================
void ScreenGeigerCounter::setIntegrationTime(const uint8_t integrationTime) {
  
  // Convert integration time to a string with unit appended to it
  _integrationTimeString  = integrationTime;
  _integrationTimeString += " ";
  _integrationTimeString += STRING_SECONDS_ABBREVIATION;

}