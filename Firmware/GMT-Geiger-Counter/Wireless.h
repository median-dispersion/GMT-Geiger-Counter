#ifndef _WIRELESS_H
#define _WIRELESS_H

#include "Arduino.h"
#include "Configuration.h"
#include "WiFi.h"
#include "SD.h"
#include "WebServer.h"
#include "Strings.h"

class Wireless {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // HTTP webserver for data logging and web app interface
    WebServer server;

    // Constructor
    Wireless();

    void        begin();                               // Initialize everything
    void        update();                              // Update the wireless interfaces
    void        enableHotspot();                       // Enable the hotspot
    void        disableHotspot();                      // Disable the hotspot
    bool        hotspotEnabled();                      // Returns if the hostpot is enabled
    void        enableWiFi();                          // Enable the WiFi
    void        disableWiFi();                         // Disable the WiFi
    bool        wifiEnabled();                         // Returns if the WiFi is enabled
    void        setWiFiName(const char *name);         // Set the WiFi name / SSID
    void        setWiFiPassword(const char *password); // Set the WiFi password
    const char* getWiFiName();                         // Get the current WiFi name
    const char* getIPAddress();                        // Get the IP address

  // ----------------------------------------------------------------------------------------------
  // Public
  
  private:

    // Static instance pointer to itself
    static Wireless *_instance;

    bool       _serverRunning;                // Flag for checking if the webserver is running
    bool       _hotspotEnabled;               // Flag for checking if the hotspot is enabled
    bool       _wifiEnabled;                  // Flag for checking if the WiFi is enabled
    String     _wifiName;                     // WiFi Name
    String     _wifiPassword;                 // WiFi password
    uint8_t    _wifiConnectionTimeoutSeconds; // WiFi connection timeout in seconds
    String     _ipAddressString;              // IP address string

    void        _startServer();               // Start the webserver
    void        _stopServer();                // Stop the webserver
    bool        _sdCardMounted();             // Check if the SD card is mounted
    static void _handleWiFiCredentials();     // Handle updates of the WiFi credentials via the web interface
    static void _handleRequest();             // Handle all HTTP request not previously defined

};

#endif