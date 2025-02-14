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
#include "ScreenRotationConfirmation.h"
#include "ScreenCosmicRayDetector.h"
#include "ScreenRadiationHistory.h"
#include "ScreenTrueRNG.h"
#include "ScreenHotspotSettings.h"
#include "ScreenWiFiSettings.h"
#include "ScreenSystemSettings.h"

class Touchscreen {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Screens
    ScreenGeigerCounter        geigerCounter;        // Geiger counter screen
    ScreenAudioSettings        audioSettings;        // Audio settings screen
    ScreenDisplaySettings      displaySettings;      // Display settings screen
    ScreenSleep                sleep;                // Sleep / timeout screen for waking the display
    ScreenRotationConfirmation rotationConfirmation; // Rotation confirmation screen
    ScreenCosmicRayDetector    cosmicRayDetector;    // Cosmic ray detector screen
    ScreenRadiationHistory     radiationHistory;     // Radiation history screen
    ScreenTrueRNG              trueRNG;              // True random number generator screen
    ScreenHotspotSettings      hotspotSettings;      // Hotspot settings screen
    ScreenWiFiSettings         wifiSettings;         // WiFi settings screen
    ScreenSystemSettings       systemSettings;       // System settings screen

    // Constructor
    Touchscreen();

    void    begin();              // Initialize everything
    void    enable();             // Enable the touchscreen
    void    disable();            // Disable the touchscreen
    bool    enabled();            // Returns if the touchscreen is enabled
    void    update();             // Update the touchscreen
    void    refresh();            // Refresh the display by writing the pixel data to it
    void    draw(Screen &screen); // Draw a screen by reference
    void    draw(Screen *screen); // Draw a screen by pointer
    void    rotateLandscape();    // Rotate the display to the landscape orientation
    void    rotatePortrait();     // Rotate the display to the portrait orientation
    void    enableTimeout();      // Enable display auto timeout
    void    disableTimeout();     // Disable display auto timeout
    bool    timeout();            // Retruns if the auto timeout has been enabled
    Screen* getScreen();          // Retruns the current screen
    Screen* getPreviousScreen();  // Retruns the previous screen

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    Adafruit_ILI9341 _display;                 // Display object
    GFXcanvas16      _canvas;                  // Frame buffer object
    XPT2046          _touch;                   // Touch object
    Screen           *_screen;                 // Pointer to the current screen
    Screen           *_previousScreen;         // Pointer to the previous screen
    bool             _enabled;                 // Flag for checking if the touchscreen is enabled
    uint64_t         _lastRefreshMilliseconds; // Timer for refreshing the display
    uint8_t          _rotation;                // Touchscreen rotation orientation
    bool             _timeout;                 // Flag for checking if auto timeout is enabled
    uint64_t         _lastTouchMilliseconds;   // Variable for keeping track of when the last touch event occurred

};

#endif