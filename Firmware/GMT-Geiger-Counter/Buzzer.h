#ifndef _BUZZER_H
#define _BUZZER_H

#include "Arduino.h"
#include "Configuration.h"
#include "NonBlockingMelody.h"
#include "Channel.h"
#include "Melody.h"

class Buzzer {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Sound structure
    struct Sound {

      Channel &channel; // Audio channel reference
      Melody  &melody;  // Melody data reference

    };

    // Audio channels
    Channel alerts;        // Audio channel for alerts
    Channel detections;    // Audio channel for detections
    Channel interface;     // AUdio channel for the UI
    Channel notifications; // Audio channel for notifications

    // Sound effects
    Sound alarm;            // Radiation alarm sound effect
    Sound back;             // UI back sound effect
    Sound click;            // UI button press sound effect
    Sound coincidenceEvent; // UI button press sound effect
    Sound detection;        // Detection sound effect
    Sound jingle;           // Power on jingle sound effect
    Sound next;             // UI next sound effect
    Sound warning;          // Radiation warning sound effect

    // Constructor
    Buzzer();

    void begin();                                  // Initialize everything
    void update();                                 // Update the Buzzer
    void mute();                                   // Mute the buzzer
    void unmute();                                 // Unmute the buzzer
    bool muted();                                  // Check if the buzzer is muted
    void play(Sound &sound, uint16_t repeats = 1); // Play a sound effect
    bool playing();                                // Check if anything is playing
    bool playing(Sound &sound);                    // Check if a specific sound is playing

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    bool _muted;               // Mute flag
    NonBlockingMelody _melody; // Melody object

};
#endif