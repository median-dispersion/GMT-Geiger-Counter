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
        double     double_value;
        const char *string_value;
        bool       bool_value;

      } value;

    };

    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void           begin();
    void           enableSerialLogging();                                                                          // Enable serial logging
    void           disableSerialLogging();                                                                         // Disable serial logging
    void           enableSDCardLogging();                                                                          // Enable SD card logging
    void           disableSDCardLogging();                                                                         // Disable SD card logging
    void           enableEventLogging();                                                                           // Enable event logging
    void           disableEventLogging();                                                                          // Disable event logging
    bool           serialLogging();                                                                                // Retruns if serial logging is enabled
    bool           sdCardLogging();                                                                                // Returns if SD card logging is enabled
    bool           eventLogging();                                                                                 // Returns if event logging is enabled
    void           log(const String &message);                                                                     // Log a message
    void           event(const KeyValuePair *data, const uint8_t size);                                            // Log event message
    static Logger& getInstance();                                                                                  // Get the single instance of the logger class
    void           getLogMessage(const char *type, const KeyValuePair *data, const uint8_t size, String &message); // Construct and append a JSON log message

  // ----------------------------------------------------------------------------------------------
  // Private

  private:
    
    // Private constructor to prevent direct instantiation
    Logger();

    bool     _initialized;     // Flag for checking if the logger has been initialized
    bool     _logFileSelected; // Flag for checking if a log file has been selected
    String   _logFilePath;     // Log file path
    uint16_t _logFileID;       // Log file ID
    uint16_t _logFilePart;     // Log file part
    bool     _serialLogging;   // Flag for checking if serial logging is enabled
    bool     _sdCardLogging;   // Flag for checking if SD card logging is enabled
    bool     _eventLogging;    // Flag for checking if event message logging is enabled

    bool        _sdCardMounted();  // Check if the SD card is mounted and the Log directory exists
    const char* _getLogFilePath(); // Get the log file path

};

// Global reference to the Logger instance for easy access
extern Logger &logger;

#endif