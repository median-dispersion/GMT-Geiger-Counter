#include "Arduino.h"
#include "Configuration.h"
#include "Logger.h"
#include "SDCard.h"
#include "Settings.h"
#include "GeigerCounter.h"
#include "CosmicRayDetector.h"
#include "Buzzer.h"
#include "Touchscreen.h"
#include "RGBLED.h"
#include "Wireless.h"
#include "Watchdog.h"

// ------------------------------------------------------------------------------------------------
// Global

// Global variables
bool     PLAYED_DOSE_WARNING           = false;
uint64_t LAST_COUNTS_VALUE             = 0;
uint64_t LAST_COINCIDENCE_EVENTS_VALUE = 0;
bool     LAST_BUZZER_MUTER_STATE       = false;
bool     LAST_RGB_LED_STATE            = true;
uint64_t LOG_TIMER                     = 0;

// Function prototypes
void setup();
void loop();
void setTouchActions();
void setUserSettings();
void audioFeedback();
void visualFeedback();
void dataFeedback();
void goToSleep();
void wakeFromSleep();
void geigerCounterDecreaseIntegrationTime();
void geigerCounterResetIntegrationTime();
void geigerCounterIncreaseIntegrationTime();
void geigerCounterDeselectAllUnits();
void cosmicRayDetectorMute();
void cosmicRayDetectorDisable();
void displayGeigerCounter();
void displayGeigerCounterSettings1();
void displayGeigerCounterSettings2();
void displayAudioSettings();
void displayDisplaySettings();
void displayRotationConfirmation();
void displayCosmicRayDetector();
void displayDisableCosmicRayDetector();
void displayRadiationHistory();
void displayTrueRNG();
void displayHotspotSettings();
void displayWiFiSettings();
void displaySystemSettings1();
void displaySystemSettings2();
void selectGeigerCounterSieverts();
void selectGeigerCounterRem();
void selectGeigerCounterRontgen();
void selectGeigerCounterGray();
void toggleGeigerCounterAutoIntegration(const bool toggled);
void toggleGeigerCounterAutoRanging(const bool toggled);
void toggleAudioDetections(const bool toggled);
void toggleAudioNotifications(const bool toggled);
void toggleAudioAlerts(const bool toggled);
void toggleAudioInterface(const bool toggled);
void toggleAudioMuteEverything(const bool toggled);
void toggleDisplayState(const bool toggled);
void toggleDisplayAutoTimeout(const bool toggled);
void toggleDisplayLEDState(const bool toggled);
void toggleHotspotState(const bool toggled);
void toggleWiFiSate(const bool toggled);
void toggleSystemSDCardMount(const bool toggled);
void toggleSystemSerialLogging(const bool toggled);
void toggleSystemSDCardLogging(const bool toggled);
void toggleSystemDataLogging(const bool toggled);
void toggleSystemEventLogging(const bool toggled);
void toggleSystemInfoLogging(const bool toggled);

// ================================================================================================
// Setup
// ================================================================================================
void setup() {

  // Initialize everything
  logger.begin();
  sdCard.begin();
  settings.begin();
  geigerCounter.begin();
  cosmicRayDetector.begin();
  buzzer.begin();
  touchscreen.begin();
  rgbLED.begin();
  wireless.begin();
  watchdog.begin();

  // Set touch actions
  setTouchActions();

  // Load and set the user settings
  setUserSettings();

  // Assign web server endpoints
  wireless.server.on("/data/geiger-counter",      sendGeigerCounterData    );
  wireless.server.on("/data/cosmic-ray-detector", sendCosmicRayDetectorData);
  wireless.server.on("/data/log",                 sendLogFileData          );
  wireless.server.on("/data/system",              sendSystemInfoData       );

  // Enable geiger counter
  geigerCounter.enable();

  // Enable touchscreen
  touchscreen.enable();

  // Play jingle
  buzzer.play(buzzer.jingle);

}

// ================================================================================================
// Main loop
// ================================================================================================
void loop() {

  // Audio feedback
  audioFeedback();

  // Visual feedback
  visualFeedback();

  // Data feedback
  dataFeedback();

  // Update user settings
  settings.update();

  // Update wireless interface
  wireless.update();

  // Update the watchdog
  watchdog.update();

}

//-------------------------------------------------------------------------------------------------
// Setup functions

// ================================================================================================
// Set touch actions
// ================================================================================================
void setTouchActions() {

  // --------------------------------------------
  // Geiger counter touch actions

  touchscreen.geigerCounter.geigerCounterSetting.action    = displayGeigerCounterSettings1;
  touchscreen.geigerCounter.audioSettings.action           = displayAudioSettings;
  touchscreen.geigerCounter.displaySettings.action         = displayDisplaySettings;
  touchscreen.geigerCounter.goToSleep.action               = goToSleep;
  touchscreen.geigerCounter.decreaseIntegrationTime.action = geigerCounterDecreaseIntegrationTime;
  touchscreen.geigerCounter.resetIntegrationTime.action    = geigerCounterResetIntegrationTime;
  touchscreen.geigerCounter.increaseIntegrationTime.action = geigerCounterIncreaseIntegrationTime;
  touchscreen.geigerCounter.cosmicRayDetector.action       = displayRotationConfirmation;
  touchscreen.geigerCounter.radiationHistory.action        = displayRadiationHistory;
  touchscreen.geigerCounter.trueRNG.action                 = displayTrueRNG;
  touchscreen.geigerCounter.hotspotSettings.action         = displayHotspotSettings;
  touchscreen.geigerCounter.wifiSettings.action            = displayWiFiSettings;
  touchscreen.geigerCounter.systemSettings.action          = displaySystemSettings1;

  // --------------------------------------------
  // Geiger counter settings 1 touch actions

  touchscreen.geigerCounterSettings1.back.action     = displayGeigerCounter;
  touchscreen.geigerCounterSettings1.next.action     = displayGeigerCounterSettings2;
  touchscreen.geigerCounterSettings1.previous.action = displayGeigerCounterSettings2;

  // --------------------------------------------
  // Geiger counter settings 2 touch actions

  touchscreen.geigerCounterSettings2.back.action          = displayGeigerCounter;
  touchscreen.geigerCounterSettings2.next.action          = displayGeigerCounterSettings1;
  touchscreen.geigerCounterSettings2.previous.action      = displayGeigerCounterSettings1;
  touchscreen.geigerCounterSettings2.autoIntegrate.action = toggleGeigerCounterAutoIntegration;
  touchscreen.geigerCounterSettings2.autoRange.action     = toggleGeigerCounterAutoRanging;
  touchscreen.geigerCounterSettings2.sieverts.action      = selectGeigerCounterSieverts;
  touchscreen.geigerCounterSettings2.rem.action           = selectGeigerCounterRem;
  touchscreen.geigerCounterSettings2.rontgen.action       = selectGeigerCounterRontgen;
  touchscreen.geigerCounterSettings2.gray.action          = selectGeigerCounterGray;

  // --------------------------------------------
  // Audio settings touch actions

  touchscreen.audioSettings.back.action           = displayGeigerCounter;
  touchscreen.audioSettings.detections.action     = toggleAudioDetections;
  touchscreen.audioSettings.notifications.action  = toggleAudioNotifications;
  touchscreen.audioSettings.alerts.action         = toggleAudioAlerts;
  touchscreen.audioSettings.interface.action      = toggleAudioInterface;
  touchscreen.audioSettings.muteEverything.action = toggleAudioMuteEverything;

  // --------------------------------------------
  // Display settings touch actions

  touchscreen.displaySettings.back.action    = displayGeigerCounter;
  touchscreen.displaySettings.display.action = toggleDisplayState;
  touchscreen.displaySettings.timeout.action = toggleDisplayAutoTimeout;
  touchscreen.displaySettings.rgbLED.action  = toggleDisplayLEDState;

  // --------------------------------------------
  // Rotation confirmation touch actions

  touchscreen.rotationConfirmation.back.action    = displayGeigerCounter;
  touchscreen.rotationConfirmation.confirm.action = displayCosmicRayDetector;

  // --------------------------------------------
  // Cosmic ray detector touch actions

  touchscreen.cosmicRayDetector.back.action  = displayDisableCosmicRayDetector;
  touchscreen.cosmicRayDetector.mute.action  = cosmicRayDetectorMute;
  touchscreen.cosmicRayDetector.sleep.action = goToSleep;

  // --------------------------------------------
  // Disable cosmic ray detector touch actions

  touchscreen.disableCosmicRayDetector.back.action    = displayCosmicRayDetector;
  touchscreen.disableCosmicRayDetector.confirm.action = cosmicRayDetectorDisable;
  touchscreen.disableCosmicRayDetector.dismiss.action = displayGeigerCounter;

  // --------------------------------------------
  // Radiation history touch actions

  touchscreen.radiationHistory.back.action = displayGeigerCounter;

  // --------------------------------------------
  // True RNG touch actions

  touchscreen.trueRNG.back.action = displayGeigerCounter;

  // --------------------------------------------
  // Hotspot settings touch actions

  touchscreen.hotspotSettings.back.action   = displayGeigerCounter;
  touchscreen.hotspotSettings.enable.action = toggleHotspotState;

  // --------------------------------------------
  // WiFi settings touch actions

  touchscreen.wifiSettings.back.action   = displayGeigerCounter;
  touchscreen.wifiSettings.enable.action = toggleWiFiSate;

  // --------------------------------------------
  // System settings 1 touch actions

  touchscreen.systemSettings1.back.action          = displayGeigerCounter;
  touchscreen.systemSettings1.next.action          = displaySystemSettings2;
  touchscreen.systemSettings1.previous.action      = displaySystemSettings2;
  touchscreen.systemSettings1.sdCardMounted.action = toggleSystemSDCardMount;
  touchscreen.systemSettings1.serialLogging.action = toggleSystemSerialLogging;
  touchscreen.systemSettings1.sdCardLogging.action = toggleSystemSDCardLogging;
  touchscreen.systemSettings1.dataLogging.action   = toggleSystemDataLogging;
  touchscreen.systemSettings1.eventLogging.action  = toggleSystemEventLogging;
  touchscreen.systemSettings1.systemLogging.action = toggleSystemInfoLogging;

  // --------------------------------------------
  // System settings 2 touch actions

  touchscreen.systemSettings2.back.action          = displayGeigerCounter;
  touchscreen.systemSettings2.next.action          = displaySystemSettings1;
  touchscreen.systemSettings2.previous.action      = displaySystemSettings1;

  // --------------------------------------------
  // Hotspot settings touch actions

  touchscreen.hotspotSettings.back.action   = displayGeigerCounter;
  touchscreen.hotspotSettings.enable.action = toggleHotspotState;

  // --------------------------------------------
  // WiFi settings touch actions

  touchscreen.wifiSettings.back.action   = displayGeigerCounter;
  touchscreen.wifiSettings.enable.action = toggleWiFiSate;
  
  // --------------------------------------------
  // Sleep screen actions

  touchscreen.sleepScreen.wakeup.action = wakeFromSleep;

}

// ================================================================================================
// Load and set the user settings
// ================================================================================================
void setUserSettings() {

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

  buzzer.detections.setMuteState(settings.data.parameters.buzzer.detections);
  buzzer.notifications.setMuteState(settings.data.parameters.buzzer.notifications);
  buzzer.alerts.setMuteState(settings.data.parameters.buzzer.alerts);
  buzzer.interface.setMuteState(settings.data.parameters.buzzer.interface);
  buzzer.setMuteState(settings.data.parameters.buzzer.muteEverything);

  // --------------------------------------------
  // Display settings

  touchscreen.setTimeoutState(settings.data.parameters.display.timeout);
  rgbLED.setLEDState(settings.data.parameters.display.rgbLED);

  // --------------------------------------------
  // Wireless settings

  wireless.setHotspotState(settings.data.parameters.wireless.hotspot);
  // wireless.setWiFiState(settings.data.parameters.wireless.wifi);

}

//-------------------------------------------------------------------------------------------------
// Loop functions

// ================================================================================================
// Visual feedback
// ================================================================================================
void visualFeedback() {

  // --------------------------------------------
  // Geiger counter screen

  touchscreen.geigerCounter.setEquivalentDose(geigerCounter.getEquivalentDose());
  touchscreen.geigerCounter.setEquivalentDoseUnit(geigerCounter.getEquivalentDoseUnit());
  touchscreen.geigerCounter.setRadiationRating(geigerCounter.getRadiationRating());
  touchscreen.geigerCounter.setCountsPerMinute(geigerCounter.getCountsPerMinute());
  touchscreen.geigerCounter.setIntegrationTime(geigerCounter.getIntegrationTime());

  // --------------------------------------------
  // Geiger counter settings 1 screen

  touchscreen.geigerCounterSettings1.setCounts(geigerCounter.getCounts());
  touchscreen.geigerCounterSettings1.setMainTubeCounts(geigerCounter.getMainTubeCounts());
  touchscreen.geigerCounterSettings1.setFollowerTubeCounts(geigerCounter.getFollowerTubeCounts());

  // --------------------------------------------
  // Geiger counter settings 2 screen

  touchscreen.geigerCounterSettings2.autoIntegrate.setToggleState(geigerCounter.getAutoIntegrateState());
  touchscreen.geigerCounterSettings2.autoRange.setToggleState(geigerCounter.getAutoRangeState());
  
  // Deselect all units radio buttons
  geigerCounterDeselectAllUnits();

  // Select the measurement unit radio button depending on the selected measurement unit
  switch (geigerCounter.getMeasurementUnit()) {

    case GeigerCounter::SIEVERTS: touchscreen.geigerCounterSettings2.sieverts.select(); break;
    case GeigerCounter::REM:      touchscreen.geigerCounterSettings2.rem.select();      break;
    case GeigerCounter::RONTGEN:  touchscreen.geigerCounterSettings2.rontgen.select();  break;
    case GeigerCounter::GRAY:     touchscreen.geigerCounterSettings2.gray.select();     break;

  }

  // --------------------------------------------
  // Audio settings screen

  touchscreen.audioSettings.detections.setToggleState(!buzzer.detections.getMuteState());
  touchscreen.audioSettings.notifications.setToggleState(!buzzer.notifications.getMuteState());
  touchscreen.audioSettings.alerts.setToggleState(!buzzer.alerts.getMuteState());
  touchscreen.audioSettings.interface.setToggleState(!buzzer.interface.getMuteState());
  touchscreen.audioSettings.muteEverything.setToggleState(buzzer.getMuteState());

  // --------------------------------------------
  // Display settings screen

  touchscreen.displaySettings.display.setToggleState(touchscreen.getTouchscreenState());
  touchscreen.displaySettings.timeout.setToggleState(touchscreen.getTimeoutState());
  touchscreen.displaySettings.rgbLED.setToggleState(rgbLED.getLEDState());

  // --------------------------------------------
  // Cosmic ray detector settings screen

  touchscreen.cosmicRayDetector.setCoincidenceEvents(cosmicRayDetector.getCoincidenceEvents());
  touchscreen.cosmicRayDetector.setCoincidenceEventsPerHour(cosmicRayDetector.getCoincidenceEventsPerHour());
  touchscreen.cosmicRayDetector.setCoincidenceEventsTotal(cosmicRayDetector.getCoincidenceEventsTotal());
  touchscreen.cosmicRayDetector.setMainTubeCounts(cosmicRayDetector.getMainTubeCounts());
  touchscreen.cosmicRayDetector.setFollowerTubeCounts(cosmicRayDetector.getFollowerTubeCounts());

  // --------------------------------------------
  // Radiation history screen

  touchscreen.radiationHistory.setRadiationHistory(geigerCounter.getHistory(), geigerCounter.getHistoryIndex());

  // --------------------------------------------
  // Hotspot screen

  touchscreen.hotspotSettings.enable.setToggleState(wireless.getHotspotState());
  touchscreen.hotspotSettings.setIPAddress(wireless.getHotspotIPAddress());

  // --------------------------------------------
  // WiFi screen

  touchscreen.wifiSettings.enable.setToggleState(wireless.getWiFiState());
  touchscreen.wifiSettings.setWiFiName(wireless.getWiFiName());
  touchscreen.wifiSettings.setIPAddress(wireless.getWiFiIPAddress());

  // --------------------------------------------
  // System settings screen

  touchscreen.systemSettings1.sdCardMounted.setToggleState(sdCard.getMountState());
  touchscreen.systemSettings1.serialLogging.setToggleState(logger.getSerialLoggingState());
  touchscreen.systemSettings1.sdCardLogging.setToggleState(logger.getSDCardLoggingState());
  touchscreen.systemSettings1.dataLogging.setToggleState(logger.getLogLevelState(Logger::DATA));
  touchscreen.systemSettings1.eventLogging.setToggleState(logger.getLogLevelState(Logger::EVENT));
  touchscreen.systemSettings1.systemLogging.setToggleState(logger.getLogLevelState(Logger::SYSTEM));

  // --------------------------------------------
  // RGB LED
  
  rgbLED.setRadiationRating(geigerCounter.getRadiationRating());

  // Update the touchscreen
  touchscreen.update();

  // Update the RGB LED
  rgbLED.update();

}

// ================================================================================================
// Audio feedback
// ================================================================================================
void audioFeedback() {

  uint64_t counts               = geigerCounter.getCounts();
  double   microsievertsPerHour = geigerCounter.getMicrosievertsPerHour();
  uint64_t coincidenceEvents    = cosmicRayDetector.getCoincidenceEvents();

  // If the dose reaches the alarm level and not already playing alarm
  if (microsievertsPerHour >= BUZZER_ALARM_LEVEL_USVH && !buzzer.getPlaybackState(buzzer.alarm)) {

    // Play the alarm sound
    buzzer.play(buzzer.alarm);

  // If dose is below alarm level
  } else {

    // If the dose reaches the warning level
    if (microsievertsPerHour >= BUZZER_WARNING_LEVEL_USVH) {

      // If warning has not been played
      if (!PLAYED_DOSE_WARNING) {

        // Play the warning sound
        buzzer.play(buzzer.warning);

        // Set the played warning flag to true
        PLAYED_DOSE_WARNING = true;

      }

    // If the dose is below the warning level 
    } else {

      // Reset the played warning flag
      PLAYED_DOSE_WARNING = false;

    }

    // If no other sound is playing
    if (!buzzer.getPlaybackState()) {

      // Calculate the coincidence events since the last update
      uint16_t newCoincidenceEvents = coincidenceEvents - LAST_COINCIDENCE_EVENTS_VALUE;

      // If there are any new coincidence events
      if (newCoincidenceEvents) {

        // Play the coincidence event sound for the number of coincidence events
        buzzer.play(buzzer.coincidenceEvent, newCoincidenceEvents);

      // If the there were no new coincidence events
      } else {

        // Calculate the counts since the last update
        uint16_t newCounts = counts - LAST_COUNTS_VALUE;

        // If there are any new counts
        if (newCounts) {

          // Play the detection sound for the number of counts
          buzzer.play(buzzer.detection, newCounts);

        }

      }

    }

  }
  
  // Update the last known values
  LAST_COUNTS_VALUE             = counts;
  LAST_COINCIDENCE_EVENTS_VALUE = coincidenceEvents;

  // Update the buzzer
  buzzer.update();

}

// ================================================================================================
// Data feedback
// ================================================================================================
void dataFeedback() {

  // If log interval has been reached
  if (millis() - LOG_TIMER >= LOG_INTERVAL_SECONDS * 1000) {

    // If system info logging is enabled
    if (logger.getLogLevelState(Logger::SYSTEM)) {

      // Get data
      Logger::KeyValuePair systemData[6] = {

        {"upTime",   Logger::UINT64_T, {.uint64_v = millis()}             },
        {"heapSize", Logger::UINT32_T, {.uint32_v = ESP.getHeapSize()}    },
        {"freeHeap", Logger::UINT32_T, {.uint32_v = ESP.getFreeHeap()}    },
        {"minHeap",  Logger::UINT32_T, {.uint32_v = ESP.getMinFreeHeap()} },
        {"maxBlock", Logger::UINT32_T, {.uint32_v = ESP.getMaxAllocHeap()}},
        {"firmware", Logger::STRING_T, {.string_v = FIRMWARE_VERSION}     }

      };

      // Log data
      logger.log(Logger::SYSTEM, "system", systemData, 6);

    }

    // If the Geiger counter is enabled
    if (geigerCounter.getGeigerCounterState()) {

      // Get data
      Logger::KeyValuePair geigerCounterData[7] = {

        {"counts",                Logger::UINT64_T, {.uint64_v = geigerCounter.getCounts()}                },
        {"mainCounts",            Logger::UINT64_T, {.uint64_v = geigerCounter.getMainTubeCounts()}        },
        {"followerCounts",        Logger::UINT64_T, {.uint64_v = geigerCounter.getFollowerTubeCounts()}    },
        {"countsPerMinute",       Logger::DOUBLE_T, {.double_v = geigerCounter.getCountsPerMinute(60)}     },
        {"microsievertsPerHour",  Logger::DOUBLE_T, {.double_v = geigerCounter.getMicrosievertsPerHour(60)}},
        {"tubes",                 Logger::UINT8_T,  {.uint8_v  = TOTAL_NUMBER_OF_TUBES}                    },
        {"tubeType",              Logger::STRING_T, {.string_v = TUBE_TYPE_NAME}                           }

      };

      // Log data
      logger.log(Logger::DATA, "geigerCounter", geigerCounterData, 7);

    }

    // If cosmic ray detector is enabled
    if (cosmicRayDetector.getCosmicRayDetectorState()) {

      // Get data
      Logger::KeyValuePair cosmicRayDetectorData[5] = {

        {"coincidenceEvents", Logger::UINT64_T, {.uint64_v = cosmicRayDetector.getCoincidenceEvents()}       },
        {"eventsTotal",       Logger::UINT64_T, {.uint64_v = cosmicRayDetector.getCoincidenceEventsTotal()}  },
        {"eventsPerHour",     Logger::UINT32_T, {.uint32_v = cosmicRayDetector.getCoincidenceEventsPerHour()}},
        {"mainCounts",        Logger::UINT64_T, {.uint64_v = cosmicRayDetector.getMainTubeCounts()}          },
        {"followerCounts",    Logger::UINT64_T, {.uint64_v = cosmicRayDetector.getFollowerTubeCounts()}      }

      };

      // Log data
      logger.log(Logger::DATA, "cosmicRayDetector", cosmicRayDetectorData, 5);

    }

    // Update log timer
    LOG_TIMER = millis();

  }

}

//-------------------------------------------------------------------------------------------------
// Touch actions

// ================================================================================================
// 
// ================================================================================================
void goToSleep() {

  // Get the buzzer mute state
  LAST_BUZZER_MUTER_STATE = buzzer.getMuteState();

  // Mute the buzzer
  buzzer.mute();

  // Get the last LED state
  LAST_RGB_LED_STATE = rgbLED.getLEDState();

  // Disable the RGB LED
  rgbLED.disable();

  // Turn off display
  touchscreen.sleep();

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void wakeFromSleep() {

  // Set the buzzer to its last state
  buzzer.setMuteState(LAST_BUZZER_MUTER_STATE);

  // Set the last LED state and update LED
  rgbLED.setLEDState(LAST_RGB_LED_STATE);
  rgbLED.update();

  // Toggle on the display toggle
  touchscreen.displaySettings.display.toggleOn();

  // Draw previous screen
  touchscreen.drawPreviousScreen();

  // Reenable the touchscreen
  touchscreen.enable();

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void geigerCounterDecreaseIntegrationTime() {

  // Set auto integration state
  geigerCounter.setAutoIntegrateState(false);

  // Update settings
  settings.data.parameters.geigerCounter.autoIntegrate = false;

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
// 
// ================================================================================================
void geigerCounterResetIntegrationTime() {

  // Set auto integration state
  geigerCounter.setAutoIntegrateState(true);

  // Update settings
  settings.data.parameters.geigerCounter.autoIntegrate = true;

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void geigerCounterIncreaseIntegrationTime() {

  // Set auto integration state
  geigerCounter.setAutoIntegrateState(false);

  // Update settings
  settings.data.parameters.geigerCounter.autoIntegrate = false;

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
// 
// ================================================================================================
void geigerCounterDeselectAllUnits() {

  touchscreen.geigerCounterSettings2.sieverts.deselect();
  touchscreen.geigerCounterSettings2.rem.deselect();
  touchscreen.geigerCounterSettings2.rontgen.deselect();
  touchscreen.geigerCounterSettings2.gray.deselect();

}

// ================================================================================================
// 
// ================================================================================================
void cosmicRayDetectorMute() {

  // Toggle the the detections mute state
  buzzer.detections.setMuteState(!buzzer.detections.getMuteState());

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void cosmicRayDetectorDisable() {

  // Disable the cosmic ray detector
  cosmicRayDetector.disable();

  // Display the Geiger counter screen
  displayGeigerCounter();

}

// ================================================================================================
// 
// ================================================================================================
void displayGeigerCounter() {

  // Enable the geiger counter
  geigerCounter.enable();

  // Rotate to correct orientation
  touchscreen.setRotationLandscape();

  // Draw the screen
  touchscreen.draw(touchscreen.geigerCounter);

  // Play a sound
  buzzer.play(buzzer.back);

}

// ================================================================================================
// 
// ================================================================================================
void displayGeigerCounterSettings1() {

  // Rotate to correct orientation
  touchscreen.setRotationLandscape();

  // Draw the screen
  touchscreen.draw(touchscreen.geigerCounterSettings1);

  // Play a sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// 
// ================================================================================================
void displayGeigerCounterSettings2() {

  // Rotate to correct orientation
  touchscreen.setRotationLandscape();

  // Draw the screen
  touchscreen.draw(touchscreen.geigerCounterSettings2);

  // Play a sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// 
// ================================================================================================
void displayAudioSettings() {

  // Rotate to correct orientation
  touchscreen.setRotationLandscape();

  // Draw the screen
  touchscreen.draw(touchscreen.audioSettings);

  // Play a sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// 
// ================================================================================================
void displayDisplaySettings() {

  // Rotate to correct orientation
  touchscreen.setRotationLandscape();

  // Draw the screen
  touchscreen.draw(touchscreen.displaySettings);

  // Play a sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// 
// ================================================================================================
void displayRotationConfirmation() {

  // Show cosmic ray detector screen if cosmic ray detector is already enabled
  if (cosmicRayDetector.getCosmicRayDetectorState()) {

    displayCosmicRayDetector();

  // If cosmic ray detector is disabled show rotation confirmation screen
  } else {

    // Rotate to correct orientation
    touchscreen.setRotationPortrait();

    // Draw the screen
    touchscreen.draw(touchscreen.rotationConfirmation);

    // Play a sound
    buzzer.play(buzzer.next);

  }

}

// ================================================================================================
// 
// ================================================================================================
void displayCosmicRayDetector() {

  // Enable the cosmic ray detector
  cosmicRayDetector.enable();

  // Apply current cosmic ray detector values
  touchscreen.cosmicRayDetector.setCoincidenceEvents(cosmicRayDetector.getCoincidenceEvents());
  touchscreen.cosmicRayDetector.setCoincidenceEventsPerHour(cosmicRayDetector.getCoincidenceEventsPerHour());
  touchscreen.cosmicRayDetector.setCoincidenceEventsTotal(cosmicRayDetector.getCoincidenceEventsTotal());
  touchscreen.cosmicRayDetector.setMainTubeCounts(cosmicRayDetector.getMainTubeCounts());
  touchscreen.cosmicRayDetector.setFollowerTubeCounts(cosmicRayDetector.getFollowerTubeCounts());

  // Rotate to correct orientation
  touchscreen.setRotationPortrait();

  // Draw the screen
  touchscreen.draw(touchscreen.cosmicRayDetector);

  // Play a sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// 
// ================================================================================================
void displayDisableCosmicRayDetector() {

  // Rotate to correct orientation
  touchscreen.setRotationPortrait();

  // Draw the screen
  touchscreen.draw(touchscreen.disableCosmicRayDetector);

  // Play a sound
  buzzer.play(buzzer.back);

}

// ================================================================================================
// 
// ================================================================================================
void displayRadiationHistory() {

  // Rotate to correct orientation
  touchscreen.setRotationLandscape();

  // Draw the screen
  touchscreen.draw(touchscreen.radiationHistory);

  // Play a sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// 
// ================================================================================================
void displayTrueRNG() {

  // Rotate to correct orientation
  touchscreen.setRotationLandscape();

  // Draw the screen
  touchscreen.draw(touchscreen.trueRNG);

  // Play a sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// 
// ================================================================================================
void displayHotspotSettings() {

  // Rotate to correct orientation
  touchscreen.setRotationLandscape();

  // Draw the screen
  touchscreen.draw(touchscreen.hotspotSettings);

  // Play a sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// 
// ================================================================================================
void displayWiFiSettings() {

  // Rotate to correct orientation
  touchscreen.setRotationLandscape();

  // Draw the screen
  touchscreen.draw(touchscreen.wifiSettings);

  // Play a sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// 
// ================================================================================================
void displaySystemSettings1() {

  // Rotate to correct orientation
  touchscreen.setRotationLandscape();

  // Draw the screen
  touchscreen.draw(touchscreen.systemSettings1);

  // Play a sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// 
// ================================================================================================
void displaySystemSettings2() {

  // Rotate to correct orientation
  touchscreen.setRotationLandscape();

  // Draw the screen
  touchscreen.draw(touchscreen.systemSettings2);

  // Play a sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// 
// ================================================================================================
void selectGeigerCounterSieverts() {

  // Set Geiger counter measurement unit
  geigerCounter.setMeasurementUnit(GeigerCounter::SIEVERTS);

  // Update settings
  settings.data.parameters.geigerCounter.measurementUnit = GeigerCounter::SIEVERTS;

  // Deselect all units
  geigerCounterDeselectAllUnits();

  // Select unit
  touchscreen.geigerCounterSettings2.sieverts.select();

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void selectGeigerCounterRem() {

  // Set Geiger counter measurement unit
  geigerCounter.setMeasurementUnit(GeigerCounter::REM);

  // Update settings
  settings.data.parameters.geigerCounter.measurementUnit = GeigerCounter::REM;

  // Deselect all units
  geigerCounterDeselectAllUnits();

  // Select unit
  touchscreen.geigerCounterSettings2.rem.select();

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void selectGeigerCounterRontgen() {

  // Set Geiger counter measurement unit
  geigerCounter.setMeasurementUnit(GeigerCounter::RONTGEN);

  // Update settings
  settings.data.parameters.geigerCounter.measurementUnit = GeigerCounter::RONTGEN;

  // Deselect all units
  geigerCounterDeselectAllUnits();

  // Select unit
  touchscreen.geigerCounterSettings2.rontgen.select();

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void selectGeigerCounterGray() {

  // Set Geiger counter measurement unit
  geigerCounter.setMeasurementUnit(GeigerCounter::GRAY);

  // Update settings
  settings.data.parameters.geigerCounter.measurementUnit = GeigerCounter::GRAY;

  // Deselect all units
  geigerCounterDeselectAllUnits();

  // Select unit
  touchscreen.geigerCounterSettings2.gray.select();

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void toggleGeigerCounterAutoIntegration(const bool toggled) {

  // Set the new state
  geigerCounter.setAutoIntegrateState(toggled);

  // Update settings
  settings.data.parameters.geigerCounter.autoIntegrate = toggled;

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void toggleGeigerCounterAutoRanging(const bool toggled) {

  // Set the new state
  geigerCounter.setAutoRangeState(toggled);

  // Update settings
  settings.data.parameters.geigerCounter.autoRange = toggled;

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void toggleAudioDetections(const bool toggled) {

  // Set the new state
  buzzer.detections.setMuteState(!toggled);

  // Update settings
  settings.data.parameters.buzzer.detections = !toggled;

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void toggleAudioNotifications(const bool toggled) {

  // Set the new state
  buzzer.notifications.setMuteState(!toggled);

  // Update settings
  settings.data.parameters.buzzer.notifications = !toggled;

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void toggleAudioAlerts(const bool toggled) {

  // Set the new state
  buzzer.alerts.setMuteState(!toggled);

  // Update settings
  settings.data.parameters.buzzer.alerts = !toggled;

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void toggleAudioInterface(const bool toggled) {

  // Set the new state
  buzzer.interface.setMuteState(!toggled);

  // Update settings
  settings.data.parameters.buzzer.interface = !toggled;

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void toggleAudioMuteEverything(const bool toggled) {

  // Set the new state
  buzzer.setMuteState(toggled);

  // Update settings
  settings.data.parameters.buzzer.muteEverything = toggled;

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void toggleDisplayState(const bool toggled) {

  // If toggled off make the touchscreen go to sleep
  if (!toggled) { touchscreen.sleep(); }
  
  // Set the new state
  touchscreen.setTouchscreenState(toggled);

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void toggleDisplayAutoTimeout(const bool toggled) {

  // Set the new state
  touchscreen.setTimeoutState(toggled);

  // Update settings
  settings.data.parameters.display.timeout = toggled;

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void toggleDisplayLEDState(const bool toggled) {

  // Set the new state
  rgbLED.setLEDState(toggled);

  // Update settings
  settings.data.parameters.display.rgbLED = toggled;

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void toggleHotspotState(const bool toggled) {

  // Set the new state
  wireless.setHotspotState(toggled);

  // Update settings
  settings.data.parameters.wireless.hotspot = toggled;

  // Change state
  touchscreen.hotspotSettings.setIPAddress(wireless.getHotspotIPAddress());

  // Update the touchscreen
  touchscreen.refresh();

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void toggleWiFiSate(const bool toggled) {

  // Set the new state
  wireless.setWiFiState(toggled);

  // Update settings
  settings.data.parameters.wireless.wifi = toggled;

  // Change state
  touchscreen.wifiSettings.setIPAddress(wireless.getWiFiIPAddress());

  // Update the touchscreen
  touchscreen.refresh();

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void toggleSystemSDCardMount(const bool toggled) {

  // Set the new state
  sdCard.setMountState(toggled);

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void toggleSystemSerialLogging(const bool toggled) {

  // Set the new state
  logger.setSerialLoggingState(toggled);

  // Update settings
  settings.data.parameters.logger.serial = toggled;

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void toggleSystemSDCardLogging(const bool toggled) {

  // Set the new state
  logger.setSDCardLoggingState(toggled);

  // Update settings
  settings.data.parameters.logger.sdCard = toggled;

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void toggleSystemDataLogging(const bool toggled) {

  // Set the new state
  logger.setLogLevelState(Logger::DATA, toggled);

  // Update settings
  settings.data.parameters.logger.data = toggled;

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void toggleSystemEventLogging(const bool toggled) {

  // Set the new state
  logger.setLogLevelState(Logger::EVENT, toggled);

  // Update settings
  settings.data.parameters.logger.event = toggled;

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ================================================================================================
// 
// ================================================================================================
void toggleSystemInfoLogging(const bool toggled) {

  // Set the new state
  logger.setLogLevelState(Logger::SYSTEM, toggled);

  // Update settings
  settings.data.parameters.logger.system = toggled;

  // Play a sound
  buzzer.play(buzzer.tap);

}

// ------------------------------------------------------------------------------------------------
// Web server actions

// ================================================================================================
// 
// ================================================================================================
void sendGeigerCounterData() {

  // Get data
  Logger::KeyValuePair data[9] = {

    {"enabled",               Logger::BOOL_T,   {.bool_v   = geigerCounter.getGeigerCounterState()}    },
    {"counts",                Logger::UINT64_T, {.uint64_v = geigerCounter.getCounts()}                },
    {"mainCounts",            Logger::UINT64_T, {.uint64_v = geigerCounter.getMainTubeCounts()}        },
    {"followerCounts",        Logger::UINT64_T, {.uint64_v = geigerCounter.getFollowerTubeCounts()}    },
    {"countsPerMinute",       Logger::DOUBLE_T, {.double_v = geigerCounter.getCountsPerMinute(60)}     },
    {"microsievertsPerHour",  Logger::DOUBLE_T, {.double_v = geigerCounter.getMicrosievertsPerHour(60)}},
    {"rating",                Logger::UINT8_T,  {.uint8_v  = geigerCounter.getRadiationRating()}       },
    {"tubes",                 Logger::UINT8_T,  {.uint8_v  = TOTAL_NUMBER_OF_TUBES}                    },
    {"tubeType",              Logger::STRING_T, {.string_v = TUBE_TYPE_NAME}                           }

  };

  // JSON data string
  String json;

  // Construct the data string
  logger.getLogMessage("geigerCounter", data, 9, json);

  // Send JSON data
  wireless.server.send(200, "application/json", json);

}

// ================================================================================================
// 
// ================================================================================================
void sendCosmicRayDetectorData() {

  // Get data
  Logger::KeyValuePair data[6] = {

    {"enabled",           Logger::BOOL_T,   {.bool_v   = cosmicRayDetector.getCosmicRayDetectorState()}  },
    {"coincidenceEvents", Logger::UINT64_T, {.uint64_v = cosmicRayDetector.getCoincidenceEvents()}       },
    {"eventsTotal",       Logger::UINT64_T, {.uint64_v = cosmicRayDetector.getCoincidenceEventsTotal()}  },
    {"eventsPerHour",     Logger::UINT32_T, {.uint32_v = cosmicRayDetector.getCoincidenceEventsPerHour()}},
    {"mainCounts",        Logger::UINT64_T, {.uint64_v = cosmicRayDetector.getMainTubeCounts()}          },
    {"followerCounts",    Logger::UINT64_T, {.uint64_v = cosmicRayDetector.getFollowerTubeCounts()}      }
    
  };

  // JSON data string
  String json;

  // Construct the data string
  logger.getLogMessage("cosmicRayDetector", data, 6, json);

  // Send JSON data
  wireless.server.send(200, "application/json", json);

}

// ================================================================================================
// 
// ================================================================================================
void sendLogFileData() {

  // If the SD card is mounted
  if (sdCard.getMountState()) {

    // Flag for checking if log file was found
    bool found = false;

    // Get the log file path
    const char *logFilePath = logger.getLogFilePath();

    // Check if log file exists
    if (sdCard.exists(logFilePath)) {

      // Open log file
      File file = sdCard.open(logFilePath);

      // If log file was successfully accessed
      if (file) {

        // Stream the log file data to the HTTP client
        wireless.server.streamFile(file, "text/plain");

        // Set the found flag to true
        found = true;

      }

      // Close the log file
      file.close();

    }

    // If the log file was not found
    if (!found) {

      // Return with a 404 - Not found!
      wireless.server.send(404, "text/plain", "404 - Not found!");

    }

  } else {

    // Return with a 500 - No SD Card Mounted!
    wireless.server.send(500, "text/plain", "500 - No SD Card Mounted!");

  }

}

// ================================================================================================
// 
// ================================================================================================
void sendSystemInfoData() {

  // Get data
  Logger::KeyValuePair data[14] = {

    {"upTime",            Logger::UINT64_T, {.uint64_v = millis()}                                     },
    {"heapSize",          Logger::UINT32_T, {.uint32_v = ESP.getHeapSize()}                            },
    {"freeHeap",          Logger::UINT32_T, {.uint32_v = ESP.getFreeHeap()}                            },
    {"minHeap",           Logger::UINT32_T, {.uint32_v = ESP.getMinFreeHeap()}                         },
    {"maxBlock",          Logger::UINT32_T, {.uint32_v = ESP.getMaxAllocHeap()}                        },
    {"sdCard",            Logger::BOOL_T,   {.bool_v   = sdCard.getMountState()}                       },
    {"geigerCounter",     Logger::BOOL_T,   {.bool_v   = geigerCounter.getGeigerCounterState()}        },
    {"cosmicRayDetector", Logger::BOOL_T,   {.bool_v   = cosmicRayDetector.getCosmicRayDetectorState()}},
    {"buzzer",            Logger::BOOL_T,   {.bool_v   = !buzzer.getMuteState()}                       },
    {"touchscreen",       Logger::BOOL_T,   {.bool_v   = touchscreen.getTouchscreenState()}            },
    {"hotspot",           Logger::BOOL_T,   {.bool_v   = wireless.getHotspotState()}                   },
    {"wifi",              Logger::BOOL_T,   {.bool_v   = wireless.getWiFiState()}                      },
    {"server",            Logger::BOOL_T,   {.bool_v   = wireless.getServerState()}                    },
    {"firmware",          Logger::STRING_T, {.string_v = FIRMWARE_VERSION}                             }

  };

  // JSON data string
  String json;

  // Construct the data string
  logger.getLogMessage("system", data, 14, json);

  // Send JSON data
  wireless.server.send(200, "application/json", json);

}