#ifndef _WATCHDOG_H
#define _WATCHDOG_H

#include "Arduino.h"
#include "Configuration.h"

class Watchdog {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Get the single instance of the class
    static Watchdog& getInstance();

    void begin();  // Initialize everything
    void update(); // Update the watchdog

  // ----------------------------------------------------------------------------------------------
  // Private
  
  private:

    // Prevent direct instantiation
    Watchdog();
    Watchdog(const Watchdog&) = delete;
    Watchdog& operator=(const Watchdog&) = delete;

    bool _initialized; // Flag for checking if SD card was initialized
    bool _psram;

};

// Global reference to the SD card instance for easy access
extern Watchdog& watchdog;

#endif