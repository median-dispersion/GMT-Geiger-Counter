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
#include "ScreenGeigerCounterInfo1.h"
#include "ScreenGeigerCounterInfo2.h"
#include "ScreenGeigerCounterInfo3.h"
#include "ScreenAudioSettings.h"
#include "ScreenDisplaySettings.h"
#include "ScreenRotationConfirmation.h"
#include "ScreenCosmicRayDetector.h"
#include "ScreenDisableCosmicRayDetector.h"
#include "ScreenRadiationHistory.h"
#include "ScreenDisableGeigerCounter.h"
#include "ScreenTrueRNG.h"
#include "ScreenHotspotSettings.h"
#include "ScreenWiFiSettings.h"
#include "ScreenSystemSettings1.h"
#include "ScreenSystemSettings2.h"
#include "ScreenSystemSettings3.h"

class Touchscreen {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    ScreenSleep                    sleepScreen;
    ScreenGeigerCounter            geigerCounter;
    ScreenGeigerCounterInfo1       geigerCounterInfo1;
    ScreenGeigerCounterInfo2       geigerCounterInfo2;
    ScreenGeigerCounterInfo3       geigerCounterInfo3;
    ScreenAudioSettings            audioSettings;
    ScreenDisplaySettings          displaySettings;
    ScreenRotationConfirmation     rotationConfirmation;
    ScreenCosmicRayDetector        cosmicRayDetector;
    ScreenDisableCosmicRayDetector disableCosmicRayDetector;
    ScreenRadiationHistory         radiationHistory;
    ScreenDisableGeigerCounter     disableGeigerCounter;
    ScreenTrueRNG                  trueRNG;
    ScreenHotspotSettings          hotspotSettings;
    ScreenWiFiSettings             wifiSettings;
    ScreenSystemSettings1          systemSettings1;
    ScreenSystemSettings2          systemSettings2;
    ScreenSystemSettings3          systemSettings3;

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