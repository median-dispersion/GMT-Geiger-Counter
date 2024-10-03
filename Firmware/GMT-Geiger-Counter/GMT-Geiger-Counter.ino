#include "Configuration.h"
#include "GeigerCounter.h"

// Create Geiger counter object
GeigerCounter geiger;

// Loop timer
uint64_t loopTimer = 0;

void setup() {

  // Start serial communication
  Serial.begin(SERIAL_BAUD_RATE);

  // Initialize Geiger counter object
  geiger.begin();

  // Enable pulse counting
  geiger.enable();

  // Set integration time
  geiger.setIntegrationTime(DEFAULT_INTEGRATION_TIME);

}

void loop() {

  // Every second
  if (millis() - loopTimer >= 1000) {

    // Update loop timer
    loopTimer = millis();

    // Print values
    Serial.println("Counts per minute: " + String(geiger.getCountsPerMinute()));
    Serial.println("Accumulative count: " + String(geiger.getAccumulativeCount()));
    Serial.println("Equivalent dose: " + String(geiger.getMicroSievertPerHour()) + " µSv/h");
    Serial.println(" ");

  }

}