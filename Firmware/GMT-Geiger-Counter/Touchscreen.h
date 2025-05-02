#ifndef _TOUCHSCREEN_H
#define _TOUCHSCREEN_H

#include "Arduino.h"
#include "Configuration.h"
#include "Logger.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Screen.h"
#include "ScreenSleep.h"
#include "ScreenGeigerCounter.h"

class Touchscreen {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Screens
    ScreenGeigerCounter geigerCounter; // Geiger counter screen

    // Get the single instance of the class
    static Touchscreen& getInstance();

    void begin();
    void enable();
    void disable();
    void update();
    void refresh();
    void draw(Screen &screen);
    void draw(Screen *screen);
    void sleep();
    void wakeup();
    void setTouchscreenState(const bool state);
    void setTimeoutState(const bool state);
    void setRotationLandscape();
    void setRotationPortrait();
    bool getTouchscreenState();
    bool getTimeoutState();

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    // Prevent direct instantiation
    Touchscreen();
    Touchscreen(const Touchscreen&) = delete;
    Touchscreen& operator=(const Touchscreen&) = delete;

    // Sleep screen
    ScreenSleep _sleep;

    bool             _initialized;             // Flag for checking if touchscreen was initialized
    Adafruit_ILI9341 _display;                 // Display object
    GFXcanvas16      _canvas;                  // Frame buffer object
    XPT2046          _touch;                   // Touch object
    bool             _enabled;                 // Flag for checking if the touchscreen is enabled
    uint8_t          _rotation;                // Touchscreen rotation orientation
    bool             _timeout;                 // Flag for checking if auto timeout is enabled
    uint64_t         _lastTouchMilliseconds;   // Variable for keeping track of when the last touch event occurred
    uint64_t         _lastRefreshMilliseconds; // Timer for refreshing the display
    Screen           *_screen;                 // Pointer to the current screen
    Screen           *_previousScreen;         // Pointer to the previous screen

};

// Global reference to the touchscreen instance for easy access
extern Touchscreen& touchscreen;

#endif