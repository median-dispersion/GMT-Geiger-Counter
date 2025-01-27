#ifndef _SCREEN_COSMIC_RAY_DETECTOR_H
#define _SCREEN_COSMIC_RAY_DETECTOR_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "XPT2046.h"
#include "Strings.h"
#include "Graphics.h"
#include "ScreenBasicPortrait.h"
#include "DisplayInfoBox.h"
#include "DisplayToastBox.h"

class ScreenCosmicRayDetector: public ScreenBasicPortrait {

  //-----------------------------------------------------------------------------------------------
  // Public
  public:

    // Screen elements
    TouchIcon mute;
    TouchIcon sleep;

    // Constructor
    ScreenCosmicRayDetector();

    // Update and draw calls
    void update(XPT2046::Point position) override;
    void draw(GFXcanvas16 &canvas) override;

    // Set screen variables
    void setCoincidenceEventsOffset(uint64_t coincidenceEventsOffset);   // Set the coincidence events offset
    void setCoincidenceEvents(uint64_t coincidenceEvents);               // Calculate and set coincidence events
    void setCoincidenceEventsPerHour(double coincidenceEventsPerHour);   // Set the coincidence events per hour value
    void setCoincidenceEventsTotal(uint64_t coincidenceEventsTotal);     // Set the total number of coincidence events
    void setMainTubeCountsOffset(uint64_t mainTubeCountsOffset);         // Set the main tube offset
    void setFollowerTubeCountsOffset(uint64_t followerTubeCountsOffset); // Set the follower tube offset 
    void setMainTubeCounts(uint64_t mainTubeCounts);                     // Calculate and set main tube counts
    void setFollowerTubeCounts(uint64_t followerTubeCounts);             // Calculate and set follower tube counts

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    uint64_t        _coincidenceEventsOffset;  // Coincidence events offset
    String          _coincidenceEvents;        // Coincidence events
    DisplayInfoBox  _coincidenceEventsPerHour; // Coincidence events per hour
    DisplayInfoBox  _coincidenceEventsTotal;   // Total number of coincidence events since power on
    uint64_t        _mainTubeCountsOffset;     // Main tube offset
    uint64_t        _followerTubeCountsOffset; // Follower tube offset
    DisplayToastBox _mainTubeCounts;           // Main tube counts
    DisplayToastBox _followerTubeCounts;       // Follower tube counts

};

#endif