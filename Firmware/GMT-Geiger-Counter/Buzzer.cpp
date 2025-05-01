#include "Buzzer.h"

// ------------------------------------------------------------------------------------------------
// Public

// Initialize global reference
Buzzer& buzzer = Buzzer::getInstance();

// ================================================================================================
// Get the single instance of the class
// ================================================================================================
Buzzer& Buzzer::getInstance() {

  // Get the single instance
  static Buzzer instance;

  // Return the instance
  return instance;

}


// ================================================================================================
// Initialize everything
// ================================================================================================
void Buzzer::begin() {

  // If not initialized
  if (!_initialized) {

    // Set initialization flag to true
    _initialized = true;

    // Initialize logger
    logger.begin();

    // Initialize melody
    _audio.begin();

  }

}

// ================================================================================================
// Update the Buzzer
// ================================================================================================
void Buzzer::update() {

  // Update the melody
  _audio.update();

}

// ================================================================================================
// Mute the buzzer
// ================================================================================================
void Buzzer::mute() {

  // If not muted
  if (!_muted) {

    // Mute the buzzer
    _muted = true;

    // Create hardware event data
    Logger::KeyValuePair event[2] = {

      {"target", Logger::STRING_T, {.string_v = "buzzer"}},
      {"action", Logger::STRING_T, {.string_v = "muted"} }

    };

    // Event data log message
    String message = "";

    // Log hardware event data
    logger.log(Logger::EVENT, "event", event, 2, message);
  
  }

}

// ================================================================================================
// Unmute the buzzer
// ================================================================================================
void Buzzer::unmute() {

  // If muted
  if (_muted) {

    // Unmute the buzzer
    _muted = false;

    // Create hardware event data
    Logger::KeyValuePair event[2] = {

      {"target", Logger::STRING_T, {.string_v = "buzzer"} },
      {"action", Logger::STRING_T, {.string_v = "unmuted"}}

    };

    // Event data log message
    String message = "";

    // Log hardware event data
    logger.log(Logger::EVENT, "event", event, 2, message);

  }

}

// ================================================================================================
// Set the buzzer mute state
// ================================================================================================
void Buzzer::setMuteState(const bool state) {

  // Depending on the state either mute or unmute the buzzer
  if (state) {

    mute();

  } else {

    unmute();

  }

}

// ================================================================================================
// Check if the buzzer is muted
// ================================================================================================
bool Buzzer::getMuteState() {

  // Check and return if the buzzer is muted
  return _muted;

}

// ================================================================================================
// Play a sound effect
// ================================================================================================
void Buzzer::play(const Sound &sound, const uint16_t repeats) {

  // If the buzzer is not muted
  if (!_muted) {

    // If the audio channel of the sound is not muted
    if (!sound.channel.getMuteState()) {

      // If not already playing this sound
      if (!_audio.playing(sound.melody.notes)) {

        // If the sound repeats at least once
        if (repeats) {

          // Play sound for the number of specified repeats
          _audio.play(sound.melody.notes, sound.melody.length, sound.melody.repeats * repeats);

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
  return _audio.playing();

}

// ================================================================================================
// Check if a specific sound is playing
// ================================================================================================
bool Buzzer::playing(const Sound &sound) {

  // Check and return if a specific sound is playing
  return _audio.playing(sound.melody.notes);

}

// ------------------------------------------------------------------------------------------------
// Private

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
  detection({detections, MELODY_DETECTION}),
  jingle({interface, MELODY_JINGLE}),
  next({interface, MELODY_NEXT}),
  tap({interface, MELODY_TAP}),
  warning({notifications, MELODY_WARNING}),
  _initialized(false),
  _muted(false),
  _audio(BUZZER_PIN)

{}