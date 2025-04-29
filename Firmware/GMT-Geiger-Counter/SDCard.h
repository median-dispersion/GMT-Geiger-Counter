#ifndef _SD_CARD_H
#define _SD_CARD_H

#include "Arduino.h"
#include "Configuration.h"
#include "SD.h"
#include "Logger.h"

class SDCard {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Get the single instance of the class
    static SDCard& getInstance();

    void begin();                                                                         // Initialize the SD card
    void mount();                                                                         // Mount the SD card
    void unmount();                                                                       // Unmount the SD card
    void setMountState(const bool state);                                                 // Set the SD card mount state
    bool getMountState();                                                                 // Return the mount state of the SD card
    File open(const char *path, const char *mode = FILE_READ, const bool create = false); // SD card wrapper function for opening files
    bool exists(const char *path);                                                        // SD card wrapper function for checking if an element exists

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    // Prevent direct instantiation
    SDCard();
    SDCard(const SDCard&) = delete;
    SDCard& operator=(const SDCard&) = delete;

    bool _initialized; // Flag for checking if SD card was initialized
    bool _mounted;     // Flag for checking if SD card is mounted

};

// Global reference to the SD card instance for easy access
extern SDCard& sdCard;

#endif