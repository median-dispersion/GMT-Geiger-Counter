/*

  Create the pulse length calibration values for a specific Geiger-Müller tube

  Median Dispersion 2025
  https://github.com/median-dispersion/GMT-Geiger-Counter

*/

#include <Arduino.h>
#include <cstddef>
#include <algorithm>

// ================================================================================================
// User configuration
// ================================================================================================

// The pin, the GM tube is connected to
// When using the tube drive board connected to the main tube header on the motherboard this doesn't need to be changed
#define TUBE_PIN 42

// Number of samples to take
#define SAMPLE_COUNT 100

// Noise threshold, the minimum length in microseconds a pulse must be to count as an actual pulse
// Anything shorter than that will be ignored as noise
#define NOISE_THRESHOLD_MICROSECONDS 30

// ================================================================================================
// Global variables
// ================================================================================================
volatile uint64_t    pulseLengthsMicroseconds[SAMPLE_COUNT];
volatile std::size_t pulseIndex = 0;
volatile uint64_t    tubeTimer  = 0;
uint64_t             loopTimer  = 0;
bool                 finished   = false;

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
    uint64_t pulseLengthMicroseconds = micros() - tubeTimer;

    // If the pulse length is more than the noise threshold
    if (pulseLengthMicroseconds > NOISE_THRESHOLD_MICROSECONDS) {

      // If still taking samples
      if (pulseIndex < SAMPLE_COUNT) {

        // Add the pulse length
        pulseLengthsMicroseconds[pulseIndex] = pulseLengthMicroseconds;

        // Increase the pulse index by one
        pulseIndex++;

      }

    }

  }

}

// ================================================================================================
// 
// ================================================================================================
uint64_t median(volatile uint64_t* values, std::size_t start, std::size_t end) {

  // Get the total length
  std::size_t length = end - start;

  // Guard for empty slices
  if (length == 0) return 0;

  // Get the middle of the array
  std::size_t middle = start + length / 2;

  // If the length is even
  if (length % 2 == 0) {

    // Average and return the middle two elements
    return (values[middle - 1] + values[middle]) / 2;
  
  // If length is odd
  } else {

    // Return the middle element
    return values[middle];

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

      // Sort the pulse lengths
      std::sort(pulseLengthsMicroseconds, pulseLengthsMicroseconds + pulseIndex);

      // Total, minimum and maximum pulse length values
      uint64_t totalPulseLengthMicroseconds   = 0;
      uint64_t minimumPulseLengthMicroseconds = UINT64_MAX;
      uint64_t maximumPulseLengthMicroseconds = 0;

      // For each pulse
      for (std::size_t pulse = 0; pulse < pulseIndex; pulse++) {

        // Add the pulse length to the total pulse length sum
        totalPulseLengthMicroseconds += pulseLengthsMicroseconds[pulse];

        // Update the minimum and maximum pulse length values if applicable
        if (pulseLengthsMicroseconds[pulse] < minimumPulseLengthMicroseconds) { minimumPulseLengthMicroseconds = pulseLengthsMicroseconds[pulse]; }
        if (pulseLengthsMicroseconds[pulse] > maximumPulseLengthMicroseconds) { maximumPulseLengthMicroseconds = pulseLengthsMicroseconds[pulse]; }

      }

      // Calculate the average pulse length value using integer arithmetic
      uint64_t averagePulseLengthMicroseconds = (totalPulseLengthMicroseconds + pulseIndex / 2) / pulseIndex;

      // Get the lower end and upper start index for the list of all pulse lengths 
      std::size_t lowerEnd   = pulseIndex / 2;
      std::size_t upperStart = (pulseIndex % 2 == 0) ? lowerEnd : lowerEnd +1;

      // Calculate the Q1, median and Q3 values
      uint64_t q1PulseLengthMicroseconds     = median(pulseLengthsMicroseconds, 0,          lowerEnd  );
      uint64_t medianPulseLengthMicroseconds = median(pulseLengthsMicroseconds, 0,          pulseIndex);
      uint64_t q3PulseLengthMicroseconds     = median(pulseLengthsMicroseconds, upperStart, pulseIndex);

      // Print the result to the serial console
      Serial.println("\n\n\n");
      Serial.println("Finished!");
      Serial.println("=========\n");
      Serial.println("Total accumulative pulse time for " + String(pulseIndex) + " samples: " + String(totalPulseLengthMicroseconds) + " µs");
      Serial.println("Minimum pulse length: " + String(minimumPulseLengthMicroseconds) + "µs");
      Serial.println("Q1 pulse length: "      + String(q1PulseLengthMicroseconds)      + "µs");
      Serial.println("Average pulse length: " + String(averagePulseLengthMicroseconds) + "µs");
      Serial.println("Median pulse length: "  + String(medianPulseLengthMicroseconds)  + "µs");
      Serial.println("Q3 pulse length: "      + String(q3PulseLengthMicroseconds)      + "µs");
      Serial.println("Maximum pulse length: " + String(maximumPulseLengthMicroseconds) + "µs");
      Serial.println("\nPlease adjust these values in your main configuration file:");
      Serial.println("\n#define TUBE_MEDIAN_PULSE_LENGTH_MICROSECONDS " + String(medianPulseLengthMicroseconds));
      Serial.println("#define TUBE_MINIMUM_PULSE_LENGTH_MICROSECONDS "  + String(minimumPulseLengthMicroseconds));
      Serial.println("#define TUBE_MAXIMUM_PULSE_LENGTH_MICROSECONDS "  + String(maximumPulseLengthMicroseconds));
      Serial.println("\n\n\n");

      // Set the finished flag to true
      finished = true;

    }

  }

}