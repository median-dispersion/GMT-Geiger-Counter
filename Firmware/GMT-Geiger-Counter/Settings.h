#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "Arduino.h"
#include "Configuration.h"
#include "SDCard.h"
#include "GeigerCounter.h"

class Settings {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Settings data structure
    struct Data {

      // CRC for validity and change checking
      uint32_t crc;
      
      struct Parameters {

        // --------------------------------------------
        // Logger parameter

        struct LoggerParameter {

          bool serial;
          bool sdCard;
          bool data;
          bool event;
          bool system;

        } logger;
        
        // --------------------------------------------
        // Geiger counter parameter

        struct GeigerCounterParameter {

          bool                           autoIntegrate;
          bool                           autoRange;
          GeigerCounter::MeasurementUnit measurementUnit;

        } geigerCounter;

        // --------------------------------------------
        // Wireless parameter
        
        struct WirelessParameter {

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