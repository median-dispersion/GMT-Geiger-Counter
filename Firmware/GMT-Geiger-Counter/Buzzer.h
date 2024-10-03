#include <Arduino.h>
#include "Configuration.h"

class Buzzer {

  public:

    // Collection of sounds
    enum Sound {

      NONE,      // When no sound is being played
      DETECTION, // Sound that plays when a particle is detected
      WARNING,   // Warning sound for medium levels of radiation
      ALARM,     // Alarm sound for high levels of radiation
      JINGLE     // Power on jingle

    };

    // Collection of audio channels
    enum Channel {

      DETECTIONS,     // Audio channel for all detections sound
      NOTIFICATIONS,  // Audio channel for sounds like warnings
      ALERTS,         // Audio channel for alarm sounds
      INTERFACE       // Audio channel for user interactions

    };

    void begin();                                 // Initialize everything
    void update();                                // Update everything
    void play(Sound sound, uint16_t repeats = 0); // Play a sound
    bool playing(Sound sound = NONE);             // Return playback status
    void mute(Channel channel);                   // Mute an audio channel
    void unmute(Channel channel);                 // Unmute an audio channel

  private:

    // Note type
    struct Note {

      uint16_t frequencyHertz;       // Frequency of the note in Hertz
      uint16_t durationMilliseconds; // Duration of the note in milliseconds

    };

    // Melody type
    struct Melody {

      Sound    sound;   // Sound effect
      Note     *notes;  // Array of notes
      uint16_t length;  // Length of the melody
      uint16_t repeats; // Number of repeats

    };

    // Track type
    struct Track {

      Channel channel; // Audio channel
      Sound   *sounds; // Sounds on this track
      uint8_t members; // Number of sounds (members)
      bool    muted;   // Flag for muting the track

    };

    // Note arrays for all the different sounds
    Note _detectionNotes[2] = {{BUZZER_FREQUENCY_HERTZ,5},{0,5}};
    Note _warningNotes[8]   = {{BUZZER_FREQUENCY_HERTZ,50},{0,50},{BUZZER_FREQUENCY_HERTZ+300,50},{0,50},{BUZZER_FREQUENCY_HERTZ,50},{0,50},{BUZZER_FREQUENCY_HERTZ+300,50},{0,550}};
    Note _alarmNotes[2]     = {{BUZZER_FREQUENCY_HERTZ,200},{0,50}};
    Note _jingleNotes[5]    = {{BUZZER_FREQUENCY_HERTZ,100},{BUZZER_FREQUENCY_HERTZ+288,150},{0,200},{BUZZER_FREQUENCY_HERTZ+611,100},{BUZZER_FREQUENCY_HERTZ+787,100}};

    Sound _detectionSounds[1]    = {DETECTION}; // All sounds that play on the DETECTIONS channel
    Sound _notificationSounds[1] = {WARNING};   // All sounds that play on the NOTIFICATION channel
    Sound _alertSounds[1]        = {ALARM};     // All sounds that play on the ALERTS channel
    Sound _interfaceSounds[1]    = {JINGLE};    // All sounds that play on the INTERFACE channel

    // Melody's for the different sounds
    // Name of the sound, its notes, length of the melody and repeat count
    Melody _melodys[4] = {

      {DETECTION, _detectionNotes, 2, 0},
      {WARNING, _warningNotes, 8, 2},
      {ALARM, _alarmNotes, 2, 1},
      {JINGLE, _jingleNotes, 5, 1}

    };

    // The 4 audio channels "DETECTIONS", "NOTIFICATION", "ALERTS" and "INTERFACE"
    // Sounds associated with that channel, number of sound (members), flag for muting the channel
    Track _tracks[4] = {

      {DETECTIONS, _detectionSounds, 1, false},
      {NOTIFICATIONS, _notificationSounds, 1, false},
      {ALERTS, _alertSounds, 1, false},
      {INTERFACE, _interfaceSounds, 1, false}

    };

    Sound    _playing                 = NONE;    // Sound that is being played
    Note     *_melody                 = nullptr; // Notes of the selected melody
    uint16_t _length                  = 0;       // Length of the melody
    uint16_t _repeats                 = 0;       // Number of repeats of the melody
    uint16_t _note                    = 0;       // Current note that is being played
    uint64_t _updateTimerMilliseconds = 0;       // Timer for measuring the update interval

    Track&  _getTrackBySound(Sound sound);       // Return audio track by sound
    Track&  _getTrackByChannel(Channel channel); // Return audio track by channel
    Melody& _getMelody(Sound sound);             // Return melody for a specific sound
    void    _advanceMelody();                    // Advance the melody

};