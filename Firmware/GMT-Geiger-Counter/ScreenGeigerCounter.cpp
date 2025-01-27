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

  _equivalentDose("0.00"),
  _equivalentDoseUnit(STRING_EQUIVALENT_DOSE_UNIT_USVH),
  _doseBorderColor(COLOR_MEDIUM_GREEN),
  _doseBackgroundColor(COLOR_DARK_GREEN),

  _radiationRating(2, 161, 160, IMAGE_RADIATION, STRING_RADIATION_RATING_NORMAL                                                      ),
  _countsPerMinute(2, 187, 160, IMAGE_PARTICLE,  "0 " + String(STRING_COUNTS_PER_MINUTE_ABBREVIATION)                                ),
  _integrationTime(2, 213, 160, IMAGE_CLOCK,     String(INTEGRATION_TIME_DEFAULT_SECONDS) + " " + String(STRING_SECONDS_ABBREVIATION))

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenGeigerCounter::update(XPT2046::Point position) {

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

  // Draw the dose screen border and background
  canvas.drawRect(2, 31, 160, 129, _doseBorderColor);
  canvas.fillRect(3, 32, 158, 127, _doseBackgroundColor);

  // Set the font family, size and color
  canvas.setFont(&FreeSansBold24pt7b);
  canvas.setTextColor(COLOR_WHITE);

  // Text position variables
  int16_t textX, textY, cursorX, cursorY;
  uint16_t textWidth, textHeight;

  // Get the bounding box of the equivalent dose text
  canvas.getTextBounds(_equivalentDose, 0, 0, &textX, &textY, &textWidth, &textHeight);

  // Calculate the X and Y cursor position so that the equivalent dose text is centered
  cursorX = ((160 - textWidth)  / 2) + 2;
  cursorY = 105;

  // Set the cursor and draw equivalent dose text to the frame buffer
  canvas.setCursor(cursorX, cursorY);
  canvas.print(_equivalentDose);

  // Set the font family, size and color
  canvas.setFont(&FreeSans9pt7b);
  canvas.setTextColor(COLOR_WHITE);

  // Get the bounding box of the equivalent dose unit text
  canvas.getTextBounds(_equivalentDoseUnit, 0, 0, &textX, &textY, &textWidth, &textHeight);

  // Calculate the X cursor position so that the equivalent dose unit text is centered
  // Use the equivalent dose text Y position and add a 25px offset
  // A little janky but works as long as the font size of the equivalent dose text doesn't change
  cursorX = ((160 - textWidth) / 2) + 2;
  cursorY += 25;

  // Set the cursor and print the equivalent dose unit text
  canvas.setCursor(cursorX, cursorY);
  canvas.print(_equivalentDoseUnit);

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
  _radiationRating.draw(canvas);
  _countsPerMinute.draw(canvas);
  _integrationTime.draw(canvas);

}

// ================================================================================================
// Set the equivalent dose
// ================================================================================================
void ScreenGeigerCounter::setEquivalentDose(String equivalentDose) {

  _equivalentDose = equivalentDose;

}

// ================================================================================================
// Set the equivalent dose unit
// ================================================================================================
void ScreenGeigerCounter::setEquivalentDoseUnit(String equivalentDoseUnit) {

  _equivalentDoseUnit = equivalentDoseUnit;

}

// ================================================================================================
// Set the radiation rating
// ================================================================================================
void ScreenGeigerCounter::setRadiationRating(GeigerCounter::RadiationRating radiationRating) {

  // Depending on the radiation rating
  // Update the dose screen border and background color
  // Update the radiation rating info box value
  
  switch (radiationRating) {

    case GeigerCounter::RADIATION_RATING_NORMAL:
      _doseBorderColor     = COLOR_MEDIUM_GREEN;
      _doseBackgroundColor = COLOR_DARK_GREEN;
      _radiationRating.setValue(STRING_RADIATION_RATING_NORMAL);
    break;

    case GeigerCounter::RADIATION_RATING_ELEVATED:
      _doseBorderColor     = COLOR_MEDIUM_ORANGE;
      _doseBackgroundColor = COLOR_DARK_ORANGE;
      _radiationRating.setValue(STRING_RADIATION_RATING_ELEVATED);
    break;

    case GeigerCounter::RADIATION_RATING_HIGH:
      _doseBorderColor     = COLOR_MEDIUM_RED;
      _doseBackgroundColor = COLOR_DARK_RED;
      _radiationRating.setValue(STRING_RADIATION_RATING_HIGH);
    break;

    case GeigerCounter::RADIATION_RATING_EXTREME:
      _doseBorderColor     = COLOR_MEDIUM_RED;
      _doseBackgroundColor = COLOR_DARK_RED;
      _radiationRating.setValue(STRING_RADIATION_RATING_EXTREME);
    break;

  }

}


// ================================================================================================
// Set the counts per minute
// ================================================================================================
void ScreenGeigerCounter::setCountsPerMinute(String countsPerMinute) {

  _countsPerMinute.setValue(countsPerMinute);

}

// ================================================================================================
// Set the integration time
// ================================================================================================
void ScreenGeigerCounter::setIntegrationTime(String integrationTime) {

  _integrationTime.setValue(integrationTime);

}