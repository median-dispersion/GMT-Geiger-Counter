#include "SDCard.h"

// ------------------------------------------------------------------------------------------------
// Public

// Initialize global reference
SDCard& sdCard = SDCard::getInstance();

// ================================================================================================
// Get the single instance of the class
// ================================================================================================
SDCard& SDCard::getInstance() {

  // Get the single instance
  static SDCard instance;

  // Return the instance
  return instance;

}

// ================================================================================================
// Initialize the SD card
// ================================================================================================
void SDCard::begin() {

  // If SD card is not initialized
  if (!_initialized) {

    // Set the initialization flag to true
    _initialized = true;

    // Setup SD card chip select pin
    pinMode(SD_CS_PIN, OUTPUT);
    digitalWrite(SD_CS_PIN, HIGH);

    // Initialize logger
    logger.begin();

    // Mount the SD card
    mount();

  }

}

// ================================================================================================
// Mount the SD card
// ================================================================================================
void SDCard::mount() {

  // If SD card is not mounted
  if (!_mounted) {

    // Try mounting SD card
    if (SD.begin(SD_CS_PIN)) {

      // If successfully mounted create directories
      SD.mkdir(SD_CARD_ROOT_DIRECTORY);
      SD.mkdir(SD_CARD_LOG_DIRECTORY);
      SD.mkdir(SD_CARD_WEB_APP_DIRECTORY);

      // Set mounted flag to true
      _mounted = true;

      // Create event data
      Logger::KeyValuePair event[2] = {

        {"source", Logger::STRING_T, {.string_v = "sdCard"} },
        {"action", Logger::STRING_T, {.string_v = "mounted"}}

      };

      // Log event message
      logger.log(Logger::EVENT, "event", event, 2);
    
    // If mounting SD card failed
    } else {

      // Manually deselect SD card
      digitalWrite(SD_CS_PIN, HIGH);

      // Set mounted flag to false
      _mounted = false;

    }

  }

}

// ================================================================================================
// Unmount the SD card
// ================================================================================================
void SDCard::unmount() {

  // If the SD card is mounted
  if (_mounted) {

    // Unmount SD card
    SD.end();

    // Set mounted flag to false
    _mounted = false;

    // Create event data
    Logger::KeyValuePair event[2] = {

      {"source", Logger::STRING_T, {.string_v = "sdCard"}   },
      {"action", Logger::STRING_T, {.string_v = "unmounted"}}

    };

    // Log event message
    logger.log(Logger::EVENT, "event", event, 2);

  }

}

// ================================================================================================
// Set the SD card mount state
// ================================================================================================
void SDCard::setMountState(const bool state) {

  // Depending on mount state either mount or unmount the SD card
  if (state) {

    mount();

  } else {

    unmount();

  }

}

// ================================================================================================
// Return the mount state of the SD card
// ================================================================================================
bool SDCard::getMountState() {

  // If SD card is flagged as mounted
  if (_mounted) {

    // Check if root directory can be accessed
    if (!SD.exists(SD_CARD_ROOT_DIRECTORY)) {

      // If accessing the root directory failed unmount SD card
      // Setting the mounted sate to false
      unmount();

    }
  
  }

  // Return the mount state
  return _mounted;

}

// ================================================================================================
// SD card wrapper function for opening files
// ================================================================================================
File SDCard::open(const char *path, const char *mode, const bool create) {

  return SD.open(path, mode, create);

}

// ================================================================================================
// SD card wrapper function for checking if an element exists
// ================================================================================================
bool SDCard::exists(const char *path) {

  return SD.exists(path);

}

// ------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Constructor
// ================================================================================================
SDCard::SDCard():

  // Initialize members
  _initialized(false),
  _mounted(false)

{}