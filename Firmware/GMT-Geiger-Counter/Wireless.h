#ifndef _WIRELESS_H
#define _WIRELESS_H

#include "Arduino.h"
#include "Configuration.h"
#include "Strings.h"
#include "Logger.h"
#include "SDCard.h"
#include "WiFi.h"
#include "WebServer.h"
#include "Preferences.h"

class Wireless {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // HTTP webserver for data logging and web-app interface
    WebServer server;

    // Get the single instance of the class
    static Wireless& getInstance();

    void        begin();                                       // Initialize everything
    void        update();                                      // Update the wireless interfaces
    void        enableWiFi();                                  // Enable the WiFi
    void        disableWiFi();                                 // Disable the WiFi
    void        resetWiFi();                                   // Reset the WiFi
    void        enableHotspot();                               // Enable the wireless hotspot
    void        disableHotspot();                              // Disable the wireless hotspot
    void        setWiFiState(const bool state);                // Set the WiFi state
    void        setHotspotState(const bool state);             // Set the wireless hotspot state
    void        setWiFiName(const char *name);                 // Set the WiFi name / SSID
    void        setWiFiPassword(const char *password);         // Set the WiFi password
    bool        getWiFiState();                                // Get the WiFi state
    bool        getHotspotState();                             // Get the wireless hotspot state
    bool        getServerState();                              // Get the HTTP server state
    const char* getWiFiName();                                 // Get the WiFi name / SSID
    const char* getWiFiIPAddress();                            // Get the WiFi IP address
    const char* getHotspotIPAddress();                         // Get the wireless hotspot IP address

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    // Prevent direct instantiation
    Wireless();
    Wireless(const Wireless&) = delete;
    Wireless& operator=(const Wireless&) = delete;

    bool        _initialized;                       // Flag for checking if the wireless interface was initialized
    bool        _wifiEnabled;                       // Flag for checking if the WiFi is enabled
    bool        _hotspotEnabled;                    // Flag for checking if the hotspot is enabled
    bool        _serverEnabled;                     // Flag for checking if the webserver is enabled
    String      _wifiName;                          // WiFi Name string
    String      _wifiPassword;                      // WiFi password string
    String      _ipAddress;                         // WiFi IP address string
    Preferences _preferences;                       // WiFi preferences stored in non-volatile memory
    uint8_t     _hotspotClients;                    // Number of connected hotspot clients
    uint64_t    _wifiConnectionAttemptMilliseconds; // Milliseconds since the last WiFi connection attempt

    void        _enableServer();          // Enable the HTTP server
    void        _disableServer();         // Disable the HTTP server
    static void _handleRequest();         // Handle all HTTP requests not previously defined
    static void _handleWiFiCredentials(); // Handle updates of the WiFi credentials via the web interface
    
};

// Global reference to the wireless instance for easy access
extern Wireless& wireless;

#endif