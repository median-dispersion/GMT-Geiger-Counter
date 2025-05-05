#include "ScreenBasicLandscape.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenBasicLandscape::ScreenBasicLandscape(const char *title):

  // Initialize members
  back(2, 2, 51, 26, IMAGE_BACK),
  _title(title)

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenBasicLandscape::update(const XPT2046::Point &position) {

  // Update back button
  back.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenBasicLandscape::draw(GFXcanvas16 &canvas) {

  // Clear framebuffer
  canvas.fillScreen(COLOR_BLACK);

  // Draw screen border
  canvas.drawRect(0, 29, 320, 211, COLOR_WHITE);

  // Draw title bar
  canvas.drawRect(0, 0, 320, 30, COLOR_WHITE);
  canvas.fillRect(1, 1, 318, 28, COLOR_GRAY_DARK);

  // Draw title text
  canvas.setFont(&FreeSans9pt7b);
  canvas.setTextColor(COLOR_WHITE);
  canvas.setCursor(60, 20);
  canvas.print(_title);

  // Draw back button
  back.draw(canvas);

}