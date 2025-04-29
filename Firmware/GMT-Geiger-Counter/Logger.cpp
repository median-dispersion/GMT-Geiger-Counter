#include "Logger.h"

// ------------------------------------------------------------------------------------------------
// Public

// Initialize global reference
Logger& logger = Logger::getInstance();

// ================================================================================================
// Get the single instance of the class
// ================================================================================================
Logger& Logger::getInstance() {

  // Get the single instance
  static Logger instance;

  // Return the instance
  return instance;

}

// ================================================================================================
// Initialize everything
// ================================================================================================
void Logger::begin() {

  // If logger was not already initialized
  if (!_initialized) {

    // Set the initialization flag to true
    _initialized = true;

    // Setup log levels
    _logLevels[DATA]   = true;
    _logLevels[EVENT]  = true;
    _logLevels[SYSTEM] = true;

    // Initialize serial communication
    Serial.begin(SERIAL_BAUD_RATE);

    // Initialize SD card
    sdCard.begin();

  }

}

// ================================================================================================
// Set serial logging state
// ================================================================================================
void Logger::setSerialLoggingState(const bool state) {

  _serialLogging = state;

}

// ================================================================================================
// Set SD card logging state
// ================================================================================================
void Logger::setSDCardLoggingState(const bool state) {

  _sdCardLogging = state;

}

// ================================================================================================
// Set a log level state
// ================================================================================================
void Logger::setLogLevelState(const LogLevel level, const bool state) {

  _logLevels[level] = state;

}

// ================================================================================================
// Get state of serial logging
// ================================================================================================
bool Logger::getSerialLoggingState() {

  return _serialLogging;

}

// ================================================================================================
// Get state of SD card logging
// ================================================================================================
bool Logger::getSDCardLoggingState() {

  return _sdCardLogging;

}

// ================================================================================================
// Get the state of a log level
// ================================================================================================
bool Logger::getLogLevelState(const LogLevel level) {

  return _logLevels[level];

}

// ================================================================================================
// Get path to log file
// ================================================================================================
const char* Logger::getLogFilePath() {

  // Check if the SD card is mounted
  if (sdCard.getMountState()) {

    // Check if the log file path is unset
    if (_logFilePath.isEmpty()) {

      // Open log directory
      File directory = sdCard.open(SD_CARD_LOG_DIRECTORY);

      // If accessing the log directory was successful
      if (directory && directory.isDirectory()) {

        // Open first element in log directory
        File element = directory.openNextFile();

        // Loop through all elements in the log directory
        while (element) {

          // If the element is not a directory
          if (!element.isDirectory()) {
            
            // Get the element name
            String name = element.name();

            // Check if it is a JSON file
            if (name.endsWith(".json")) {

              // Increase the log file ID
              _logFileID++;

            }

          }
          
          // Close each element after use
          element.close();

          // Open the next element
          element = directory.openNextFile();

        }

        // Close last element
        element.close();

        // Increase the log file ID to the next log file
        _logFileID++;

        // Construct the full log file path with the new log file ID
        _logFilePath  = SD_CARD_LOG_DIRECTORY;
        _logFilePath += "/Log";
        _logFilePath += _logFileID;
        _logFilePath += ".json";
      
      }
    
    // If a log file is already selected
    } else {

      // Open the selected log file
      File file = sdCard.open(_logFilePath.c_str());

      // If log file size is larger than the maximum allowed size
      if (file && file.size() >= LOG_FILE_MAXIMUM_SIZE_BYTES) {

        // Increase log file part
        _logFilePart++;

        // Construct new file path to the next log file part
        _logFilePath  = SD_CARD_LOG_DIRECTORY;
        _logFilePath += "/Log";
        _logFilePath += _logFileID;
        _logFilePath += ".json.part";
        _logFilePath += _logFilePart;

      }

      // Close log file
      file.close();

    }

  }

  // Return the log file path
  return _logFilePath.c_str();

}

// ================================================================================================
// Construct and return a log message
// ================================================================================================
void Logger::getLogMessage(const char *type, const KeyValuePair *data, const uint8_t size, String &message) {

  // Clear the log message
  message = "";

  // Add log message type
  message += "{\"type\":\"";
  message += type;

  // Add log message time
  message += "\",\"time\":";
  message += millis();

  // Add log message data
  message += ",\"data\":{";

  // For each key value pair
  for (uint8_t pair = 0; pair < size; pair++) {

    // Add the key
    message += "\"";
    message += data[pair].key;
    message += "\":";

    // Add the value depending on the type
    switch (data[pair].type) {

      // If value is an unsigned integer simply add it to the message
      case UINT8_T:  message += data[pair].value.uint8_v;  break;
      case UINT32_T: message += data[pair].value.uint32_v; break;
      case UINT64_T: message += data[pair].value.uint64_v; break;

      // If value is a floating point number format it to have 5 decimal places
      case DOUBLE_T: message += String(data[pair].value.double_v, 5); break;

      // If value is a sting
      case STRING_T:

        // Escape the value with '"'
        message += "\"";
        message += data[pair].value.string_v;
        message += "\"";

      break;

      // If the value is a boolean
      case BOOL_T:

        // Add a "true" or "false" depending on the boolean value
        if (data[pair].value.bool_v) { message += "true";  }
        else                         { message += "false"; }

      break;

    }

    // If not the last the key value pair
    if (pair < size - 1) {

      // Add a trailing ","
      message += ",";

    }

  }

  // Add the trailing "}}" to the message
  message += "}}";

}

// ================================================================================================
// Log data
// ================================================================================================
void Logger::log(const LogLevel level, const String &message) {

  // If selected log level is enabled
  if (_logLevels[level]) {

    // If serial logging is enabled
    if (_serialLogging) {

      // Print log message to serial console
      Serial.println(message);

    }

    // If SD card logging is enabled
    if (_sdCardLogging) {

      // If SD card is mounted
      if (sdCard.getMountState()) {

        // Get the log file path
        const char *path = getLogFilePath();

        // Open the log file in append mode
        File file = sdCard.open(path, FILE_APPEND);

        // If opening the log file was successful
        if (file) {

          // Write log message to log file
          file.print(message);

        }

        // Close the log file
        file.close();

      }

    }
  
  }

}

// ================================================================================================
// Directly log data without returning log message
// ================================================================================================
void Logger::log(const LogLevel level, const char *type, const KeyValuePair *data, const uint8_t size, String &message){

  // Construct the log message
  getLogMessage(type, data, size, message);

  // Log the constructed message
  log(level, message);

}

// ------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Constructor
// ================================================================================================
Logger::Logger():

  // Initialize members
  _initialized(false),
  _serialLogging(true),
  _sdCardLogging(true),
  _logFilePath(""),
  _logFileID(0),
  _logFilePart(0)

{}