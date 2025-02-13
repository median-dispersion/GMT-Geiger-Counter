#include "Logger.h"

// ------------------------------------------------------------------------------------------------
// Public

// Define the global reference
Logger& logger = Logger::getInstance();

// ================================================================================================
// Constructor
// ================================================================================================
Logger::Logger():

  // Initialize members
  _serialLogging(false),
  _sdCardLogging(false),
  _systemLogging(true),
  _logFileSelected(false),
  _logFileID(0),
  _logFilePart(0),
  _logFilePath("")

{

  // Begin Serial communication if not already established
  Serial.begin(SERIAL_BAUD_RATE);

  // Enable Serial logging
  enableSerialLogging();

  // Check if SD card is mounted
  if (_sdCardMounted()) {

    // Get the log file path
    _getLogFilePath();

    // Enable SD card logging
    enableSDCardLogging();

  }

}

// ================================================================================================
// Get the single instance of the logger class
// ================================================================================================
Logger& Logger::getInstance() {

  // Ensures only one instance
  static Logger instance;  
  
  // Return instance
  return instance;

}

// ================================================================================================
// Construct and append a JSON log message
// ================================================================================================
void Logger::appendLogMessage(const char *type, KeyValuePair *data, uint16_t size, String &message) {

  // Add log message type
  message += "{\"type\":\"";
  message += type;

  // Add log message time
  message += "\",\"time\":";
  message += millis();

  // Add log message data
  message += ",\"data\":{";

  // For each key value pair
  for (uint16_t pair = 0; pair < size; pair++) {

    // Add the key
    message += "\"";
    message += data[pair].key;
    message += "\":";

    // Depending on the type add the value
    switch (data[pair].type) {

      case UINT8_T:  message += data[pair].value.uint8_t_value;  break;
      case UINT16_T: message += data[pair].value.uint16_t_value; break;
      case UINT32_T: message += data[pair].value.uint32_t_value; break;
      case UINT64_T: message += data[pair].value.uint64_t_value; break;
      case FLOAT:    message += data[pair].value.float_value;    break;
      case DOUBLE:   message += data[pair].value.float_value;    break;

      // If value is a sting
      case STRING:

        // Escape value with '"'
        message += "\"";
        message += data[pair].value.string_value;
        message += "\"";

      break;

      // If value is a boolean
      case BOOL:

        // If boolean is true
        if (data[pair].value.bool_value) {

          // Add true as the value
          message += "true";
        
        // If boolean is false
        } else {

          // Add false as the value
          message += "false";

        }

      break;

    }

    // If not the last key value pair
    if (pair < size - 1) {

      // Add trailing ","
      message += ",";

    }

  }

  // Add trailing "}}"
  message += "}}";

}

// ================================================================================================
// 
// ================================================================================================
void Logger::log(const String &message) {

  // If serial logging is enabled print log message to the serial console
  if (serialLogging()) { Serial.print(message); }

  // If SD card logging is enabled
  if (sdCardLogging()) {
    
    // Check if the SD card is mounted
    if (_sdCardMounted()) {

      // Get log file path
      const char *path = _getLogFilePath();

      // Open log file
      File file = SD.open(path, FILE_APPEND);

      // If successfully opend log file
      if (file) {

        // Write log message to file
        file.print(message);

      }
    
    // If the SD card is not or no longer mounted
    } else {

      // Disable SD card logging
      disableSDCardLogging();

    }

  }

}

// ================================================================================================
// Enable serial logging
// ================================================================================================
void Logger::enableSerialLogging() {

  _serialLogging = true;

}

// ================================================================================================
// Disable serial logging
// ================================================================================================
void Logger::disableSerialLogging() {

  _serialLogging = false;

}

// ================================================================================================
// Retruns if serial logging is enabled
// ================================================================================================
bool Logger::serialLogging() {

  return _serialLogging;

}

// ================================================================================================
// Enable SD card logging
// ================================================================================================
void Logger::enableSDCardLogging() {

  _sdCardLogging = true;

}

// ================================================================================================
// Disable SD card logging
// ================================================================================================
void Logger::disableSDCardLogging() {

  _sdCardLogging = false;

}

// ================================================================================================
// Retruns if SD card logging is enabled
// ================================================================================================
bool Logger::sdCardLogging() {

  return _sdCardLogging;

}

// ================================================================================================
// Enable system message logging
// ================================================================================================
void Logger::enableSystemLogging() {

  _systemLogging = true;

}

// ================================================================================================
// Disable system message logging
// ================================================================================================
void Logger::disableSystemLogging() {

  _systemLogging = false;

}

// ================================================================================================
// Retruns if system message logging is enabled
// ================================================================================================
bool Logger::systemLogging() {

  return _systemLogging;

}

// ------------------------------------------------------------------------------------------------
// Private

// ================================================================================================
// Check if the SD card is mounted and the Log directory exists
// ================================================================================================
bool Logger::_sdCardMounted() {
  
  // Check if the log directory exists
  if (!SD.exists(SD_CARD_LOG_DIRECTORY)) {

    // If it doesn't exists, try creating it
    if (!SD.mkdir(SD_CARD_ROOT_DIRECTORY) || !SD.mkdir(SD_CARD_LOG_DIRECTORY)) {

      // If creating the log directory failed, unmount the SD card
      SD.end();

      // Try remounting the SD card
      if (!SD.begin(SD_CS_PIN)) {
        
        // If remounting fails return false
        return false;

      // If remounting was successful
      } else {

        // Again check if the log directory exists
        if (!SD.exists(SD_CARD_LOG_DIRECTORY)) {

          // If it doesn't exists, try creating it
          if (!SD.mkdir(SD_CARD_ROOT_DIRECTORY) || !SD.mkdir(SD_CARD_LOG_DIRECTORY)) {

            // If something went wrong while trying to create the log directory for a second time return false
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
// Get the log file path
// ================================================================================================
const char* Logger::_getLogFilePath() {

  // If log file has not been selected
  if (!_logFileSelected) {

    // Set the path to the log directory
    _logFilePath = SD_CARD_LOG_DIRECTORY;

    // Open log directory
    File root = SD.open(_logFilePath);

    // If successfully opend the log directory
    if (root && root.isDirectory()) { 

      // Open first element in log directory
      File file = root.openNextFile();

      // Loop through all elements in the log directory
      while (file) {

        // If the element is not a directory
        if (!file.isDirectory()) {
          
          // Get the file name
          String name = file.name();

          // Check if it is a JSON file
          if (name.endsWith(".json")) {

            // Increase the log file ID
            _logFileID++;

          }

        }
        
        // Open next element
        file = root.openNextFile();

      }

      // Construct log file path
      _logFilePath += "/Log";
      _logFilePath += _logFileID;
      _logFilePath += ".json";

      // Set the log file selected flag to true
      _logFileSelected = true;

    }
  
  // If a log file has been selected
  } else {

    // Open log file
    File file = SD.open(_logFilePath);

    // If log file size is larger than the maximum allowed size
    if (file && file.size() >= LOG_FILE_MAXIMUM_SIZE_BYTES) {

      // Increase log file part
      _logFilePart++;

      // Construct new file path to the next log file part
      _logFilePath  = SD_CARD_LOG_DIRECTORY;
      _logFilePath += "/Log";
      _logFilePath += _logFileID;
      _logFilePath += ".part";
      _logFilePath += _logFilePart;

    }

  }

  // Return path to log file
  return _logFilePath.c_str();

}