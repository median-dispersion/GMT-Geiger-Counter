#ifndef _SCREE_GEIGER_COUNTER_INFO_2_H
#define _SCREE_GEIGER_COUNTER_INFO_2_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "Graphics.h"
#include "ScreenBasicLandscape.h"
#include "TouchIcon.h"
#include "DisplayDoseScreen.h"
#include "DisplayToastBox.h"

class ScreenGeigerCounterInfo2: public ScreenBasicLandscape {

  //-----------------------------------------------------------------------------------------------
  // Public
  
  public:

    // Screen elements
    TouchIcon next;
    TouchIcon previous;

    // Constructor
    ScreenGeigerCounterInfo2();

    // Update and draw calls
    void update(const XPT2046::Point &position) override;
    void draw(GFXcanvas16 &canvas) override;

    void setTotalAbsorbedDose(const uint64_t &dose);    // Set the total absorbed dose
    void setMainAbsorbedDose(const uint64_t &dose);     // Set the main tube absorbed dose
    void setFollowerAbsorbedDose(const uint64_t &dose); // Set the follower tube absorbed dose

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    String            _absorbedDoseString;         // Absorbed dose string
    String            _mainAbsorbedDoseString;     // Main tube absorbed dose string
    String            _followerAbsorbedDoseString; // Follower tube absorbed dose string
    DisplayDoseScreen _absorbedDoseScreen;         // Absorbed dose screen
    DisplayToastBox   _mainAbsorbedDose;           // Main tube absorbed dose screen element
    DisplayToastBox   _followerAbsorbedDose;       // Follower tube absorbed dose screen element

};

#endif