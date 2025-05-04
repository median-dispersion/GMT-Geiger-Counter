#include "Arduino.h"
#include "Configuration.h"
#include "Logger.h"
#include "SDCard.h"
#include "Settings.h"
#include "GeigerCounter.h"
#include "CosmicRayDetector.h"
#include "Buzzer.h"
#include "Touchscreen.h"

// ------------------------------------------------------------------------------------------------
// Global

// Global variables
bool     playedDoseWarning     = false;
uint64_t lastCounts            = 0;
uint64_t lastCoincidenceEvents = 0;
bool     lastBuzzerMuteState   = false;

// Function prototypes
void setup();
void loop();
void setTouchActions();
void setUserSettings();
void audioFeedback();
void visualFeedback();
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
void displaySystemSettings();
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
void toggleDisplayPower(const bool toggled);
void toggleDisplayAutoTimeout(const bool toggled);
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

  // Set touch actions
  setTouchActions();

  // Load and set the user settings
  setUserSettings();

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

  // Update user settings
  settings.update();

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
  touchscreen.geigerCounter.systemSettings.action          = displaySystemSettings;

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
  touchscreen.displaySettings.display.action = toggleDisplayPower;
  touchscreen.displaySettings.timeout.action = toggleDisplayAutoTimeout;

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
  // System settings touch actions

  touchscreen.systemSettings.back.action          = displayGeigerCounter;
  touchscreen.systemSettings.sdCardMounted.action = toggleSystemSDCardMount;
  touchscreen.systemSettings.serialLogging.action = toggleSystemSerialLogging;
  touchscreen.systemSettings.sdCardLogging.action = toggleSystemSDCardLogging;
  touchscreen.systemSettings.dataLogging.action   = toggleSystemDataLogging;
  touchscreen.systemSettings.eventLogging.action  = toggleSystemEventLogging;
  touchscreen.systemSettings.systemLogging.action = toggleSystemInfoLogging;

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

  // --------------------------------------------
  // Cosmic ray detector settings screen

  touchscreen.cosmicRayDetector.setCoincidenceEvents(cosmicRayDetector.getCoincidenceEvents());
  touchscreen.cosmicRayDetector.setCoincidenceEventsPerHour(cosmicRayDetector.getCoincidenceEventsPerHour());
  touchscreen.cosmicRayDetector.setCoincidenceEventsTotal(cosmicRayDetector.getCoincidenceEventsTotal());
  touchscreen.cosmicRayDetector.setMainTubeCounts(cosmicRayDetector.getMainTubeCounts());
  touchscreen.cosmicRayDetector.setFollowerTubeCounts(cosmicRayDetector.getFollowerTubeCounts());

  // --------------------------------------------
  // Radiation history screen screen

  touchscreen.radiationHistory.setRadiationHistory(geigerCounter.getHistory(), geigerCounter.getHistoryIndex());

  // --------------------------------------------
  // System settings screen

  touchscreen.systemSettings.sdCardMounted.setToggleState(sdCard.getMountState());
  touchscreen.systemSettings.serialLogging.setToggleState(logger.getSerialLoggingState());
  touchscreen.systemSettings.sdCardLogging.setToggleState(logger.getSDCardLoggingState());
  touchscreen.systemSettings.dataLogging.setToggleState(logger.getLogLevelState(Logger::DATA));
  touchscreen.systemSettings.eventLogging.setToggleState(logger.getLogLevelState(Logger::EVENT));
  touchscreen.systemSettings.systemLogging.setToggleState(logger.getLogLevelState(Logger::SYSTEM));

  touchscreen.update();

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
    if (!buzzer.getPlaybackState()) {

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

//-------------------------------------------------------------------------------------------------
// Touch actions

// ================================================================================================
// 
// ================================================================================================
void goToSleep() {

  // Get the buzzer mute state
  lastBuzzerMuteState = buzzer.getMuteState();

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
  buzzer.setMuteState(lastBuzzerMuteState);

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

  // Rotate to correct orientation
  touchscreen.setRotationPortrait();

  // Draw the screen
  touchscreen.draw(touchscreen.rotationConfirmation);

  // Play a sound
  buzzer.play(buzzer.next);

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
void displaySystemSettings() {

  // Rotate to correct orientation
  touchscreen.setRotationLandscape();

  // Draw the screen
  touchscreen.draw(touchscreen.systemSettings);

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
void toggleDisplayPower(const bool toggled) {

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
void toggleHotspotState(const bool toggled) {}

// ================================================================================================
// 
// ================================================================================================
void toggleWiFiSate(const bool toggled) {}

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