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
  _ipAddress(STRING_NON_APPLICABLE_ABBREVIATION)

{}

// ================================================================================================
// Initialize everything
// ================================================================================================
void Wireless::begin() {

  // Set the instance pointer to this instance of the class
  _instance = this;

  // Handle updates of the WiFi credentials via the web interface
  server.on("/wifi-credentials", HTTP_PUT, _handleWiFiCredentials);

  // Handle all HTTP request not previously defined
  server.onNotFound(_handleRequest);

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

    // Try creating a new hotspot
    if (WiFi.softAP(HOTSPOT_NAME, HOTSPOT_PASSWORD)) {

      // Start the webserver
      _startServer();

      // Set the enable flag to true
      _hotspotEnabled = true;

    }

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

    // Start the webserver
    _startServer();

    // Set the enabled flag to true
    _wifiEnabled = true;

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

  return _wifiName.c_str();

}

// ================================================================================================
// Get the WiFi IP address
// ================================================================================================
const char* Wireless::getWiFiIPAddress() {

  // If WiFi is enabled
  if (_wifiEnabled) {

    // If WiFi is connected
    if (WiFi.status() == WL_CONNECTED) {

      // Get the WiFi IP address
      _ipAddress = WiFi.localIP().toString();
    
    // If not connected
    } else {

      // Set the IP address string to connecting
      _ipAddress = STRING_CONNECTING;

    }

    // Return the IP address string
    return _ipAddress.c_str();
  
  // If WiFi is disabled
  } else {

    // Return the non applicable string
    return STRING_NON_APPLICABLE_ABBREVIATION;

  }

}

// ================================================================================================
// Get the WiFi IP address
// ================================================================================================
const char* Wireless::getHotspotIPAddress() {

  // If hotspot is enabled
  if (_hotspotEnabled) {

    // Get the hotspot IP address
    _ipAddress = WiFi.softAPIP().toString();

    // Return the IP address string
    return _ipAddress.c_str();
  
  // If hotspot is disabled
  } else {

    // Return the non applicable string
    return STRING_NON_APPLICABLE_ABBREVIATION;

  }

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
// Check if the SD card is mounted
// ================================================================================================
bool Wireless::_sdCardMounted() {

  // Check if the root directory exists
  if (!SD.exists(SD_CARD_ROOT_DIRECTORY)) {

    // If it doesn't exists, try creating it
    if (!SD.mkdir(SD_CARD_ROOT_DIRECTORY)) {

      // If creating the root directory failed, unmount the SD card
      SD.end();

      // Try remounting the SD card
      if (!SD.begin(SD_CS_PIN)) {
        
        // If remounting fails return false
        return false;

      // If remounting was successful
      } else {

        // Again check if the root directory exists
        if (!SD.exists(SD_CARD_ROOT_DIRECTORY)) {

          // If it doesn't exists, try creating it
          if (!SD.mkdir(SD_CARD_ROOT_DIRECTORY)) {

            // If something went wrong while trying to create the root directory for a second time return false
            return false;

          }

        }

      }

    }

  }

  // If mounting was successful and the main directory exists return true
  return true;

}

// ================================================================================================
// Handle updates of the WiFi credentials via the web interface
// ================================================================================================
void Wireless::_handleWiFiCredentials() {

  // Flag for checking if the credentials data was valid 
  bool validCredentials = false;

  // If the request has a body
  if (_instance->server.args()){
    
    // Get the request body
    String json = _instance->server.arg(0);

    // Get start and end index of the WiFi name value
    int16_t wifiNameStart = json.indexOf("{\"wifiName\":\"") + 13;
    int16_t wifiNameEnd   = json.indexOf("\",");

    // If the request body contains the WiFi name value
    if (wifiNameStart != -1 && wifiNameEnd != -1) {

      // Get start and end index of the WiFi password value
      int16_t wifiPasswordStart = json.indexOf("\",\"wifiPassword\":\"") + 18;
      int16_t wifiPasswordEnd   = json.indexOf("\"}");

      // If the request body contains the WiFi password value
      if (wifiPasswordStart != -1 && wifiPasswordEnd != -1) {

        // Extract and set the WiFi name and password
        _instance->setWiFiName(json.substring(wifiNameStart, wifiNameEnd).c_str());
        _instance->setWiFiPassword(json.substring(wifiPasswordStart, wifiPasswordEnd).c_str());

        // Set the valid credentials flag to true
        validCredentials = true;

      }

    }

  }

  // If valid credentials were received
  if (validCredentials) {

    // Reply with a success message
    _instance->server.send(200, "application/json", "{\"success\":true}");

  // If no valid credentials were received
  } else {

    // Reply with an error message
    _instance->server.send(400, "application/json", "{\"success\":false}");

  }

}

// ================================================================================================
// Handle all HTTP request not previously defined
// ================================================================================================
void Wireless::_handleRequest() {

  // Flag for checking if the request was handled
  bool handled = false;

  // Check if the SD card is mounted
  if (_instance->_sdCardMounted()) {

    // Construct the path of the requested file
    String path  = SD_CARD_WEB_APP_DIRECTORY;
           path += _instance->server.uri();
    
    // Check if the path is a directory if so add index.html to it
    if (path.endsWith("/")) { path += "index.html"; }

    // Check if the HTML file exists on the SD card
    if (SD.exists(path)) {

      // Open the file
      File file = SD.open(path);

      // If file was successfully opened and is not a directory
      if (file && !file.isDirectory()) {

        // Send data
        _instance->server.streamFile(file, "text/html");

        // Set the handled flag to true
        handled = true;

      }

      // Close the file
      file.close();

    }

  }

  // If request was not handled
  if (!handled) {

    // Send 404 response
    _instance->server.send(404, "text/plain", "404 - Not found!");

  }

}