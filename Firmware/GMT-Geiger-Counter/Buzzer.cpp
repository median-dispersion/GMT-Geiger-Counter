#include "Buzzer.h"

// ------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Initialize everything
// ================================================================================================
void Buzzer::begin() {

  //  If the buzzer is enabled, set the buzzer pin mode to output
  if (ENABLE_BUZZER) { pinMode(BUZZER_PIN, OUTPUT); }

}

// ================================================================================================
// Update everything
// ================================================================================================
void Buzzer::update() {

  // Only update if the buzzer is enabled
  if (ENABLE_BUZZER) { _advanceMelody(); }

}

// ================================================================================================
// Play a sound
// ================================================================================================
void Buzzer::play(Sound sound, uint16_t repeats) {

  // Get the track, the sound is played on
  Track& track = _getTrackBySound(sound);

  // If the track is not muted
  if (!track.muted) {

    // Get the melody for that sound
    Melody& melody = _getMelody(sound);

    // If not already playing this sound
    if (!playing(melody.sound)) {

      _playing = melody.sound;   // Set the current playback status to this sound
      _melody  = melody.notes;   // Set the melody
      _length  = melody.length;  // Set the length
      _repeats = melody.repeats; // Set the default number of repeats
      _note    = 0;              // Reset the current note

    }

    // Add the number of additional repeats
    _repeats += repeats;

  }

}

// ================================================================================================
// Return playback status
// ================================================================================================
bool Buzzer::playing(Sound sound) {

  // If a sound was provided
  if (sound) {

    // If that sound is playing
    if (sound == _playing) {
      
      // Return true
      return true;

    // If a different or no sound is playing
    } else {

      // Return false
      return false;

    }

  // If no sound was provided
  } else {

    // If anything is playing
    if (_playing) {
      
      // Return true
      return true;

    // If nothing is playing
    } else {

      // Return false
      return false;

    }

  }

}

// ================================================================================================
// Mute an audio channel
// ================================================================================================
void Buzzer::mute(Channel channel) {

  // Get the track for that audio channel
  Track& track = _getTrackByChannel(channel);

  // Mute that track
  track.muted = true;

}

// ================================================================================================
// Unmute an audio channel
// ================================================================================================
void Buzzer::unmute(Channel channel) {

  // Get the track for that audio channel
  Track& track = _getTrackByChannel(channel);

  // Unmute that track
  track.muted = false;

}

// ------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Return audio track by sound
// ================================================================================================
Buzzer::Track& Buzzer::_getTrackBySound(Sound sound) {

  // For all audio tracks
  for (uint8_t t = 0; t < sizeof(_tracks) / sizeof(_tracks[0]); t++) {

    // Select a track
    Track& track = _tracks[t];

    // For all sounds on that track
    for (uint8_t s = 0; s < track.members; s++) {

      // Check if the provided sound is in the list of sounds
      if (track.sounds[s] == sound) {

        // If sound in track return track
        return track;

      }

    }

  }

}

// ================================================================================================
// Return audio track by channel
// ================================================================================================
Buzzer::Track& Buzzer::_getTrackByChannel(Channel channel) {

  // For all audio tracks
  for (uint8_t t = 0; t < sizeof(_tracks) / sizeof(_tracks[0]); t++) {

    // Select a track
    Track& track = _tracks[t];

    // If provided channel is the tracks channel
    if (track.channel == channel) {

      // Return track
      return track;

    }

  }

}

// ================================================================================================
// Return melody for a specific sound
// ================================================================================================
Buzzer::Melody& Buzzer::_getMelody(Sound sound) {

  // For all melodies
  for (uint8_t m = 0; m < sizeof(_melodys) / sizeof(_melodys[0]); m++) {

    // Select a melody
    Melody& melody = _melodys[m];

    // If the provided sound is in that melody
    if (melody.sound == sound) {

      // Return melody
      return melody;

    }

  }

}

// ================================================================================================
// Advance the melody
// ================================================================================================
void Buzzer::_advanceMelody() {

  // If playing a sound
  if (_playing) {

    // If the melody repeats
    if (_repeats) {

      // Set the previous note to the current note
      uint16_t previousNote = _note;

      // If not the first note, decrease to the previous note
      if (previousNote) { previousNote--; }

      // Calculate the time difference since the last update
      // If last update time is greater or equal to the last notes duration
      // Then the last note has finished playing
      if (millis() - _updateTimerMilliseconds >= _melody[previousNote].durationMilliseconds) {

        // If the note has not reached the length of the melody
        // Last note has not been played
        if (_note < _length) {
          
          // Store current time in milliseconds
          _updateTimerMilliseconds = millis();

          // If the note has a frequency meaning it's not a pause
          if (_melody[_note].frequencyHertz) {

            // Use tone library to play on pin, with frequency, for duration
            tone(BUZZER_PIN, _melody[_note].frequencyHertz, _melody[_note].durationMilliseconds);

          }

          // Increase note to the next note
          _note++;

        // When the end of the melody has been reached
        } else {
          
          // Stop playing
          noTone(BUZZER_PIN);

          // Reset to the first note
          _note = 0;

          // Decrease the repeats count
          // If 0 is reached, melody will stop playing
          _repeats--;

        }

      }

    // If no more repeats
    } else {

      // Clear everything and set playback to NONE
      _playing = NONE;
      _melody  = nullptr;
      _length  = 0;
      _repeats = 0;
      _note    = 0;

    }

  }

}