#include "Settings.h"

// ------------------------------------------------------------------------------------------------
// Public

// Initialize global reference
Settings& settings = Settings::getInstance();

// ================================================================================================
// Get the single instance of the class
// ================================================================================================
Settings& Settings::getInstance() {

  // Get the single instance
  static Settings instance;

  // Return the instance
  return instance;

}

// ================================================================================================
// Initialize everything
// ================================================================================================
void Settings::begin() {

  // If not initialized
  if (!_initialized) {

    // Set initialization flag to true
    _initialized = true;

    // Initialize SD Card
    sdCard.begin();

    // Load settings
    load();

  }

}

// ================================================================================================
// Update the settings
// ================================================================================================
void Settings::update() {

  // Check if settings have changed
  if (data.crc != _getCRC(data.parameters)) {

    // Save changed settings to the settings file
    save();

  }

}

// ================================================================================================
// Load settings from the SD card
// ================================================================================================
void Settings::load() {
  
  // Flag for checking if loaded settings are valid
  bool valid = false;

  // If the SD card is mounted
  if (sdCard.getMountState()) {
    
    // If the settings file exists
    if (sdCard.exists(SD_CARD_SETTINGS_FILE)) {

      // Open settings file
      File file = sdCard.open(SD_CARD_SETTINGS_FILE);

      // If the settings file is the correct size
      if (file && file.size() == sizeof(Data)) {

        // Read the file into the data struct
        file.read((uint8_t *)&data, sizeof(Data));

        // Check if the CRC is valid
        if (data.crc == _getCRC(data.parameters)) {

          // Set the valid flag to true
          valid = true;

        }

      }

      // Close settings file
      file.close();
      
    }

  }

  // If loaded setting were not valid
  if (!valid) {

    // Reset settings to the default
    reset();

  }

}

// ================================================================================================
// Save settings to the SD card
// ================================================================================================
void Settings::save() {

  // If SD card is mounted
  if (sdCard.getMountState()) {

    // Get CRC
    data.crc = _getCRC(data.parameters);

    // Open settings file in write mode
    File file = sdCard.open(SD_CARD_SETTINGS_FILE, FILE_WRITE);

    // If successfully accessed settings file
    if (file) {

      // Write settings data to file
      file.write((uint8_t *)&data, sizeof(Data));

    }

    // Close settings file
    file.close();

  }

}

// ================================================================================================
// Reset all settings to their default values
// ================================================================================================
void Settings::reset() {

  // Set all parameters to their default value

  // --------------------------------------------
  // Logger parameter

  data.parameters.logger.serial = true;
  data.parameters.logger.sdCard = true;
  data.parameters.logger.data   = true;
  data.parameters.logger.event  = true;
  data.parameters.logger.system = true;

  // --------------------------------------------
  // Geiger counter parameter

  data.parameters.geigerCounter.autoIntegrate   = true;
  data.parameters.geigerCounter.autoRange       = true;
  data.parameters.geigerCounter.measurementUnit = GeigerCounter::SIEVERTS;

  // --------------------------------------------
  // Buzzer parameter

  data.parameters.buzzer.alerts        = true;
  data.parameters.buzzer.detections    = true;
  data.parameters.buzzer.interface     = true;
  data.parameters.buzzer.notifications = true;
  data.parameters.buzzer.muted         = false;

  // --------------------------------------------
  // Wireless parameter

  data.parameters.wireless.hotspot = false;
  data.parameters.wireless.wifi    = false;

  // Save settings to file
  save();

}

// ------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Constructor
// ================================================================================================
Settings::Settings():

  // Initialize members
  _initialized(false)

{}

// ================================================================================================
// Calculate CRC
// ================================================================================================
uint32_t Settings::_getCRC(const Settings::Data::Parameters &data) {

  // Generic CRC algorithm for calculating the CRC of the parameters struct

  const uint8_t *bytes = (const uint8_t *)&data;
  size_t        length = sizeof(Settings::Data::Parameters);
  uint32_t      crc    = 0xFFFFFFFF;

  for (size_t i = 0; i < length; i++) {

    crc ^= bytes[i];

    for (uint8_t j = 0; j < 8; j++) {

      if (crc & 1) { crc = (crc >> 1) ^ 0xEDB88320; }
      else         { crc >>= 1;                     }

    }

  }

  return ~crc;

}