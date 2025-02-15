#ifndef _SCREE_GEIGER_COUNTER_SETTINGS_1_H
#define _SCREE_GEIGER_COUNTER_SETTINGS_1_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "Graphics.h"
#include "ScreenBasic.h"
#include "TouchIcon.h"
#include "DisplayDoseScreen.h"
#include "DisplayToastBox.h"

class ScreenGeigerCounterSettings1: public ScreenBasic {

  //-----------------------------------------------------------------------------------------------
  // Public
  
  public:

    // Screen elements
    TouchIcon next;
    TouchIcon previous;

    // Constructor
    ScreenGeigerCounterSettings1();

    // Update and draw calls
    void update(const XPT2046::Point &position) override;
    void draw(GFXcanvas16 &canvas) override;

    void setCounts(const uint64_t &counts);             // Set the total counts
    void setMainTubeCounts(const uint64_t &counts);     // Set the main tube counts
    void setFollowerTubeCounts(const uint64_t &counts); // Set the follower tube counts

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    String            _countsString;             // Counts string
    String            _mainTubeCountsString;     // Main tube counts string
    String            _followerTubeCountsString; // Follower tube counts string
    DisplayDoseScreen _equivalentDoseScreen;     // Counts screen
    DisplayToastBox   _mainTubeCounts;           // Main tube counts screen element
    DisplayToastBox   _followerTubeCounts;       // Follower tube counts screen element

};

#endif