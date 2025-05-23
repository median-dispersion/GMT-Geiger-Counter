#ifndef _CHANNEL_H
#define _CHANNEL_H

#include "Arduino.h"

class Channel {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Constructor
    Channel(const bool muted = false);

    void mute();                         // Mute the audio channel
    void unmute();                       // Unmute the audio channel
    void setMuteState(const bool state); // Set a mute state
    bool getMuteState();                 // Check if audio channel is muted

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    bool _muted; // Flag for checking if the audio channel is muted

};

#endif