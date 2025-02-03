#include "ScreenRotationConfirmation.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenRotationConfirmation::ScreenRotationConfirmation():

  // Initialize members
  ScreenBasicPortrait(STRING_COSMIC_RAY_DETECTOR_TITLE),
  confirm(2, 56, 236, 262)

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenRotationConfirmation::update(const XPT2046::Point position) {

  // Update the basic screen
  ScreenBasicPortrait::update(position);

  // Update screen elements
  confirm.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenRotationConfirmation::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasicPortrait::draw(canvas);

  // Draw the confirmation background
  canvas.fillRect(2, 31, 236, 287, COLOR_DARK_GRAY);

  canvas.drawRGBBitmap(105, 67, IMAGE_ROTATE.data, IMAGE_ROTATE.width, IMAGE_ROTATE.height);

  // Set the font family, size and color
  canvas.setFont(&FreeSans9pt7b);
  canvas.setTextColor(COLOR_WHITE);

  // Text position variables
  int16_t textX, textY, cursorX, cursorY;
  uint16_t textWidth, textHeight;

  // Get the bounding box
  canvas.getTextBounds(STRING_ROTATE_BY_90_DEGREES, 0, 0, &textX, &textY, &textWidth, &textHeight);

  // Calculate the X and Y cursor position
  cursorX = ((236 - textWidth)  / 2) + 2;
  cursorY = ((287 - textHeight) / 2) + 31;

  // Set the cursor and draw text to the frame buffer
  canvas.setCursor(cursorX, cursorY);
  canvas.print(STRING_ROTATE_BY_90_DEGREES);

  // Get the bounding box
  canvas.getTextBounds(STRING_TAP_TO_CONTINUE, 0, 0, &textX, &textY, &textWidth, &textHeight);

  // Calculate the X and Y cursor position
  cursorX = ((236 - textWidth) / 2) + 2;
  cursorY = cursorY + 30;

  // Set the cursor and draw text to the frame buffer
  canvas.setCursor(cursorX, cursorY);
  canvas.print(STRING_TAP_TO_CONTINUE);

  // Draw bottom lines
  canvas.drawFastHLine(12, 301, 215, COLOR_WHITE);
  canvas.drawFastHLine(12, 307, 215, COLOR_WHITE);

}