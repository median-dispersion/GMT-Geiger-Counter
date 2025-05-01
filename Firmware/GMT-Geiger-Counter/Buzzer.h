#ifndef _BUZZER_H
#define _BUZZER_H

#include "Arduino.h"
#include "Configuration.h"
#include "Logger.h"
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
    Sound coincidenceEvent; // UI coincidence event sound effect
    Sound detection;        // Detection sound effect
    Sound jingle;           // Power on jingle sound effect
    Sound next;             // UI next sound effect
    Sound tap;              // UI touch press sound effect
    Sound warning;          // Radiation warning sound effect

    // Get the single instance of the class
    static Buzzer& getInstance();

    void begin();                                              // Initialize everything
    void update();                                             // Update the Buzzer
    void mute();                                               // Mute the buzzer
    void unmute();                                             // Unmute the buzzer
    void setMuteState(const bool state);                       // Set the buzzer mute state
    bool getMuteState();                                       // Check if the buzzer is muted
    void play(const Sound &sound, const uint16_t repeats = 1); // Play a sound effect
    bool playing();                                            // Check if anything is playing
    bool playing(const Sound &sound);                          // Check if a specific sound is playing

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    // Prevent direct instantiation
    Buzzer();
    Buzzer(const Buzzer&) = delete;
    Buzzer& operator=(const Buzzer&) = delete;

    bool              _initialized; // Flag for checking if initalized
    bool              _muted;       // Flag for checking if the buzzer is muted
    NonBlockingMelody _audio;       // Audio player object

};

// Global reference to the buzzer instance for easy access
extern Buzzer& buzzer;

#endif