#include "Arduino.h"
#include "Configuration.h"
#include "Logger.h"
#include "SDCard.h"
#include "Wireless.h"
#include "Settings.h"

// ------------------------------------------------------------------------------------------------
// Global

// Global variables
uint64_t logTimer = 0;

// Function prototypes
void setup();
void loop();
void loadSettings();
void dataFeedback();

// ================================================================================================
// Setup
// ================================================================================================
void setup() {

  // --------------------------------------------
  // Initialize everything

  logger.begin();
  sdCard.begin();
  wireless.begin();
  settings.begin();

  // Load user settings
  loadSettings();

}

// ================================================================================================
// Main loop
// ================================================================================================
void loop() {

  // Give data feedback
  dataFeedback();

  // Update the wireless inteface
  wireless.update();

  // Update the user settings
  settings.update();

}

// ------------------------------------------------------------------------------------------------
// Setup functions

// ================================================================================================
// Load user settings
// ================================================================================================
void loadSettings() {

  // Load settings from SD card
  settings.load();

  // --------------------------------------------
  // Logger settings

  logger.setSerialLoggingState(settings.data.parameters.logger.serial);
  logger.setSDCardLoggingState(settings.data.parameters.logger.sdCard);
  logger.setLogLevelState(Logger::DATA, settings.data.parameters.logger.data);
  logger.setLogLevelState(Logger::EVENT, settings.data.parameters.logger.event);
  logger.setLogLevelState(Logger::SYSTEM, settings.data.parameters.logger.system);

  // --------------------------------------------
  // Wireless settings

  wireless.setHotspotState(settings.data.parameters.wireless.hotspot);
  // wireless.setWiFiState(settings.data.parameters.wireless.wifi);

}

// ------------------------------------------------------------------------------------------------
// Loop functions

// ================================================================================================
// Data feedback
// ================================================================================================
void dataFeedback() {

  // If the log interval has been reached
  if (millis() - logTimer > LOG_INTERVAL_SECONDS * 1000) {

    // Creat a log message string
    String message = "";

    // --------------------------------------------
    // System data

    // If system logging is enabled
    if (logger.getLogLevelState(Logger::SYSTEM)) {

      Logger::KeyValuePair systemData[10] = {

        {"upTime",   Logger::UINT64_T, {.uint64_v = millis()}                  },
        {"totalHeap",Logger::UINT32_T, {.uint32_v = ESP.getHeapSize()}         },
        {"freeHeap", Logger::UINT32_T, {.uint32_v = ESP.getFreeHeap()}         },
        {"minHeap",  Logger::UINT32_T, {.uint32_v = ESP.getMinFreeHeap()}      },
        {"maxBlock", Logger::UINT32_T, {.uint32_v = ESP.getMaxAllocHeap()}     },
        {"sdCard",   Logger::BOOL_T,   {.bool_v   = sdCard.getMountState()}    },
        {"hotspot",  Logger::BOOL_T,   {.bool_v   = wireless.getHotspotState()}},
        {"wifi",     Logger::BOOL_T,   {.bool_v   = wireless.getWiFiState()}   },
        {"server",   Logger::BOOL_T,   {.bool_v   = wireless.getServerState()} },
        {"firmware", Logger::STRING_T, {.string_v = FIRMWARE_VERSION}          }

      };

      logger.log(Logger::SYSTEM, "system", systemData, 10, message);
    
    }

    logTimer = millis();

  }

}