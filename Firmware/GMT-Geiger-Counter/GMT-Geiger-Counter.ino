#include "Arduino.h"
#include "Configuration.h"
#include "Strings.h"
#include "SD.h"
#include "GeigerCounter.h"
#include "CosmicRayDetector.h"
#include "Buzzer.h"
#include "Touchscreen.h"
#include "Wireless.h"
#include "Watchdog.h"
#include "Logger.h"

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
bool     playedDoseWarning           = false;
uint64_t lastCounts                  = 0;
uint64_t lastCoincidenceEvents       = 0;
bool     lastMute                    = false;
uint64_t logTimer                    = 0;
uint64_t loops                       = 1;
uint64_t totalLoopTimeMicroseconds   = 0;
uint64_t averageLoopTimeMicroseconds = 0;
uint64_t maximumLoopTimeMicroseconds = 0;

// Function prototypes
void setup();
void loop();
void audioFeedback();
void visualFeedback();
void dataFeedback();
void displayGeigerCounter();
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
void toggleAudioDetections(bool toggled);
void toggleAudioNotifications(bool toggled);
void toggleAudioAlerts(bool toggled);
void toggleAudioInterface(bool toggled);
void toggleAudioMuteEverything(bool toggled);
void toggleDisplayPower(bool toggled);
void toggleDisplayTimeout(bool toggled);
void goToSleep();
void wakeFromSleep();
void cosmicRayDetectorMute();
void toggleEnableHotspot(bool toggled);
void toggleEnableWiFi(bool toggled);
void toggleSerialLogging(bool toggled);
void toggleSDCardLogging(bool toggled);
void toggleSystemLogging(bool toggled);
void sendGeigerCounterData();
void sendCosmicRayDetectorData();
void sendSystemData();

// ================================================================================================
// Setup
// ================================================================================================
void setup() {

  // --------------------------------------------
  // Initialize everything

  Serial.begin(SERIAL_BAUD_RATE);
  SD.begin(SD_CS_PIN);
  geigerCounter.begin();
  cosmicRayDetector.begin();
  buzzer.begin();
  touchscreen.begin();
  wireless.begin();

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
  touchscreen.cosmicRayDetector.sleep.action               = goToSleep;

  // True random number generator screen
  touchscreen.trueRNG.back.action                          = displayGeigerCounter;

  // Hotspot settings screen
  touchscreen.hotspotSettings.back.action                  = displayGeigerCounter;
  touchscreen.hotspotSettings.enable.action                = toggleEnableHotspot;

  // WiFi settings screen
  touchscreen.wifiSettings.back.action                     = displayGeigerCounter;
  touchscreen.wifiSettings.enable.action                   = toggleEnableWiFi;

  // System settings screen
  touchscreen.systemSettings.back.action                   = displayGeigerCounter;
  touchscreen.systemSettings.serialLogging.action          = toggleSerialLogging;
  touchscreen.systemSettings.sdCardLogging.action          = toggleSDCardLogging;
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

  // Variable for keeping track of the total loop time
  uint64_t loopStartMicroseconds = micros();

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

  // Calculate current loop time
  uint64_t currentLoopTimeMicroseconds = micros() - loopStartMicroseconds;

  // Update the maximum loop time if the current loop time is longer than the old max
  if (maximumLoopTimeMicroseconds < currentLoopTimeMicroseconds) { maximumLoopTimeMicroseconds = currentLoopTimeMicroseconds; }

  // Add current loop time to the total loop time
  totalLoopTimeMicroseconds += currentLoopTimeMicroseconds;

  // Calculate the average loop time
  averageLoopTimeMicroseconds = totalLoopTimeMicroseconds / loops;

  // Increase loop counter
  loops++;

  // After 1000 loops
  if (loops > 1000) {

    // Reset total loop time
    totalLoopTimeMicroseconds = 0;

    // Reset maximum loop time
    maximumLoopTimeMicroseconds = 0;

    // Reset loop counter
    loops = 1;

  }

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
  if (microsievertsPerHour >= BUZZER_ALARM_LEVEL_USVH) {

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
  touchscreen.geigerCounter.setEquivalentDose(geigerCounter.getMicrosievertsPerHour());
  touchscreen.geigerCounter.setEquivalentDoseUnit(STRING_EQUIVALENT_DOSE_UNIT_USVH);
  touchscreen.geigerCounter.setRadiationRating(geigerCounter.getRadiationRating());
  touchscreen.geigerCounter.setCountsPerMinute(geigerCounter.getCountsPerMinute());
  touchscreen.geigerCounter.setIntegrationTime(geigerCounter.getIntegrationTime());

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
  touchscreen.systemSettings.systemLogging.toggle(logger.systemLogging());

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

  if (millis() - logTimer > LOG_INTERVAL_SECONDS * 1000) {

    // Creat a log message string
    String logMessage;

    // --------------------------------------------
    // System data

    // If system message logging is enabled
    if (logger.systemLogging()) {

      // Get system data
      Logger::KeyValuePair systemData[10] = {

        {"upTimeMilliseconds",          Logger::UINT64_T, {.uint64_t_value = millis()}                   },
        {"averageLoopTimeMicroseconds", Logger::UINT64_T, {.uint64_t_value = averageLoopTimeMicroseconds}},
        {"maximumLoopTimeMicroseconds", Logger::UINT64_T, {.uint64_t_value = maximumLoopTimeMicroseconds}},
        {"totalHeapBytes",              Logger::UINT32_T, {.uint32_t_value = ESP.getHeapSize()}          },
        {"freeHeapBytes",               Logger::UINT32_T, {.uint32_t_value = ESP.getFreeHeap()}          },
        {"minimumHeapBytes",            Logger::UINT32_T, {.uint32_t_value = ESP.getMinFreeHeap()}       },
        {"maximumHeapBlockBytes",       Logger::UINT32_T, {.uint32_t_value = ESP.getMaxAllocHeap()}      },
        {"geigerCounterEnabled",        Logger::BOOL,     {.bool_value     = geigerCounter.enabled()}    },
        {"cosmicRayDetectorEnabled",    Logger::BOOL,     {.bool_value     = cosmicRayDetector.enabled()}},
        {"firmware",                    Logger::STRING,   {.string_value   = FIRMWARE_VERSION}           }

      };

      // Append the system data
      logger.appendLogMessage("system", systemData, 8, logMessage);

    }

    // --------------------------------------------
    // Geiger counter data

    // If the Geiger counter is enabled
    if (geigerCounter.enabled()) {

      // Get the current integration time
      uint8_t integrationTime = geigerCounter.getIntegrationTime();

      // Set integration time to the max value
      geigerCounter.setIntegrationTime(60);

      // Get Geiger counter data
      Logger::KeyValuePair geigerCounterData[6] = {

        {"totalCounts",          Logger::UINT64_T, {.uint64_t_value = geigerCounter.getCounts()}              },
        {"mainTubeCounts",       Logger::UINT64_T, {.uint64_t_value = geigerCounter.getMainTubeCounts()}      },
        {"mainFollowerCounts",   Logger::UINT64_T, {.uint64_t_value = geigerCounter.getFollowerTubeCounts()}  },
        {"countsPerMinute",      Logger::DOUBLE,   {.double_value   = geigerCounter.getCountsPerMinute()}     },
        {"microsievertsPerHour", Logger::DOUBLE,   {.double_value   = geigerCounter.getMicrosievertsPerHour()}},
        {"tubeType",             Logger::STRING,   {.string_value   = TUBE_TYPE_NAME}                         }

      };

      // Reset integration time
      geigerCounter.setIntegrationTime(integrationTime);

      // Append the Geiger counter data
      logger.appendLogMessage("geigerCounter", geigerCounterData, 6, logMessage);

    }

    // --------------------------------------------
    // Cosmic ray detector data

    // If the cosmic ray detector is enabled
    if (cosmicRayDetector.enabled()) {

      // Get the cosmic ray detector data
      Logger::KeyValuePair cosmicRayDetectorData[2] = {

        {"coincidenceEvents",        Logger::UINT64_T, {.uint64_t_value = cosmicRayDetector.getCoincidenceEvents()}       },
        {"coincidenceEventsPerHour", Logger::DOUBLE,   {.double_value   = cosmicRayDetector.getCoincidenceEventsPerHour()}}

      };

      // Append the cosmic ray detector data
      logger.appendLogMessage("cosmicRayDetector", cosmicRayDetectorData, 2, logMessage);

    }
    
    // Log the log message
    logger.log(logMessage);

    // Update log timer
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

  // Rotate to landscape orientation
  touchscreen.rotateLandscape();

  // Draw the Geiger counter screen
  touchscreen.draw(touchscreen.geigerCounter);

  // Play the go back sound
  buzzer.play(buzzer.back);

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
void toggleAudioDetections(bool toggled) {

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
void toggleAudioNotifications(bool toggled) {

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
void toggleAudioAlerts(bool toggled) {

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
void toggleAudioInterface(bool toggled) {

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
void toggleAudioMuteEverything(bool toggled) {

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
void toggleDisplayPower(bool toggled) {

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
void toggleDisplayTimeout(bool toggled) {

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

  // Play a sound
  buzzer.play(buzzer.tap);

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
// Enable the wireless hotspot
// ================================================================================================
void toggleEnableHotspot(bool toggled) {

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
void toggleEnableWiFi(bool toggled) {

  // If toggled on
  if (toggled) {

    // Update screen elements
    touchscreen.wifiSettings.setIPAddress(STRING_CONNECTING);

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
void toggleSerialLogging(bool toggled) {

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
void toggleSDCardLogging(bool toggled) {

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
// Toggle system message logging
// ================================================================================================
void toggleSystemLogging(bool toggled) {

  // If toggled on
  if (toggled) {

    // Enable system message logging
    logger.enableSystemLogging();

  // If toggled off
  } else {

    // Disable system message logging
    logger.disableSystemLogging();

  }

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ------------------------------------------------------------------------------------------------
// Web server actions

// ================================================================================================
// Send Geiger counter data via the web server
// ================================================================================================
void sendGeigerCounterData() {

  // Get Geiger counter data
  Logger::KeyValuePair data[8] = {

    {"enabled",                Logger::BOOL,     {.bool_value     = geigerCounter.enabled()}                },
    {"totalCounts",            Logger::UINT64_T, {.uint64_t_value = geigerCounter.getCounts()}              },
    {"mainTubeCounts",         Logger::UINT64_T, {.uint64_t_value = geigerCounter.getMainTubeCounts()}      },
    {"mainFollowerCounts",     Logger::UINT64_T, {.uint64_t_value = geigerCounter.getFollowerTubeCounts()}  },
    {"countsPerMinute",        Logger::DOUBLE,   {.double_value   = geigerCounter.getCountsPerMinute()}     },
    {"microsievertsPerHour",   Logger::DOUBLE,   {.double_value   = geigerCounter.getMicrosievertsPerHour()}},
    {"integrationTimeSeconds", Logger::UINT8_T,  {.uint8_t_value  = geigerCounter.getIntegrationTime()}     },
    {"tubeType",               Logger::STRING,   {.string_value   = TUBE_TYPE_NAME}                         }

  };

  // JSON data string
  String json;

  // Construct the data string
  logger.appendLogMessage("geigerCounter", data, 8, json);

  // Send JSON data
  wireless.server.send(200, "application/json", json);

}

// ================================================================================================
// Send cosmic ray detector data via the web server
// ================================================================================================
void sendCosmicRayDetectorData() {

  // Get the cosmic ray detector data
  Logger::KeyValuePair data[3] = {

    {"enabled",                  Logger::BOOL,     {.bool_value     = cosmicRayDetector.enabled()}                    },
    {"coincidenceEvents",        Logger::UINT64_T, {.uint64_t_value = cosmicRayDetector.getCoincidenceEvents()}       },
    {"coincidenceEventsPerHour", Logger::DOUBLE,   {.double_value   = cosmicRayDetector.getCoincidenceEventsPerHour()}}

  };

  // JSON data string
  String json;

  // Construct the data string
  logger.appendLogMessage("cosmicRayDetector", data, 3, json);

  // Send JSON data
  wireless.server.send(200, "application/json", json);

}

// ================================================================================================
// Send system data via the web server
// ================================================================================================
void sendSystemData() {

  // Get system data
  Logger::KeyValuePair data[10] = {

    {"upTimeMilliseconds",          Logger::UINT64_T, {.uint64_t_value = millis()}                   },
    {"averageLoopTimeMicroseconds", Logger::UINT64_T, {.uint64_t_value = averageLoopTimeMicroseconds}},
    {"maximumLoopTimeMicroseconds", Logger::UINT64_T, {.uint64_t_value = maximumLoopTimeMicroseconds}},
    {"totalHeapBytes",              Logger::UINT32_T, {.uint32_t_value = ESP.getHeapSize()}          },
    {"freeHeapBytes",               Logger::UINT32_T, {.uint32_t_value = ESP.getFreeHeap()}          },
    {"minimumHeapBytes",            Logger::UINT32_T, {.uint32_t_value = ESP.getMinFreeHeap()}       },
    {"maximumHeapBlockBytes",       Logger::UINT32_T, {.uint32_t_value = ESP.getMaxAllocHeap()}      },
    {"geigerCounterEnabled",        Logger::BOOL,     {.bool_value     = geigerCounter.enabled()}    },
    {"cosmicRayDetectorEnabled",    Logger::BOOL,     {.bool_value     = cosmicRayDetector.enabled()}},
    {"firmware",                    Logger::STRING,   {.string_value   = FIRMWARE_VERSION}           }

  };

  // JSON data string
  String json;

  // Construct the data string
  logger.appendLogMessage("system", data, 10, json);

  // Send JSON data
  wireless.server.send(200, "application/json", json);

}