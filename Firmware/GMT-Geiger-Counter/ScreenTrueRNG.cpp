#include "ScreenTrueRNG.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenTrueRNG::ScreenTrueRNG():

  // Initialize members
  ScreenBasicLandscape(STRING_TRUE_RANDOM_NUMBERS),

  increaseMinimum(170, 41,  64,  26, IMAGE_INCREASE_UP  ),
  decreaseMinimum(170, 141, 64,  26, IMAGE_DECREASE_DOWN),
  increaseMaximum(244, 41,  64,  26, IMAGE_INCREASE_UP  ),
  decreaseMaximum(244, 141, 64,  26, IMAGE_DECREASE_DOWN),
  roll(           170, 177, 138, 51, IMAGE_DICE_SMALL   ),
  mute(           12,  202, 64,  26, IMAGE_MUTE         ),
  sleep(          86,  202, 63,  26, IMAGE_SLEEP_SMALL  ),

  _screenString("0"),
  _descriptionString(STRING_RESULT),
  _minimumString("1"),
  _maximumString("6"),

  _resultScreen(12, 41, 137, 151, COLOR_YELLOW_MEDIUM, COLOR_YELLOW_DARK, _screenString.c_str(), _descriptionString.c_str())

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenTrueRNG::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasicLandscape::update(position);

  // Update screen elements
  increaseMinimum.update(position);
  decreaseMinimum.update(position);
  increaseMaximum.update(position);
  decreaseMaximum.update(position);
  roll.update(position);
  mute.update(position);
  sleep.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenTrueRNG::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasicLandscape::draw(canvas);

  // Draw the background
  canvas.fillRect(2,   31, 157, 207, COLOR_GRAY_DARK);
  canvas.fillRect(160, 31, 158, 207, COLOR_GRAY_DARK);

  // Set the font family, size and color
  canvas.setFont(&FreeSans9pt7b);
  canvas.setTextColor(COLOR_GRAY_LIGHT);

  // Set the cursor and draw text to the frame buffer
  canvas.setCursor(185, 138);
  canvas.print(STRING_MINIMUM_SHORTENED);
  canvas.setCursor(257, 138);
  canvas.print(STRING_MAXIMUM_SHORTENED);

  // Set the font family, size and color
  canvas.setFont(&FreeSansBold24pt7b);
  canvas.setTextColor(COLOR_WHITE);

  // Text position variables
  int16_t textX, textY, cursorX;
  uint16_t textWidth, textHeight;

  // Get the bounding box
  canvas.getTextBounds(_minimumString, 0, 0, &textX, &textY, &textWidth, &textHeight);

  // Calculate the X cursor position
  cursorX = 200 - (textWidth / 2);

  // Set the cursor and draw text to the frame buffer
  canvas.setCursor(cursorX, 117);
  canvas.print(_minimumString);

  // Get the bounding box
  canvas.getTextBounds(_maximumString, 0, 0, &textX, &textY, &textWidth, &textHeight);

  // Calculate the X cursor position
  cursorX = 275 - (textWidth / 2);

  // Set the cursor and draw text to the frame buffer
  canvas.setCursor(cursorX, 117);
  canvas.print(_maximumString);

  // Draw screen elements
  _resultScreen.draw(canvas);
  increaseMinimum.draw(canvas);
  decreaseMinimum.draw(canvas);
  increaseMaximum.draw(canvas);
  decreaseMaximum.draw(canvas);
  roll.draw(canvas);
  mute.draw(canvas);
  sleep.draw(canvas);

}

// ================================================================================================
// Set the screen value
// ================================================================================================
void ScreenTrueRNG::setValue(const uint8_t value) {

  _screenString = value;

}

// ================================================================================================
// Set the rolling state
// ================================================================================================
void ScreenTrueRNG::setState(const bool rolling) {

  if (rolling) {

    _screenString += "%";

    _descriptionString = STRING_ROLLED;

  } else {

    _descriptionString = STRING_RESULT;

  }

}

// ================================================================================================
// Set the minimum value
// ================================================================================================
void ScreenTrueRNG::setMinimum(const uint8_t minimum) {

  _minimumString = minimum;

}

// ================================================================================================
// Set the maximum value
// ================================================================================================
void ScreenTrueRNG::setMaximum(const uint8_t maximum) {

  _maximumString = maximum;

}