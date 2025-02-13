#ifndef _LOGGER_H
#define _LOGGER_H

#include "Arduino.h"
#include "Configuration.h"
#include "SD.h"

class Logger {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Value type enumerator
    enum ValueType {

      UINT8_T,
      UINT16_T,
      UINT32_T,
      UINT64_T,
      FLOAT,
      DOUBLE,
      STRING,
      BOOL

    };

    // Key value pair structure
    struct KeyValuePair {

      const char *key;
      ValueType  type;

      union {

        uint8_t    uint8_t_value;
        uint16_t   uint16_t_value;
        uint32_t   uint32_t_value;
        uint64_t   uint64_t_value;
        float      float_value;
        double     double_value;
        const char *string_value;
        bool       bool_value;

      } value;

    };

    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Get the single instance of the logger class
    static Logger& getInstance();

    void appendLogMessage(const char *type, KeyValuePair *data, uint16_t size, String &message); // Construct and append a JSON log message
    void log(const String &message);                                                             // Log a message
    void enableSerialLogging();                                                                  // Enable serial logging
    void disableSerialLogging();                                                                 // Disable serial logging
    bool serialLogging();                                                                        // Retruns if serial logging is enabled
    void enableSDCardLogging();                                                                  // Enable SD card logging
    void disableSDCardLogging();                                                                 // Disable SD card logging
    bool sdCardLogging();                                                                        // Returns if SD card logging is enabled
    void enableSystemLogging();                                                                  // Enable system message logging
    void disableSystemLogging();                                                                 // Disable system smessage logging
    bool systemLogging();                                                                        // Returns if system message logging is enabled

  // ----------------------------------------------------------------------------------------------
  // Private

  private:
    
    // Private constructor to prevent direct instantiation
    Logger();

    bool     _serialLogging;   // Flag for checking if serial logging is enabled
    bool     _sdCardLogging;   // Flag for checking if SD card logging is enabled
    bool     _systemLogging;   // Flag for checking if system message logging is enebaled
    bool     _logFileSelected; // Flag for checking if a log file has been selected
    uint16_t _logFileID;       // Log file ID
    uint16_t _logFilePart;     // Log file part
    String   _logFilePath;     // Log file path

    bool        _sdCardMounted();  // Check if the SD card is mounted and the Log directory exists
    const char* _getLogFilePath(); // Get the log file path

};

// Global reference to the Logger instance for easy access
extern Logger& logger;

#endif