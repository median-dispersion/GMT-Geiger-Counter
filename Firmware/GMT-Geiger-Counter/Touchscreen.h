#ifndef _TOUCHSCREEN_H
#define _TOUCHSCREEN_H

#include "Arduino.h"
#include "Configuration.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Screen.h"
#include "ScreenGeigerCounter.h"
#include "ScreenAudioSettings.h"
#include "ScreenDisplaySettings.h"
#include "ScreenSleep.h"
#include "ScreenRadiationHistory.h"
#include "ScreenCosmicRayDetector.h"
#include "ScreenTrueRNG.h"
#include "ScreenHotspotSettings.h"
#include "ScreenWiFiSettings.h"
#include "ScreenSystemSettings.h"

class Touchscreen {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Screens
    ScreenGeigerCounter     geigerCounter;     // Geiger counter screen
    ScreenAudioSettings     audioSettings;     // Audio settings screen
    ScreenDisplaySettings   displaySettings;   // Display settings screen
    ScreenSleep             sleep;             // Sleep / timeout screen for waking the display
    ScreenRadiationHistory  radiationHistory;  // Radiation history screen
    ScreenCosmicRayDetector cosmicRayDetector; // Cosmic ray detector screen
    ScreenTrueRNG           trueRNG;           // True random number generator screen
    ScreenHotspotSettings   hotspotSettings;   // Hotspot settings screen
    ScreenWiFiSettings      wifiSettings;      // WiFi settings screen
    ScreenSystemSettings    systemSettings;    // System settings screen

    // Constructor
    Touchscreen();

    void    begin();              // Initialize everything
    void    update();             // Update the touchscreen
    void    draw(Screen &screen); // Draw a screen by reference
    void    draw(Screen *screen); // Draw a screen by pointer
    void    on();                 // Turn display on
    void    off();                // Turn display off
    Screen* getScreen();          // Retruns the current screen

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    Adafruit_ILI9341 _display;                 // Display object
    GFXcanvas16      _canvas;                  // Frame buffer object
    XPT2046          _touch;                   // Touch object
    Screen           *_screen;                 // Pointer to a screen
    uint64_t         _lastRefreshMilliseconds; // Timer for refreshing the display

};

#endif