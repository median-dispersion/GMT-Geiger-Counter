#include "Arduino.h"
#include "Configuration.h"
#include "Strings.h"
#include "GeigerCounter.h"
#include "CosmicRayDetector.h"
#include "Buzzer.h"
#include "Touchscreen.h"

// ------------------------------------------------------------------------------------------------
// Global

// Global objects
GeigerCounter     geigerCounter;
CosmicRayDetector cosmicRayDetector;
Buzzer            buzzer;
Touchscreen       touchscreen;

// Global variables
bool     playedDoseWarning     = false;
uint64_t lastCounts            = 0;
uint64_t lastCoincidenceEvents = 0;
bool     lastMute              = false;
Screen   *lastScreen           = nullptr;

// Function prototypes
void setup();
void loop();
void audioFeedback();
void visualFeedback();
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
void goToSleep();
void wakeFromSleep();
void cosmicRayDetectorMute();

// ================================================================================================
// Setup
// ================================================================================================
void setup() {

  // --------------------------------------------
  // Initialize everything

  Serial.begin(SERIAL_BAUD_RATE);
  geigerCounter.begin();
  cosmicRayDetector.begin();
  buzzer.begin();
  touchscreen.begin();

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

  // WiFi settings screen
  touchscreen.wifiSettings.back.action                     = displayGeigerCounter;

  // System settings screen
  touchscreen.systemSettings.back.action                   = displayGeigerCounter;

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

  // Set cosmic ray detector screen values
  touchscreen.cosmicRayDetector.setCoincidenceEvents(cosmicRayDetector.getCoincidenceEvents());
  touchscreen.cosmicRayDetector.setCoincidenceEventsPerHour(cosmicRayDetector.getCoincidenceEventsPerHour());
  touchscreen.cosmicRayDetector.setCoincidenceEventsTotal(cosmicRayDetector.getCoincidenceEvents());
  touchscreen.cosmicRayDetector.setMainTubeCounts(geigerCounter.getMainTubeCounts());
  touchscreen.cosmicRayDetector.setFollowerTubeCounts(geigerCounter.getFollowerTubeCounts());

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

    // Store the last screen
    lastScreen = touchscreen.getScreen();

    // Turn off the display
    touchscreen.disable();

    // Draw the sleep screen
    touchscreen.draw(touchscreen.sleep);

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

  // Store the last screen state
  lastScreen = touchscreen.getScreen();

  // Mute the buzzer
  buzzer.mute();

  // Turn off the touchscreen
  touchscreen.disable();

  // Toggle off the appropriate toggles
  touchscreen.audioSettings.muteEverything.toggleOn();
  touchscreen.displaySettings.display.toggleOff();

  // Draw the sleep screen
  touchscreen.draw(touchscreen.sleep);

}

// ================================================================================================
// Wake from sleep
// ================================================================================================
void wakeFromSleep() {

  // Draw the last screen
  touchscreen.draw(lastScreen);

  // Toggle on the display toggle
  touchscreen.displaySettings.display.toggleOn();

  // If buzzer was previously unmuted
  if (lastMute == false) {

    // Toggle off the mute everything toggle
    touchscreen.audioSettings.muteEverything.toggleOff();

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

    // Toggle off the mute everything toggle
    touchscreen.audioSettings.muteEverything.toggleOff();

    // Unmute the buzzer
    buzzer.unmute();

  } else {

    // Toggle on the mute everything toggle
    touchscreen.audioSettings.muteEverything.toggleOn();

    // Mute the buzzer
    buzzer.mute();

  }

}