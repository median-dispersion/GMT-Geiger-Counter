// ================================================================================================
// Wireless setting
// ================================================================================================

// Wireless hotspot name / SSID
// The name of the wireless hotspot the Geiger counter will create for wireless communication
#define HOTSPOT_NAME "GMT-Geiger-Counter"

// Wireless hotspot password
// The password of the wireless hotspot the Geiger counter will create for wireless communication
// This must be at least 8 characters long!
// PLEASE CHANGE THIS FOR SECURITY!
#define HOTSPOT_PASSWORD "Change-This-Immediately!1!1!"

// WiFi name / SSID
// The name of the default WiFi that the Geiger counter will try to connect to
// This value is only used for the initial flash and can later be changed during runtime
#define WIFI_NAME "Your-WiFi-Name"

// WiFi password
// The password of the default WiFi that the Geiger counter will try to connect to
// This value is only used for the initial flash and can later be changed during runtime
#define WIFI_PASSWORD "Your-WiFi-Password"

// ================================================================================================
// System setting
// ================================================================================================

// Interface language
// Available options:
// LANGUAGE_ENGLISH
#define INTERFACE_LANGUAGE LANGUAGE_ENGLISH

// Log interval in seconds
// How often the data should be logged to the serial console and the SD card (if available)
// This should be treated as long-term data acquisition and therefore should have a relatively high value >= 1 minute
// Default: 60
#define LOG_INTERVAL_SECONDS 60

// The filesystem structure the Geiger counter is using
// This is used for all applications that require storage, like logging or the web interface
// The root directory will be used to detect if an SD card is present
// The specific subdirectories and files are based on the root directory
// These values should not be changed!
#define SD_CARD_ROOT_DIRECTORY     "/GMT-Geiger-Counter"
#define SD_CARD_LOG_DIRECTORY      SD_CARD_ROOT_DIRECTORY"/Logs"
#define SD_CARD_WEB_APP_DIRECTORY  SD_CARD_ROOT_DIRECTORY"/Web-App"
#define SD_CARD_SETTINGS_FILE      SD_CARD_ROOT_DIRECTORY"/Settings.bin"

// The maximum log file size in bytes
// If the log file reaches a size larger than this, it will be split up into parts
// This value should not be changed!
// Default: 8388608 (8 MiB)
#define LOG_FILE_MAXIMUM_SIZE_BYTES 8388608

// Baud rate for serial communication
// This value should not be changed!
// Default: 115200
#define SERIAL_BAUD_RATE 115200

// Firmware version
// This can be set to any arbitrary string
// This value should not be changed!
#define FIRMWARE_VERSION "GMTGC-REWRITE"

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