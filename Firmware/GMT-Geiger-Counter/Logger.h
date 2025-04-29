#ifndef _LOGGER_H
#define _LOGGER_H

#include "Arduino.h"
#include "Configuration.h"
#include "SDCard.h"

class Logger {

  // ----------------------------------------------------------------------------------------------
  // Public

  public:

    // Log level enumerator
    enum LogLevel {

      DATA,  // Log level for recorded data
      EVENT, // Log level for hardware events
      SYSTEM // Log level for system information

    };

    // Value type enumerator
    enum ValueType {

      UINT8_T,
      UINT32_T,
      UINT64_T,
      DOUBLE_T,
      STRING_T,
      BOOL_T

    };

    // Key value pair structure
    struct KeyValuePair {

      const char *key; // Pair key
      ValueType type;  // Type of value e.g. uint64_t

      // Pair value
      union {

        uint8_t    uint8_v;
        uint32_t   uint32_v;
        uint64_t   uint64_v;
        double     double_v;
        const char *string_v;
        bool       bool_v;

      } value;

    };

    // Get the single instance of the class
    static Logger& getInstance();

    void        begin();                                                                                                    // Initialize everything
    void        setSerialLoggingState(const bool state);                                                                    // Set serial logging state
    void        setSDCardLoggingState(const bool state);                                                                    // Set SD card logging state
    void        setLogLevelState(const LogLevel level, const bool state);                                                   // Set a log level state
    bool        getSerialLoggingState();                                                                                    // Get state of serial logging
    bool        getSDCardLoggingState();                                                                                    // Get state of SD card logging
    bool        getLogLevelState(const LogLevel level);                                                                     // Get the state of a log level
    const char* getLogFilePath();                                                                                           // Get path to log file
    void        getLogMessage(const char *type, const KeyValuePair *data, const uint8_t size, String &message);             // Construct and return a log message
    void        log(const LogLevel level, const String &message);                                                           // Log data
    void        log(const LogLevel level, const char *type, const KeyValuePair *data, const uint8_t size, String &message); // Directly log data without returning log message

  // ----------------------------------------------------------------------------------------------
  // Private

  private:

    // Prevent direct instantiation
    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    bool     _initialized;   // Flag for checking if SD card was initialized
    bool     _serialLogging; // Flag for checking if serial logging is enabled
    bool     _sdCardLogging; // Flag for checking if SD card logging is enabled
    bool     _logLevels[3];  // Log level flags
    String   _logFilePath;   // Log file path
    uint16_t _logFileID;     // Log file ID
    uint16_t _logFilePart;   // Log file part

};

// Global reference to the logger instance for easy access
extern Logger& logger;

#endif