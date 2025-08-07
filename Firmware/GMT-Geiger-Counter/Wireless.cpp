#include "Wireless.h"

// ------------------------------------------------------------------------------------------------
// Public

// Initialize global reference
Wireless& wireless = Wireless::getInstance();

// ================================================================================================
// Get the single instance of the class
// ================================================================================================
Wireless& Wireless::getInstance() {

  // Get the single instance
  static Wireless instance;

  // Return the instance
  return instance;

}

// ================================================================================================
// Initialize everything
// ================================================================================================
void Wireless::begin() {

  // If the wireless interface is not initialized
  if (!_initialized) {

    // Set the initialization flag to true
    _initialized = true;

    // Initialize logger
    logger.begin();

    // Initialize SD card
    sdCard.begin();

    // Open non-volatile storage
    _preferences.begin("wifi", false);

    // Read WiFi name, password and last state from the non-volatile storage
    _wifiName      = _preferences.getString("wifiName", "");
    _wifiPassword  = _preferences.getString("wifiPassword", "");
    bool wifiState = _preferences.getBool("wifiEnabled", false);

    // Close non-volatile storage
    _preferences.end();

    // If no WiFi name or password stored in the non-volatile storage
    if (_wifiName.isEmpty() || _wifiPassword.isEmpty()) {

      // Use the WiFi name and password defined in the configuration
      setWiFiName(WIFI_NAME);
      setWiFiPassword(WIFI_PASSWORD);

    }

    // Restore the last WiFi state
    setWiFiState(wifiState);

    // Handle updates of the WiFi credentials via the web interface
    server.on("/system/wifi-credentials", HTTP_PUT, _handleWiFiCredentials);

    // Handle all HTTP request not previously defined
    server.onNotFound(_handleRequest);

  }

}

// ================================================================================================
// Update the wireless interfaces
// ================================================================================================
void Wireless::update() {

  // Update HTTP clients
  server.handleClient();

  // Get the current number of hotspot clients
  uint8_t currentHotspotClients = WiFi.softAPgetStationNum();

  // If the last known and current number of hotspot clients dont match
  if (_hotspotClients != currentHotspotClients) {

    // If the number of clients increased
    if (_hotspotClients < currentHotspotClients) { 
    
      // Create event data
      Logger::KeyValuePair event[2] = {

        {"source", Logger::STRING_T, {.string_v = "hotspotClient"}},
        {"action", Logger::STRING_T, {.string_v = "connected"}    }

      };

      // Log event message
      logger.log(Logger::EVENT, "event", event, 2);

    } else {

      // Create event data
      Logger::KeyValuePair event[2] = {

        {"source", Logger::STRING_T, {.string_v = "hotspotClient"}},
        {"action", Logger::STRING_T, {.string_v = "disconnected"} }

      };

      // Log event message
      logger.log(Logger::EVENT, "event", event, 2);

    }

    // Update the number of connected clients
    _hotspotClients = currentHotspotClients;

  }

  // If the WiFi is enabled but disconnected and the connection timeout has been reached
  if (
    
      getWiFiState() == true
      && 
      WiFi.status() != WL_CONNECTED
      && 
      millis() - _wifiConnectionAttemptMilliseconds >= WIFI_CONNECTION_TIMEOUT_SECONDS * 1000
    
  ) {

    // Disable and then reenable the WiFi to manually try a reconnection
    disableWiFi();
    enableWiFi();

  }

}

// ================================================================================================
// Enable the WiFi
// ================================================================================================
void Wireless::enableWiFi() {

  // If WiFi is not enabled
  if (!_wifiEnabled) {

    // Disable the wireless hotspot
    disableHotspot();

    // Automatically reconnect WiFi if the connection is lost (is not reliable for some reason?)
    WiFi.setAutoReconnect(true);

    // Connect to WiFi
    WiFi.begin(_wifiName.c_str(), _wifiPassword.c_str());

    // Enable the HTTP server
    _enableServer();

    // Store WiFi state in non-volatile memory
    _preferences.begin("wifi", false);
    _preferences.putBool("wifiEnabled", true);
    _preferences.end();

    // Set the enabled flag to true
    _wifiEnabled = true;

    // Set the wifi connection attempt to the current time in milliseconds
    _wifiConnectionAttemptMilliseconds = millis();

    // Create event data
    Logger::KeyValuePair event[2] = {

      {"source", Logger::STRING_T, {.string_v = "wifi"}   },
      {"action", Logger::STRING_T, {.string_v = "enabled"}}

    };

    // Log event message
    logger.log(Logger::EVENT, "event", event, 2);

  }

}

// ================================================================================================
// Disable the WiFi
// ================================================================================================
void Wireless::disableWiFi() {

  // If WiFi is enabled
  if (_wifiEnabled) {

    // Disable the HTTP server
    _disableServer();

    // Disconnect the WiFi and turn off the antenna
    WiFi.disconnect(true);

    // Store WiFi state in non-volatile memory
    _preferences.begin("wifi", false);
    _preferences.putBool("wifiEnabled", false);
    _preferences.end();

    // Set the enabled flag to false
    _wifiEnabled = false;

    // Create event data
    Logger::KeyValuePair event[2] = {

      {"source", Logger::STRING_T, {.string_v = "wifi"}    },
      {"action", Logger::STRING_T, {.string_v = "disabled"}}

    };

    // Log event message
    logger.log(Logger::EVENT, "event", event, 2);

  }

}

// ================================================================================================
// Reset the WiFi
// ================================================================================================
void Wireless::resetWiFi() {

  // Disable the WiFi
  disableWiFi();

  // Set the WiFi name and password to the configuration values
  setWiFiName(WIFI_NAME);
  setWiFiPassword(WIFI_PASSWORD);

}

// ================================================================================================
// Enable the wireless hotspot
// ================================================================================================
void Wireless::enableHotspot() {

  // If the wireless hotspot is disabled
  if (!_hotspotEnabled) {

    // Disable WiFi
    disableWiFi();

    // Create wireless hotspot
    WiFi.softAP(HOTSPOT_NAME, HOTSPOT_PASSWORD);

    // Enable the HTTP server
    _enableServer();

    // Set the enable flag to true
    _hotspotEnabled = true;

    // Create event data
    Logger::KeyValuePair event[2] = {

      {"source", Logger::STRING_T, {.string_v = "hotspot"}},
      {"action", Logger::STRING_T, {.string_v = "enabled"}}

    };

    // Log event message
    logger.log(Logger::EVENT, "event", event, 2);

  }

}

// ================================================================================================
// Disable the wireless hotspot
// ================================================================================================
void Wireless::disableHotspot() {

  // If wireless hotspot is enabled
  if (_hotspotEnabled) {

    // Disable the HTTP server
    _disableServer();

    // Disable wireless hotspot and turn off the WiFi antenna
    WiFi.softAPdisconnect(true);

    // Set the enable flag to false
    _hotspotEnabled = false;

    // Create event data
    Logger::KeyValuePair event[2] = {

      {"source", Logger::STRING_T, {.string_v = "hotspot"} },
      {"action", Logger::STRING_T, {.string_v = "disabled"}}

    };

    // Log event message
    logger.log(Logger::EVENT, "event", event, 2);

  }

}

// ================================================================================================
// Set the WiFi state
// ================================================================================================
void Wireless::setWiFiState(const bool state) {

  // Depending on the state either enable or disable the wifi
  if (state) {

    enableWiFi();

  } else {

    disableWiFi();

  }

}

// ================================================================================================
// Set the wireless hotspot state
// ================================================================================================
void Wireless::setHotspotState(const bool state) {

  // Depending on the state either enable or disable the wireless hotspot
  if (state) {

    enableHotspot();

  } else {

    disableHotspot();

  }

}

// ================================================================================================
// Set the WiFi name / SSID
// ================================================================================================
void Wireless::setWiFiName(const char *name) {

  // Change WiFi name
  _wifiName = name;

  // Open non-volatile storage
  _preferences.begin("wifi", false);

  // Store WiFi name in non-volatile storage
  _preferences.putString("wifiName", _wifiName.c_str());

  // Close non-volatile storage
  _preferences.end();

}

// ================================================================================================
// Set the WiFi password
// ================================================================================================
void Wireless::setWiFiPassword(const char *password) {

  // Change WiFi password
  _wifiPassword = password;

  // Open non-volatile storage
  _preferences.begin("wifi", false);

  // Store WiFi password in non-volatile storage
  _preferences.putString("wifiPassword", _wifiPassword.c_str());

  // Close non-volatile storage
  _preferences.end();

}

// ================================================================================================
// Get the WiFi state
// ================================================================================================
bool Wireless::getWiFiState() {

  return _wifiEnabled;

}

// ================================================================================================
// Get the wireless hotspot state
// ================================================================================================
bool Wireless::getHotspotState() {

  return _hotspotEnabled;

}

// ================================================================================================
// Get the HTTP server state
// ================================================================================================
bool Wireless::getServerState() {

  return _serverEnabled;

}

// ================================================================================================
// Get the WiFi name / SSID
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

      // Return the IP address string
      return _ipAddress.c_str();
    
    // If not connected
    } else {

      // Return the connecting string
      return STRING_CONNECTING;

    }
  
  // If WiFi is disabled
  } else {

    // Return the not connected string
    return STRING_NOT_CONNECTED;

  }

}

// ================================================================================================
// Get the wireless hotspot IP address
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

    // Return the not connected string
    return STRING_NOT_CONNECTED;

  }

}

// ------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Constructor
// ================================================================================================
Wireless::Wireless():

  // Initialize members
  server(80),
  _initialized(false),
  _wifiEnabled(false),
  _hotspotEnabled(false),
  _serverEnabled(false),
  _wifiName(""),
  _wifiPassword(""),
  _ipAddress(""),
  _hotspotClients(0),
  _wifiConnectionAttemptMilliseconds(0)

{}

// ================================================================================================
// 
// ================================================================================================
void Wireless::_enableServer() {

  // If the HTTP server is disabled
  if (!_serverEnabled) {

    // Start the HTTP server
    server.begin();

    // Set the enabled flag to true
    _serverEnabled = true;

    // Create event data
    Logger::KeyValuePair event[2] = {

      {"source", Logger::STRING_T, {.string_v = "server"} },
      {"action", Logger::STRING_T, {.string_v = "enabled"}}

    };

    // Log event message
    logger.log(Logger::EVENT, "event", event, 2);

  }

}

// ================================================================================================
// 
// ================================================================================================
void Wireless::_disableServer() {

  // If the HTTP server is enabled
  if (_serverEnabled) {

    // Stop the HTTP server
    server.stop();

    // Set the enabled flag to false
    _serverEnabled = false;

    // Create event data
    Logger::KeyValuePair event[2] = {

      {"source", Logger::STRING_T, {.string_v = "server"}  },
      {"action", Logger::STRING_T, {.string_v = "disabled"}}

    };

    // Log event message
    logger.log(Logger::EVENT, "event", event, 2);

  }

}

// ================================================================================================
// 
// ================================================================================================
void Wireless::_handleRequest() {

  // If the SD card is mounted
  if (sdCard.getMountState()) {

    // Construct resource path
    String path  = SD_CARD_WEB_APP_DIRECTORY;
           path += wireless.server.uri();

    // Flag for checking if resource was found
    bool found = false;

    // Check if resource exists
    if (sdCard.exists(path.c_str())) {

      // Open path
      File element = sdCard.open(path.c_str());

      // Check if the path element is a directory
      if (element && element.isDirectory()) {

        // Add a generic "index.html" to the end of the path either with or without a leading "/"
        if (path.endsWith("/")) { path += "index.html";  }
        else                    { path += "/index.html"; }

      }

      // Close the path element
      element.close();

      // Check if the resource path exists
      if (sdCard.exists(path.c_str())) {

        // Open the resource
        File file = sdCard.open(path.c_str());

        // If the resource was successfully accessed
        if (file) {

          // Data type string
          String type = "text/html";

          // Depending on the file type change the data type string
          if      (path.endsWith(".html")) { type = "text/html";              }
          else if (path.endsWith(".css") ) { type = "text/css";               }
          else if (path.endsWith(".js")  ) { type = "application/javascript"; }
          else if (path.endsWith(".json")) { type = "application/json";       }
          else if (path.endsWith(".svg") ) { type = "image/svg+xml";          }
          else                             { type = "text/plain";             }

          // Stream the resource data to the HTTP client
          wireless.server.streamFile(file, type);

          // Set the resource found flag to true
          found = true;

        }

        // Close the resource
        file.close();

      }

    }
    
    // If resource was not found
    if (!found) {
    
      // Return with a 404 - Not found!
      wireless.server.send(404, "text/plain", "404 - Not found!");

    } 
  
  // If the SD card is not mounted
  } else {

    // Return with a 500 - No SD Card Mounted!
    wireless.server.send(500, "text/plain", "500 - No SD Card Mounted!");

  }

}

// ================================================================================================
// Handle updates of the WiFi credentials via the web interface
// ================================================================================================
void Wireless::_handleWiFiCredentials() {

  // Flag for checking if the credentials data was valid 
  bool validCredentials = false;

  // If the request has a body
  if (wireless.server.args()){
    
    // Get the request body
    String json = wireless.server.arg(0);

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
        wireless.setWiFiName(json.substring(wifiNameStart, wifiNameEnd).c_str());
        wireless.setWiFiPassword(json.substring(wifiPasswordStart, wifiPasswordEnd).c_str());

        // Set the valid credentials flag to true
        validCredentials = true;

      }

    }

  }

  // If valid credentials were received
  if (validCredentials) {

    // Reply with a success message
    wireless.server.send(200, "application/json", "{\"success\":true}");

  // If no valid credentials were received
  } else {

    // Reply with an error message
    wireless.server.send(400, "application/json", "{\"success\":false}");

  }

}