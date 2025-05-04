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

  // If not initalized
  if (!_initalized) {

    // Set initialization flag to true
    _initalized = true;

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

  // Mute the buzzer
  _muted = true;

  // Create event data
  Logger::KeyValuePair event[2] = {

    {"source", Logger::STRING_T, {.string_v = "buzzer"}},
    {"muted",  Logger::BOOL_T,   {.bool_v   = _muted}  }

  };

  // Log event message
  logger.log(Logger::EVENT, "event", event, 2);

}

// ================================================================================================
// Unmute the buzzer
// ================================================================================================
void Buzzer::unmute() {

  // Unmute the buzzer
  _muted = false;

  // Create event data
  Logger::KeyValuePair event[2] = {

    {"source", Logger::STRING_T, {.string_v = "buzzer"}},
    {"muted",  Logger::BOOL_T,   {.bool_v   = _muted}  }

  };

  // Log event message
  logger.log(Logger::EVENT, "event", event, 2);

}

// ================================================================================================
// Play a sound effect
// ================================================================================================
void Buzzer::play(const Sound &sound, const uint16_t repeats) {

  // If the buzzer is not muted
  if (!_muted) {

    // If the audio channel of the sound is not muted
    if (!sound.channel.getMuteState()) {

      // If the sound repeats at least once
      if (repeats) {

        // Play sound for the number of specified repeats
        _audio.play(sound.melody.notes, sound.melody.length, sound.melody.repeats * repeats);

      }

    }

  }

}

// ================================================================================================
// Set mute state
// ================================================================================================
void Buzzer::setMuteState(const bool state) {

  _muted = state;

}

// ================================================================================================
// Check if the buzzer is muted
// ================================================================================================
bool Buzzer::getMuteState() {

  // Check and return if the buzzer is muted
  return _muted;

}

// ================================================================================================
// Check if anything is playing
// ================================================================================================
bool Buzzer::getPlaybackState() {

  // Check and return if anything is playing
  return _audio.playing();

}

// ================================================================================================
// Check if a specific sound is playing
// ================================================================================================
bool Buzzer::getPlaybackState(const Sound &sound) {

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
  _muted(false),
  _audio(BUZZER_PIN)

{}