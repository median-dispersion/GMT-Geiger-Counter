#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "Arduino.h"
#include "Configuration.h"
#include "SDCard.h"

class Settings {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Settings data structure
    struct Data {

      // CRC for validity and change checking
      uint32_t crc;
      
      struct Parameters {

        // Logger parameters
        struct Logger {

          bool serial;
          bool sdCard;
          bool data;
          bool event;
          bool system;

        } logger;

        // Wireless parameters
        struct Wireless {

          bool hotspot;
          bool wifi;

        } wireless;

      } parameters;

    } data;

    // Get the single instance of the class
    static Settings& getInstance();

    void begin();  // Initialize everything
    void update(); // Update the settings
    void load();   // Load settings from the SD card
    void save();   // Save settings to the SD card
    void reset();  // Reset all settings to their default values

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    // Prevent direct instantiation
    Settings();
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    bool _initialized; // Flag for checking if settings have been initialized

    uint32_t _getCRC(const Settings::Data::Parameters &data); // Calculate CRC

};

// Global reference to the settings instance for easy access
extern Settings& settings;

#endif