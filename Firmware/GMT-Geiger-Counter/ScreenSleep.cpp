#include "ScreenSleep.h"

// Get the max screen size
#if DISPLAY_WIDTH > DISPLAY_HEIGHT
  #define MAX_DISPLAY_SIZE DISPLAY_WIDTH
#else
  #define MAX_DISPLAY_SIZE DISPLAY_HEIGHT
#endif

// ------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenSleep::ScreenSleep():

  wakeup(0, 0, MAX_DISPLAY_SIZE, MAX_DISPLAY_SIZE)

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenSleep::update(XPT2046::Point position) {

  // Update screen elements
  wakeup.update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenSleep::draw(GFXcanvas16 &canvas) {}