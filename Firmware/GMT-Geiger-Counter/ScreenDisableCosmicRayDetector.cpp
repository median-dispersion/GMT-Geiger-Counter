#include "ScreenDisableCosmicRayDetector.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenDisableCosmicRayDetector::ScreenDisableCosmicRayDetector():

  // Initialize members
  ScreenBasicPortrait(STRING_COSMIC_RAY_DETECTOR),
  confirm(12,  200, 103, 27, STRING_YES),
  dismiss(126, 200, 103, 27, STRING_NO)

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenDisableCosmicRayDetector::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasicPortrait::update(position);

  // Update screen elements
  confirm.update(position);
  dismiss.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenDisableCosmicRayDetector::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasicPortrait::draw(canvas);

  // Draw the confirmation background
  canvas.fillRect(2, 31, 236, 287, COLOR_GRAY_DARK);

  // Draw the icon
  canvas.drawRGBBitmap(105, 67, IMAGE_QUESTION.data, IMAGE_QUESTION.width, IMAGE_QUESTION.height);

  // Set the font family, size and color
  canvas.setFont(&FreeSans9pt7b);
  canvas.setTextColor(COLOR_WHITE);

  // Text position variables
  int16_t textX, textY, cursorX, cursorY;
  uint16_t textWidth, textHeight;

  // Get the bounding box
  canvas.getTextBounds(STRING_DISABLE_COSMIC_RAY_DETECTOR, 0, 0, &textX, &textY, &textWidth, &textHeight);

  // Calculate the X and Y cursor position
  cursorX = ((236 - textWidth)  / 2) + 2;
  cursorY = ((287 - textHeight) / 2) + 31;

  // Set the cursor and draw text to the frame buffer
  canvas.setCursor(cursorX, cursorY);
  canvas.print(STRING_DISABLE_COSMIC_RAY_DETECTOR);

  // Draw bottom lines
  canvas.drawFastHLine(12, 301, 215, COLOR_GRAY_LIGHT);
  canvas.drawFastHLine(12, 307, 215, COLOR_GRAY_LIGHT);

  // Draw screen elements
  confirm.draw(canvas);
  dismiss.draw(canvas);

}