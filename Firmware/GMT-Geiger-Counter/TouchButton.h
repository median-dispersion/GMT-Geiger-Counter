#ifndef _TOUCH_BUTTON_H
#define _TOUCH_BUTTON_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Graphics.h"

class TouchButton {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Constructor
    TouchButton(
      const uint16_t x, 
      const uint16_t y, 
      const uint16_t width, 
      const uint16_t height,
      const char     *text,
      const uint16_t backgroundColor = COLOR_GRAY_MEDIUM, 
      const uint16_t borderColor = COLOR_GRAY_LIGHT
    );

    void update(const XPT2046::Point &position); // Update the touch button with the touch position
    void draw(GFXcanvas16 &canvas);              // Draw the touch button to the frame buffer
    void (*action)();                            // Function pointer to execute a function on touchdown

  //-----------------------------------------------------------------------------------------------
  // Private

  private:

    const uint16_t _x;               // X position
    const uint16_t _y;               // Y position
    const uint16_t _width;           // Touch button width
    const uint16_t _height;          // Touch button height
    const char    *_text;            // Touch button text
    const uint16_t _backgroundColor; // Touch button background color
    const uint16_t _borderColor;     // Touch button border color

};

#endif