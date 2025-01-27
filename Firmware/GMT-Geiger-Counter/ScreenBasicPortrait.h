#ifndef _SCREEN_BASIC_PORTRAIT_H
#define _SCREEN_BASIC_PORTRAIT_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Graphics.h"
#include "Screen.h"
#include "TouchIcon.h"

class ScreenBasicPortrait: public Screen {

  //-----------------------------------------------------------------------------------------------
  // Public

  public:

    // Screen elements
    TouchIcon back;

    // Constructor
    ScreenBasicPortrait(String title);

    // Update and draw calls
    void update(XPT2046::Point position) override;
    void draw(GFXcanvas16 &canvas) override;

  //-----------------------------------------------------------------------------------------------
  // Private
  
  private:

    // Screen title
    String _title;

};

#endif