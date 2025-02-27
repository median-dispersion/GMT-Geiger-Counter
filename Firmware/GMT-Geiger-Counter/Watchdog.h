#ifndef _WATCHDOG_H
#define _WATCHDOG_H

#include "Arduino.h"
#include "Configuration.h"

class Watchdog {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    void begin();  // Initialize everything
    void update(); // Update the watchdog

};

#endif