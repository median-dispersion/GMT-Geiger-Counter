/*

  Calibrate the average pulse time for a Geiger-Müller tube
  This is not the tube's dead time, but the pulse length the tube driver generates for that specific tube!

  Median Dispersion 2024
  https://github.com/median-dispersion/GMT-Geiger-Counter

*/

// ================================================================================================
// User configuration
// ================================================================================================

// The pin, the GM tube is connected to
// When using the tube drive board and connected to the main tube header, this doesn't need to be changed
#define TUBE_PIN 42

// How many samples to taken before averaging them
#define SAMPLE_COUNT 100

// Noise threshold, the minimum length in microseconds a pulse must be to count as an actual pulse
// Anything shorter than that will be ignored as noise
#define NOISE_THRESHOLD_MICROSECONDS 30

// ================================================================================================
// Global variables
// ================================================================================================
volatile uint64_t totalPulseTimeMicroseconds = 0;
volatile uint64_t maxPulseTimeMicroseconds = 0;
volatile uint64_t minPulseTimeMicroseconds = UINT64_MAX;
volatile uint16_t pulseIndex = 0;
volatile uint64_t tubeTimer = 0;
uint64_t loopTimer = 0;
uint8_t finished = false;

// ================================================================================================
// ISR for measuring pulse length
// ================================================================================================
void IRAM_ATTR measurePulseLength() {

  // If the pin is HIGH (rising edge)
  if (digitalRead(TUBE_PIN) == HIGH) {

    // Capture the starting time in microseconds
    tubeTimer = micros();

  // If the pin is LOW (falling edge)
  } else {

    // Calculate the difference between the current and starting time in microseconds
    uint64_t pulseLength = micros() - tubeTimer;

    // If the pulse length is higher than the noise threshold
    if (pulseLength > NOISE_THRESHOLD_MICROSECONDS) {

      // Add the pulse length to the total pulse time
      totalPulseTimeMicroseconds += pulseLength;

      // Set the maximum pulse length
      if (pulseLength > maxPulseTimeMicroseconds) { maxPulseTimeMicroseconds = pulseLength; }

      // Set the minimum pulse length
      if (pulseLength < minPulseTimeMicroseconds) { minPulseTimeMicroseconds = pulseLength; }

      // Increase the index by one until the sample count is reached
      if (pulseIndex < SAMPLE_COUNT) { pulseIndex++; }

    }

  }

}

// ================================================================================================
// Main setup
// ================================================================================================
void setup() {

  // Setup serial communication
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n\n");

  // Set the pin mode for the tube pin to INPUT
  pinMode(TUBE_PIN, INPUT);

  // Attach a hardware interrupt to the tube pin that calls an ISR when the pin state is changing
  attachInterrupt(digitalPinToInterrupt(TUBE_PIN), measurePulseLength, CHANGE);

}

// ================================================================================================
// Main loop
// ================================================================================================
void loop() {

  // Check if samples are still being taken
  if (pulseIndex < SAMPLE_COUNT) {

    // Every second
    if (millis() - loopTimer >= 1000) {

      // Print status message to the serial console
      Serial.println("Collecting samples (" + String(pulseIndex) + "/" + String(SAMPLE_COUNT) + ")...");

      // Update the loop timer to the current time
      loopTimer = millis();

    }

  // If sampling process has finished
  } else {

    // Check if result was not already printed to the serial console
    if(!finished) {

      // Detach the hardware interrupt to stop the measurement
      detachInterrupt(digitalPinToInterrupt(TUBE_PIN));

      // Calculate the average pulse time in microseconds
      uint16_t averagePulseLengthMicroseconds = round(static_cast<double>(totalPulseTimeMicroseconds) / SAMPLE_COUNT);

      // Print the result to the serial console
      Serial.println("\n\n\n");
      Serial.println("Finished!");
      Serial.println("=========\n");
      Serial.println("Total accumulative pulse time for " + String(SAMPLE_COUNT) + " samples: " + String(totalPulseTimeMicroseconds) + " µs");
      Serial.println("Maximum pulse length: " + String(maxPulseTimeMicroseconds) + "µs");
      Serial.println("Minimum pulse length: " + String(minPulseTimeMicroseconds) + "µs");
      Serial.println("Average pulse length: " + String(averagePulseLengthMicroseconds) + "µs");
      
      // Set the finished flag to true
      finished = true;

    }

  }

}