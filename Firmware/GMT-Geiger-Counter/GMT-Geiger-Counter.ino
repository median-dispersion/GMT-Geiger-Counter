#include "Arduino.h"
#include "Configuration.h"
#include "SD.h"
#include "Logger.h"
#include "GeigerCounter.h"
#include "CosmicRayDetector.h"
#include "Buzzer.h"
#include "Touchscreen.h"
#include "Wireless.h"
#include "Watchdog.h"

// ------------------------------------------------------------------------------------------------
// Global

// Global objects
GeigerCounter     geigerCounter;
CosmicRayDetector cosmicRayDetector;
Buzzer            buzzer;
Touchscreen       touchscreen;
Wireless          wireless;
Watchdog          watchdog;

// Global variables
bool     playedDoseWarning     = false;
uint64_t lastCounts            = 0;
uint64_t lastCoincidenceEvents = 0;
bool     systemLogging         = false;
uint64_t logTimer              = 0;
bool     lastMute              = false;

// Function prototypes
void setup();
void loop();
void audioFeedback();
void visualFeedback();
void dataFeedback();
void displayGeigerCounter();
void displayGeigerCounterSettings1();
void displayGeigerCounterSettings2();
void displayAudioSettings();
void displayDisplaySettings();
void displayRadiationHistory();
void displayRotationConfirmation();
void displayCosmicRayDetector();
void displayTrueRNG();
void displayHotspotSettings();
void displayWiFiSettings();
void displaySystemSettings();
void decreaseIntegrationTime();
void resetIntegrationTime();
void increaseIntegrationTime();
void toggleAudioDetections(const bool toggled);
void toggleAudioNotifications(const bool toggled);
void toggleAudioAlerts(const bool toggled);
void toggleAudioInterface(const bool toggled);
void toggleAudioMuteEverything(const bool toggled);
void toggleDisplayPower(const bool toggled);
void toggleDisplayTimeout(const bool toggled);
void toggleGeigerCounterAutoRange(const bool toggled);
void deselectAllUnits();
void selectSievertUnit();
void selectRemUnit();
void selectRontgenUnit();
void selectGrayUnit();
void goToSleep();
void wakeFromSleep();
void cosmicRayDetectorMute();
void cosmicRayDetectorSleep();
void toggleHotspot(const bool toggled);
void toggleWiFi(const bool toggled);
void toggleSerialLogging(const bool toggled);
void toggleSDCardLogging(const bool toggled);
void toggleEventLogging(const bool toggled);
void toggleSystemLogging(const bool toggled);
void sendGeigerCounterData();
void sendCosmicRayDetectorData();
void sendSystemData();

// ================================================================================================
// Setup
// ================================================================================================
void setup() {

  // --------------------------------------------
  // Initialize everything

  SD.begin(SD_CS_PIN);
  logger.begin();
  geigerCounter.begin();
  cosmicRayDetector.begin();
  buzzer.begin();
  touchscreen.begin();
  wireless.begin();
  watchdog.begin();

  // --------------------------------------------
  // Assign touch actions

  // Geiger counter screen touch actions
  touchscreen.geigerCounter.audioSettings.action           = displayAudioSettings;
  touchscreen.geigerCounter.displaySettings.action         = displayDisplaySettings;
  touchscreen.geigerCounter.goToSleep.action               = goToSleep;
  touchscreen.geigerCounter.decreaseIntegrationTime.action = decreaseIntegrationTime;
  touchscreen.geigerCounter.resetIntegrationTime.action    = resetIntegrationTime;
  touchscreen.geigerCounter.increaseIntegrationTime.action = increaseIntegrationTime;
  touchscreen.geigerCounter.radiationHistory.action        = displayRadiationHistory;
  touchscreen.geigerCounter.cosmicRayDetector.action       = displayRotationConfirmation;
  touchscreen.geigerCounter.trueRNG.action                 = displayTrueRNG;
  touchscreen.geigerCounter.hotspotSettings.action         = displayHotspotSettings;
  touchscreen.geigerCounter.wifiSettings.action            = displayWiFiSettings;
  touchscreen.geigerCounter.systemSettings.action          = displaySystemSettings;
  touchscreen.geigerCounter.geigerCounterSetting.action    = displayGeigerCounterSettings1;

  // Geiger counter settings 1 touch actions
  touchscreen.geigerCounterSettings1.back.action           = displayGeigerCounter;
  touchscreen.geigerCounterSettings1.next.action           = displayGeigerCounterSettings2;
  touchscreen.geigerCounterSettings1.previous.action       = displayGeigerCounterSettings2;

  // Geiger counter settings 2 touch actions
  touchscreen.geigerCounterSettings2.back.action           = displayGeigerCounter;
  touchscreen.geigerCounterSettings2.autoRange.action      = toggleGeigerCounterAutoRange;
  touchscreen.geigerCounterSettings2.sievert.action        = selectSievertUnit;
  touchscreen.geigerCounterSettings2.rem.action            = selectRemUnit;
  touchscreen.geigerCounterSettings2.rontgen.action        = selectRontgenUnit;
  touchscreen.geigerCounterSettings2.gray.action           = selectGrayUnit;
  touchscreen.geigerCounterSettings2.next.action           = displayGeigerCounterSettings1;
  touchscreen.geigerCounterSettings2.previous.action       = displayGeigerCounterSettings1;

  // Audio settings screen touch actions
  touchscreen.audioSettings.back.action                    = displayGeigerCounter;
  touchscreen.audioSettings.detections.action              = toggleAudioDetections;
  touchscreen.audioSettings.notifications.action           = toggleAudioNotifications;
  touchscreen.audioSettings.alerts.action                  = toggleAudioAlerts;
  touchscreen.audioSettings.interface.action               = toggleAudioInterface;
  touchscreen.audioSettings.muteEverything.action          = toggleAudioMuteEverything;

  // Display settings screen touch actions
  touchscreen.displaySettings.back.action                  = displayGeigerCounter;
  touchscreen.displaySettings.display.action               = toggleDisplayPower;
  touchscreen.displaySettings.timeout.action               = toggleDisplayTimeout;

  // Sleep screen touch actions
  touchscreen.sleep.wakeup.action                          = wakeFromSleep;

  // Radiation history screen
  touchscreen.radiationHistory.back.action                 = displayGeigerCounter;

  // Rotation confirmation screen
  touchscreen.rotationConfirmation.back.action             = displayGeigerCounter;
  touchscreen.rotationConfirmation.confirm.action          = displayCosmicRayDetector;

  // Cosmic ray detector screen
  touchscreen.cosmicRayDetector.back.action                = displayGeigerCounter;
  touchscreen.cosmicRayDetector.mute.action                = cosmicRayDetectorMute;
  touchscreen.cosmicRayDetector.sleep.action               = cosmicRayDetectorSleep;

  // True random number generator screen
  touchscreen.trueRNG.back.action                          = displayGeigerCounter;

  // Hotspot settings screen
  touchscreen.hotspotSettings.back.action                  = displayGeigerCounter;
  touchscreen.hotspotSettings.enable.action                = toggleHotspot;

  // WiFi settings screen
  touchscreen.wifiSettings.back.action                     = displayGeigerCounter;
  touchscreen.wifiSettings.enable.action                   = toggleWiFi;

  // System settings screen
  touchscreen.systemSettings.back.action                   = displayGeigerCounter;
  touchscreen.systemSettings.serialLogging.action          = toggleSerialLogging;
  touchscreen.systemSettings.sdCardLogging.action          = toggleSDCardLogging;
  touchscreen.systemSettings.eventLogging.action           = toggleEventLogging;
  touchscreen.systemSettings.systemLogging.action          = toggleSystemLogging;

  // --------------------------------------------
  // Assign web server endpoints

  wireless.server.on("/data/geiger-counter",      sendGeigerCounterData    );
  wireless.server.on("/data/cosmic-ray-detector", sendCosmicRayDetectorData);
  wireless.server.on("/data/system",              sendSystemData           );

  // --------------------------------------------
  // Start

  // Enable the Geiger counter
  geigerCounter.enable();
  
  // Play th power on jingle
  buzzer.play(buzzer.jingle);

}

// ================================================================================================
// Main loop
// ================================================================================================
void loop() {

  // Update the buzzer
  audioFeedback();

  // Update the touchscreen
  visualFeedback();

  // Update the data output
  dataFeedback();

  // Update the wireless interfaces
  wireless.update();

  // Update the memory watchdog
  watchdog.update();

}

// ------------------------------------------------------------------------------------------------
// Loop functions

// ================================================================================================
// Audio feedback
// ================================================================================================
void audioFeedback() {

  uint64_t counts               = geigerCounter.getCounts();
  double   microsievertsPerHour = geigerCounter.getMicrosievertsPerHour();
  uint64_t coincidenceEvents    = cosmicRayDetector.getCoincidenceEvents();

  // If the dose reaches the alarm level
  if (microsievertsPerHour >= BUZZER_ALARM_LEVEL_USVH && !buzzer.alerts.muted()) {

    // Play the alarm sound
    buzzer.play(buzzer.alarm);

  // If dose is below alarm level
  } else {

    // If the dose reaches the warning level
    if (microsievertsPerHour >= BUZZER_WARNING_LEVEL_USVH) {

      // If warning has not been played
      if (!playedDoseWarning) {

        // Play the warning sound
        buzzer.play(buzzer.warning);

        // Set the played warning flag to true
        playedDoseWarning = true;

      }

    // If the dose is below the warning level 
    } else {

      // Reset the played warning flag
      playedDoseWarning = false;

    }

    // If no other sound is playing
    if (!buzzer.playing()) {

      // Calculate the coincidence events since the last update
      uint16_t newCoincidenceEvents = coincidenceEvents - lastCoincidenceEvents;

      // If there are any new coincidence events
      if (newCoincidenceEvents) {

        // Play the coincidence event sound for the number of coincidence events
        buzzer.play(buzzer.coincidenceEvent, newCoincidenceEvents);

      // If the there were no new coincidence events
      } else {

        // Calculate the counts since the last update
        uint16_t newCounts = counts - lastCounts;

        // If there are any new counts
        if (newCounts) {

          // Play the detection sound for the number of counts
          buzzer.play(buzzer.detection, newCounts);

        }

      }

    }

  }
  
  // Update the last known values
  lastCounts            = counts;
  lastCoincidenceEvents = coincidenceEvents;

  // Update the buzzer
  buzzer.update();

}

// ================================================================================================
// Visual feedback
// ================================================================================================
void visualFeedback() {

  // Set Geiger counter screen values
  touchscreen.geigerCounter.setEquivalentDose(geigerCounter.getEquivalentDose());
  touchscreen.geigerCounter.setEquivalentDoseUnit(geigerCounter.getEquivalentDoseUnit());
  touchscreen.geigerCounter.setRadiationRating(geigerCounter.getRadiationRating());
  touchscreen.geigerCounter.setCountsPerMinute(geigerCounter.getCountsPerMinute());
  touchscreen.geigerCounter.setIntegrationTime(geigerCounter.getIntegrationTime());

  // Set Geiger counter settings 1 screen values
  touchscreen.geigerCounterSettings1.setCounts(geigerCounter.getCounts());
  touchscreen.geigerCounterSettings1.setMainTubeCounts(geigerCounter.getMainTubeCounts());
  touchscreen.geigerCounterSettings1.setFollowerTubeCounts(geigerCounter.getFollowerTubeCounts());

  // Set Geiger counter settings 2 screen values
  touchscreen.geigerCounterSettings2.autoRange.toggle(geigerCounter.getAutoRangeState());

  // Set audio settings screen values
  touchscreen.audioSettings.detections.toggle(!buzzer.detections.muted());
  touchscreen.audioSettings.notifications.toggle(!buzzer.notifications.muted());
  touchscreen.audioSettings.alerts.toggle(!buzzer.alerts.muted());
  touchscreen.audioSettings.interface.toggle(!buzzer.interface.muted());
  touchscreen.audioSettings.muteEverything.toggle(buzzer.muted());

  // Set display settings screen values
  touchscreen.displaySettings.display.toggle(touchscreen.enabled());
  touchscreen.displaySettings.timeout.toggle(touchscreen.timeout());

  // Set cosmic ray detector screen values
  touchscreen.cosmicRayDetector.setCoincidenceEvents(cosmicRayDetector.getCoincidenceEvents());
  touchscreen.cosmicRayDetector.setCoincidenceEventsPerHour(cosmicRayDetector.getCoincidenceEventsPerHour());
  touchscreen.cosmicRayDetector.setCoincidenceEventsTotal(cosmicRayDetector.getCoincidenceEvents());
  touchscreen.cosmicRayDetector.setMainTubeCounts(geigerCounter.getMainTubeCounts());
  touchscreen.cosmicRayDetector.setFollowerTubeCounts(geigerCounter.getFollowerTubeCounts());

  // Set hotspot settings screen values
  touchscreen.hotspotSettings.enable.toggle(wireless.hotspotEnabled());
  touchscreen.hotspotSettings.setIPAddress(wireless.getHotspotIPAddress());

  // Set WiFi settings screen values
  touchscreen.wifiSettings.enable.toggle(wireless.wifiEnabled());
  touchscreen.wifiSettings.setWiFiName(wireless.getWiFiName());
  touchscreen.wifiSettings.setIPAddress(wireless.getWiFiIPAddress());

  // Set system settings screen values
  touchscreen.systemSettings.serialLogging.toggle(logger.serialLogging());
  touchscreen.systemSettings.sdCardLogging.toggle(logger.sdCardLogging());
  touchscreen.systemSettings.eventLogging.toggle(logger.eventLogging());
  touchscreen.systemSettings.systemLogging.toggle(systemLogging);

  // Store the current integration time
  uint8_t integrationTime = geigerCounter.getIntegrationTime();

  // Increase the integration time to the max
  geigerCounter.setIntegrationTime(60);

  // Set radiation history screen values
  touchscreen.radiationHistory.setRadiationHistory(geigerCounter.getCountsPerMinute());

  // Reset the integration time to the previous value
  geigerCounter.setIntegrationTime(integrationTime);

  // Update the touchscreen
  touchscreen.update();

}

// ================================================================================================
// Data feedback
// ================================================================================================
void dataFeedback() {

  // If the log interval has been reached
  if (millis() - logTimer > LOG_INTERVAL_SECONDS * 1000) {

    // Creat a log message string
    String logMessage;

    // --------------------------------------------
    // System data

    // If system data logging is enabled
    if (systemLogging) {

      // Clear the log message
      logMessage = "";

      // Get system data
      Logger::KeyValuePair systemData[11] = {

        {"upTime",            Logger::UINT64_T, {.uint64_t_value = millis()}                   },
        {"heap",              Logger::UINT32_T, {.uint32_t_value = ESP.getHeapSize()}          },
        {"freeHeap",          Logger::UINT32_T, {.uint32_t_value = ESP.getFreeHeap()}          },
        {"minHeap",           Logger::UINT32_T, {.uint32_t_value = ESP.getMinFreeHeap()}       },
        {"maxBlock",          Logger::UINT32_T, {.uint32_t_value = ESP.getMaxAllocHeap()}      },
        {"geigerCounter",     Logger::BOOL,     {.bool_value     = geigerCounter.enabled()}    },
        {"cosmicRayDetector", Logger::BOOL,     {.bool_value     = cosmicRayDetector.enabled()}},
        {"hotspot",           Logger::BOOL,     {.bool_value     = wireless.hotspotEnabled()}  },
        {"wifi",              Logger::BOOL,     {.bool_value     = wireless.wifiEnabled()}     },
        {"server",            Logger::BOOL,     {.bool_value     = wireless.serverRunning()}   },
        {"firmware",          Logger::STRING,   {.string_value   = FIRMWARE_VERSION}           }

      };

      // Append the system data
      logger.getLogMessage("system", systemData, 11, logMessage);

      // Log the log message
      logger.log(logMessage);

    }

    // --------------------------------------------
    // Geiger counter data

    // If the Geiger counter is enabled
    if (geigerCounter.enabled()) {

      // Clear the log message
      logMessage = "";

      // Get the current integration time
      uint8_t integrationTime = geigerCounter.getIntegrationTime();

      // Set integration time to the max value
      geigerCounter.setIntegrationTime(60);

      // Get Geiger counter data
      Logger::KeyValuePair geigerCounterData[7] = {

        {"counts",   Logger::UINT64_T, {.uint64_t_value = geigerCounter.getCounts()}                            },
        {"main",     Logger::UINT64_T, {.uint64_t_value = geigerCounter.getMainTubeCounts()}                    },
        {"follower", Logger::UINT64_T, {.uint64_t_value = geigerCounter.getFollowerTubeCounts()}                },
        {"cpm",      Logger::UINT64_T, {.uint64_t_value = (uint64_t)(round(geigerCounter.getCountsPerMinute()))}},
        {"usvh",     Logger::DOUBLE,   {.double_value   = geigerCounter.getMicrosievertsPerHour()}              },
        {"tubes",    Logger::UINT8_T,  {.uint8_t_value  = TOTAL_NUMBER_OF_TUBES}                                },
        {"type",     Logger::STRING,   {.string_value   = TUBE_TYPE_NAME}                                       }

      };

      // Reset integration time
      geigerCounter.setIntegrationTime(integrationTime);

      // Append the Geiger counter data
      logger.getLogMessage("geigerCounter", geigerCounterData, 7, logMessage);

      // Log the log message
      logger.log(logMessage);

    }

    // --------------------------------------------
    // Cosmic ray detector data

    // If the cosmic ray detector is enabled
    if (cosmicRayDetector.enabled()) {

      // Clear the log message
      logMessage = "";

      // Get the cosmic ray detector data
      Logger::KeyValuePair cosmicRayDetectorData[2] = {

        {"events", Logger::UINT64_T, {.uint64_t_value = cosmicRayDetector.getCoincidenceEvents()}       },
        {"cph",    Logger::UINT32_T, {.uint32_t_value = cosmicRayDetector.getCoincidenceEventsPerHour()}}

      };

      // Append the cosmic ray detector data
      logger.getLogMessage("cosmicRayDetector", cosmicRayDetectorData, 2, logMessage);

      // Log the log message
      logger.log(logMessage);

    }

    logTimer = millis();

  }

}

// ------------------------------------------------------------------------------------------------
// Touch actions

// ================================================================================================
// Display the Geiger counter screen
// ================================================================================================
void displayGeigerCounter() {

  // Disable the cosmic ray detector when returning to the Geiger counter screen
  cosmicRayDetector.disable();

  // Enable the geiger counter when returning to the Geiger counter screen
  geigerCounter.enable();

  // Rotate to landscape orientation
  touchscreen.rotateLandscape();

  // Draw the Geiger counter screen
  touchscreen.draw(touchscreen.geigerCounter);

  // Play the go back sound
  buzzer.play(buzzer.back);

}

// ================================================================================================
// Display the Geiger counter settings 1 screen
// ================================================================================================
void displayGeigerCounterSettings1() {

  // Rotate to landscape orientation
  touchscreen.rotateLandscape();

  // Draw the Geiger counter settings 1 screen
  touchscreen.draw(touchscreen.geigerCounterSettings1);

  // Play a sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// Display the Geiger counter settings 2 screen
// ================================================================================================
void displayGeigerCounterSettings2() {

  // Rotate to landscape orientation
  touchscreen.rotateLandscape();

  // Draw the Geiger counter settings 2 screen
  touchscreen.draw(touchscreen.geigerCounterSettings2);

  // Play a sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// Display the audio settings screen
// ================================================================================================
void displayAudioSettings() {

  // Rotate to landscape orientation
  touchscreen.rotateLandscape();

  // Draw the audio settings screen
  touchscreen.draw(touchscreen.audioSettings);

  // Play the next screen sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// Display the display settings screen
// ================================================================================================
void displayDisplaySettings() {

  // Rotate to landscape orientation
  touchscreen.rotateLandscape();

  // Draw the audio settings screen
  touchscreen.draw(touchscreen.displaySettings);

  // Play the next screen sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// Display the radiation history screen
// ================================================================================================
void displayRadiationHistory() {

  // Rotate to landscape orientation
  touchscreen.rotateLandscape();

  // Draw the screen
  touchscreen.draw(touchscreen.radiationHistory);

  // Play the next screen sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// Display the rotation confirmation screen
// ================================================================================================
void displayRotationConfirmation() {

  // Set the screen into portrait orientation
  touchscreen.rotatePortrait();

  // Draw the screen
  touchscreen.draw(touchscreen.rotationConfirmation);

  // Play the next screen sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// Display the cosmic ray detector screen
// ================================================================================================
void displayCosmicRayDetector() {

  // Set the screen into portrait orientation
  touchscreen.rotatePortrait();

  // Reset the screen values
  touchscreen.cosmicRayDetector.setCoincidenceEventsOffset(0);
  touchscreen.cosmicRayDetector.setMainTubeCountsOffset(0);
  touchscreen.cosmicRayDetector.setFollowerTubeCountsOffset(0);
  touchscreen.cosmicRayDetector.setCoincidenceEvents(0);
  touchscreen.cosmicRayDetector.setCoincidenceEventsPerHour(0.0);
  touchscreen.cosmicRayDetector.setMainTubeCounts(0);
  touchscreen.cosmicRayDetector.setFollowerTubeCounts(0);

  // Set offset counts
  touchscreen.cosmicRayDetector.setCoincidenceEventsOffset(cosmicRayDetector.getCoincidenceEvents());
  touchscreen.cosmicRayDetector.setMainTubeCountsOffset(geigerCounter.getMainTubeCounts());
  touchscreen.cosmicRayDetector.setFollowerTubeCountsOffset(geigerCounter.getFollowerTubeCounts());

  // Draw the screen
  touchscreen.draw(touchscreen.cosmicRayDetector);

  // Play the next screen sound
  buzzer.play(buzzer.next);

  // Enable the cosmic ray detector
  cosmicRayDetector.enable();

}

// ================================================================================================
// Display the true random number generator screen
// ================================================================================================
void displayTrueRNG() {

  // Rotate to landscape orientation
  touchscreen.rotateLandscape();

  // Draw the screen
  touchscreen.draw(touchscreen.trueRNG);

  // Play the next screen sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// Display the hotspot settings screen
// ================================================================================================
void displayHotspotSettings() {

  // Rotate to landscape orientation
  touchscreen.rotateLandscape();

  // Draw the screen
  touchscreen.draw(touchscreen.hotspotSettings);

  // Play the next screen sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// Display the WiFi settings screen
// ================================================================================================
void displayWiFiSettings() {

  // Rotate to landscape orientation
  touchscreen.rotateLandscape();

  // Draw the screen
  touchscreen.draw(touchscreen.wifiSettings);

  // Play the next screen sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// Display the system settings screen
// ================================================================================================
void displaySystemSettings() {

  // Rotate to landscape orientation
  touchscreen.rotateLandscape();

  // Draw the screen
  touchscreen.draw(touchscreen.systemSettings);

  // Play the next screen sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// Decrease the integration time
// ================================================================================================
void decreaseIntegrationTime() {

  // Get current integration time
  uint8_t integrationTimeSeconds = geigerCounter.getIntegrationTime();

  // If integration time is larger tha 5 seconds
  if (integrationTimeSeconds > 5) {

    // Decrease integration time by 5 seconds
    integrationTimeSeconds -= 5;

  // If integration time not not already 1 second
  } else if (integrationTimeSeconds > 1) {

    // Decrease integration time by 1 second
    integrationTimeSeconds -= 1;

  }

  // Set the new integration time
  geigerCounter.setIntegrationTime(integrationTimeSeconds);

  // Update Geiger counter screen with the new integration time value
  touchscreen.geigerCounter.setIntegrationTime(integrationTimeSeconds);

  // Play a sound
  buzzer.play(buzzer.back);

}

// ================================================================================================
// Reset the integration time
// ================================================================================================
void resetIntegrationTime() {

  // Reset integration time to the default value
  geigerCounter.setIntegrationTime(INTEGRATION_TIME_DEFAULT_SECONDS);

  // Update Geiger counter screen with the new integration time value
  touchscreen.geigerCounter.setIntegrationTime(INTEGRATION_TIME_DEFAULT_SECONDS);

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// Increase the integration time
// ================================================================================================
void increaseIntegrationTime() {

  // Get current integration time
  uint8_t integrationTimeSeconds = geigerCounter.getIntegrationTime();

  // If integration time is smaller than 60 seconds but larger or equal to 5 seconds
  if (integrationTimeSeconds < 60 && integrationTimeSeconds >= 5) {

    // Increase integration time by 5 seconds
    integrationTimeSeconds += 5;

  // If the integration time is smaller than 5 seconds
  } else if (integrationTimeSeconds < 5) {

    // Increase integration time by 1 second
    integrationTimeSeconds += 1;

  }

  // Set the new integration time
  geigerCounter.setIntegrationTime(integrationTimeSeconds);

  // Update Geiger counter screen with the new integration time value
  touchscreen.geigerCounter.setIntegrationTime(integrationTimeSeconds);

  // Play a sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// Toggle detections audio channel
// ================================================================================================
void toggleAudioDetections(const bool toggled) {

  // If toggled on
  if (toggled) {

    // Unmute channel
    buzzer.detections.unmute();

  // If toggled off
  } else {

    // Mute channel
    buzzer.detections.mute();

  }

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// Toggle notifications audio channel
// ================================================================================================
void toggleAudioNotifications(const bool toggled) {

  // If toggled on
  if (toggled) {

    // Unmute channel
    buzzer.notifications.unmute();

  // If toggled off
  } else {

    // Mute channel
    buzzer.notifications.mute();

  }

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// Toggle alerts audio channel
// ================================================================================================
void toggleAudioAlerts(const bool toggled) {

  // If toggled on
  if (toggled) {

    // Unmute channel
    buzzer.alerts.unmute();

  // If toggled off
  } else {

    // Mute channel
    buzzer.alerts.mute();

  }

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// Toggle interface audio channel
// ================================================================================================
void toggleAudioInterface(const bool toggled) {

  // If toggled on
  if (toggled) {

    // Unmute channel
    buzzer.interface.unmute();

  // If toggled off
  } else {

    // Mute channel
    buzzer.interface.mute();

  }

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// Toggle buzzer mute
// ================================================================================================
void toggleAudioMuteEverything(const bool toggled) {

  // If toggled on
  if (toggled) {

    // Mute buzzer
    buzzer.mute();

  // If toggled off
  } else {

    // Unmute buzzer
    buzzer.unmute();

  }

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// Toggle the display power state
// ================================================================================================
void toggleDisplayPower(const bool toggled) {

  // If toggled on
  if (toggled) {

    touchscreen.enable();

  // If toggled off
  } else {

    // Store the last mute state
    lastMute = buzzer.muted();

    // Turn off the display
    touchscreen.disable();

    // Draw the sleep screen
    touchscreen.draw(touchscreen.sleep);

  }

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// Toggle the display auto timeout
// ================================================================================================
void toggleDisplayTimeout(const bool toggled) {

  // If toggled on
  if (toggled) {

    touchscreen.enableTimeout();

  // If toggled off
  } else {

    // Turn off the display
    touchscreen.disableTimeout();

  }

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// Toggle Geiger counter equivalent dose auto range
// ================================================================================================
void toggleGeigerCounterAutoRange(const bool toggled) {

  // If toggled on
  if (toggled) {

    // Enable auto ranging
    geigerCounter.setAutoRangeState(true);

  // If toggled off
  } else {

    // Disable auto ranging
    geigerCounter.setAutoRangeState(false);

  }

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// Deselect all radiation unit radio buttons
// ================================================================================================
void deselectAllUnits() {

  touchscreen.geigerCounterSettings2.sievert.deselect();
  touchscreen.geigerCounterSettings2.rem.deselect();
  touchscreen.geigerCounterSettings2.rontgen.deselect();
  touchscreen.geigerCounterSettings2.gray.deselect();

}

// ================================================================================================
// Set Sievert as the measurement unit
// ================================================================================================
void selectSievertUnit() {

  // Set Sieverts as the measurement unit
  geigerCounter.setMeasurementUnit(GeigerCounter::MEASUREMENT_UNIT_SIEVERT);

  // Deselect all other units
  deselectAllUnits();

  // Select unit
  touchscreen.geigerCounterSettings2.sievert.select();

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// Set Rem as the measurement unit
// ================================================================================================
void selectRemUnit() {

  // Set Rem as the measurement unit
  geigerCounter.setMeasurementUnit(GeigerCounter::MEASUREMENT_UNIT_REM);

  // Deselect all other units
  deselectAllUnits();

  // Select unit
  touchscreen.geigerCounterSettings2.rem.select();

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// Set Rontgen as the measurement unit
// ================================================================================================
void selectRontgenUnit() {

  // Set Rontgen as the measurement unit
  geigerCounter.setMeasurementUnit(GeigerCounter::MEASUREMENT_UNIT_RONTGEN);

  // Deselect all other units
  deselectAllUnits();

  // Select unit
  touchscreen.geigerCounterSettings2.rontgen.select();

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// Set Gray as the measurement unit
// ================================================================================================
void selectGrayUnit() {

  // Set Gray as the measurement unit
  geigerCounter.setMeasurementUnit(GeigerCounter::MEASUREMENT_UNIT_GRAY);

  // Deselect all other units
  deselectAllUnits();

  // Select unit
  touchscreen.geigerCounterSettings2.gray.select();

    // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// Go to sleep
// ================================================================================================
void goToSleep() {

  // Store the last mute state
  lastMute = buzzer.muted();

  // Mute the buzzer
  buzzer.mute();

  // Turn off the touchscreen
  touchscreen.disable();

  // Draw the sleep screen
  touchscreen.draw(touchscreen.sleep);

}

// ================================================================================================
// Wake from sleep
// ================================================================================================
void wakeFromSleep() {

  // Draw the previous screen
  touchscreen.draw(touchscreen.getPreviousScreen());

  // Toggle on the display toggle
  touchscreen.displaySettings.display.toggleOn();

  // If buzzer was previously unmuted
  if (lastMute == false) {

    // Unmute the buzzer
    buzzer.unmute();

  }

  // Turn on the touchscreen
  touchscreen.enable();

}

// ================================================================================================
// Mute function one the cosmic ray detector screen 
// ================================================================================================
void cosmicRayDetectorMute() {

  if (buzzer.muted()) {

    // Unmute the buzzer
    buzzer.unmute();

  } else {

    // Mute the buzzer
    buzzer.mute();

  }

}

// ================================================================================================
// Turn off the touchscreen from the cosmic ray detector screen 
// ================================================================================================
void cosmicRayDetectorSleep() {

  // Use the display toggle to turn of the screen
  toggleDisplayPower(false);

}

// ================================================================================================
// Enable the wireless hotspot
// ================================================================================================
void toggleHotspot(const bool toggled) {

  // If toggled on
  if (toggled) {

    // Update screen elements
    touchscreen.hotspotSettings.setIPAddress(STRING_CONNECTING);

    // Refresh the display
    touchscreen.refresh();

    // Enable hotspot
    wireless.enableHotspot();

  // If toggled off
  } else {

    // Update screen elements
    touchscreen.hotspotSettings.setIPAddress(STRING_NON_APPLICABLE_ABBREVIATION);

    // Disable hotspot
    wireless.disableHotspot();

  }

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// Enable the WiFi
// ================================================================================================
void toggleWiFi(const bool toggled) {

  // If toggled on
  if (toggled) {

    // Update screen elements
    touchscreen.wifiSettings.setIPAddress(STRING_CONNECTING);

    // Refresh the display
    touchscreen.refresh();

    // Enable WiFi
    wireless.enableWiFi();

  // If toggled off
  } else {

    // Update screen elements
    touchscreen.hotspotSettings.setIPAddress(STRING_NON_APPLICABLE_ABBREVIATION);

    // Disable WiFi
    wireless.disableWiFi();

  }

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// Toggle serial logging
// ================================================================================================
void toggleSerialLogging(const bool toggled) {

  // If toggled on
  if (toggled) {

    // Enable serial logging
    logger.enableSerialLogging();

  // If toggled off
  } else {

    // Disable serial logging
    logger.disableSerialLogging();

  }

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// Toggle SD card logging
// ================================================================================================
void toggleSDCardLogging(const bool toggled) {

  // If toggled on
  if (toggled) {

    // Enable SD card logging
    logger.enableSDCardLogging();

  // If toggled off
  } else {

    // Disable SD card logging
    logger.disableSDCardLogging();

  }

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// Toggle event message logging
// ================================================================================================
void toggleEventLogging(const bool toggled) {

  // If toggled on
  if (toggled) {

    // Enable SD card logging
    logger.enableEventLogging();

  // If toggled off
  } else {

    // Disable SD card logging
    logger.disableEventLogging();

  }

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// Toggle system message logging
// ================================================================================================
void toggleSystemLogging(const bool toggled) {

  // Set the system logging state
  systemLogging = toggled;

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ------------------------------------------------------------------------------------------------
// Web server actions

// ================================================================================================
// Send Geiger counter data via the web server
// ================================================================================================
void sendGeigerCounterData() {

  // Get the current integration time
  uint8_t integrationTime = geigerCounter.getIntegrationTime();

  // Set integration time to the max value
  geigerCounter.setIntegrationTime(60);

  // Get Geiger counter data
  Logger::KeyValuePair data[9] = {

    {"enabled",     Logger::BOOL,     {.bool_value     = geigerCounter.enabled()}                 },
    {"counts",      Logger::UINT64_T, {.uint64_t_value = geigerCounter.getCounts()}               },
    {"main",        Logger::UINT64_T, {.uint64_t_value = geigerCounter.getMainTubeCounts()}       },
    {"follower",    Logger::UINT64_T, {.uint64_t_value = geigerCounter.getFollowerTubeCounts()}   },
    {"cpm",         Logger::DOUBLE,   {.double_value   = geigerCounter.getCountsPerMinute()}      },
    {"usvh",        Logger::DOUBLE,   {.double_value   = geigerCounter.getMicrosievertsPerHour()} },
    {"rating",      Logger::UINT8_T,  {.uint8_t_value  = geigerCounter.getRadiationRating()}      },
    {"tubes",       Logger::UINT8_T,  {.uint8_t_value  = TOTAL_NUMBER_OF_TUBES}                   },
    {"type",        Logger::STRING,   {.string_value   = TUBE_TYPE_NAME}                          }

  };

  // Reset integration time
  geigerCounter.setIntegrationTime(integrationTime);

  // JSON data string
  String json;

  // Construct the data string
  logger.getLogMessage("geigerCounter", data, 9, json);

  // Send JSON data
  wireless.server.send(200, "application/json", json);

}

// ================================================================================================
// Send cosmic ray detector data via the web server
// ================================================================================================
void sendCosmicRayDetectorData() {

  // Get the cosmic ray detector data
  Logger::KeyValuePair data[5] = {

    {"enabled",  Logger::BOOL,     {.bool_value     = cosmicRayDetector.enabled()}                    },
    {"events",   Logger::UINT64_T, {.uint64_t_value = cosmicRayDetector.getCoincidenceEvents()}       },
    {"cph",      Logger::UINT32_T, {.uint32_t_value = cosmicRayDetector.getCoincidenceEventsPerHour()}},
    {"main",     Logger::UINT64_T, {.uint64_t_value = geigerCounter.getMainTubeCounts()}              },
    {"follower", Logger::UINT64_T, {.uint64_t_value = geigerCounter.getFollowerTubeCounts()}          }

  };

  // JSON data string
  String json;

  // Construct the data string
  logger.getLogMessage("cosmicRayDetector", data, 5, json);

  // Send JSON data
  wireless.server.send(200, "application/json", json);

}

// ================================================================================================
// Send system data via the web server
// ================================================================================================
void sendSystemData() {

  // Get system data
  Logger::KeyValuePair data[11] = {

    {"upTime",            Logger::UINT64_T, {.uint64_t_value = millis()}                   },
    {"heap",              Logger::UINT32_T, {.uint32_t_value = ESP.getHeapSize()}          },
    {"freeHeap",          Logger::UINT32_T, {.uint32_t_value = ESP.getFreeHeap()}          },
    {"minHeap",           Logger::UINT32_T, {.uint32_t_value = ESP.getMinFreeHeap()}       },
    {"maxBlock",          Logger::UINT32_T, {.uint32_t_value = ESP.getMaxAllocHeap()}      },
    {"geigerCounter",     Logger::BOOL,     {.bool_value     = geigerCounter.enabled()}    },
    {"cosmicRayDetector", Logger::BOOL,     {.bool_value     = cosmicRayDetector.enabled()}},
    {"hotspot",           Logger::BOOL,     {.bool_value     = wireless.hotspotEnabled()}  },
    {"wifi",              Logger::BOOL,     {.bool_value     = wireless.wifiEnabled()}     },
    {"server",            Logger::BOOL,     {.bool_value     = wireless.serverRunning()}   },
    {"firmware",          Logger::STRING,   {.string_value   = FIRMWARE_VERSION}           }

  };

  // JSON data string
  String json;

  // Construct the data string
  logger.getLogMessage("system", data, 11, json);

  // Send JSON data
  wireless.server.send(200, "application/json", json);

}