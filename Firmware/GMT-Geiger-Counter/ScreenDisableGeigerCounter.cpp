#include "ScreenDisableGeigerCounter.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenDisableGeigerCounter::ScreenDisableGeigerCounter():

  // Initialize members
  ScreenBasicLandscape(STRING_TRUE_RANDOM_NUMBERS),
  confirm(53,  170, 103, 27, STRING_YES),
  dismiss(166, 170, 103, 27, STRING_NO)

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenDisableGeigerCounter::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasicLandscape::update(position);

  // Update screen elements
  confirm.update(position);
  dismiss.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenDisableGeigerCounter::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasicLandscape::draw(canvas);

  // Draw the confirmation background
  canvas.fillRect(2, 31, 316, 207, COLOR_GRAY_DARK);

  // Draw the icon
  canvas.drawRGBBitmap(142, 52, IMAGE_QUESTION.data, IMAGE_QUESTION.width, IMAGE_QUESTION.height);

  // Set the font family, size and color
  canvas.setFont(&FreeSans9pt7b);
  canvas.setTextColor(COLOR_WHITE);

  // Text position variables
  int16_t textX, textY, cursorX, cursorY;
  uint16_t textWidth, textHeight;

  // Get the bounding box
  canvas.getTextBounds(STRING_DISABLE_THE, 0, 0, &textX, &textY, &textWidth, &textHeight);

  // Calculate the cursor X position
  cursorX = (320 - textWidth) / 2;
  cursorY = 124 - (textHeight / 2);

  // Set the cursor and draw text to the frame buffer
  canvas.setCursor(cursorX, cursorY);
  canvas.print(STRING_DISABLE_THE);

  // Get the bounding box
  canvas.getTextBounds(STRING_GEIGER_COUNTER_QUESTION_MARK, 0, 0, &textX, &textY, &textWidth, &textHeight);

  // Calculate the cursor X position
  cursorX = (320 - textWidth) / 2;
  cursorY += 20;

  // Set the cursor and draw text to the frame buffer
  canvas.setCursor(cursorX, cursorY);
  canvas.print(STRING_GEIGER_COUNTER_QUESTION_MARK);

  // Draw bottom lines
  canvas.drawFastHLine(13, 221, 296, COLOR_GRAY_LIGHT);
  canvas.drawFastHLine(13, 227, 296, COLOR_GRAY_LIGHT);

  // Draw screen elements
  confirm.draw(canvas);
  dismiss.draw(canvas);

}