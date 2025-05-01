#include "Arduino.h"
#include "Configuration.h"
#include "Logger.h"
#include "SDCard.h"
#include "Wireless.h"
#include "Settings.h"
#include "GeigerCounter.h"
#include "CosmicRayDetector.h"
#include "Buzzer.h"
#include "Touchscreen.h"

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
  geigerCounter.begin();
  cosmicRayDetector.begin();
  buzzer.begin();
  touchscreen.begin();

  // Load user settings
  loadSettings();

  // Enable the Geiger counter
  geigerCounter.enable();

  // Play the power on jingle
  buzzer.play(buzzer.jingle);

}

// ================================================================================================
// Main loop
// ================================================================================================
void loop() {

  // Give data feedback
  dataFeedback();

  // Update all objects
  wireless.update();
  settings.update();
  buzzer.update();
  touchscreen.update();

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
  // Geiger counter settings

  geigerCounter.setAutoIntegrateState(settings.data.parameters.geigerCounter.autoIntegrate);
  geigerCounter.setAutoRangeState(settings.data.parameters.geigerCounter.autoRange);
  geigerCounter.setMeasurementUnit(settings.data.parameters.geigerCounter.measurementUnit);

  // --------------------------------------------
  // Buzzer settings

  buzzer.alerts.setMuteState(settings.data.parameters.buzzer.alerts);
  buzzer.detections.setMuteState(settings.data.parameters.buzzer.detections);
  buzzer.interface.setMuteState(settings.data.parameters.buzzer.interface);
  buzzer.notifications.setMuteState(settings.data.parameters.buzzer.notifications);
  buzzer.setMuteState(settings.data.parameters.buzzer.muted);

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

      // Get system data
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

      // Log system data
      logger.log(Logger::SYSTEM, "system", systemData, 10, message);
    
    }

    // --------------------------------------------
    // Geiger counter data

    // If the Geiger counter is enabled
    if (geigerCounter.getGeigerCounterState()) {

      // Get Geiger counter data
      Logger::KeyValuePair geigerCounterData[7] = {

        {"counts",               Logger::UINT64_T, {.uint64_v = geigerCounter.getCounts()}                },
        {"mainCounts",           Logger::UINT64_T, {.uint64_v = geigerCounter.getMainTubeCounts()}        },
        {"followerCounts",       Logger::UINT64_T, {.uint64_v = geigerCounter.getFollowerTubeCounts()}    },
        {"countsPerMinute",      Logger::DOUBLE_T, {.double_v = geigerCounter.getCountsPerMinute(60)}     },
        {"microsievertsPerHour", Logger::DOUBLE_T, {.double_v = geigerCounter.getMicrosievertsPerHour(60)}},
        {"tubes",                Logger::UINT8_T,  {.uint8_v  = TOTAL_NUMBER_OF_TUBES}                    },
        {"tubeType",             Logger::STRING_T, {.string_v = TUBE_TYPE_NAME}                           }

      };

      // Log Geiger counter data
      logger.log(Logger::DATA, "geigerCounter", geigerCounterData, 7, message);

    }

    // --------------------------------------------
    // Cosmic ray detector data

    // If the cosmic ray detector is enabled
    if (cosmicRayDetector.getCosmicRayDetectorSate()) {

      // Get cosmic ray detector data
      Logger::KeyValuePair cosmicRayDetectorData[4] = {

        {"coincidenceEvents", Logger::UINT64_T, {.uint64_v = cosmicRayDetector.getCoincidenceEvents()}       },
        {"eventsPerHour",     Logger::UINT32_T, {.uint32_v = cosmicRayDetector.getCoincidenceEventsPerHour()}},
        {"mainCounts",        Logger::UINT64_T, {.uint64_v = cosmicRayDetector.getMainTubeCounts()}          },
        {"followerCounts",    Logger::UINT64_T, {.uint64_v = cosmicRayDetector.getFollowerTubeCounts()}      }

      };

      // Log cosmic ray detector data
      logger.log(Logger::DATA, "cosmicRayDetector", cosmicRayDetectorData, 4, message);

    }

    logTimer = millis();

  }

}