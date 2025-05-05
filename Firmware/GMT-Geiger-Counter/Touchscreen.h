#ifndef _TOUCHSCREEN_H
#define _TOUCHSCREEN_H

#include "Arduino.h"
#include "Configuration.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Logger.h"
#include "Screen.h"
#include "ScreenSleep.h"
#include "ScreenGeigerCounter.h"
#include "ScreenGeigerCounterSettings1.h"
#include "ScreenGeigerCounterSettings2.h"
#include "ScreenAudioSettings.h"
#include "ScreenDisplaySettings.h"
#include "ScreenRotationConfirmation.h"
#include "ScreenCosmicRayDetector.h"
#include "ScreenDisableCosmicRayDetector.h"
#include "ScreenRadiationHistory.h"
#include "ScreenTrueRNG.h"
#include "ScreenHotspotSettings.h"
#include "ScreenWiFiSettings.h"
#include "ScreenSystemSettings1.h"
#include "ScreenSystemSettings2.h"

class Touchscreen {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    ScreenSleep                    sleepScreen;
    ScreenGeigerCounter            geigerCounter;
    ScreenGeigerCounterSettings1   geigerCounterSettings1;
    ScreenGeigerCounterSettings2   geigerCounterSettings2;
    ScreenAudioSettings            audioSettings;
    ScreenDisplaySettings          displaySettings;
    ScreenRotationConfirmation     rotationConfirmation;
    ScreenCosmicRayDetector        cosmicRayDetector;
    ScreenDisableCosmicRayDetector disableCosmicRayDetector;
    ScreenRadiationHistory         radiationHistory;
    ScreenTrueRNG                  trueRNG;
    ScreenHotspotSettings          hotspotSettings;
    ScreenWiFiSettings             wifiSettings;
    ScreenSystemSettings1          systemSettings1;
    ScreenSystemSettings2          systemSettings2;

    // Get the single instance of the class
    static Touchscreen& getInstance();

    void begin();                               // Initialize everything
    void enable();                              // Enable the touchscreen
    void disable();                             // Disable the touchscreen
    void update();                              // Update the touchscreen
    void refresh();                             // Refresh the display by writing the pixel data to it
    void sleep();                               // Go to sleep
    void draw(Screen &screen);                  // Draw a screen by reference
    void draw(Screen *screen);                  // Draw a screen by pointer
    void drawPreviousScreen();                  // Draw the previous screen
    void setTouchscreenState(const bool state); // Set the touchscreen state
    void setTimeoutState(const bool state);     // Set auto timeout state
    void setRotationLandscape();                // Rotate the touchscreen into landscape orientation
    void setRotationPortrait();                 // Rotate the touchscreen into portrait orientation
    bool getTouchscreenState();                 // Get the touchscreen state
    bool getTimeoutState();                     // Get the auto timeout state

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    // Prevent direct instantiation
    Touchscreen();
    Touchscreen(const Touchscreen&) = delete;
    Touchscreen& operator=(const Touchscreen&) = delete;

    bool             _initialized;             // Flag for checking if touchscreen was initialized
    Adafruit_ILI9341 _display;                 // Display object
    GFXcanvas16      _canvas;                  // Frame buffer object
    XPT2046          _touch;                   // Touch object
    Screen           *_screen;                 // Current screen
    Screen           *_previousScreen;         // Previous screen
    bool             _enabled;                 // Flag for checking if the touchscreen is enabled
    uint8_t          _rotation;                // Touchscreen rotation orientation
    bool             _timeout;                 // Flag for checking if auto timeout is enabled
    uint64_t         _lastTouchMilliseconds;   // Variable for keeping track of when the last touch event occurred
    uint64_t         _lastRefreshMilliseconds; // Timer for refreshing the display

};

// Global reference to the touchscreen instance for easy access
extern Touchscreen& touchscreen;

#endif