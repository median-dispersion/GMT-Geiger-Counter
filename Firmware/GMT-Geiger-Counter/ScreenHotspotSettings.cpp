#include "ScreenHotspotSettings.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenHotspotSettings::ScreenHotspotSettings():

  // Initialize members
  ScreenBasic(STRING_HOTSPOT_SETTINGS_TITLE),
  placeholder(2, 31, 316, "Info", "Not yet available!")

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenHotspotSettings::update(XPT2046::Point position) {

  // Update the basic screen
  ScreenBasic::update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenHotspotSettings::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasic::draw(canvas);

  // Draw screen elements
  placeholder.draw(canvas);
  
}