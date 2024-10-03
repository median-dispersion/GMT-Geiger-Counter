#include "Configuration.h"
#include "GeigerCounter.h"
#include "Buzzer.h"

// ================================================================================================
// Global
// ================================================================================================

// Create objects
GeigerCounter geiger;
Buzzer buzzer;

// Loop timer
uint64_t loopTimer = 0;

// Flag for checking if the warning sound has been played
bool playedWarning = false;

// Variable for keeping track of new counts
uint64_t lastKnownAccumulativeCount = 0;

// ================================================================================================
// Setup
// ================================================================================================
void setup() {

  // Start serial communication
  Serial.begin(SERIAL_BAUD_RATE);

  // Initialize Geiger counter
  geiger.begin();

  // Enable pulse counting
  geiger.enable();

  // Set integration time
  geiger.setIntegrationTime(DEFAULT_INTEGRATION_TIME);

  // Initialize buzzer
  buzzer.begin();

  // Play the power on jingle
  buzzer.play(Buzzer::JINGLE);

  // Wait until the buzzer has finished playing
  while (buzzer.playing()) { buzzer.update(); }

}

// ================================================================================================
// Main loop
// ================================================================================================
void loop() {

  // Get values
  uint64_t accumulativeCount = geiger.getAccumulativeCount();
  double   countsPerMinute   = geiger.getCountsPerMinute();
  double   equivalentDose    = geiger.getMicroSievertPerHour();

  // If the equivalent dose reaches the alarm level
  if (equivalentDose >= BUZZER_ALARM_LEVEL_USVH) {

    // Play the alarm sound
    buzzer.play(Buzzer::ALARM);
    
  // If the equivalent dose is below the alarm level
  } else {

    // If the equivalent dose reaches the warning level
    if (equivalentDose >= BUZZER_WARNING_LEVEL_USVH) {

      // If warning has not been played
      if (!playedWarning) {

        // Play the warning sound
        buzzer.play(Buzzer::WARNING);

        // Set the played warning flag to true
        playedWarning = true;

      }

    // If the equivalent dose is below the warning level 
    } else {

      // Reset the played warning flag
      playedWarning = false;

    }

    // If no other sound is playing
    if (!buzzer.playing()) {

      // Calculate the counts since the last update
      uint16_t counts = accumulativeCount - lastKnownAccumulativeCount;

      // Play the detection sound for the number of counts
      buzzer.play(Buzzer::DETECTION, counts);

    }

  }

  // Update the last known accumulative count value
  lastKnownAccumulativeCount = accumulativeCount;

  // Update the buzzer
  buzzer.update();

  // Every second
  if (millis() - loopTimer >= 1000) {

    // Update loop timer
    loopTimer = millis();

    // Print values
    Serial.println("Accumulative count: " + String(accumulativeCount));
    Serial.println("Counts per minute: " + String(countsPerMinute));
    Serial.println("Equivalent dose: " + String(equivalentDose) + " µSv/h");
    Serial.println(" ");

  }

}