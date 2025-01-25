#include "Buzzer.h"

// ------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
Buzzer::Buzzer():

  // Initialize members
  alerts(false),
  detections(false),
  interface(false),
  notifications(false),
  alarm({alerts, MELODY_ALARM}),
  back({interface, MELODY_BACK}),
  coincidenceEvent({detections, MELODY_COINCIDENCE_EVENT}),
  click({interface, MELODY_CLICK}),
  detection({detections, MELODY_DETECTION}),
  jingle({interface, MELODY_JINGLE}),
  next({interface, MELODY_NEXT}),
  warning({notifications, MELODY_WARNING}),
  _muted(false),
  _melody(BUZZER_PIN)

{}

// ================================================================================================
// Initialize everything
// ================================================================================================
void Buzzer::begin() {

  // Initialize melody
  _melody.begin();

}

// ================================================================================================
// Update the Buzzer
// ================================================================================================
void Buzzer::update() {

  // Update the melody
  _melody.update();

}

// ================================================================================================
// Mute the buzzer
// ================================================================================================
void Buzzer::mute() {

  // Mute the buzzer
  _muted = true;

}

// ================================================================================================
// Unmute the buzzer
// ================================================================================================
void Buzzer::unmute() {

  // Unmute the buzzer
  _muted = false;

}

// ================================================================================================
// Check if the buzzer is muted
// ================================================================================================
bool Buzzer::muted() {

  // Check and return if the buzzer is muted
  return _muted;

}

// ================================================================================================
// Play a sound effect
// ================================================================================================
void Buzzer::play(Sound &sound, uint16_t repeats) {

  // If the buzzer is not muted
  if (!_muted) {

    // If the audio channel of the sound is not muted
    if (!sound.channel.muted()) {

      // If not already playing this sound
      if (!_melody.playing(sound.melody.notes)) {

        // If the sound repeats at least once
        if (repeats) {

          // Play sound for the number of specified repeats
          _melody.play(sound.melody.notes, sound.melody.length, sound.melody.repeats * repeats);

        }

      }

    }

  }

}

// ================================================================================================
// Check if anything is playing
// ================================================================================================
bool Buzzer::playing() {

  // Check and return if anything is playing
  return _melody.playing();

}

// ================================================================================================
// Check if a specific sound is playing
// ================================================================================================
bool Buzzer::playing(Sound &sound) {

  // Check and return if a specific sound is playing
  return _melody.playing(sound.melody.notes);

}