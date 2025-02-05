#include "Wireless.h"

//-------------------------------------------------------------------------------------------------
// Public

// Initialize static instance pointer
Wireless *Wireless::_instance = nullptr; 

// ================================================================================================
// Constructor
// ================================================================================================
Wireless::Wireless():

  // Initialize members
  server(80),
  _serverRunning(false),
  _hotspotEnabled(false),
  _wifiEnabled(false),
  _wifiName(WIFI_NAME),
  _wifiPassword(WIFI_PASSWORD),
  _wifiConnectionTimeoutSeconds(WIFI_CONNECTION_TIMEOUT_SECONDS),
  _ipAddressString(STRING_NON_APPLICABLE_ABBREVIATION)

{}

// ================================================================================================
// Initialize everything
// ================================================================================================
void Wireless::begin() {

  // Set the instance pointer to this instance of the class
  _instance = this;

  // Set the 404 not found handler
  server.onNotFound(_handleNotFound);

}

// ================================================================================================
// Update the wireless interfaces
// ================================================================================================
void Wireless::update() {

  // Update web clients
  server.handleClient();

}

// ================================================================================================
// Enable the hotspot
// ================================================================================================
void Wireless::enableHotspot() {

  // If hotspot not enabled
  if (!_hotspotEnabled) {

    // Disable WiFi
    disableWiFi();

    // Start a new hotspot
    WiFi.softAP(HOTSPOT_NAME, HOTSPOT_PASSWORD);

    // Start the webserver
    _startServer();

    // Set the enable flag to true
    _hotspotEnabled = true;

  }

}

// ================================================================================================
// Disable the hotspot
// ================================================================================================
void Wireless::disableHotspot() {

  // If hotspot enabled
  if (_hotspotEnabled) {

    // Disable hotspot and turn off the WiFi antenna
    WiFi.softAPdisconnect(true);

    // Stop the webserver
    _stopServer();

    // Set the enabled flag to false
    _hotspotEnabled = false;

  }

}

// ================================================================================================
// Returns if the hostpot is enabled
// ================================================================================================
bool Wireless::hotspotEnabled() {

  return _hotspotEnabled;

}

// ================================================================================================
// Enable the WiFi
// ================================================================================================
void Wireless::enableWiFi() {

  // If WiFi is not enabled
  if (!_wifiEnabled) {

    // Disable the hotspot
    disableHotspot();

    // Connect to WiFi
    WiFi.begin(_wifiName, _wifiPassword);

    // Get the current time in milliseconds
    uint64_t connectionStartMilliseconds = millis();

    // While connecting to WiFi
    while (WiFi.status() != WL_CONNECTED) {
      
      // Check if connection timeout has been reached
      if (millis() - connectionStartMilliseconds >= _wifiConnectionTimeoutSeconds * 1000){

        // Stop waiting for connection
        break;

      }

      // Keep the watchdog happy :)
      yield();

    }

    // If connected to WiFi
    if (WiFi.status() == WL_CONNECTED) {

      // Start the webserver
      _startServer();

      // Set the enabled flag to true
      _wifiEnabled = true;

    }

  }

}

// ================================================================================================
// Disable the WiFi
// ================================================================================================
void Wireless::disableWiFi() {

  // If WiFi is enabled
  if (_wifiEnabled) {

    // Disconnect the WiFi and turn off the antenna
    WiFi.disconnect(true);

    // Stop the webserver
    _stopServer();

    // Set the enabled flag to false
    _wifiEnabled = false;

  }

}

// ================================================================================================
// Returns if the WiFi is enabled
// ================================================================================================
bool Wireless::wifiEnabled() {

  return _wifiEnabled;

}

// ================================================================================================
// Set the WiFi name / SSID
// ================================================================================================
void Wireless::setWiFiName(const char *name) {

  _wifiName = name;

}

// ================================================================================================
// Set the WiFi password
// ================================================================================================
void Wireless::setWiFiPassword(const char *password) {

  _wifiPassword = password;

}

// ================================================================================================
// Get the current WiFi name
// ================================================================================================
const char* Wireless::getWiFiName() {

  return _wifiName;

}


// ================================================================================================
// Get the IP address
// ================================================================================================
const char* Wireless::getIPAddress() {

  // If hotspot is enabled
  if (_hotspotEnabled) {
    
    // Get the hotspot IP address
    _ipAddressString = WiFi.softAPIP().toString();

  // If WiFi is enabled
  } else if (_wifiEnabled) {

    // Get the WiFi IP address
    _ipAddressString = WiFi.localIP().toString();

  // If nothing is enabled
  } else {
    
    // Set IP address to non-applicable
    _ipAddressString = STRING_NON_APPLICABLE_ABBREVIATION;

  }

  // Return the IP address string
  return _ipAddressString.c_str();

}

//-------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Start the webserver
// ================================================================================================
void Wireless::_startServer() {

  // If server is not running
  if (!_serverRunning) {

    // Start the server
    server.begin();

    // Set the running flag to true
    _serverRunning = true;

  }

}

// ================================================================================================
// Stop the webserver
// ================================================================================================
void Wireless::_stopServer() {

  // If the server is running
  if (_serverRunning) {

    // Stop the server
    server.stop();

    // Set the running flag to false
    _serverRunning = false;

  }

}

// ================================================================================================
// Handle 404 not found events
// ================================================================================================
void Wireless::_handleNotFound() {

  // Send 404 response
  _instance->server.send(404, "text/plain", "404 - Not found!");

}