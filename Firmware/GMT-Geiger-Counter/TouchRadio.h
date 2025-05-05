#ifndef _TOUCH_RADIO_H
#define _TOUCH_RADIO_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Graphics.h"

class TouchRadio {

  //-----------------------------------------------------------------------------------------------
  // Public

  public:

    // Constructor
    TouchRadio(const uint8_t row, const char *text, const bool selected);

    void update(const XPT2046::Point &position); // Update the touch radio with the touch position
    void draw(GFXcanvas16 &canvas);              // Draw the touch radio to the frame buffer
    void (*action)();                            // Function pointer to execute a function on touchdown
    void select();                               // Select radio
    void deselect();                             // Deselect radio
    void setSelectState(const bool selected);    // Set a specific select state
    bool getSelectState();                       // Return the radio state

  //-----------------------------------------------------------------------------------------------
  // Private

  private:

    const uint16_t _y;        // Y position
    const char     *_text;    // Radio text
    bool           _selected; // Radio state

};

#endif