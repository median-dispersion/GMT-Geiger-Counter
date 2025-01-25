#ifndef _TOUCH_TOGGLE_H
#define _TOUCH_TOGGLE_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Graphics.h"

class TouchToggle {

  //-----------------------------------------------------------------------------------------------
  // Public

  public:

    // Constructor
    TouchToggle(uint8_t row, String text, bool toggled);

    void update(XPT2046::Point position); // Update the touch toggle with the touch position
    void draw(GFXcanvas16 &canvas);       // Draw the touch toggle to the frame buffer
    void (*action)(bool toggled);         // Function pointer to execute a function on touchdown
    void toggle();                        // Toggle state
    void toggleOn();                      // Toggle on
    void toggleOff();                     // Toggle off
    bool toggled();                       // Return the toggle state

  //-----------------------------------------------------------------------------------------------
  // Private

  private:

    uint16_t _y;       // Y position
    String   _text;    // Toggle text
    bool     _toggled; // Toggle state

};

#endif