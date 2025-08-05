#include "ScreenSystemSettings3.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenSystemSettings3::ScreenSystemSettings3():

  // Initialize members
  ScreenBasicLandscape(STRING_SYSTEM_SETTINGS),
  next(    160, 213, 158, 25, IMAGE_NEXT),
  previous(2,   213, 157, 25, IMAGE_BACK),
  reboot(  55,  110,  51,  51, IMAGE_POWER,        COLOR_YELLOW_DARK, COLOR_YELLOW_MEDIUM),
  reset(   214, 110,  51,  51, IMAGE_RESET_SYSTEM, COLOR_RED_DARK,    COLOR_RED_MEDIUM)

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenSystemSettings3::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasicLandscape::update(position);

  // Update screen elements
  next.update(position);
  previous.update(position);
  reboot.update(position);
  reset.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenSystemSettings3::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasicLandscape::draw(canvas);

  // Draw the background
  canvas.fillRect(2,   31, 157, 181, COLOR_GRAY_DARKEST);
  canvas.fillRect(160, 31, 158, 181, COLOR_GRAY_DARKEST);

  // Set the font family, size and color
  canvas.setFont(&FreeSans9pt7b);
  canvas.setTextColor(COLOR_WHITE);

  // Text position variables
  int16_t textX, textY, cursorX, cursorY;
  uint16_t textWidth, textHeight;

  // Get the bounding box
  canvas.getTextBounds(STRING_REBOOT, 0, 0, &textX, &textY, &textWidth, &textHeight);

  // Calculate the X and Y cursor position
  cursorX = 81 - (textWidth / 2) - 2;
  cursorY = 90;

  // Set the cursor and draw text to the frame buffer
  canvas.setCursor(cursorX, cursorY);
  canvas.print(STRING_REBOOT);

  // Get the bounding box
  canvas.getTextBounds(STRING_RESET, 0, 0, &textX, &textY, &textWidth, &textHeight);

  // Calculate the X position
  cursorX = 240 - (textWidth / 2) - 2;

  // Set the cursor and draw text to the frame buffer
  canvas.setCursor(cursorX, cursorY);
  canvas.print(STRING_RESET);

  // Draw screen elements
  next.draw(canvas);
  previous.draw(canvas);
  reboot.draw(canvas);
  reset.draw(canvas);

}