// ================================================================================================
// System setting
// ================================================================================================

// The root directories on the SD card the Geiger counter will look for
// This is used for all applications that require storage, like logging or the web interface
// The root directory will be used to detect if an SD card is present
// The specific subdirectories are based on the root directory
// These values should not be changed!
#define SD_CARD_ROOT_DIRECTORY     "/GMT-Geiger-Counter"
#define SD_CARD_LOG_DIRECTORY      SD_CARD_ROOT_DIRECTORY"/Logs"
#define SD_CARD_WEB_APP_DIRECTORY  SD_CARD_ROOT_DIRECTORY"/Web-App"

// The maximum log file size in bytes
// If the log file reaches a size larger than this, it will be split up into parts
// This value should not be changed!
// Default: 8388608 (8 MiB)
#define LOG_FILE_MAXIMUM_SIZE_BYTES 8388608

// Baud rate for serial communication
// This value should not be changed!
// Default: 115200
#define SERIAL_BAUD_RATE 115200

// ================================================================================================
// Pin assignments
// ================================================================================================

// These values should not be changed!
#define MOSI_PIN            11
#define MISO_PIN            13
#define SCK_PIN             12
#define DISPLAY_LED_PIN     9
#define DISPLAY_RST_PIN     8
#define DISPLAY_DC_PIN      3
#define DISPLAY_CS_PIN      18
#define TOUCH_CS_PIN        10
#define TOUCH_IRQ_PIN       14
#define BUZZER_PIN          47
#define SD_CS_PIN           17
#define MAIN_TRG_PIN        42
#define FOLLOWER_TRG_PIN    41
#define COINCIDENCE_TRG_PIN 40