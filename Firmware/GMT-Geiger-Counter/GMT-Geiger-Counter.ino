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

// Variable for keeping track of the last sates
uint64_t lastCountsValue       = 0;
uint64_t lastCoincidenceEvents = 0;
bool     lastMuteValue         = false;
Screen   *lastScreenValue      = nullptr;

// Flag for checking if the warning sound has been played
bool playedWarning = false;

// Variables for keeping track of main loop performance
uint64_t loops = 0;
uint64_t totalLoopTimeMicroseconds   = 0;
uint64_t averageLoopTimeMicroseconds = 0;
uint64_t maximumLoopTimeMicroseconds = 0;

// Timer for logging
uint64_t logTimer  = 0;

// ================================================================================================
// Setup
// ================================================================================================
void setup() {

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
  touchscreen.rotationConfirmation.back.action             = cosmicRayDetectorBack;
  touchscreen.rotationConfirmation.confirm.action          = displayCosmicRayDetector;

  // Cosmic ray detector screen
  touchscreen.cosmicRayDetector.back.action                = cosmicRayDetectorBack;
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

  // TODO
  // Clean up the main loop
  // Add additional features

  // Variable for keeping track of the total loop time
  uint64_t loopStartMicroseconds = micros();

  // --------------------------------------------
  // Audio feedback

  // Get values for usage in audio feedback
  double   microsievertsPerHour = geigerCounter.getMicrosievertsPerHour();
  uint64_t coincidenceEvents    = cosmicRayDetector.getCoincidenceEvents();
  uint64_t counts               = geigerCounter.getCounts();

  // If the dose reaches the alarm level
  if (microsievertsPerHour >= BUZZER_ALARM_LEVEL_USVH) {

    // Play the alarm sound
    buzzer.play(buzzer.alarm);

  // If dose is below alarm level
  } else {

    // If the dose reaches the warning level
    if (microsievertsPerHour >= BUZZER_WARNING_LEVEL_USVH) {

      // If warning has not been played
      if (!playedWarning) {

        // Play the warning sound
        buzzer.play(buzzer.warning);

        // Set the played warning flag to true
        playedWarning = true;

      }

    // If the dose is below the warning level 
    } else {

      // Reset the played warning flag
      playedWarning = false;

    }

    // If no other sound is playing
    if (!buzzer.playing()) {

      // Calculate the coincidence events since the last update
      uint16_t newCoincidenceEvents = coincidenceEvents - lastCoincidenceEvents;

      // Play the coincidence event sound for the number of coincidence events
      buzzer.play(buzzer.coincidenceEvent, newCoincidenceEvents);

      // If still not playing any other sound
      if (!buzzer.playing()) {

        // Calculate the counts since the last update
        uint16_t newCounts = counts - lastCountsValue;

        // Play the detection sound for the number of counts
        buzzer.play(buzzer.detection, newCounts);

      }

    }

  }

  // Update the last known values
  lastCoincidenceEvents = coincidenceEvents;
  lastCountsValue       = counts;

  // --------------------------------------------
  // Visual feedback

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

  // --------------------------------------------
  // Temporary logging function

  // TODO
  // Create a logger class that can log this data more properly
  // Instead of just dumping it to the serial console
  // This also take A LONG time (+80ms) to print...

  // Every 3 seconds log to Serial console
  if (millis() - logTimer >= 3000) {
    
    Serial.println("Up time:"                      + String(millis())                                        + " ms");
    Serial.println("Average loop time: "           + String(averageLoopTimeMicroseconds)                     + " us");
    Serial.println("Maximum loop time: "           + String(maximumLoopTimeMicroseconds)                     + " us");
    Serial.println("Total heap: "                  + String(ESP.getHeapSize())                               + " bytes");
    Serial.println("Free heap: "                   + String(ESP.getFreeHeap())                               + " bytes");
    Serial.println("Minimum free heap: "           + String(ESP.getMinFreeHeap())                            + " bytes");
    Serial.println("Firmware version: "            + String(FIRMWARE_VERSION));
    Serial.println("Counts: "                      + String(geigerCounter.getCounts()));
    Serial.println("Main tube counts: "            + String(geigerCounter.getMainTubeCounts()));
    Serial.println("Follower tube counts: "        + String(geigerCounter.getFollowerTubeCounts()));
    Serial.println("Counts per minute: "           + String(geigerCounter.getCountsPerMinute())              + " CPM");
    Serial.println("Equivalent dose: "             + String(geigerCounter.getMicrosievertsPerHour())         + " µSv/h");
    Serial.println("Integration time: "            + String(geigerCounter.getIntegrationTime())              + " s");
    Serial.println("Radiation rating: "            + String(geigerCounter.getRadiationRating()));
    Serial.println("Tube type: "                   + String(TUBE_TYPE_NAME));
    Serial.println("Coincidence events: "          + String(cosmicRayDetector.getCoincidenceEvents()));
    Serial.println("Coincidence events per hour: " + String(cosmicRayDetector.getCoincidenceEventsPerHour()) + " CPH");
    Serial.println(" ");

    // Update log timer
    logTimer = millis();

  }

  // --------------------------------------------
  // Update everything

  // Update the buzzer
  buzzer.update();

  // Update the touchscreen
  touchscreen.update();

  // --------------------------------------------
  // Update loop stats

  // Calculate current loop time
  uint64_t currentLoopTimeMicroseconds = micros() - loopStartMicroseconds;

  // Increase loops counter
  loops++;

  // Add current loop time to the total loop time
  totalLoopTimeMicroseconds += currentLoopTimeMicroseconds;

  // Calculate the average loop time
  averageLoopTimeMicroseconds = totalLoopTimeMicroseconds / loops;

  // Update the maximum loop time if the current loop time is longer than the old max
  if (maximumLoopTimeMicroseconds < currentLoopTimeMicroseconds) { maximumLoopTimeMicroseconds = currentLoopTimeMicroseconds; }

}

// ------------------------------------------------------------------------------------------------
// Touch actions

// ================================================================================================
// Display the Geiger counter screen
// ================================================================================================
void displayGeigerCounter() {

  // Draw the Geiger counter screen
  touchscreen.draw(touchscreen.geigerCounter);

  // Play the back sound
  buzzer.play(buzzer.back);

}

// ================================================================================================
// Display the audio settings screen
// ================================================================================================
void displayAudioSettings() {

  // Draw the audio settings screen
  touchscreen.draw(touchscreen.audioSettings);

  // Play the enter sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// Display the display settings screen
// ================================================================================================
void displayDisplaySettings() {

  // Draw the audio settings screen
  touchscreen.draw(touchscreen.displaySettings);

  // Play the enter sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// Display the radiation history screen
// ================================================================================================
void displayRadiationHistory() {

  // Draw the screen
  touchscreen.draw(touchscreen.radiationHistory);

  // Play the enter sound
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

  // Play the enter sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// Display the cosmic ray detector screen
// ================================================================================================
void displayCosmicRayDetector() {

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

  // Play the enter sound
  buzzer.play(buzzer.next);

  // Enable the cosmic ray detector
  cosmicRayDetector.enable();

}

// ================================================================================================
// Display the true random number generator screen
// ================================================================================================
void displayTrueRNG() {

  // Draw the screen
  touchscreen.draw(touchscreen.trueRNG);

  // Play the enter sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// Display the hotspot settings screen
// ================================================================================================
void displayHotspotSettings() {

  // Draw the screen
  touchscreen.draw(touchscreen.hotspotSettings);

  // Play the enter sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// Display the WiFi settings screen
// ================================================================================================
void displayWiFiSettings() {

  // Draw the screen
  touchscreen.draw(touchscreen.wifiSettings);

  // Play the enter sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// Display the system settings screen
// ================================================================================================
void displaySystemSettings() {

  // Draw the screen
  touchscreen.draw(touchscreen.systemSettings);

  // Play the enter sound
  buzzer.play(buzzer.next);

}

// ================================================================================================
// Decrease the integration time
// ================================================================================================
void decreaseIntegrationTime() {

  // Get current integration time
  uint16_t integrationTimeSeconds = geigerCounter.getIntegrationTime();

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
  buzzer.play(buzzer.click);

}

// ================================================================================================
// Increase the integration time
// ================================================================================================
void increaseIntegrationTime() {

  // Get current integration time
  uint16_t integrationTimeSeconds = geigerCounter.getIntegrationTime();

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
  buzzer.play(buzzer.click);

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
  buzzer.play(buzzer.click);

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
  buzzer.play(buzzer.click);

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
  buzzer.play(buzzer.click);

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
  buzzer.play(buzzer.click);

}

// ================================================================================================
// Toggle the display power state
// ================================================================================================
void toggleDisplayPower(bool toggled) {

  // If toggled on
  if (toggled) {

    touchscreen.on();

  // If toggled off
  } else {

    // Store the last mute state
    lastMuteValue = buzzer.muted();

    // Store the last screen
    lastScreenValue = touchscreen.getScreen();

    // Turn off the display
    touchscreen.off();

    // Draw the sleep screen
    touchscreen.draw(touchscreen.sleep);

  }

  // Play a sound
  buzzer.play(buzzer.click);

}

// ================================================================================================
// Go to sleep
// ================================================================================================
void goToSleep() {

  // Store the last mute state
  lastMuteValue = buzzer.muted();

  // Store the last screen state
  lastScreenValue = touchscreen.getScreen();

  // Mute the buzzer
  buzzer.mute();

  // Turn off the touchscreen
  touchscreen.off();

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
  touchscreen.draw(lastScreenValue);

  // Toggle on the display toggle
  touchscreen.displaySettings.display.toggleOn();

  // If buzzer was previously unmuted
  if (lastMuteValue == false) {

    // Toggle off the mute everything toggle
    touchscreen.audioSettings.muteEverything.toggleOff();

    // Unmute the buzzer
    buzzer.unmute();

  }

  // Turn on the touchscreen
  touchscreen.on();

}

// ================================================================================================
// Retrun from the cosmic ray detector screen
// ================================================================================================
void cosmicRayDetectorBack() {

  // Disable the cosmic ray detector
  cosmicRayDetector.disable();

  // Set the default screen rotation
  touchscreen.rotateLandscape();

  // Display the geiger counter screen
  displayGeigerCounter();

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